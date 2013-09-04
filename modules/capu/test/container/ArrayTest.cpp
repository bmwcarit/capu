/*
 * Copyright (C) 2012 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "capu/container/Array.h"
#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/util/Swap.h"

class ComplexCopyable
{
public:

    ComplexCopyable()
        : mValue(0)
    {
        mValuePtr = &mValue;
    }

    ComplexCopyable(capu::int32_t value)
        : mValue(value)
    {
        mValuePtr = &mValue;
    }

    ComplexCopyable(const ComplexCopyable& other)
        : mValue(other.mValue)
    {
        mValuePtr = &mValue;
    }

    ComplexCopyable& operator=(const ComplexCopyable& other)
    {
        mValue = other.mValue;
        mValuePtr = &mValue;
        return *this;
    }

    capu::int32_t getValue()
    {
        return mValue;
    }

    capu::int32_t getValueByPtr()
    {
        return *mValuePtr;
    }

private:
    capu::int32_t mValue;
    capu::int32_t* mValuePtr;
};

void testSet(capu::Array<capu::uint32_t>& array,
             capu::uint32_t initialValue,
             capu::uint32_t valueToSet,
             capu::uint32_t offset,
             capu::uint32_t count)
{

    array.set(initialValue);

    for (capu::uint32_t i = 0; i < array.size(); i++)
    {
        EXPECT_EQ(initialValue, array[i]);
    }

    EXPECT_EQ(capu::CAPU_OK, array.set(valueToSet, offset, count));

    for (capu::uint32_t i = 0; i < array.size(); ++i)
    {
        capu::uint32_t expected = i < offset || i >= offset + count ? initialValue : valueToSet;
        EXPECT_EQ(expected, array[i]);
    }
}

TEST(Array, SetPerformance)
{
    capu::Array<capu::uint32_t> array(10000);

    for (int i = 0; i < 10000; ++i)
    {
        array.set(5);
    }
}


TEST(Array, Constructor)
{
    capu::Array<capu::uint32_t> emptyArray;
    EXPECT_EQ(0u, emptyArray.size());

    capu::Array<capu::uint32_t>* myArray = NULL;
    myArray = new capu::Array<capu::uint32_t>(10);
    EXPECT_TRUE(myArray != NULL);
    EXPECT_EQ(10u, myArray->size());
    delete myArray;
}

TEST(Array, SetAndGetValues)
{
    capu::Array<capu::uint32_t> myArray(10);
    myArray.set(5);
    for (capu::int32_t i = 0; i < 10; i++)
    {
        EXPECT_EQ(5u, myArray[i]);
    }

    EXPECT_EQ(capu::CAPU_OK, myArray.set(3, 0, 3));
    EXPECT_EQ(3u, myArray[0]);
    EXPECT_EQ(3u, myArray[1]);
    EXPECT_EQ(3u, myArray[2]);

    EXPECT_EQ(capu::CAPU_OK, myArray.set(7, 7, 3));
    EXPECT_EQ(7u, myArray[7]);
    EXPECT_EQ(7u, myArray[8]);
    EXPECT_EQ(7u, myArray[9]);

    EXPECT_EQ(capu::CAPU_OK, myArray.set(4, 4, 3));
    EXPECT_EQ(4u, myArray[4]);
    EXPECT_EQ(4u, myArray[5]);
    EXPECT_EQ(4u, myArray[6]);

    capu::Array<capu::uint32_t> myArray2(20);
    myArray2.set(5);
    for (capu::int32_t i = 0; i < 20; i++)
    {
        EXPECT_EQ(5u, myArray2[i]);
    }

    EXPECT_EQ(capu::CAPU_OK, myArray2.set(3, 0, 3));
    EXPECT_EQ(3u, myArray2[0]);
    EXPECT_EQ(3u, myArray2[1]);
    EXPECT_EQ(3u, myArray2[2]);

    EXPECT_EQ(capu::CAPU_OK, myArray2.set(7, 17, 3));
    EXPECT_EQ(7u, myArray2[17]);
    EXPECT_EQ(7u, myArray2[18]);
    EXPECT_EQ(7u, myArray2[19]);

    EXPECT_EQ(capu::CAPU_OK, myArray2.set(4, 5, 12));
    EXPECT_EQ(5u, myArray2[4]);
    EXPECT_EQ(4u, myArray2[5]);
    EXPECT_EQ(4u, myArray2[6]);
    EXPECT_EQ(4u, myArray2[7]);
    EXPECT_EQ(4u, myArray2[8]);
    EXPECT_EQ(4u, myArray2[9]);
    EXPECT_EQ(4u, myArray2[10]);
    EXPECT_EQ(4u, myArray2[11]);
    EXPECT_EQ(4u, myArray2[12]);
    EXPECT_EQ(4u, myArray2[13]);
    EXPECT_EQ(4u, myArray2[14]);
    EXPECT_EQ(4u, myArray2[15]);
    EXPECT_EQ(4u, myArray2[16]);
    EXPECT_EQ(7u, myArray2[17]);

    testSet(myArray2, 5, 3, 2, 0);
    testSet(myArray2, 5, 3, 2, 1);
    testSet(myArray2, 5, 3, 2, 2);
    testSet(myArray2, 5, 3, 2, 3);
    testSet(myArray2, 5, 3, 2, 4);
    testSet(myArray2, 5, 3, 2, 5);
    testSet(myArray2, 5, 3, 2, 6);
    testSet(myArray2, 5, 3, 2, 7);
    testSet(myArray2, 5, 3, 2, 8);
    testSet(myArray2, 5, 3, 2, 9);
    testSet(myArray2, 5, 3, 2, 10);
    testSet(myArray2, 5, 3, 2, 11);


    myArray2[0] = 1;
    myArray2[1] = 2;

    EXPECT_EQ(1u, myArray2[0]);
    EXPECT_EQ(2u, myArray2[1]);

    EXPECT_EQ(capu::CAPU_OK, myArray2.set(4, 0, 20));
    EXPECT_EQ(capu::CAPU_ERANGE, myArray2.set(4, 0, 21));
    EXPECT_EQ(capu::CAPU_ERANGE, myArray2.set(4, 1, 20));
}

TEST(Array, Swap)
{
    capu::Array<capu::uint32_t> myArray1(3);
    myArray1[0] = 1;
    myArray1[1] = 2;
    myArray1[2] = 3;

    capu::Array<capu::uint32_t> myArray2(5);
    myArray2[0] = 5;
    myArray2[1] = 4;
    myArray2[2] = 3;
    myArray2[3] = 2;
    myArray2[4] = 1;

    capu::swap(myArray1, myArray2);

    // check first array
    EXPECT_EQ(5u, myArray1.size());
    EXPECT_EQ(5u, myArray1[0]);
    EXPECT_EQ(4u, myArray1[1]);
    EXPECT_EQ(3u, myArray1[2]);
    EXPECT_EQ(2u, myArray1[3]);
    EXPECT_EQ(1u, myArray1[4]);

    // check second array
    EXPECT_EQ(3u, myArray2.size());
    EXPECT_EQ(1u, myArray2[0]);
    EXPECT_EQ(2u, myArray2[1]);
    EXPECT_EQ(3u, myArray2[2]);

    // test swapping between heap and stack arrays
    capu::Array<capu::uint32_t> myArrayStack;
    capu::Array<capu::uint32_t> *myArrayHeap = new capu::Array<capu::uint32_t>();

    capu::swap(myArrayStack, *myArrayHeap);
    EXPECT_EQ(0u, myArrayHeap->size());
    delete myArrayHeap;

    EXPECT_EQ(0u, myArrayStack.size());


}

TEST(Array, Move)
{
    capu::Array<capu::uint32_t> myArray(10);
    myArray.set(5);

    myArray[0] = 10;
    myArray[1] = 15;
    myArray[2] = 20;

    EXPECT_EQ(capu::CAPU_OK, myArray.move(0, 3, 7));
    EXPECT_EQ(10u, myArray[7]);
    EXPECT_EQ(15u, myArray[8]);
    EXPECT_EQ(20u, myArray[9]);

    EXPECT_EQ(capu::CAPU_ERANGE, myArray.move(13, 3, 7));
    EXPECT_EQ(capu::CAPU_ERANGE, myArray.move(0, 13, 7));
    EXPECT_EQ(capu::CAPU_ERANGE, myArray.move(0, 3, 13));


    // complex type

    capu::Array<ComplexCopyable> complexArray(10);
    for (capu::int32_t i = 0; i < 10; ++i)
    {
        complexArray[i] = i;
    }
    for (capu::int32_t i = 0; i < 10; ++i)
    {
        ComplexCopyable& tmp = complexArray[i];
        EXPECT_EQ(i, tmp.getValue());
        EXPECT_EQ(i, tmp.getValueByPtr());
    }

    // move with overlap
    EXPECT_EQ(capu::CAPU_OK, complexArray.move(0, 7, 3));
    for (capu::int32_t i = 3; i < 10; ++i)
    {
        ComplexCopyable& tmp = complexArray[i];
        EXPECT_EQ(i - 3, tmp.getValue());
        EXPECT_EQ(i - 3, tmp.getValueByPtr());
    }

}

TEST(Array, CopyConstructor)
{

    capu::Array<capu::uint32_t> myArray(10);
    myArray.set(5);

    capu::Array<capu::uint32_t> myArray2(myArray);

    EXPECT_EQ(5u, myArray2[0]);
    EXPECT_EQ(5u, myArray2[4]);
    EXPECT_EQ(5u, myArray2[9]);
}

TEST(Array, Assignment)
{
    capu::Array<capu::uint32_t> myEmptyArray;

    capu::Array<capu::uint32_t> myArray(10);
    myArray.set(5);

    myEmptyArray = myArray;

    EXPECT_EQ(5u, myEmptyArray[0]);
    EXPECT_EQ(5u, myEmptyArray[4]);
    EXPECT_EQ(5u, myEmptyArray[9]);

    capu::Array<capu::uint32_t> myArray2(10);
    myArray2 = myArray;

    EXPECT_EQ(5u, myArray2[0]);
    EXPECT_EQ(5u, myArray2[4]);
    EXPECT_EQ(5u, myArray2[9]);

    capu::Array<capu::uint32_t> myArray3(10);
    capu::Array<capu::uint32_t> myArray4(20);
    myArray3.set(1);
    myArray4.set(2);

    myArray3 = myArray4;
    EXPECT_EQ(2u, myArray3[0]);
    EXPECT_EQ(2u, myArray3[4]);
    EXPECT_EQ(2u, myArray3[9]);
    EXPECT_EQ(20u, myArray3.size());


    // complex type
    capu::Array<ComplexCopyable> complexArrayAssigned;
    EXPECT_EQ(0u, complexArrayAssigned.size());
    {
        capu::Array<ComplexCopyable> complexArrayFirst(10);
        for (capu::int32_t i = 0; i < 10; ++i)
        {
            complexArrayFirst[i] = i;
        }
        for (capu::int32_t i = 0; i < 10; ++i)
        {
            ComplexCopyable& tmp = complexArrayFirst[i];
            EXPECT_EQ(i, tmp.getValue());
            EXPECT_EQ(i, tmp.getValueByPtr());
        }

        // now assign and check contents
        complexArrayAssigned = complexArrayFirst;
        EXPECT_EQ(10u, complexArrayAssigned.size());
        for (capu::int32_t i = 0; i < 10; ++i)
        {
            ComplexCopyable& tmp = complexArrayAssigned[i];
            EXPECT_EQ(i, tmp.getValue());
            EXPECT_EQ(i, tmp.getValueByPtr());
        }

        // now change original array
        for (capu::int32_t i = 0; i < 10; ++i)
        {
            complexArrayFirst[i] = i + 10;
        }

        // check assigned array again
        for (capu::int32_t i = 0; i < 10; ++i)
        {
            ComplexCopyable& tmp = complexArrayAssigned[i];
            EXPECT_EQ(i, tmp.getValue());
            EXPECT_EQ(i, tmp.getValueByPtr());
        }

    }

    // check assigned array again
    for (capu::int32_t i = 0; i < 10; ++i)
    {
        ComplexCopyable& tmp = complexArrayAssigned[i];
        EXPECT_EQ(i, tmp.getValue());
        EXPECT_EQ(i, tmp.getValueByPtr());
    }

    // copy empty array into empty array
    capu::Array<capu::uint32_t> myEmptyArray2;
    capu::Array<capu::uint32_t> myEmptyArray3;

    myEmptyArray2 = myEmptyArray3;

    // one array on stack, one on heap
    capu::Array<capu::uint32_t> myEmptyArrayStack1;
    capu::Array<capu::uint32_t> myEmptyArrayStack2;
    capu::Array<capu::uint32_t> *myEmptyArrayHeap1 = new capu::Array<capu::uint32_t>();
    capu::Array<capu::uint32_t> *myEmptyArrayHeap2 = new capu::Array<capu::uint32_t>();


    myEmptyArrayStack1 = *myEmptyArrayHeap1;
    EXPECT_EQ(0u, myEmptyArrayHeap1->size());
    EXPECT_EQ(0u, myEmptyArrayStack1.size());
    delete myEmptyArrayHeap1;

    *myEmptyArrayHeap2 = myEmptyArrayStack2;
    EXPECT_EQ(0u, myEmptyArrayHeap2->size());
    EXPECT_EQ(0u, myEmptyArrayStack2.size());
    delete myEmptyArrayHeap2;


}

TEST(Array, CopyMethodNormal)
{
    capu::Array<capu::uint32_t> original(5);
    capu::uint32_t newVals[5];

    // fill with default values
    for (capu::uint32_t i = 0; i < original.size(); i++)
    {
        original[i] = i;
        newVals[i] = i + 1;
    }

    capu::status_t status = original.copy(newVals, original.size());
    EXPECT_EQ(capu::CAPU_OK, status);

    for (capu::uint32_t i = 0; i < original.size(); i++)
    {
        EXPECT_EQ(i + 1, original[i]);
    }
}

TEST(Array, CopyMethodNormalWithBiggerArray)
{
    capu::Array<capu::uint32_t> original(5);
    capu::uint32_t newVals[50]; // this one's bigger

    // fill with default values
    for (capu::uint32_t i = 0; i < original.size(); i++)
    {
        original[i] = i;
    }
    for (capu::uint32_t i = 0; i < 50; i++)
    {
        newVals[i] = i + 1;
    }

    capu::status_t status = original.copy(newVals, original.size()); // only copy the first 5 values
    EXPECT_EQ(capu::CAPU_OK, status);

    for (capu::uint32_t i = 0; i < original.size(); i++)
    {
        EXPECT_EQ(i + 1, original[i]);
    }
}

TEST(Array, CopyMethodInvalidSize)
{
    capu::Array<capu::uint32_t> original(5);
    for (capu::uint32_t i = 0; i < original.size(); i++)
    {
        original[i] = i;
    }
    capu::uint32_t newVals[7];

    capu::status_t status = original.copy(newVals, 7); // give invalid size
    EXPECT_EQ(capu::CAPU_ERANGE, status);

    // no changes in original array
    for (capu::uint32_t i = 0; i < original.size(); i++)
    {
        EXPECT_EQ(i, original[i]);
    }
}

TEST(Array, CopyMethodZeroSize)
{
    capu::Array<capu::uint32_t> src(0);
    capu::Array<capu::uint32_t> destination(5);

    EXPECT_EQ(capu::CAPU_OK, destination.copy(src.getRawData(), src.size()));


}

TEST(Array, CopyMethodZeroSizeNormal)
{
    capu::Array<capu::uint32_t> original(0);
    capu::uint32_t newVals[1];

    capu::status_t status = original.copy(newVals, original.size()); // copy with zero, no error expected
    EXPECT_EQ(capu::CAPU_OK, status);
}

TEST(Array, CopyMethodZeroSizeInvalidSize)
{
    capu::Array<capu::uint32_t> original(0);
    capu::uint32_t newVals[1];

    capu::status_t status = original.copy(newVals, 1);
    EXPECT_EQ(capu::CAPU_ERANGE, status);
}

TEST(Array, SetRawData)
{
    capu::Array<capu::uint32_t> array(3u);

    array.setRawData(0);

    EXPECT_EQ(0u, array[0]);
    EXPECT_EQ(0u, array[1]);
    EXPECT_EQ(0u, array[2]);
}
