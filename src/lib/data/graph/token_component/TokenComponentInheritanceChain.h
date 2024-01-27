#ifndef TOKEN_COMPONENT_INHERITANCE_CHAIN_H
#define TOKEN_COMPONENT_INHERITANCE_CHAIN_H
#include <vector>

#include "TokenComponent.h"
#include "types.h"

class TokenComponentInheritanceChain : public TokenComponent {
public:
  TokenComponentInheritanceChain(const std::vector<Id>& inheritanceEdgeIds_) : inheritanceEdgeIds(inheritanceEdgeIds) {}

  virtual std::shared_ptr<TokenComponent> copy() const {
    return std::make_shared<TokenComponentInheritanceChain>(*this);
  }

  const std::vector<Id> inheritanceEdgeIds;
};

#endif    // TOKEN_COMPONENT_INHERITANCE_CHAIN_H
