/*!
 * \file       tests.cpp
 * \author     Jurena
 * \date       9. 2. 2022
 * \brief      Unit tests for vector module.
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/* Includes --------------------------------------------------------------------------------------*/

#include "gmock/gmock.h"

#include "gtest/gtest.h"
#include <limits>
#include <vector>

extern "C" {
#include "vector.h"
}

/* Private types ---------------------------------------------------------------------------------*/
class VectorTest : public ::testing::Test
{
  // Test interface
protected:
  virtual void SetUp() override { v = Vector_Create(10, 100); }

  virtual void TearDown() override { Vector_Destroy(&v); }

  Vector_t *v;
};

class VectorFullTest : public ::testing::Test
{
  // Test interface
protected:
  virtual void SetUp() override
  {
    v = Vector_Create(10, 100);
    Vector_Append(v, 123);
    Vector_Append(v, 321);
    Vector_Append(v, 123);
  }

  virtual void TearDown() override { Vector_Destroy(&v); }

  Vector_t *v;
};

/* Private macros --------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
/* Exported functions definitions ----------------------------------------------------------------*/
TEST(vector, createSmallVector)
{
  Vector_t *v = Vector_Create(10, 100);

  ASSERT_NE(v, nullptr);
  ASSERT_EQ(v->size, 10);
  ASSERT_EQ(v->next, v->items);
  ASSERT_EQ(v->alloc_step, 100);
}

TEST(vector, createBiggestInitialSize)
{
  Vector_t *v = Vector_Create(SIZE_MAX, 100);

  ASSERT_EQ(v, nullptr);
}

TEST(vector, destroyVector)
{
  Vector_t *v = Vector_Create(10, 100);

  Vector_Destroy(&v);
  ASSERT_EQ(v, nullptr);
}

TEST_F(VectorFullTest, copyVector)
{
  Vector_t *c = Vector_Copy(v);

  ASSERT_NE(c, nullptr);
  ASSERT_EQ(v->alloc_step, c->alloc_step);
  ASSERT_EQ(v->size, c->size);
  ASSERT_NE(v->items, c->items);
  ASSERT_NE(v->next, c->next);
  ASSERT_THAT(std::vector<Vector_DataType_t>(c->items, c->items + Vector_Length(c)),
              ::testing::ElementsAreArray(v->items, Vector_Length(v)));

  Vector_Destroy(&c);
}

TEST(vector, copyVectorNull)
{
  Vector_t *c = Vector_Copy(nullptr);
  ASSERT_EQ(c, nullptr);
}

TEST_F(VectorTest, clearVector)
{
  Vector_Clear(v);
  ASSERT_EQ(v->alloc_step, 100);
  ASSERT_EQ(v->size, 0);
  ASSERT_EQ(v->next, nullptr);
  ASSERT_EQ(v->items, nullptr);
}

TEST(vector, clearVectorNull)
{
  Vector_Clear(nullptr);
}

TEST_F(VectorTest, appendSingleItem)
{
  Vector_Append(v, 10);

  ASSERT_EQ(v->items[0], 10);
}

TEST_F(VectorTest, appendMultipleItems)
{
  for (Vector_DataType_t i = 0; i < 20; ++i) {
    Vector_Append(v, i);
  }

  ASSERT_THAT(std::vector<Vector_DataType_t>(v->items, v->items + 20),
              ::testing::ElementsAreArray(
                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}));

  ASSERT_EQ(v->size, 110);
}

TEST(vector, appendVectorNull)
{
  Vector_Append(nullptr, 10);
}

TEST_F(VectorFullTest, getElementAtValidPosition)
{
  Vector_DataType_t val;
  ASSERT_TRUE(Vector_At(v, 1, &val));
  ASSERT_EQ(val, 321);
}

TEST_F(VectorTest, getElementOutOfBounds)
{
  Vector_DataType_t val = 1;
  ASSERT_FALSE(Vector_At(v, 5, &val));
  ASSERT_EQ(val, 1);
}

TEST_F(VectorTest, getElementWithInvalidReturn)
{
  ASSERT_FALSE(Vector_At(v, 5, nullptr));
}

TEST(vector, getElementAtPositionWithNullContainer)
{
  Vector_DataType_t val;
  Vector_At(nullptr, 1, &val);
}

TEST_F(VectorTest, lengthOfEmptyVectorIsZero)
{
  ASSERT_EQ(Vector_Length(v), 0);
}

TEST_F(VectorTest, insertThreeItemsAndCheckTheLengthIsThree)
{
  ASSERT_EQ(Vector_Append(v, 123), 0);
  ASSERT_EQ(Vector_Append(v, 123), 1);
  ASSERT_EQ(Vector_Append(v, 123), 2);
  ASSERT_EQ(Vector_Length(v), 3);
}

TEST(vector, invalidVectorReturnsSizeMax)
{
  ASSERT_EQ(Vector_Length(nullptr), SIZE_MAX);
}

TEST_F(VectorFullTest, removeFirstItem)
{
  Vector_DataType_t val;

  Vector_DataType_t *oldNext = v->next;

  ASSERT_TRUE(Vector_Remove(v, 0));
  ASSERT_EQ(Vector_Length(v), 2);
  Vector_At(v, 0, &val);
  ASSERT_EQ(val, 321);
  ASSERT_EQ(v->next, oldNext - 1);
}

TEST_F(VectorFullTest, removeOutOfBoundsItem)
{
  ASSERT_FALSE(Vector_Remove(v, 3));
}

TEST(vector, removeItemWithNullContainer)
{
  ASSERT_FALSE(Vector_Remove(nullptr, 0));
}

TEST_F(VectorFullTest, containsValidValue)
{
  ASSERT_TRUE(Vector_Contains(v, 123));
}

TEST_F(VectorFullTest, containsInvalidValue)
{
  ASSERT_FALSE(Vector_Contains(v, 111));
}

TEST(vector, containsItemWithNullContainer)
{
  ASSERT_FALSE(Vector_Contains(nullptr, 123));
}

TEST_F(VectorFullTest, indexOfValidItemWithZeroOffset)
{
  ASSERT_EQ(Vector_IndexOf(v, 123, 0), 0);
}

TEST_F(VectorFullTest, indexOfValidItemWithOffset)
{
  ASSERT_EQ(Vector_IndexOf(v, 123, 1), 2);
}

TEST_F(VectorFullTest, indexOfInvalidItemWithZeroOffset)
{
  ASSERT_EQ(Vector_IndexOf(v, 111, 0), -1);
}

TEST_F(VectorFullTest, indexOfValidItemWithOffsetBehindIt)
{
  ASSERT_EQ(Vector_IndexOf(v, 123, 10), -1);
}

TEST(vector, indexOfItemWithNullcontainer)
{
  ASSERT_EQ(Vector_IndexOf(nullptr, 123, 0), -1);
}

TEST_F(VectorTest, fillPartOfVectorFromBegin)
{
  Vector_DataType_t val;
  for (int i = 0; i < 10; i++) {
    Vector_Append(v, i + 1);
  }

  Vector_Fill(v, 54321, 0, 4);
  for (unsigned int i = 0; i < 5; ++i) {
    Vector_At(v, i, &val);
    ASSERT_EQ(val, 54321);
  }

  for (unsigned int i = 5; i < 10; ++i) {
    Vector_At(v, i, &val);
    ASSERT_EQ(val, i + 1);
  }

  ASSERT_EQ(Vector_Length(v), 10);
}

TEST_F(VectorTest, fillWholeVectorFromBegin)
{
  Vector_DataType_t val;
  for (int i = 0; i < 10; i++) {
    Vector_Append(v, i + 1);
  }

  Vector_Fill(v, 12345, 0, 10);
  for (unsigned int i = 0; i < Vector_Length(v); ++i) {
    Vector_At(v, i, &val);
    ASSERT_EQ(val, 12345);
  }

  ASSERT_EQ(Vector_Length(v), 10);
}

TEST_F(VectorTest, fillVectorWithStartIndexBehindItsBounds)
{
  Vector_DataType_t val;
  for (int i = 0; i < 10; i++) {
    Vector_Append(v, i + 1);
  }

  Vector_Fill(v, 0, 50, 5);
  for (unsigned int i = 0; i < Vector_Length(v); ++i) {
    Vector_At(v, i, &val);
    ASSERT_EQ(val, i + 1);
  }

  ASSERT_EQ(Vector_Length(v), 10);
}

TEST_F(VectorTest, fillOnlyAllocatedSpace)
{
  Vector_DataType_t val;
  for (int i = 0; i < 10; i++) {
    Vector_Append(v, i + 1);
  }

  Vector_Fill(v, 1, 0, 100);
  for (unsigned int i = 0; i < Vector_Length(v); ++i) {
    Vector_At(v, i, &val);
    ASSERT_EQ(val, 1);
  }
  ASSERT_EQ(Vector_Length(v), 10);
}

TEST(vector, fillVectorWithNullcontainer)
{
  Vector_Fill(nullptr, 123, 0, 10);
}

TEST_F(VectorFullTest, vectorSetValidPosition)
{
  Vector_Set(v, 1, 0);
  Vector_DataType_t val = 10;
  Vector_At(v, 1, &val);

  ASSERT_EQ(val, 0);
}

TEST_F(VectorTest, vectorSetInvalidPosition)
{
  size_t index = Vector_Append(v, 1);
  auto afterInserted = v->items[index + 1];
  Vector_Set(v, index + 1, !afterInserted);
  Vector_DataType_t val = 10;
  Vector_At(v, index, &val);

  ASSERT_EQ(val, 1);
  ASSERT_EQ(v->items[index + 1], afterInserted);
}

TEST(vector, vectorSetNullContainer)
{
  Vector_Set(nullptr, 0, 0);
}

/* Private function definitions ------------------------------------------------------------------*/