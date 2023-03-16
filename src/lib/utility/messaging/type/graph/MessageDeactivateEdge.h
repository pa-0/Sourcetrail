#pragma once
// internal
#include "Message.h"
#include "TabId.h"

class MessageDeactivateEdge : public Message<MessageDeactivateEdge> {
public:
  MessageDeactivateEdge(bool scrollToDefinition_) : scrollToDefinition(scrollToDefinition_) {
    setSchedulerId(TabId::currentTab());
  }

  static const std::string getStaticType() {
    return "MessageDeactivateEdge";
  }

  bool scrollToDefinition;
};
