#include "MessageQueue.h"
// STL
#include <chrono>
#include <mutex>
#include <thread>
// internal
#include "MessageBase.h"
#include "MessageFilter.h"
#include "MessageListenerBase.h"
#include "TabId.h"
#include "TaskGroupParallel.h"
#include "TaskGroupSequence.h"
#include "TaskLambda.h"
#include "logging.h"

MessageQueue::Ptr MessageQueue::getInstance() {
  if(!s_instance) {
    s_instance = std::shared_ptr<MessageQueue>(new MessageQueue());
  }
  return s_instance;
}

MessageQueue::~MessageQueue() {
  std::scoped_lock<std::mutex> lock(m_listenersMutex);
  for(auto& m_listener : m_listeners) {
    m_listener->removedListener();
  }
  m_listeners.clear();
}

void MessageQueue::registerListener(MessageListenerBase* pListener) {
  std::scoped_lock<std::mutex> lock(m_listenersMutex);
  m_listeners.push_back(pListener);
}

void MessageQueue::unregisterListener(MessageListenerBase* pListener) {
  std::scoped_lock<std::mutex> lock(m_listenersMutex);
  for(size_t i = 0; i < m_listeners.size(); i++) {
    if(m_listeners[i] == pListener) {
      m_listeners.erase(m_listeners.begin() + static_cast<long>(i));

      // m_currentListenerIndex and m_listenersLength need to be updated in case this happens
      // while a message is handled.
      if(i <= m_currentListenerIndex) {
        m_currentListenerIndex--;
      }

      if(i < m_listenersLength) {
        m_listenersLength--;
      }

      return;
    }
  }

  LOG_ERROR("Listener was not found");
}

MessageListenerBase* MessageQueue::getListenerById(Id listenerId) const {
  std::scoped_lock<std::mutex> lock(m_listenersMutex);
  for(auto* m_listener : m_listeners) {
    if(m_listener->getId() == listenerId) {
      return m_listener;
    }
  }
  return nullptr;
}

void MessageQueue::addMessageFilter(std::shared_ptr<MessageFilter> pFilter) {
  m_filters.push_back(std::move(pFilter));
}

void MessageQueue::pushMessage(std::shared_ptr<MessageBase> pMessage) {
  std::scoped_lock<std::mutex> lock(m_messageBufferMutex);
  m_messageBuffer.push_back(std::move(pMessage));
}

void MessageQueue::processMessage(const std::shared_ptr<MessageBase>& pMessage, bool asNextTask) {
  if(pMessage->isLogged()) {
    LOG_INFO_BARE(L"send " + pMessage->str());
  }

  if(m_sendMessagesAsTasks && pMessage->sendAsTask()) {
    sendMessageAsTask(pMessage, asNextTask);
    return;
  }

  sendMessage(pMessage);
}

void MessageQueue::startMessageLoopThreaded() {
  std::thread(&MessageQueue::startMessageLoop, this).detach();

  std::scoped_lock<std::mutex> lock(m_threadMutex);
  m_threadIsRunning = true;
}

void MessageQueue::startMessageLoop() {
  {
    std::scoped_lock<std::mutex> lock(m_loopMutex);

    if(m_loopIsRunning) {
      LOG_ERROR("Loop is already running");
      return;
    }

    m_loopIsRunning = true;
  }

  while(true) {
    processMessages();

    {
      std::scoped_lock<std::mutex> lock(m_loopMutex);

      if(!m_loopIsRunning) {
        break;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(25));
  }

  {
    std::scoped_lock<std::mutex> lock(m_threadMutex);
    if(m_threadIsRunning) {
      m_threadIsRunning = false;
    }
  }
}

void MessageQueue::stopMessageLoop() {
  {
    std::scoped_lock<std::mutex> lock(m_loopMutex);

    if(!m_loopIsRunning) {
      LOG_WARNING("Loop is not running");
    }

    m_loopIsRunning = false;
  }

  while(true) {
    {
      std::scoped_lock<std::mutex> lock(m_threadMutex);
      if(!m_threadIsRunning) {
        break;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(25));
  }
}

bool MessageQueue::loopIsRunning() const {
  std::scoped_lock<std::mutex> lock(m_loopMutex);
  return m_loopIsRunning;
}

bool MessageQueue::hasMessagesQueued() const {
  std::scoped_lock<std::mutex> lock(m_messageBufferMutex);
  return !m_messageBuffer.empty();
}

void MessageQueue::setSendMessagesAsTasks(bool sendMessagesAsTasks) {
  m_sendMessagesAsTasks = sendMessagesAsTasks;
}

MessageQueue::Ptr MessageQueue::s_instance; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MessageQueue::MessageQueue()
    : m_currentListenerIndex(0)
    , m_listenersLength(0)
    , m_loopIsRunning(false)
    , m_threadIsRunning(false)
    , m_sendMessagesAsTasks(false) {}

void MessageQueue::processMessages() {
  while(true) {
    std::shared_ptr<MessageBase> message;
    {
      std::scoped_lock<std::mutex> lock(m_messageBufferMutex);

      for(const auto& filter : m_filters) {
        if(m_messageBuffer.empty()) {
          break;
        }

        filter->filter(&m_messageBuffer);
      }

      if(m_messageBuffer.empty()) {
        break;
      }

      message = m_messageBuffer.front();
      m_messageBuffer.pop_front();
    }

    processMessage(message, false);
  }
}

void MessageQueue::sendMessage(const std::shared_ptr<MessageBase>& pMessage) {
  std::scoped_lock<std::mutex> lock(m_listenersMutex);

  // m_listenersLength is saved, so that new listeners registered within message handling don't
  // get the current message and the length can be reduced when a listener gets unregistered.
  m_listenersLength = m_listeners.size();

  // The currentListenerIndex holds the index of the current listener being handled, so it can be
  // changed when a listener gets removed while message handling.
  for(m_currentListenerIndex = 0; m_currentListenerIndex < m_listenersLength;
      m_currentListenerIndex++) {
    MessageListenerBase* listener = m_listeners[m_currentListenerIndex];

    if(listener->getType() == pMessage->getType() &&
       (pMessage->getSchedulerId() == 0 || listener->getSchedulerId() == 0 ||
        listener->getSchedulerId() == pMessage->getSchedulerId())) {
      // The listenersMutex gets unlocked so changes to listeners are possible while message handling.
      m_listenersMutex.unlock();
      listener->handleMessageBase(pMessage.get());
      m_listenersMutex.lock();
    }
  }
}

void MessageQueue::sendMessageAsTask(const std::shared_ptr<MessageBase>& pMessage, bool asNextTask) const {
  std::shared_ptr<TaskGroup> taskGroup;
  if(pMessage->isParallel()) {
    taskGroup = std::make_shared<TaskGroupParallel>();
  } else {
    taskGroup = std::make_shared<TaskGroupSequence>();
  }

  {
    std::scoped_lock<std::mutex> lock(m_listenersMutex);
    for(auto pListener : m_listeners) {
      if(pListener->getType() == pMessage->getType() &&
         (pMessage->getSchedulerId() == 0 || pListener->getSchedulerId() == 0 ||
          pListener->getSchedulerId() == pMessage->getSchedulerId())) {
        Id listenerId = pListener->getId();
        taskGroup->addTask(std::make_shared<TaskLambda>([listenerId, pMessage]() {
          auto* pInnerListener = MessageQueue::getInstance()->getListenerById(listenerId);
          if(pInnerListener != nullptr) {
            pInnerListener->handleMessageBase(pMessage.get());
          }
        }));
      }
    }
  }

  Id schedulerId = pMessage->getSchedulerId();
  if(schedulerId == 0U) {
    schedulerId = TabId::app();
  }

  if(asNextTask) {
    Task::dispatchNext(schedulerId, taskGroup);
  } else {
    Task::dispatch(schedulerId, taskGroup);
  }
}