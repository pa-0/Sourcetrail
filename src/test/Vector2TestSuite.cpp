#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Vector2.h"

using namespace testing;

TEST(Vec2i, constructors) {
  Vec2i vec0;
  Vec2i vec1(1, 1);
  Vec2i vec2(vec1);

  EXPECT_EQ(0, vec0.x);
  EXPECT_EQ(0, vec0.y);

  EXPECT_EQ(1, vec1.x);
  EXPECT_EQ(1, vec1.y);

  EXPECT_EQ(1, vec2.x);
  EXPECT_EQ(1, vec2.y);
}

TEST(Vec2f, constructors) {
  Vec2f vec0;
  Vec2f vec1(1.0F, 1.0F);
  Vec2f vec2(vec1);

  EXPECT_FLOAT_EQ(0.0F, vec0.x);
  EXPECT_FLOAT_EQ(0.0F, vec0.y);

  EXPECT_FLOAT_EQ(1.0F, vec1.x);
  EXPECT_FLOAT_EQ(1.0F, vec1.y);

  EXPECT_FLOAT_EQ(1.0F, vec2.x);
  EXPECT_FLOAT_EQ(1.0F, vec2.y);
}

TEST(Vec2f, getLength) {
  Vec2f vec0(3.0f, 3.0f);
  EXPECT_FLOAT_EQ(18.0f, vec0.getLengthSquared());
  vec0.x = -3.0f;
  EXPECT_FLOAT_EQ(18.0f, vec0.getLengthSquared());
  EXPECT_FLOAT_EQ(std::sqrt(18.0f), vec0.getLength());

  Vec2i vec1(4, 2);
  EXPECT_FLOAT_EQ(20.0f, vec1.getLengthSquared());
  EXPECT_FLOAT_EQ(std::sqrt(20.0f), vec1.getLength());
}

TEST(Vec2f, normalization) {
  Vec2f vec0(1.0f, 1.0f);

  float targetXY = 1.0f / std::sqrt(2.0f);

  EXPECT_FLOAT_EQ(targetXY, vec0.normalized().x);
  EXPECT_FLOAT_EQ(targetXY, vec0.normalized().y);

  vec0.normalize();
  EXPECT_FLOAT_EQ(targetXY, vec0.x);
  EXPECT_FLOAT_EQ(targetXY, vec0.y);

  float x = 17.53f;
  float y = 42.42f;
  vec0.x = x;
  vec0.y = y;
  float targetX = x / std::sqrt(x * x + y * y);
  float targetY = y / std::sqrt(x * x + y * y);
  vec0.normalize();
  EXPECT_FLOAT_EQ(targetX, vec0.x);
  EXPECT_FLOAT_EQ(targetY, vec0.y);
}

TEST(Vector2, comparison) {
  Vec2i vec0(42, 24);
  Vec2i vec1(42, 24);
  Vec2i vec2(69, 96);

  EXPECT_TRUE(vec0.isEqual(vec1));
  EXPECT_FALSE(vec0.isEqual(vec2));
  EXPECT_TRUE(vec0.isSame(vec0));
  EXPECT_FALSE(vec0.isSame(vec1));

  Vec2f vec3(42.0f, 24.0f);
  Vec2f vec4(42.0f, 24.0f);
  Vec2f vec5(69.0f, 96.0f);

  EXPECT_TRUE(vec3.isEqual(vec4));
  EXPECT_FALSE(vec3.isEqual(vec5));
  EXPECT_TRUE(vec3.isSame(vec3));
  EXPECT_FALSE(vec3.isSame(vec4));
}

TEST(Vector2, comparisonOperators) {
  Vec2i vec0(42, 24);
  Vec2i vec1(42, 24);
  Vec2i vec2(69, 96);

  EXPECT_TRUE((vec0 == vec0));
  EXPECT_TRUE((vec0 == vec1));
  EXPECT_FALSE((vec0 == vec2));
  EXPECT_FALSE((vec0 != vec0));
  EXPECT_FALSE((vec0 != vec1));
  EXPECT_TRUE((vec0 != vec2));

  Vec2f vec3(42.0f, 24.0f);
  Vec2f vec4(42.0f, 24.0f);
  Vec2f vec5(69.0f, 96.0f);

  EXPECT_TRUE((vec3 == vec3));
  EXPECT_TRUE((vec3 == vec4));
  EXPECT_FALSE((vec3 == vec5));
  EXPECT_FALSE((vec3 != vec3));
  EXPECT_FALSE((vec3 != vec4));
  EXPECT_TRUE((vec3 != vec5));
}

TEST(Vec2i, assignmentOperator) {
  Vec2i vec0(42, 24);
  Vec2i vec1(69, 96);

  vec1 = vec0;
  EXPECT_FLOAT_EQ(42, vec1.x);
  EXPECT_FLOAT_EQ(24, vec1.y);
  EXPECT_TRUE(vec0.isEqual(vec1));
  EXPECT_FALSE(vec0.isSame(vec1));

  Vec2i vec2(42, 24);
  Vec2i vec3(69, 96);

  vec2 = vec3;
  EXPECT_FLOAT_EQ(69, vec2.x);
  EXPECT_FLOAT_EQ(96, vec2.y);
  EXPECT_TRUE(vec3.isEqual(vec2));
  EXPECT_FALSE(vec3.isSame(vec2));
}

TEST(Vec2i, additionOperators) {
  Vec2i vec0(-2, 2);
  Vec2i vec1(3, -3);
  Vec2i vec2 = vec0 + vec1;

  EXPECT_FLOAT_EQ(1, vec2.x);
  EXPECT_FLOAT_EQ(-1, vec2.y);

  EXPECT_FLOAT_EQ(-2, vec0.x);
  EXPECT_FLOAT_EQ(2, vec0.y);
  EXPECT_FLOAT_EQ(3, vec1.x);
  EXPECT_FLOAT_EQ(-3, vec1.y);

  vec0 += vec1;
  EXPECT_FLOAT_EQ(1, vec0.x);
  EXPECT_FLOAT_EQ(-1, vec0.y);
}

TEST(Vec2f, subtractionOperators) {
  Vec2f vec0(-2.0f, 2.0f);
  Vec2f vec1(3.0f, -3.0f);
  Vec2f vec2 = vec0 - vec1;

  EXPECT_FLOAT_EQ(-5.0f, vec2.x);
  EXPECT_FLOAT_EQ(5.0f, vec2.y);

  EXPECT_FLOAT_EQ(-2.0f, vec0.x);
  EXPECT_FLOAT_EQ(2.0f, vec0.y);
  EXPECT_FLOAT_EQ(3.0f, vec1.x);
  EXPECT_FLOAT_EQ(-3.0f, vec1.y);

  vec0 -= vec1;
  EXPECT_FLOAT_EQ(-5.0f, vec0.x);
  EXPECT_FLOAT_EQ(5.0f, vec0.y);
}

TEST(Vec2f, scalarMultiplicationOperators) {
  Vec2f vec0(-2.0f, 2.0f);
  Vec2f vec1 = vec0 * 42.0f;

  EXPECT_FLOAT_EQ(-84.0f, vec1.x);
  EXPECT_FLOAT_EQ(84.0f, vec1.y);

  EXPECT_FLOAT_EQ(-2.0f, vec0.x);
  EXPECT_FLOAT_EQ(2.0f, vec0.y);

  vec0 *= 42.0f;

  EXPECT_FLOAT_EQ(-84.0f, vec0.x);
  EXPECT_FLOAT_EQ(84.0f, vec0.y);
#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable : 4244)
#endif
  Vec2i vec2(-2, 2);
  Vec2i vec3 = vec2 * 42.4f;
#ifdef _WIN32
#  pragma warning(pop)
#endif

  EXPECT_FLOAT_EQ(-84, (int)vec3.x);
  EXPECT_FLOAT_EQ(84, (int)vec3.y);

  Vec2i vec3b = vec2 * 42.5f;

  EXPECT_FLOAT_EQ(-85, (int)vec3b.x);
  EXPECT_FLOAT_EQ(85, (int)vec3b.y);

  EXPECT_FLOAT_EQ(-2, vec2.x);
  EXPECT_FLOAT_EQ(2, vec2.y);

  vec2 *= 42;
  EXPECT_FLOAT_EQ(-84, vec2.x);
  EXPECT_FLOAT_EQ(84, vec2.y);

  vec2 *= 0.5f;
  EXPECT_FLOAT_EQ(-42, vec2.x);
  EXPECT_FLOAT_EQ(42, vec2.y);
}

TEST(Vec2f, dotProductOperator) {
  Vec2f vec0(2.0f, 4.0f);
  Vec2f vec1(3.0f, 6.0f);
  Vec2f vec2(-2.0f, -1.0f);

  EXPECT_FLOAT_EQ(30.0f, vec0.dotProduct(vec1));
  EXPECT_FLOAT_EQ(-8.0f, vec0.dotProduct(vec2));

  Vec2f vec3(2, 4);
  Vec2f vec4(3, 6);
  Vec2f vec5(-2, -1);

  EXPECT_FLOAT_EQ(30, vec3.dotProduct(vec4));
  EXPECT_FLOAT_EQ(-8, vec3.dotProduct(vec5));
}

TEST(Vec2f, scalarDivisionOperators) {
  Vec2f vec0(42.0f, 24.0f);
  Vec2f vec1 = vec0 / 2.0f;
  Vec2f vec2 = vec0 / 0.5f;

  EXPECT_FLOAT_EQ(42.0f, vec0.x);
  EXPECT_FLOAT_EQ(24.0f, vec0.y);
  EXPECT_FLOAT_EQ(21.0f, vec1.x);
  EXPECT_FLOAT_EQ(12.0f, vec1.y);
  EXPECT_FLOAT_EQ(84.0f, vec2.x);
  EXPECT_FLOAT_EQ(48.0f, vec2.y);

  vec0 /= 2.0f;
  EXPECT_FLOAT_EQ(21.0f, vec0.x);
  EXPECT_FLOAT_EQ(12.0f, vec0.y);

  Vec2i vec3(42, 24);
  Vec2i vec4 = vec3 / 2;

  EXPECT_FLOAT_EQ(42, vec3.x);
  EXPECT_FLOAT_EQ(24, vec3.y);
  EXPECT_FLOAT_EQ(21, vec4.x);
  EXPECT_FLOAT_EQ(12, vec4.y);

  vec3 /= 2;
  EXPECT_FLOAT_EQ(21, vec3.x);
  EXPECT_FLOAT_EQ(12, vec3.y);
}

TEST(Vec2i, getValue) {
  Vec2i vec(1, 2);

  EXPECT_EQ(1, vec.getValue(0));
  EXPECT_EQ(2, vec.getValue(1));
}

TEST(Vec2i, setValue) {
  Vec2i vec;
  vec.setValue(0, 1);
  vec.setValue(1, 2);

  EXPECT_EQ(1, vec.getValue(0));
  EXPECT_EQ(2, vec.getValue(1));
}

TEST(Vec2i, toWString) {
  const Vec2i vec(1, 2);

  EXPECT_THAT(vec.toWString(), StrEq(L"[1, 2]"));
}