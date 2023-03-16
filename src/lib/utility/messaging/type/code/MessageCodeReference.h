#pragma once

#include "Message.h"
#include "TabId.h"

class MessageCodeReference : public Message<MessageCodeReference> {
public:
  enum ReferenceType { REFERENCE_PREVIOUS, REFERENCE_NEXT };

  MessageCodeReference(ReferenceType type_, bool localReference_)
      : type(type_), localReference(localReference_) {
    setSchedulerId(TabId::currentTab());
  }

  static const std::string getStaticType() {
    return "MessageCodeReference";
  }

  virtual void print(std::wostream& ostream) const {
    if(type == REFERENCE_PREVIOUS) {
      ostream << L"previous";
    } else {
      ostream << L"next";
    }

    if(localReference) {
      ostream << L" local";
    }
  }

  const ReferenceType type;
  bool localReference;
};
