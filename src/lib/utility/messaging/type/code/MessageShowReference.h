#pragma once
// internal
#include "Message.h"
#include "TabId.h"
#include "types.h"

class MessageShowReference : public Message<MessageShowReference> {
public:
  MessageShowReference(size_t refIndex_, Id tokenId_, Id locationId_, bool fromUser_)
      : refIndex(refIndex_), tokenId(tokenId_), locationId(locationId_), fromUser(fromUser_) {
    setSchedulerId(TabId::currentTab());
  }

  static const std::string getStaticType() {
    return "MessageShowReference";
  }

  virtual void print(std::wostream& ostream) const {
    ostream << L"index: " << refIndex << L" token: " << tokenId << L" location: " << locationId;
  }

  const size_t refIndex;
  const Id tokenId;
  const Id locationId;
  const bool fromUser;
};
