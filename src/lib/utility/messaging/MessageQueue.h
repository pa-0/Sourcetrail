#pragma once
// STL
#include <deque>
#include <memory>
#include <mutex>
#include <vector>
// internal
#include "types.h"

class MessageBase;
class MessageFilter;
class MessageListenerBase;

class MessageQueue final {
public:
  using Ptr = std::shared_ptr<MessageQueue>;
  using MessageBufferType = std::deque<std::shared_ptr<MessageBase>>;

  static Ptr getInstance();

  MessageQueue(const MessageQueue&) = delete;
  MessageQueue(MessageQueue&&) = delete;
  MessageQueue& operator=(const MessageQueue&) = delete;
  MessageQueue& operator=(MessageQueue&&) = delete;

  ~MessageQueue();

  void registerListener(MessageListenerBase* pListener);
  void unregisterListener(MessageListenerBase* listener);

  MessageListenerBase* getListenerById(Id listenerId) const;

  void addMessageFilter(std::shared_ptr<MessageFilter> pFilter);

  void pushMessage(std::shared_ptr<MessageBase> pMessage);
  void processMessage(const std::shared_ptr<MessageBase>&, bool asNextTask);

  void startMessageLoopThreaded();
  void startMessageLoop();
  void stopMessageLoop();

  bool loopIsRunning() const;
  bool hasMessagesQueued() const;

  void setSendMessagesAsTasks(bool sendMessagesAsTasks);

private:
  static Ptr s_instance;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  MessageQueue();

  void processMessages();
  void sendMessage(const std::shared_ptr<MessageBase>& pMessage);
  void sendMessageAsTask(const std::shared_ptr<MessageBase>& pMessage, bool asNextTask) const;

  MessageBufferType m_messageBuffer;
  std::vector<MessageListenerBase*> m_listeners;
  std::vector<std::shared_ptr<MessageFilter>> m_filters;

  size_t m_currentListenerIndex;
  size_t m_listenersLength;

  bool m_loopIsRunning;
  bool m_threadIsRunning;

  mutable std::mutex m_messageBufferMutex;
  mutable std::mutex m_listenersMutex;
  mutable std::mutex m_loopMutex;
  mutable std::mutex m_threadMutex;

  bool m_sendMessagesAsTasks;
};