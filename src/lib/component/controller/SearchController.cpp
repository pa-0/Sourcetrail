#include "SearchController.h"

#include "logging.h"
#include "SearchView.h"
#include "StorageAccess.h"
#include "tracing.h"
#include "type/tab/MessageTabState.h"

SearchController::SearchController(StorageAccess* storageAccess) : m_storageAccess(storageAccess) {}

Id SearchController::getSchedulerId() const {
  return Controller::getTabId();
}

void SearchController::handleActivation(const MessageActivateBase* message) {
  if(const MessageActivateTokens* m = dynamic_cast<const MessageActivateTokens*>(message)) {
    if(!m->isEdge) {
      updateMatches(message, !m->keepContent());
    }
  } else if(const MessageActivateTrail* messageActivateTrail = dynamic_cast<const MessageActivateTrail*>(message)) {
    if(messageActivateTrail->custom) {
      updateMatches(message);
    }
  } else {
    updateMatches(message);
  }
}

void SearchController::handleMessage(MessageFind* message) {
  if(message->findFulltext) {
    getView()->findFulltext();
  } else {
    getView()->setFocus();
  }
}

void SearchController::handleMessage(MessageSearchAutocomplete* message) {
  TRACE("search autocomplete");

  SearchView* view = getView();

  // Don't autocomplete if autocompletion request is not up-to-date anymore
  if(message->query != view->getQuery()) {
    return;
  }

  LOG_INFO_W(L"autocomplete string: \"" + message->query + L"\"");
  view->setAutocompletionList(m_storageAccess->getAutocompletionMatches(message->query, message->acceptedNodeTypes, true));
}

SearchView* SearchController::getView() {
  return Controller::getView<SearchView>();
}

void SearchController::clear() {
  updateMatches(nullptr);
}

void SearchController::updateMatches(const MessageActivateBase* message, bool updateView) {
  std::vector<SearchMatch> matches;

  if(message) {
    matches = message->getSearchMatches();
  }

  if(updateView) {
    getView()->setMatches(matches);
  }

  MessageTabState(Controller::getTabId(), matches).dispatch();
}
