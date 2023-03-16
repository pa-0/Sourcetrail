#pragma once
// internal
#include "Message.h"
#include "TabId.h"
#include "types.h"

class MessageCodeShowDefinition : public Message<MessageCodeShowDefinition> {
public:
  static const std::string getStaticType() {
    return "MessageCodeShowDefinition";
  }

  MessageCodeShowDefinition(Id nodeId_, bool inIDE_ = false) : nodeId(nodeId_), inIDE(inIDE_) {
    setSchedulerId(TabId::currentTab());
  }

  virtual void print(std::wostream& os) const {
    os << nodeId;
  }

  const Id nodeId;
  const bool inIDE;
};
