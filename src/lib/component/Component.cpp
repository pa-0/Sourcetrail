#include "Component.h"

#include <utility>

#include "Controller.h"
#include "View.h"

Component::Component(std::shared_ptr<View> view, std::shared_ptr<Controller> controller)
    : m_controller(std::move(controller)), m_view(std::move(view)), m_tabId(0) {
  if(m_controller) {
    m_controller->setComponent(this);
  }

  if(m_view) {
    m_view->setComponent(this);
  }
}

Component::~Component() {
  if(m_controller) {
    m_controller->setComponent(nullptr);
  }

  if(m_view) {
    m_view->setComponent(nullptr);
  }
}

Controller* Component::getControllerPtr() const {
  return m_controller.get();
}

View* Component::getViewPtr() const {
  return m_view.get();
}