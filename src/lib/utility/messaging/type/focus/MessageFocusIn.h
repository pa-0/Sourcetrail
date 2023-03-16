#pragma once
// STL
#include <vector>
// internal
#include "Message.h"
#include "TabId.h"
#include "TooltipOrigin.h"
#include "types.h"

class MessageFocusIn : public Message<MessageFocusIn> {
public:
  MessageFocusIn(const std::vector<Id>& tokenIds_, TooltipOrigin origin_ = TOOLTIP_ORIGIN_NONE)
      : tokenIds(tokenIds_), origin(origin_) {
    setIsLogged(false);
    setSchedulerId(TabId::currentTab());
  }

  static const std::string getStaticType() {
    return "MessageFocusIn";
  }

  void print(std::wostream& os) const override {
    for(const Id& id : tokenIds) {
      os << id << L" ";
    }
  }

  const std::vector<Id> tokenIds;
  const TooltipOrigin origin;
};
