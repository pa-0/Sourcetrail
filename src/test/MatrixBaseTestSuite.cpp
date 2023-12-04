#include <gtest/gtest.h>

#include "MatrixBase.h"
#include "VectorBase.h"
namespace {
/**
 * C++ functions can't return statically allocated arrays.
 * I don't want to use dynamically allocated arrays, so here's my work around for that...
 *
 * Update: actually they can... see MatrixBase [] operator (in MatrixBase.cpp)
 */
template <class T>
struct Array3x5 {
  T array[3][5];
};

template <class T>
struct Array5x3 {
  T array[5][3];
};

Array3x5<int> getTestValues3x5() {
  Array3x5<int> result;

  for(unsigned int i = 0; i < 3; i++) {
    for(unsigned int j = 0; j < 5; j++) {
      result.array[i][j] = i + j;
    }
  }

  return result;
}

Array3x5<int> getTestValues3x5_b() {
  Array3x5<int> result;

  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 5; j++) {
      result.array[i][j] = -i - j;
    }
  }

  return result;
}

Array5x3<int> getTestValues5x3() {
  Array5x3<int> result;

  for(unsigned int i = 0; i < 5; i++) {
    for(unsigned int j = 0; j < 3; j++) {
      result.array[i][j] = i + j;
    }
  }

  return result;
}

MatrixBase<int, 3, 5> getTestMatrix3x5() {
  Array3x5<int> testValues = getTestValues3x5();

  return MatrixBase<int, 3, 5>(testValues.array);
}

MatrixBase<int, 3, 5> getTestMatrix3x5_b() {
  Array3x5<int> testValues = getTestValues3x5_b();

  return MatrixBase<int, 3, 5>(testValues.array);
}

MatrixBase<int, 5, 3> getTestMatrix5x3() {
  Array5x3<int> testValues = getTestValues5x3();

  return MatrixBase<int, 5, 3>(testValues.array);
}
}    // namespace

TEST(MatrixBase, constructors) {
  MatrixBase<int, 4, 5> matrix0;

  EXPECT_TRUE(4 == matrix0.getColumnsCount());
  EXPECT_TRUE(5 == matrix0.getRowsCount());

  Array3x5<int> testValues = getTestValues3x5();

  MatrixBase<int, 3, 5> matrix1(testValues.array);

  EXPECT_TRUE(3 == matrix1.getColumnsCount());
  EXPECT_TRUE(5 == matrix1.getRowsCount());

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(6 == matrix1.getValue(2, 4));

  MatrixBase<int, 3, 5> matrix2(matrix1);

  EXPECT_TRUE(0 == matrix2.getValue(0, 0));
  EXPECT_TRUE(6 == matrix2.getValue(2, 4));
}

TEST(MatrixBase, getSetValue) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();

  int value2_2 = matrix0.getValue(2, 2);
  matrix0.setValue(2, 2, value2_2 * 2);
  EXPECT_TRUE(value2_2 * 2 == matrix0.getValue(2, 2));
  EXPECT_TRUE(3 == matrix0.getValue(1, 2));
  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
}

TEST(MatrixBase, getRowsColumnsCount) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 5, 3> matrix1 = getTestMatrix5x3();

  EXPECT_TRUE(3 == matrix0.getColumnsCount());
  EXPECT_TRUE(5 == matrix0.getRowsCount());

  EXPECT_TRUE(5 == matrix1.getColumnsCount());
  EXPECT_TRUE(3 == matrix1.getRowsCount());
}

TEST(MatrixBase, transposed) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  EXPECT_TRUE(3 == matrix0.getColumnsCount());
  EXPECT_TRUE(5 == matrix0.getRowsCount());

  MatrixBase<int, 5, 3> matrix1 = matrix0.transposed();
  EXPECT_TRUE(5 == matrix1.getColumnsCount());
  EXPECT_TRUE(3 == matrix1.getRowsCount());

  EXPECT_TRUE(matrix0.getValue(0, 0) == matrix1.getValue(0, 0));
  EXPECT_TRUE(matrix0.getValue(0, 1) == matrix1.getValue(1, 0));
  EXPECT_TRUE(matrix0.getValue(0, 4) == matrix1.getValue(4, 0));
  EXPECT_TRUE(matrix0.getValue(1, 4) == matrix1.getValue(4, 1));
  EXPECT_TRUE(matrix0.getValue(2, 4) == matrix1.getValue(4, 2));
  EXPECT_TRUE(matrix0.getValue(0, 3) == matrix1.getValue(3, 0));
  EXPECT_TRUE(matrix0.getValue(1, 3) == matrix1.getValue(3, 1));
  EXPECT_TRUE(matrix0.getValue(2, 3) == matrix1.getValue(3, 2));
}

TEST(MatrixBase, assign) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix1 = getTestMatrix3x5_b();

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(2 == matrix0.getValue(1, 1));
  EXPECT_TRUE(4 == matrix0.getValue(2, 2));

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(-2 == matrix1.getValue(1, 1));
  EXPECT_TRUE(-4 == matrix1.getValue(2, 2));

  matrix0.assign(matrix1);

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(-2 == matrix0.getValue(1, 1));
  EXPECT_TRUE(-4 == matrix0.getValue(2, 2));
}

TEST(MatrixBase, addSubtract) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix1 = getTestMatrix3x5_b();

  matrix0.add(matrix1);

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(0 == matrix0.getValue(1, 1));
  EXPECT_TRUE(0 == matrix0.getValue(2, 2));

  matrix0.subtract(matrix1);

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(2 == matrix0.getValue(1, 1));
  EXPECT_TRUE(4 == matrix0.getValue(2, 2));
}

TEST(MatrixBase, multiplyDivideScalar) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable : 4244)
#endif
  matrix0.scalarMultiplication(2.0f);    // float is on porpoise (so is porpoise, womp womp)

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(4 == matrix0.getValue(1, 1));
  EXPECT_TRUE(8 == matrix0.getValue(2, 2));

  matrix0.scalarMultiplication(0.5f);

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(2 == matrix0.getValue(1, 1));
  EXPECT_TRUE(4 == matrix0.getValue(2, 2));

  matrix0.scalarMultiplication(0.5f);

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(1 == matrix0.getValue(1, 1));
  EXPECT_TRUE(2 == matrix0.getValue(2, 2));

  matrix0.scalarMultiplication(0.5f);

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(0 == matrix0.getValue(1, 1));
  EXPECT_TRUE(1 == matrix0.getValue(2, 2));
#ifdef _WIN32
#  pragma warning(pop)
#endif
}

TEST(MatrixBase, multiplyMatrix) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix1 = getTestMatrix3x5_b();
  MatrixBase<int, 5, 3> matrix1t = matrix1.transposed();

  MatrixBase<int, 5, 5> matrix2 = matrix0.matrixMultiplication(matrix1t);
  MatrixBase<int, 3, 3> matrix3 = matrix1t.matrixMultiplication(matrix0);

  // expected results
  // matrix0 * matrix1t
  /**
   * -5, -8, -11, -14, -17
   * -8, -14, -20, -26, -32
   * -11, -20, -29, -38, -47
   * -14, -26, -38, -50, -62
   * -17, -32, -47, -62, -77
   */

  // matrix1t * matrix0
  /**
   * -30, -40, -50
   * -40, -55, -70
   * -50, -70, -90
   */


  EXPECT_TRUE(5 == matrix2.getColumnsCount());
  EXPECT_TRUE(5 == matrix2.getRowsCount());

  EXPECT_TRUE(3 == matrix3.getColumnsCount());
  EXPECT_TRUE(3 == matrix3.getRowsCount());

  EXPECT_TRUE(-5 == matrix2.getValue(0, 0));
  EXPECT_TRUE(-77 == matrix2.getValue(4, 4));
  EXPECT_TRUE(-29 == matrix2.getValue(2, 2));
  EXPECT_TRUE(-11 == matrix2.getValue(2, 0));
  EXPECT_TRUE(-11 == matrix2.getValue(0, 2));
  EXPECT_TRUE(-38 == matrix2.getValue(3, 2));


  EXPECT_TRUE(-30 == matrix3.getValue(0, 0));
  EXPECT_TRUE(-90 == matrix3.getValue(2, 2));
  EXPECT_TRUE(-50 == matrix3.getValue(2, 0));
  EXPECT_TRUE(-50 == matrix3.getValue(0, 2));
  EXPECT_TRUE(-55 == matrix3.getValue(1, 1));
}

TEST(MatrixBase, isEqual) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix0_b = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix1 = getTestMatrix3x5_b();

  EXPECT_TRUE(true == matrix0.isEqual(matrix0_b));
  EXPECT_TRUE(false == matrix0.isEqual(matrix1));
  EXPECT_TRUE(true == matrix0.isEqual(matrix0));
}

TEST(MatrixBase, isSame) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix0_b = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix1 = getTestMatrix3x5_b();

  EXPECT_TRUE(false == matrix0.isSame(matrix0_b));
  EXPECT_TRUE(false == matrix0.isSame(matrix1));
  EXPECT_TRUE(true == matrix0.isSame(matrix0));
}

TEST(MatrixBase, accessOperator) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();

  EXPECT_TRUE(0 == matrix0[0][0]);
  EXPECT_TRUE(4 == matrix0[2][2]);
  EXPECT_TRUE(6 == matrix0[2][4]);

  matrix0[0][0] = 42;

  EXPECT_TRUE(42 == matrix0[0][0]);
  EXPECT_TRUE(4 == matrix0[2][2]);
  EXPECT_TRUE(6 == matrix0[2][4]);
}

TEST(MatrixBase, operators) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix0_b = getTestMatrix3x5_b();

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(4 == matrix0.getValue(2, 2));
  EXPECT_TRUE(6 == matrix0.getValue(2, 4));

  EXPECT_TRUE(0 == matrix0_b.getValue(0, 0));
  EXPECT_TRUE(-4 == matrix0_b.getValue(2, 2));
  EXPECT_TRUE(-6 == matrix0_b.getValue(2, 4));

  MatrixBase<int, 3, 5> matrix1 = matrix0 + matrix0_b;
  MatrixBase<int, 3, 5> matrix2 = matrix0 - matrix0_b;

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(0 == matrix1.getValue(2, 2));
  EXPECT_TRUE(0 == matrix1.getValue(2, 4));

  EXPECT_TRUE(0 == matrix2.getValue(0, 0));
  EXPECT_TRUE(8 == matrix2.getValue(2, 2));
  EXPECT_TRUE(12 == matrix2.getValue(2, 4));

  MatrixBase<int, 3, 5> matrix3 = matrix0 * 3;
  MatrixBase<int, 3, 5> matrix4 = matrix0 / 2;
  MatrixBase<int, 3, 5> matrix5 = matrix0 * 3.3f;    // float is on purpose

  EXPECT_TRUE(0 == matrix3.getValue(0, 0));
  EXPECT_TRUE(12 == matrix3.getValue(2, 2));
  EXPECT_TRUE(18 == matrix3.getValue(2, 4));

  EXPECT_TRUE(0 == matrix4.getValue(0, 0));
  EXPECT_TRUE(2 == matrix4.getValue(2, 2));
  EXPECT_TRUE(3 == matrix4.getValue(2, 4));

  EXPECT_TRUE(0 == matrix5.getValue(0, 0));
  EXPECT_TRUE(13 == matrix5.getValue(2, 2));
  EXPECT_TRUE(19 == matrix5.getValue(2, 4));
}

TEST(MatrixBase, assignOperators) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix0_b = getTestMatrix3x5_b();

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(4 == matrix0.getValue(2, 2));
  EXPECT_TRUE(6 == matrix0.getValue(2, 4));

  EXPECT_TRUE(0 == matrix0_b.getValue(0, 0));
  EXPECT_TRUE(-4 == matrix0_b.getValue(2, 2));
  EXPECT_TRUE(-6 == matrix0_b.getValue(2, 4));

  MatrixBase<int, 3, 5> matrix1 = getTestMatrix3x5();
  matrix1 += matrix0;

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(8 == matrix1.getValue(2, 2));
  EXPECT_TRUE(12 == matrix1.getValue(2, 4));

  matrix1 += matrix0_b;

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(4 == matrix1.getValue(2, 2));
  EXPECT_TRUE(6 == matrix1.getValue(2, 4));

  matrix1 -= matrix0_b;

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(8 == matrix1.getValue(2, 2));
  EXPECT_TRUE(12 == matrix1.getValue(2, 4));

  matrix1 *= 3.3f;

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(26 == matrix1.getValue(2, 2));
  EXPECT_TRUE(39 == matrix1.getValue(2, 4));

  matrix1 /= 3;

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(8 == matrix1.getValue(2, 2));
  EXPECT_TRUE(13 == matrix1.getValue(2, 4));
}

TEST(MatrixBase, comparisonOperators) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  MatrixBase<int, 3, 5> matrix0_b = getTestMatrix3x5_b();
  MatrixBase<int, 3, 5> matrix1 = getTestMatrix3x5();

  EXPECT_TRUE(0 == matrix0.getValue(0, 0));
  EXPECT_TRUE(4 == matrix0.getValue(2, 2));
  EXPECT_TRUE(6 == matrix0.getValue(2, 4));

  EXPECT_TRUE(0 == matrix0_b.getValue(0, 0));
  EXPECT_TRUE(-4 == matrix0_b.getValue(2, 2));
  EXPECT_TRUE(-6 == matrix0_b.getValue(2, 4));

  EXPECT_TRUE(0 == matrix1.getValue(0, 0));
  EXPECT_TRUE(4 == matrix1.getValue(2, 2));
  EXPECT_TRUE(6 == matrix1.getValue(2, 4));

  EXPECT_TRUE(true == (matrix0 == matrix0));
  EXPECT_TRUE(true == (matrix0 == matrix1));
  EXPECT_TRUE(true == (matrix0 != matrix0_b));

  EXPECT_TRUE(false == (matrix0 != matrix0));
  EXPECT_TRUE(false == (matrix0 != matrix1));
  EXPECT_TRUE(false == (matrix0 == matrix0_b));
}

TEST(MatrixBase, vectorMultiplication) {
  MatrixBase<int, 3, 5> matrix0 = getTestMatrix3x5();
  VectorBase<int, 3> vector0;

  for(unsigned int i = 0; i < vector0.getDimensions(); i++) {
    vector0.setValue(i, i + 1);
  }

  VectorBase<int, 5> vector0_r = multiply(matrix0, vector0);

  EXPECT_TRUE(8 == vector0_r[0]);
  EXPECT_TRUE(14 == vector0_r[1]);
  EXPECT_TRUE(20 == vector0_r[2]);
  EXPECT_TRUE(26 == vector0_r[3]);
  EXPECT_TRUE(32 == vector0_r[4]);

  MatrixBase<int, 5, 3> matrix1 = getTestMatrix5x3();
  VectorBase<int, 3> vector1;

  for(unsigned int i = 0; i < vector1.getDimensions(); i++) {
    vector1.setValue(i, i + 1);
  }

  VectorBase<int, 5> vector1_r = multiply(vector1, matrix1);

  EXPECT_TRUE(8 == vector1_r[0]);
  EXPECT_TRUE(14 == vector1_r[1]);
  EXPECT_TRUE(20 == vector1_r[2]);
  EXPECT_TRUE(26 == vector1_r[3]);
  EXPECT_TRUE(32 == vector1_r[4]);

  MatrixBase<int, 3, 5> matrix2 = getTestMatrix3x5();
  VectorBase<int, 5> vector2;

  for(unsigned int i = 0; i < vector2.getDimensions(); i++) {
    vector2.setValue(i, i + 1);
  }

  VectorBase<int, 3> vector2_r = multiply(vector2, matrix2);

  EXPECT_TRUE(40 == vector2_r[0]);
  EXPECT_TRUE(55 == vector2_r[1]);
  EXPECT_TRUE(70 == vector2_r[2]);
}
