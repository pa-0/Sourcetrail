#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ComponentFactory.h"
#include "mocks/MockedViewFactory.hpp"
#include "mocks/MockedStorageAccess.hpp"
#include "ActivationController.h"

using namespace testing;

TEST(ComponentFactory, goodCase) {
  MockedViewFactory   mockedViewFactory;
  MockedStorageAccess mockedStorageAccess;

  ComponentFactory factory(&mockedViewFactory, &mockedStorageAccess);
  ASSERT_EQ(&mockedViewFactory,   factory.getViewFactory());
  ASSERT_EQ(&mockedStorageAccess, factory.getStorageAccess());

  auto activationComponent = factory.createActivationComponent();
  EXPECT_TRUE(activationComponent);

  Sequence sequence;
  EXPECT_CALL(mockedViewFactory, createBookmarkView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto bookmarkComponent = factory.createBookmarkComponent(nullptr);
  EXPECT_TRUE(bookmarkComponent);

  EXPECT_CALL(mockedViewFactory, createCodeView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto codeComponent = factory.createCodeComponent(nullptr);
  EXPECT_TRUE(codeComponent);

  EXPECT_CALL(mockedViewFactory, createCustomTrailView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto customTrailComponent = factory.createCustomTrailComponent(nullptr);
  EXPECT_TRUE(customTrailComponent);

  EXPECT_CALL(mockedViewFactory, createErrorView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto errorComponent = factory.createErrorComponent(nullptr);
  EXPECT_TRUE(errorComponent);

  EXPECT_CALL(mockedViewFactory, createGraphView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto graphComponent = factory.createGraphComponent(nullptr);
  EXPECT_TRUE(graphComponent);

  EXPECT_CALL(mockedViewFactory, createRefreshView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto refreshComponent = factory.createRefreshComponent(nullptr);
  EXPECT_TRUE(refreshComponent);

  EXPECT_CALL(mockedViewFactory, createScreenSearchView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto screenSearchComponent = factory.createScreenSearchComponent(nullptr);
  EXPECT_TRUE(screenSearchComponent);

  EXPECT_CALL(mockedViewFactory, createSearchView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto searchComponent = factory.createSearchComponent(nullptr);
  EXPECT_TRUE(searchComponent);

  EXPECT_CALL(mockedViewFactory, createStatusBarView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto statusBarComponent = factory.createStatusBarComponent(nullptr);
  EXPECT_TRUE(statusBarComponent);

  EXPECT_CALL(mockedViewFactory, createStatusView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto statusComponent = factory.createStatusComponent(nullptr);
  EXPECT_TRUE(statusComponent);

  EXPECT_CALL(mockedViewFactory, createTabsView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto tabsComponent = factory.createTabsComponent(nullptr, nullptr);
  EXPECT_TRUE(tabsComponent);

  EXPECT_CALL(mockedViewFactory, createTooltipView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto tooltipComponent = factory.createTooltipComponent(nullptr);
  EXPECT_TRUE(tooltipComponent);

  EXPECT_CALL(mockedViewFactory, createUndoRedoView(_))
    .InSequence(sequence)
    .WillOnce(Return(nullptr));
  auto undoRedoComponent = factory.createUndoRedoComponent(nullptr);
  EXPECT_TRUE(undoRedoComponent);
}
