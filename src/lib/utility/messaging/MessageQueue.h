#pragma once
#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

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

  void registerListener(MessageListenerBase* listener);
  void unregisterListener(MessageListenerBase* listener);

  [[nodiscard]] MessageListenerBase* getListenerById(Id listenerId) const;

  void addMessageFilter(std::shared_ptr<MessageFilter> filter);

  void pushMessage(std::shared_ptr<MessageBase> message);
  void processMessage(const std::shared_ptr<MessageBase>&, bool asNextTask);

  void startMessageLoopThreaded();
  void startMessageLoop();
  void stopMessageLoop();

  bool loopIsRunning() const;
  bool hasMessagesQueued() const;

  void setSendMessagesAsTasks(bool sendMessagesAsTasks);

private:
  static Ptr sInstance;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  MessageQueue();

  void processMessages();
  void sendMessage(const std::shared_ptr<MessageBase>& message);
  void sendMessageAsTask(const std::shared_ptr<MessageBase>& message, bool asNextTask) const;

  MessageBufferType mMessageBuffer;
  std::vector<MessageListenerBase*> mListeners;
  std::vector<std::shared_ptr<MessageFilter>> mFilters;

  size_t mCurrentListenerIndex = 0;
  size_t mListenersLength = 0;

  std::atomic_bool mLoopIsRunning = false;
  std::atomic_bool mThreadIsRunning = false;

  mutable std::mutex mMessageBufferMutex;
  mutable std::mutex mListenersMutex;

  bool mSendMessagesAsTasks = false;
};