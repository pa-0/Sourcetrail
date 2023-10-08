#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "GraphViewStyle.h"
#include "GraphViewStyleImpl.h"
#include "NodeType.h"

using namespace testing;

struct MockedGraphViewStyleImpl : GraphViewStyleImpl {
  MOCK_METHOD(float, getCharWidth, (const std::string&, size_t), (override));
  MOCK_METHOD(float, getCharHeight, (const std::string&, size_t), (override));
  MOCK_METHOD(float, getGraphViewZoomDifferenceForPlatform, (), (override));
};

TEST(GraphViewStyle, Impl) {
  // No impl found
  ASSERT_EQ(nullptr, GraphViewStyle::getImpl());
  // Set impl
  auto graphViewStyle = std::make_shared<MockedGraphViewStyleImpl>();
  GraphViewStyle::setImpl(graphViewStyle);
  ASSERT_EQ(graphViewStyle, GraphViewStyle::getImpl());
  GraphViewStyle::setImpl(nullptr);
}

struct GraphViewStyleFix : Test {
  void SetUp() override {
    graphViewStyle = std::make_shared<MockedGraphViewStyleImpl>();
    GraphViewStyle::setImpl(graphViewStyle);
  }

  void TearDown() override {
    GraphViewStyle::setImpl(nullptr);
    graphViewStyle.reset();
  }

  std::shared_ptr<MockedGraphViewStyleImpl> graphViewStyle;
};

TEST_F(GraphViewStyleFix, loadStyleSettings) {
  EXPECT_CALL(*graphViewStyle, getGraphViewZoomDifferenceForPlatform()).WillOnce(Return(0));
  EXPECT_CALL(*graphViewStyle, getCharHeight(_, _)).WillOnce(Return(0));

  GraphViewStyle::loadStyleSettings();
  EXPECT_EQ(11, GraphViewStyle::getFontSizeForStyleType(NodeType::StyleType::STYLE_PACKAGE));
  EXPECT_EQ(12, GraphViewStyle::getFontSizeOfAccessNode());
  EXPECT_EQ(11, GraphViewStyle::getFontSizeOfExpandToggleNode());
  EXPECT_EQ(11, GraphViewStyle::getFontSizeOfCountCircle());
  EXPECT_EQ(11, GraphViewStyle::getFontSizeOfQualifier());
  EXPECT_EQ(114, GraphViewStyle::getFontSizeOfTextNode(100));
  EXPECT_EQ(12, GraphViewStyle::getFontSizeOfGroupNode());

  EXPECT_EQ("Source Code Pro, consolas, monospace, sans-serif", GraphViewStyle::getFontNameForDataNode());
  EXPECT_EQ("Fira Sans, sans-serif", GraphViewStyle::getFontNameOfAccessNode());
  EXPECT_EQ("Fira Sans, sans-serif", GraphViewStyle::getFontNameOfExpandToggleNode());
  EXPECT_EQ("Fira Sans, sans-serif", GraphViewStyle::getFontNameOfTextNode());
  EXPECT_EQ("Source Code Pro, consolas, monospace, sans-serif", GraphViewStyle::getFontNameOfGroupNode());
}

