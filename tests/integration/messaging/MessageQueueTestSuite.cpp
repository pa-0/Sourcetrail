// STL
#include <chrono>
#include <thread>
// catch2
#include <catch2/catch_all.hpp>
// internal
#include "Message.h"
#include "MessageListener.h"
#include "MessageQueue.h"

namespace {
class TestMessage : public Message<TestMessage> {
public:
  static std::string getStaticType() {
    return "TestMessage";
  }
};

class Test2Message : public Message<Test2Message> {
public:
  static std::string getStaticType() {
    return "TestMessage2";
  }
};

class TestMessageListener : public MessageListener<TestMessage> {
public:
  TestMessageListener() = default;

  int m_messageCount = 0;

private:
  void handleMessage(TestMessage* /*message*/) override {
    m_messageCount++;
  }
};

class Test2MessageListener : public MessageListener<Test2Message> {
public:
  Test2MessageListener() = default;

  int m_messageCount = 0;

private:
  void handleMessage(Test2Message* /*message*/) override {
    m_messageCount++;
    TestMessage().dispatch();
  }
};

class Test3MessageListener : public MessageListener<Test2Message> {
public:
  std::shared_ptr<TestMessageListener> m_listener;

private:
  void handleMessage(Test2Message* /*message*/) override {
    m_listener = std::make_shared<TestMessageListener>();
  }
};

class Test4MessageListener
    : public MessageListener<TestMessage>
    , public MessageListener<Test2Message> {
public:
  std::shared_ptr<TestMessageListener> m_listener;

private:
  void handleMessage(TestMessage* /*message*/) override {
    if(!m_listener) {
      m_listener = std::make_shared<TestMessageListener>();
    }
  }

  void handleMessage(Test2Message* /*message*/) override {
    m_listener.reset();
  }
};

class Test5MessageListener : public MessageListener<TestMessage> {
public:
  std::vector<std::shared_ptr<TestMessageListener>> m_listeners;

private:
  void handleMessage(TestMessage* /*message*/) override {
    if(m_listeners.empty()) {
      for(size_t i = 0; i < 5; i++) {
        m_listeners.push_back(std::make_shared<TestMessageListener>());
      }
    }
  }
};

void waitForThread() {
  static const int THREAD_WAIT_TIME_MS = 20;
  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_WAIT_TIME_MS));
  } while(MessageQueue::getInstance()->hasMessagesQueued());
}
}    // namespace

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("message loop starts and stops", "[messaging]") {
  REQUIRE(!MessageQueue::getInstance()->loopIsRunning());

  MessageQueue::getInstance()->startMessageLoopThreaded();

  waitForThread();

  REQUIRE(MessageQueue::getInstance()->loopIsRunning());

  MessageQueue::getInstance()->stopMessageLoop();

  waitForThread();

  REQUIRE(!MessageQueue::getInstance()->loopIsRunning());
}

TEST_CASE("registered listener receives messages", "[messaging]") {
  MessageQueue::getInstance()->startMessageLoopThreaded();

  TestMessageListener listener;
  Test2MessageListener listener2;

  TestMessage().dispatch();
  TestMessage().dispatch();
  TestMessage().dispatch();

  waitForThread();

  MessageQueue::getInstance()->stopMessageLoop();

  REQUIRE(3 == listener.m_messageCount);
  REQUIRE(0 == listener2.m_messageCount);
}

TEST_CASE("message dispatching within message handling", "[messaging]") {
  MessageQueue::getInstance()->startMessageLoopThreaded();

  TestMessageListener listener;
  Test2MessageListener listener2;

  Test2Message().dispatch();

  waitForThread();

  MessageQueue::getInstance()->stopMessageLoop();

  REQUIRE(1 == listener.m_messageCount);
  REQUIRE(1 == listener2.m_messageCount);
}

TEST_CASE("listener registration within message handling", "[messaging]") {
  MessageQueue::getInstance()->startMessageLoopThreaded();

  Test3MessageListener listener;

  Test2Message().dispatch();
  TestMessage().dispatch();

  waitForThread();

  MessageQueue::getInstance()->stopMessageLoop();

  REQUIRE(listener.m_listener);
  if(listener.m_listener) {
    REQUIRE(1 == listener.m_listener->m_messageCount);
  }
}

TEST_CASE("listener unregistration within message handling", "[messaging]") {
  MessageQueue::getInstance()->startMessageLoopThreaded();

  Test4MessageListener listener;

  TestMessage().dispatch();

  Test2Message().dispatch();

  TestMessage().dispatch();
  TestMessage().dispatch();
  TestMessage().dispatch();

  waitForThread();

  MessageQueue::getInstance()->stopMessageLoop();

  REQUIRE(listener.m_listener);
  if(listener.m_listener) {
    REQUIRE(2 == listener.m_listener->m_messageCount);
  }
}

TEST_CASE("listener registration to front and back within message handling", "[messaging]") {
  MessageQueue::getInstance()->startMessageLoopThreaded();

  Test5MessageListener listener;

  TestMessage().dispatch();
  TestMessage().dispatch();
  TestMessage().dispatch();

  waitForThread();

  MessageQueue::getInstance()->stopMessageLoop();

  REQUIRE(5 == listener.m_listeners.size());
  REQUIRE(2 == listener.m_listeners[0]->m_messageCount);
  REQUIRE(2 == listener.m_listeners[1]->m_messageCount);
  REQUIRE(2 == listener.m_listeners[2]->m_messageCount);
  REQUIRE(2 == listener.m_listeners[3]->m_messageCount);
  REQUIRE(2 == listener.m_listeners[4]->m_messageCount);
}