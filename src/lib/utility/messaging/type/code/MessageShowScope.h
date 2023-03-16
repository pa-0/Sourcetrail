#pragma once

#include "Message.h"
#include "TabId.h"
#include "types.h"

class MessageShowScope : public Message<MessageShowScope> {
public:
  MessageShowScope(Id scopeLocationId_, bool showErrors_)
      : scopeLocationId(scopeLocationId_), showErrors(showErrors_) {
    setSchedulerId(TabId::currentTab());
  }

  static const std::string getStaticType() {
    return "MessageShowScope";
  }

  virtual void print(std::wostream& os) const {
    os << scopeLocationId;
  }

  const Id scopeLocationId;
  const bool showErrors;
};
