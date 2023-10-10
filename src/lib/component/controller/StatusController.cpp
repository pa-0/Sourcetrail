#include "StatusController.h"
// internal
#include "ApplicationSettings.h"
#include "StatusView.h"
#include "utility.h"

StatusController::StatusController() {
  m_statusFilter = ApplicationSettings::getInstance()->getStatusFilter();
}

StatusController::~StatusController() = default;

StatusView* StatusController::getView() const {
  return Controller::getView<StatusView>();
}

void StatusController::clear() {}

void StatusController::handleMessage(MessageClearStatusView* /*message*/) {
  m_status.clear();
  getView()->clear();
}

void StatusController::handleMessage(MessageShowStatus* /*message*/) {
  getView()->showDockWidget();
}

void StatusController::handleMessage(MessageStatus* message) {
  if(message->status().empty()) {
    return;
  }

  std::vector<Status> stati;

  for(const std::wstring& status : message->stati()) {
    stati.emplace_back(status, message->isError);
  }

  utility::append(m_status, stati);

  addStatus(stati);
}

void StatusController::handleMessage(MessageStatusFilterChanged* message) {
  m_statusFilter = message->statusFilter;

  getView()->clear();
  addStatus(m_status);

  auto* settings = ApplicationSettings::getInstance().get();
  settings->setStatusFilter(m_statusFilter);
  settings->save();
}

void StatusController::addStatus(const std::vector<Status>& statuses) {
  std::vector<Status> filteredStatus;

  for(const Status& status : statuses) {
    if((status.type & m_statusFilter) != 0) {
      filteredStatus.push_back(status);
    }
  }

  getView()->addStatus(filteredStatus);
}