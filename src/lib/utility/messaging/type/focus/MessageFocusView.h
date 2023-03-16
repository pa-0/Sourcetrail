#pragma once
// internal
#include "Message.h"
#include "TabId.h"

class MessageFocusView : public Message<MessageFocusView> {
public:
  enum class ViewType { GRAPH, CODE, TOGGLE };

  MessageFocusView(ViewType type_) : type(type_) {
    setIsLogged(false);
    setSchedulerId(TabId::currentTab());
  }

  static const std::string getStaticType() {
    return "MessageFocusView";
  }

  void print(std::wostream& os) const override {
    switch(type) {
    case ViewType::GRAPH:
      os << "graph";
      break;
    case ViewType::CODE:
      os << "code";
      break;
    case ViewType::TOGGLE:
      os << "toggle";
      break;
    }
  }

  const ViewType type;
};
