#include "MessageListenerBase.h"

MessageListenerBase::~MessageListenerBase() {
  if(m_alive) {
    m_alive = false;
    MessageQueue::getInstance()->unregisterListener(this);
  }
}

Id MessageListenerBase::s_nextId = 1;