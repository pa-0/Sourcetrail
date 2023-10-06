#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Tab.h"
#include "mocks/MockedBookmarkButtonsView.hpp"
#include "mocks/MockedCodeView.hpp"
#include "mocks/MockedGraphView.hpp"
#include "mocks/MockedRefreshView.hpp"
#include "mocks/MockedScreenSearchSender.hpp"
#include "mocks/MockedSearchView.hpp"
#include "mocks/MockedUndoRedoView.hpp"
#include "mocks/MockedViewFactory.hpp"
#include "mocks/MockedViewLayout.hpp"
#include "mocks/MockedCompositeView.hpp"

using namespace testing;

struct TabIdFix : Test {
  void SetUp() override {
    auto mockedCompositeView = std::make_shared<MockedCompositeView>(&mockedLayout);
    EXPECT_CALL(viewFactory, createCompositeView(_, _, _, _)).WillOnce(Return(mockedCompositeView));
    auto undoRedoView = std::make_shared<MockedUndoRedoView>(&mockedLayout);
    EXPECT_CALL(viewFactory, createUndoRedoView(_)).WillOnce(Return(undoRedoView));
    auto refreshView = std::make_shared<MockedRefreshView>(&mockedLayout);
    EXPECT_CALL(viewFactory, createRefreshView(_)).WillOnce(Return(refreshView));
    auto searchView = std::make_shared<MockedSearchView>(&mockedLayout);
    EXPECT_CALL(viewFactory, createSearchView(_)).WillOnce(Return(searchView));

    auto bookmarkButtonsView = std::make_shared<MockedBookmarkButtonsView>(&mockedLayout);
    EXPECT_CALL(viewFactory, createBookmarkButtonsView(_)).WillOnce(Return(bookmarkButtonsView));

    EXPECT_CALL(screenSearchSender, addResponder(_)).Times(2);

    auto graphView = std::make_shared<MockedGraphView>(&mockedLayout);
    EXPECT_CALL(viewFactory, createGraphView(_)).WillOnce(Return(graphView));
    auto codeView = std::make_shared<MockedCodeView>(&mockedLayout);
    EXPECT_CALL(viewFactory, createCodeView(_)).WillOnce(Return(codeView));
  }

  StrictMock<MockedViewFactory> viewFactory;
  MockedScreenSearchSender screenSearchSender;
  MockedViewLayout mockedLayout;
};

// NOLINTNEXTLINE
TEST_F(TabIdFix, goodCase) {
  Tab tab(0, &viewFactory, nullptr, &screenSearchSender);
}