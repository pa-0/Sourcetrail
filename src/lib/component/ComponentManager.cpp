#include "ComponentManager.h"

#include "BookmarkButtonsView.h"
#include "BookmarkView.h"
#include "CodeView.h"
#include "CompositeView.h"
#include "Controller.h"
#include "DialogView.h"
#include "GraphView.h"
#include "RefreshView.h"
#include "ScreenSearchController.h"
#include "SearchView.h"
#include "TabbedView.h"
#include "UndoRedoView.h"
#include "ViewFactory.h"
#include "logging.h"

namespace {
template <class Container>
void reverseErase(Container& container) {
  while(!container.empty())
    container.pop_back();
}
}    // namespace

ComponentManager::ComponentManager(const ViewFactory* viewFactory, StorageAccess* storageAccess)
    : m_componentFactory(viewFactory, storageAccess) {}

void ComponentManager::clear() {
  m_dialogViews.clear();
  reverseErase(m_components);
  reverseErase(m_singleViews);
}

void ComponentManager::setupMain(ViewLayout* viewLayout, Id appId) {
  const auto* viewFactory = m_componentFactory.getViewFactory();
  auto compositeView = viewFactory->createCompositeView(viewLayout, CompositeView::DIRECTION_HORIZONTAL, "Search", 0);
  m_singleViews.push_back(compositeView);

  auto undoRedoView = viewFactory->createUndoRedoView(compositeView.get());
  auto refreshView  = viewFactory->createRefreshView(compositeView.get());
  auto searchView   = viewFactory->createSearchView(compositeView.get());

  auto bookmarkView = viewFactory->createBookmarkButtonsView(compositeView.get());
  bookmarkView->setTabId(appId);

  auto graphView = viewFactory->createGraphView(viewLayout);
  auto codeView  = viewFactory->createCodeView(viewLayout);

  for(auto& view : std::vector<std::shared_ptr<View>>({undoRedoView, searchView, bookmarkView, graphView, codeView})) {
    view->setEnabled(false);
  }

  m_singleViews.push_back(undoRedoView);
  m_singleViews.push_back(refreshView);
  m_singleViews.push_back(searchView);
  m_singleViews.push_back(bookmarkView);
  m_singleViews.push_back(graphView);
  m_singleViews.push_back(codeView);

  auto screenSearchComponent = m_componentFactory.createScreenSearchComponent(viewLayout);
  auto* screenSearchController = screenSearchComponent->getController<ScreenSearchController>();
  screenSearchController->addResponder(graphView.get());
  screenSearchController->addResponder(codeView.get());
  m_components.push_back(screenSearchComponent);

  auto tabsComponent = m_componentFactory.createTabsComponent(viewLayout, screenSearchController);
  m_components.push_back(tabsComponent);

  auto tooltipComponent = m_componentFactory.createTooltipComponent(viewLayout);
  m_components.push_back(tooltipComponent);

  for(DialogView::UseCase useCase :
      {DialogView::UseCase::GENERAL, DialogView::UseCase::INDEXING, DialogView::UseCase::PROJECT_SETUP}) {
    m_dialogViews.emplace(
        useCase, viewFactory->createDialogView(viewLayout, useCase, m_componentFactory.getStorageAccess()));
  }

  m_dialogViews[DialogView::UseCase::INDEXING]->setDialogsHideable(true);

  std::shared_ptr<TabbedView> tabbedView = viewFactory->createTabbedView(viewLayout, "Status");
  m_singleViews.push_back(tabbedView);

  std::shared_ptr<Component> statusComponent = m_componentFactory.createStatusComponent(tabbedView.get());
  m_components.push_back(statusComponent);

  std::shared_ptr<Component> errorComponent = m_componentFactory.createErrorComponent(tabbedView.get());
  m_components.push_back(errorComponent);

  std::shared_ptr<Component> bookmarkComponent = m_componentFactory.createBookmarkComponent(compositeView.get());
  m_components.push_back(bookmarkComponent);

  std::shared_ptr<Component> activationComponent = m_componentFactory.createActivationComponent();
  m_components.push_back(activationComponent);

  std::shared_ptr<Component> statusBarComponent = m_componentFactory.createStatusBarComponent(viewLayout);
  m_components.push_back(statusBarComponent);

  std::shared_ptr<Component> customTrailComponent = m_componentFactory.createCustomTrailComponent(viewLayout);
  m_components.push_back(customTrailComponent);
}

void ComponentManager::setupTab(ViewLayout* viewLayout, Id tabId, ScreenSearchSender* screenSearchSender) {
  std::shared_ptr<CompositeView> compositeView = m_componentFactory.getViewFactory()->createCompositeView(
      viewLayout, CompositeView::DIRECTION_HORIZONTAL, "Search", tabId);
  m_singleViews.push_back(compositeView);

  std::shared_ptr<Component> undoRedoComponent = m_componentFactory.createUndoRedoComponent(compositeView.get());
  undoRedoComponent->setTabId(tabId);
  m_components.push_back(undoRedoComponent);

  std::shared_ptr<Component> refreshComponent = m_componentFactory.createRefreshComponent(compositeView.get());
  refreshComponent->setTabId(tabId);
  m_components.push_back(refreshComponent);

  std::shared_ptr<Component> searchComponent = m_componentFactory.createSearchComponent(compositeView.get());
  searchComponent->setTabId(tabId);
  m_components.push_back(searchComponent);

  std::shared_ptr<BookmarkButtonsView> bookmarkView = m_componentFactory.getViewFactory()->createBookmarkButtonsView(
      compositeView.get());
  bookmarkView->setTabId(tabId);
  m_singleViews.push_back(bookmarkView);

  std::shared_ptr<Component> graphComponent = m_componentFactory.createGraphComponent(viewLayout);
  graphComponent->setTabId(tabId);
  m_components.push_back(graphComponent);

  std::shared_ptr<Component> codeComponent = m_componentFactory.createCodeComponent(viewLayout);
  codeComponent->setTabId(tabId);
  m_components.push_back(codeComponent);

  screenSearchSender->addResponder(graphComponent->getView<GraphView>());
  screenSearchSender->addResponder(codeComponent->getView<CodeView>());
}

void ComponentManager::teardownTab(ScreenSearchSender* screenSearchSender) {
  for(const auto& component : m_components) {
    screenSearchSender->removeResponder(component->getView<GraphView>());
    screenSearchSender->removeResponder(component->getView<CodeView>());
  }
}

void ComponentManager::clearComponents() {
  for(const std::shared_ptr<Component>& component : m_components) {
    Controller* controller = component->getController<Controller>();

    if(controller) {
      controller->clear();
    }
  }
}

void ComponentManager::refreshViews() {
  for(const std::shared_ptr<Component>& component : m_components) {
    View* view = component->getView<View>();

    if(view) {
      view->refreshView();
    }
  }

  for(const std::shared_ptr<View>& view : m_singleViews) {
    view->refreshView();
  }
}

View* ComponentManager::getView(const std::string& name) const {
  for(const std::shared_ptr<Component>& component : m_components) {
    View* view = component->getViewPtr();

    if(view != nullptr && view->getName() == name) {
      return view;
    }
  }

  for(const auto& view : m_singleViews) {
    if(view->getName() == name) {
      return view.get();
    }
  }

  return nullptr;
}

std::shared_ptr<DialogView> ComponentManager::getDialogView(DialogView::UseCase useCase) const {
  auto iterator = m_dialogViews.find(useCase);
  if(iterator == m_dialogViews.end()) {
    LOG_ERROR_STREAM(<< "No DialogView available for useCase " << int(useCase));
    return nullptr;
  }

  return iterator->second;
}
