/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

// Workaround for integrityOS compiler
// It supports typed tests, but gtest does not know this yet
#if defined(__GHS_VERSION_NUMBER)
# define GTEST_HAS_TYPED_TEST 1
# define GTEST_HAS_TYPED_TEST_P 1
#endif

#include "gmock/gmock.h"
#include "capu/container/Vector.h"
#include "capu/container/String.h"
#include "util/BidirectionalTestContainer.h"
#include "util/ComplexTestType.h"

    template<typename T>
    struct NumberOfExistingObjectsHelper
    {
        static void assertNumberOfExistingObjectsEquals(capu::uint_t expectedNumber)
        {
            UNUSED(expectedNumber);
        }
    };

    template<>
    struct NumberOfExistingObjectsHelper<ComplexTestType>
    {
        static void assertNumberOfExistingObjectsEquals(capu::uint_t expectedNumber)
        {
            ASSERT_EQ(expectedNumber, ComplexTestType::ctor_count + ComplexTestType::copyctor_count - ComplexTestType::dtor_count);
        }
    };

    template <typename ElementType>
    class TypedVectorTest : public testing::Test
    {
    };

    template <typename ElementType>
    class TypedVectorEnsureSTLCompatibility : public testing::Test
    {
    };

    template <typename ElementType>
    class TypedVectorPerformanceTest : public testing::Test
    {
    };

    typedef ::testing::Types
        <
        capu::uint_t,
        ComplexTestType
        > ElementTypes;

    TYPED_TEST_CASE(TypedVectorTest, ElementTypes);
    TYPED_TEST_CASE(TypedVectorEnsureSTLCompatibility, ElementTypes);
    TYPED_TEST_CASE(TypedVectorPerformanceTest, ElementTypes);

    TYPED_TEST(TypedVectorPerformanceTest, DISABLED_STLinsertLots)
    {
        std::vector<TypeParam> stlvector(0);

        for (capu::uint_t i = 0; i < 100000; ++i)
        {
            stlvector.push_back(TypeParam(i));
        }
    }

    TYPED_TEST(TypedVectorPerformanceTest, DISABLED_CAPUinsertLots)
    {
        capu::Vector<TypeParam> capuVector(0);

        for (capu::uint_t i = 0; i < 100000; ++i)
        {
            capuVector.push_back(TypeParam(i));
        }
    }

    TYPED_TEST(TypedVectorPerformanceTest, DISABLED_STLReserveLots)
    {
        std::vector<TypeParam>* stlvector = new std::vector<TypeParam>(1000000);
        delete stlvector;
    }

    TYPED_TEST(TypedVectorPerformanceTest, DISABLED_CAPUReserveLots)
    {
        capu::Vector<TypeParam>* capuVector = new capu::Vector<TypeParam>(1000000);
        delete capuVector;
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, initialDefault)
    {
        std::vector<TypeParam> stlvector;
        capu::Vector<TypeParam> capuVector;

        EXPECT_EQ(stlvector.size(), capuVector.size());
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, sizeBehaviour)
    {
        std::vector<TypeParam> stlvector(0);
        capu::Vector<TypeParam> capuVector(0);

        EXPECT_EQ(0u, stlvector.capacity());
        EXPECT_EQ(0u, capuVector.capacity());

        EXPECT_EQ(0u, stlvector.size());
        EXPECT_EQ(0u, capuVector.size());

        stlvector.push_back(0);
        capuVector.push_back(0);

        EXPECT_EQ(1u, stlvector.size());
        EXPECT_EQ(1u, capuVector.size());

        stlvector.pop_back();
        capuVector.pop_back();

        EXPECT_EQ(0u, stlvector.size());
        EXPECT_EQ(0u, capuVector.size());
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, sizeBehaviour2)
    {
        std::vector<TypeParam> stlvector(5);
        capu::Vector<TypeParam> capuVector(5);

        EXPECT_EQ(5u, stlvector.capacity());
        EXPECT_EQ(5u, capuVector.capacity());

        EXPECT_EQ(5u, stlvector.size());
        EXPECT_EQ(5u, capuVector.size());

        stlvector.push_back(0);
        capuVector.push_back(0);

        EXPECT_EQ(6u, stlvector.size());
        EXPECT_EQ(6u, capuVector.size());

        stlvector.pop_back();
        capuVector.pop_back();

        EXPECT_EQ(5u, stlvector.size());
        EXPECT_EQ(5u, capuVector.size());
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, reserveBahaviour)
    {
        std::vector<TypeParam> stlvector(5);
        capu::Vector<TypeParam> capuVector(5);

        EXPECT_EQ(5u, stlvector.capacity());
        EXPECT_EQ(5u, capuVector.capacity());

        EXPECT_EQ(5u, stlvector.size());
        EXPECT_EQ(5u, capuVector.size());

        stlvector.reserve(15);
        capuVector.reserve(15);

        EXPECT_EQ(15u, stlvector.capacity());
        EXPECT_EQ(15u, capuVector.capacity());

        EXPECT_EQ(5u, stlvector.size());
        EXPECT_EQ(5u, capuVector.size());

    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, reserveConstructsClass)
    {
        {
            std::vector<TypeParam> stlvector(5);
            NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(5u);
        }
        {
            capu::Vector<TypeParam> capuvector(5);
            NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(5u);
        }
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, deletingVectorAlsoCallsDestructorsOfElements)
    {
        {
            {
                std::vector<TypeParam> stlvector(5);
            }
            NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(0u);
        }
        {
            {
                capu::Vector<TypeParam> capuvector(5);
            }
            NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(0u);
        }
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, resizeSmallerDeconstructsElements)
    {
        capu::Vector<TypeParam> capuVector(5);
        NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(5u);

        capuVector.resize(0);
        NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(0u);
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, resizeBehaviour)
    {
        std::vector<TypeParam> stlvector(5);
        capu::Vector<TypeParam> capuVector(5);

        EXPECT_EQ(5u, stlvector.capacity());
        EXPECT_EQ(5u, capuVector.capacity());

        EXPECT_EQ(5u, stlvector.size());
        EXPECT_EQ(5u, capuVector.size());

        stlvector.resize(15);
        capuVector.resize(15);

        EXPECT_EQ(15u, stlvector.capacity());
        EXPECT_EQ(15u, capuVector.capacity());

        EXPECT_EQ(15u, stlvector.size());
        EXPECT_EQ(15u, capuVector.size());

    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, initialRequestedSize)
    {
        std::vector<TypeParam> stlvector(876);
        capu::Vector<TypeParam> capuVector(876);

        EXPECT_EQ(876u, stlvector.capacity());
        EXPECT_EQ(876u, capuVector.capacity());
        EXPECT_EQ(stlvector.size(), capuVector.size());
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, resize)
    {
        std::vector<TypeParam> stlvector;
        capu::Vector<TypeParam> capuVector;

        stlvector.resize(128);
        capuVector.resize(128);

        EXPECT_EQ(stlvector.size(), capuVector.size());
        EXPECT_EQ(stlvector.capacity(), capuVector.capacity());
    }

    TYPED_TEST(TypedVectorEnsureSTLCompatibility, reserve)
    {
        std::vector<TypeParam> stlvector;
        capu::Vector<TypeParam> capuVector;

        stlvector.reserve(128);
        capuVector.reserve(128);

        EXPECT_EQ(stlvector.size(), capuVector.size());
        EXPECT_EQ(stlvector.capacity(), capuVector.capacity());
    }

    TYPED_TEST(TypedVectorTest, insertSingleValueWhileNeedingToGrow)
    {
        capu::Vector<TypeParam> capuVector(0);

        for (capu::uint_t i = 0; i < 11; ++i)
        {
            capuVector.insert(capuVector.begin(), TypeParam(i));
        }

        EXPECT_EQ(TypeParam(10u), capuVector[0]);
        EXPECT_EQ(TypeParam(9u), capuVector[1]);
        EXPECT_EQ(TypeParam(8u), capuVector[2]);
        EXPECT_EQ(TypeParam(7u), capuVector[3]);
        EXPECT_EQ(TypeParam(6u), capuVector[4]);
        EXPECT_EQ(TypeParam(5u), capuVector[5]);
        EXPECT_EQ(TypeParam(4u), capuVector[6]);
        EXPECT_EQ(TypeParam(3u), capuVector[7]);
        EXPECT_EQ(TypeParam(2u), capuVector[8]);
        EXPECT_EQ(TypeParam(1u), capuVector[9]);
        EXPECT_EQ(TypeParam(0u), capuVector[10]);
    }

    TYPED_TEST(TypedVectorTest, insertSingleValueIntoReservedSpace)
    {
        capu::Vector<TypeParam> capuVector;
        capuVector.reserve(64);

        for (capu::uint_t i = 0; i < 11; ++i)
        {
            capuVector.insert(capuVector.begin(), TypeParam(i));
        }

        EXPECT_EQ(TypeParam(10u), capuVector[0]);
        EXPECT_EQ(TypeParam(9u), capuVector[1]);
        EXPECT_EQ(TypeParam(8u), capuVector[2]);
        EXPECT_EQ(TypeParam(7u), capuVector[3]);
        EXPECT_EQ(TypeParam(6u), capuVector[4]);
        EXPECT_EQ(TypeParam(5u), capuVector[5]);
        EXPECT_EQ(TypeParam(4u), capuVector[6]);
        EXPECT_EQ(TypeParam(3u), capuVector[7]);
        EXPECT_EQ(TypeParam(2u), capuVector[8]);
        EXPECT_EQ(TypeParam(1u), capuVector[9]);
        EXPECT_EQ(TypeParam(0u), capuVector[10]);
    }

    TYPED_TEST(TypedVectorTest, insertSingleValueAtEndPosition)
    {
        capu::Vector<TypeParam> capuVector(2, TypeParam(0));
        capuVector.insert(capuVector.end(), TypeParam(123));

        EXPECT_EQ(TypeParam(0u), capuVector[0]);
        EXPECT_EQ(TypeParam(0u), capuVector[1]);
        EXPECT_EQ(TypeParam(123u), capuVector[2]);
    }

    TYPED_TEST(TypedVectorTest, insertSingleValueIntoMiddle)
    {
        capu::Vector<TypeParam> capuVector(2, TypeParam(0));
        capuVector.insert(capuVector.begin()+1u, TypeParam(123));

        EXPECT_EQ(TypeParam(0u), capuVector[0]);
        EXPECT_EQ(TypeParam(123u), capuVector[1]);
        EXPECT_EQ(TypeParam(0u), capuVector[2]);
    }

    TYPED_TEST(TypedVectorTest, insertRangeWhileNeedingToGrow)
    {
        capu::Vector<TypeParam> source(3);
        source[0] = 1;
        source[1] = 2;
        source[2] = 3;
        capu::Vector<TypeParam> capuVector(0);

        capuVector.insert(capuVector.begin(), source.begin(), source.end());

        EXPECT_EQ(TypeParam(1u), capuVector[0]);
        EXPECT_EQ(TypeParam(2u), capuVector[1]);
        EXPECT_EQ(TypeParam(3u), capuVector[2]);
    }

    TYPED_TEST(TypedVectorTest, insertRangeIntoReservedSpace)
    {
        capu::Vector<TypeParam> source(3);
        source[0] = 2;
        source[1] = 3;
        source[2] = 4;
        capu::Vector<TypeParam> capuVector(2);
        capuVector[0] = 1;
        capuVector[1] = 5;

        capuVector.insert(capuVector.begin() + 1u, source.begin(), source.end());

        EXPECT_EQ(TypeParam(1u), capuVector[0]);
        EXPECT_EQ(TypeParam(2u), capuVector[1]);
        EXPECT_EQ(TypeParam(3u), capuVector[2]);
        EXPECT_EQ(TypeParam(4u), capuVector[3]);
        EXPECT_EQ(TypeParam(5u), capuVector[4]);
    }

    TYPED_TEST(TypedVectorTest, insertRangeIntoReservedSpace_NumberToMoveIsLargerThanNumberToCopyIntoRaw)
    {
        capu::Vector<TypeParam> source(1);
        source[0] = 2;
        capu::Vector<TypeParam> capuVector(4);
        capuVector[0] = 1;
        capuVector[1] = 3;
        capuVector[2] = 4;
        capuVector[3] = 5;

        capuVector.insert(capuVector.begin() + 1u, source.begin(), source.end());

        EXPECT_EQ(TypeParam(1u), capuVector[0]);
        EXPECT_EQ(TypeParam(2u), capuVector[1]);
        EXPECT_EQ(TypeParam(3u), capuVector[2]);
        EXPECT_EQ(TypeParam(4u), capuVector[3]);
        EXPECT_EQ(TypeParam(5u), capuVector[4]);
    }

    TYPED_TEST(TypedVectorTest, insertRangeAtEndPosition)
    {
        capu::Vector<TypeParam> source(3);
        source[0] = 3;
        source[1] = 4;
        source[2] = 5;
        capu::Vector<TypeParam> capuVector(2);
        capuVector[0] = 1;
        capuVector[1] = 2;

        capuVector.insert(capuVector.end(), source.begin(), source.end());

        EXPECT_EQ(TypeParam(1u), capuVector[0]);
        EXPECT_EQ(TypeParam(2u), capuVector[1]);
        EXPECT_EQ(TypeParam(3u), capuVector[2]);
        EXPECT_EQ(TypeParam(4u), capuVector[3]);
        EXPECT_EQ(TypeParam(5u), capuVector[4]);
    }

    TYPED_TEST(TypedVectorTest, insertRangeFromBidirectionalIterator)
    {
        capu::BidirectionalTestContainer<TypeParam> source(3);
        source[0] = 2;
        source[1] = 3;
        source[2] = 4;
        capu::Vector<TypeParam> capuVector(2);
        capuVector[0] = 1;
        capuVector[1] = 5;

        capuVector.insert(capuVector.begin() + 1u, source.begin(), source.end());

        EXPECT_EQ(TypeParam(1u), capuVector[0]);
        EXPECT_EQ(TypeParam(2u), capuVector[1]);
        EXPECT_EQ(TypeParam(3u), capuVector[2]);
        EXPECT_EQ(TypeParam(4u), capuVector[3]);
        EXPECT_EQ(TypeParam(5u), capuVector[4]);
    }

    TYPED_TEST(TypedVectorTest, Constructor)
    {
        capu::Vector<TypeParam> vector;
        EXPECT_EQ(0U, vector.size());
    }

    TYPED_TEST(TypedVectorTest, ConstructorWithSize)
    {
        capu::Vector<TypeParam> vector(3);
        EXPECT_EQ(3u, vector.size());
    }

    TYPED_TEST(TypedVectorTest, ConstructorWithCapacityAndValue)
    {
        capu::Vector<TypeParam> vector(3, 5);

        EXPECT_EQ(TypeParam(5u), vector[0]);
        EXPECT_EQ(TypeParam(5u), vector[1]);
        EXPECT_EQ(TypeParam(5u), vector[2]);
    }

    TYPED_TEST(TypedVectorTest, CopyConstructor)
    {
        capu::Vector<TypeParam>* vector = new capu::Vector<TypeParam>(0);

        for(uint32_t i = 0; i < 32; ++i)
        {
            vector->push_back(TypeParam(i));
        }

        const capu::Vector<TypeParam> vectorCopy(*vector);
        delete vector;

        for(uint32_t i = 0; i < 32; ++i)
        {
            EXPECT_EQ(TypeParam(i), vectorCopy[i]);
        }

        EXPECT_EQ(32u, vectorCopy.size());
    }

    TYPED_TEST(TypedVectorTest, AssignmentOperator)
    {
        capu::Vector<TypeParam>* vector = new capu::Vector<TypeParam>(0);

        for (uint32_t i = 0; i < 32; ++i)
        {
            vector->push_back(TypeParam(i));
        }

        capu::Vector<TypeParam> vectorCopy;
        vectorCopy = *vector;
        delete vector;

        ASSERT_EQ(32u, vectorCopy.size());
        for (uint32_t i = 0; i < 32; ++i)
        {
            EXPECT_EQ(TypeParam(i), vectorCopy[i]);
        }
    }

    TYPED_TEST(TypedVectorTest, reserveThenResizeIntoIt)
    {
        capu::Vector<TypeParam> vector;
        vector.reserve(64);
        vector.resize(32);
        EXPECT_EQ(32U, vector.size());
    }

    TYPED_TEST(TypedVectorTest, PushBack)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(42u));
        vector.push_back(TypeParam(47u));

        EXPECT_EQ(TypeParam(42u), vector[0]);
        EXPECT_EQ(TypeParam(47u), vector[1]);
    }

    TYPED_TEST(TypedVectorTest, PushBack2)
    {
        capu::Vector<TypeParam> vector(2);

        vector[0] = TypeParam(42u);
        vector[1] = TypeParam(47u);

        EXPECT_EQ(TypeParam(42u), vector[0]);
        EXPECT_EQ(TypeParam(47u), vector[1]);
    }

    TYPED_TEST(TypedVectorTest, PushBack3)
    {
        capu::Vector<TypeParam> vector(0);

        vector.push_back(TypeParam(42u));

        EXPECT_EQ(TypeParam(42u), vector[0]);
    }

    TYPED_TEST(TypedVectorTest, PopBack)
    {
        capu::Vector<TypeParam> vector(0);

        vector.push_back(TypeParam(42u));
        capu::status_t status = vector.pop_back();

        EXPECT_EQ(capu::CAPU_OK, status);
        EXPECT_EQ(TypeParam(0u), vector.size());
    }

    TYPED_TEST(TypedVectorTest, PopBack2)
    {
        capu::Vector<TypeParam> vector(0);

        vector.push_back(TypeParam(42u));
        vector.push_back(TypeParam(43u));
        capu::status_t status = vector.pop_back();
        capu::status_t status2 = vector.pop_back();
        vector.push_back(TypeParam(44u));

        EXPECT_EQ(capu::CAPU_OK, status);
        EXPECT_EQ(capu::CAPU_OK, status2);
        EXPECT_EQ(1u, vector.size());
        EXPECT_EQ(TypeParam(44u), vector[0]);
    }

    TYPED_TEST(TypedVectorTest, empty)
    {
        capu::Vector<TypeParam> vector(0);

        EXPECT_TRUE(vector.empty());

        vector.push_back(TypeParam(123u));
        EXPECT_FALSE(vector.empty());
    }

    TYPED_TEST(TypedVectorTest, PopBackNonExistingElementReturnsError)
    {
        capu::Vector<TypeParam> vector(0);

        vector.push_back(TypeParam(42u));
        capu::status_t status = vector.pop_back();
        capu::status_t status2 = vector.pop_back();

        EXPECT_EQ(capu::CAPU_OK, status);
        EXPECT_NE(capu::CAPU_OK, status2);
    }

    TYPED_TEST(TypedVectorTest, IteratorInc)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));

        typename capu::Vector<TypeParam>::Iterator current = vector.begin();

        EXPECT_EQ(TypeParam(1u), *current);
        ++current;
        EXPECT_EQ(TypeParam(2u), *current);
        ++current;
        EXPECT_EQ(TypeParam(3u), *current);
        ++current;
        EXPECT_EQ(TypeParam(4u), *current);
    }

    TYPED_TEST(TypedVectorTest, IteratorDec)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));

        typename capu::Vector<TypeParam>::Iterator end = vector.end();
        --end;
        EXPECT_EQ(TypeParam(4u), *end);
        --end;
        EXPECT_EQ(TypeParam(3u), *end);
        --end;
        EXPECT_EQ(TypeParam(2u), *end);
        --end;
        EXPECT_EQ(TypeParam(1u), *end);
    }

    TYPED_TEST(TypedVectorTest, IteratorNotEqual)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));

        typename capu::Vector<TypeParam>::Iterator start = vector.begin();
        typename capu::Vector<TypeParam>::Iterator end = vector.end();

        EXPECT_TRUE(start != end);
        end = start;
        EXPECT_FALSE(start != end);

    }

    TYPED_TEST(TypedVectorTest, IteratorSmaller)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));

        typename capu::Vector<TypeParam>::Iterator start = vector.begin();
        typename capu::Vector<TypeParam>::Iterator end = vector.end();

        EXPECT_TRUE(start < end);
        EXPECT_FALSE(end < start);
    }

    TYPED_TEST(TypedVectorTest, IteratorBigger)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));

        typename capu::Vector<TypeParam>::Iterator start = vector.begin();
        typename capu::Vector<TypeParam>::Iterator end = vector.end();

        EXPECT_TRUE(end > start);
        EXPECT_FALSE(start > end);
    }

    TYPED_TEST(TypedVectorTest, IteratorAddValue)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));

        typename capu::Vector<TypeParam>::Iterator start = vector.begin();

        EXPECT_EQ(TypeParam(1u), *(start + 0u));
        EXPECT_EQ(TypeParam(2u), *(start + 1u));
        EXPECT_EQ(TypeParam(3u), *(start + 2u));
        EXPECT_EQ(TypeParam(4u), *(start + 3u));
    }

    TYPED_TEST(TypedVectorTest, IteratorSubValue)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));

        typename capu::Vector<TypeParam>::Iterator end = vector.end();

        EXPECT_EQ(TypeParam(4u), *(end - 1u));
        EXPECT_EQ(TypeParam(3u), *(end - 2u));
        EXPECT_EQ(TypeParam(2u), *(end - 3u));
        EXPECT_EQ(TypeParam(1u), *(end - 4u));
    }

    TYPED_TEST(TypedVectorTest, Iterator)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(42u));
        vector.push_back(TypeParam(47u));

        capu::Vector<TypeParam> vector2;

        for (typename capu::Vector<TypeParam>::Iterator iter = vector.begin(); iter != vector.end(); ++iter)
        {
            vector2.push_back(*iter);
        }

        EXPECT_EQ(TypeParam(42u), vector2[0]);
        EXPECT_EQ(TypeParam(47u), vector2[1]);
    }

    TYPED_TEST(TypedVectorTest, IteratorOnConstVector)
    {
        capu::Vector<TypeParam> vector;

        const capu::Vector<TypeParam>& constVector = vector;

        vector.push_back(TypeParam(42u));
        vector.push_back(TypeParam(47u));

        capu::Vector<TypeParam> vector2;

        for (typename capu::Vector<TypeParam>::ConstIterator iter = constVector.begin(); iter != constVector.end(); ++iter)
        {
            vector2.push_back(*iter);
        }

        EXPECT_EQ(TypeParam(2u), vector2.size());
        EXPECT_EQ(TypeParam(42u), vector2[0]);
        EXPECT_EQ(TypeParam(47u), vector2[1]);
    }

    TYPED_TEST(TypedVectorTest, IteratorOnConstVectorWithInitialCapacity)
    {
        capu::Vector<TypeParam> vector(2);

        const capu::Vector<TypeParam>& constVector = vector;

        vector[0] = TypeParam(42u);
        vector[1] = TypeParam(47u);

        capu::Vector<TypeParam> vector2;

        for (typename capu::Vector<TypeParam>::ConstIterator iter = constVector.begin(); iter != constVector.end(); ++iter)
        {
            vector2.push_back(*iter);
        }

        EXPECT_EQ(2u, vector2.size());
        EXPECT_EQ(TypeParam(42u), vector2[0]);
        EXPECT_EQ(TypeParam(47u), vector2[1]);
    }

    TYPED_TEST(TypedVectorTest, IteratorOnConstVectorWithInitialCapacityAndValues)
    {
        capu::Vector<TypeParam> vector(12, TypeParam(55u));

        const capu::Vector<TypeParam>& constVector = vector;

        capu::Vector<TypeParam> vector2(0);

        for (typename capu::Vector<TypeParam>::ConstIterator iter = constVector.begin(); iter != constVector.end(); ++iter)
        {
            vector2.push_back(*iter);
        }

        EXPECT_EQ(12u, vector2.size());
        EXPECT_EQ(TypeParam(55u), vector2[0]);
        EXPECT_EQ(TypeParam(55u), vector2[1]);
        EXPECT_EQ(TypeParam(55u), vector2[2]);
        EXPECT_EQ(TypeParam(55u), vector2[3]);
        EXPECT_EQ(TypeParam(55u), vector2[4]);
        EXPECT_EQ(TypeParam(55u), vector2[5]);
        EXPECT_EQ(TypeParam(55u), vector2[6]);
        EXPECT_EQ(TypeParam(55u), vector2[7]);
        EXPECT_EQ(TypeParam(55u), vector2[8]);
        EXPECT_EQ(TypeParam(55u), vector2[9]);
        EXPECT_EQ(TypeParam(55u), vector2[10]);
        EXPECT_EQ(TypeParam(55u), vector2[11]);
    }

    TYPED_TEST(TypedVectorTest, AccessOperator)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(42u));
        vector.push_back(TypeParam(47u));

        vector[0] = TypeParam(47u);
        vector[1] = TypeParam(42u);

        EXPECT_EQ(TypeParam(47u), vector[0]);
        EXPECT_EQ(TypeParam(42u), vector[1]);
    }

    TYPED_TEST(TypedVectorTest, Resize)
    {
        capu::Vector<TypeParam> vector(0);

        vector.push_back(TypeParam(1));
        vector.push_back(TypeParam(2));

        vector.resize(19);

        EXPECT_EQ(TypeParam(1u), vector[0]);
        EXPECT_EQ(TypeParam(2u), vector[1]);

        vector.resize(1);

        EXPECT_EQ(TypeParam(1u), vector[0]);
    }

    TYPED_TEST(TypedVectorTest, Clear)
    {
        TypeParam struct1 = TypeParam(47);

        TypeParam struct2 = TypeParam(8);

        capu::Vector<TypeParam> vector;

        vector.push_back(struct1);
        vector.push_back(struct2);

        vector.clear();

        vector.push_back(struct2);
        vector.push_back(struct1);

        EXPECT_EQ(struct2, vector[0]);
        EXPECT_EQ(struct1, vector[1]);
    }

    TYPED_TEST(TypedVectorTest, EraseIteratorFromMiddle)
    {
        capu::Vector<TypeParam> vector(0);

        vector.push_back(TypeParam(1));
        vector.push_back(TypeParam(2));
        vector.push_back(TypeParam(3));
        vector.push_back(TypeParam(4));
        vector.push_back(TypeParam(5));

        vector.erase(vector.begin() + 2u);

        EXPECT_EQ(4u, vector.size());
        NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(4);
        EXPECT_EQ(TypeParam(1), vector[0]);
        EXPECT_EQ(TypeParam(2), vector[1]);
        EXPECT_EQ(TypeParam(4), vector[2]);
        EXPECT_EQ(TypeParam(5), vector[3]);
    }

    TYPED_TEST(TypedVectorTest, EraseIterator)
    {
        capu::Vector<TypeParam> vector(0);

        vector.push_back(TypeParam(1));
        vector.push_back(TypeParam(2));
        vector.push_back(TypeParam(3));
        vector.push_back(TypeParam(4));
        vector.push_back(TypeParam(5));

        typename capu::Vector<TypeParam>::Iterator iter = vector.begin();

        EXPECT_EQ(capu::CAPU_EINVAL, vector.erase(iter + 8u));

        vector.erase(iter + 2u);

        EXPECT_EQ(4u, vector.size());
        NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(4);
        EXPECT_EQ(TypeParam(1), vector[0]);
        EXPECT_EQ(TypeParam(2), vector[1]);
        EXPECT_EQ(TypeParam(4), vector[2]);
        EXPECT_EQ(TypeParam(5), vector[3]);

        vector.erase(vector.begin());

        EXPECT_EQ(3u, vector.size());
        NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(3);
        EXPECT_EQ(TypeParam(2), vector[0]);
        EXPECT_EQ(TypeParam(4), vector[1]);
        EXPECT_EQ(TypeParam(5), vector[2]);

        vector.erase(vector.end() - 1u);

        EXPECT_EQ(2u, vector.size());
        NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(2);
        EXPECT_EQ(TypeParam(2), vector[0]);
        EXPECT_EQ(TypeParam(4), vector[1]);

        vector.erase(vector.begin());
        vector.erase(vector.begin());

        EXPECT_EQ(TypeParam(0u), vector.size());
        NumberOfExistingObjectsHelper<TypeParam>::assertNumberOfExistingObjectsEquals(0);
    }

    TYPED_TEST(TypedVectorTest, EraseIndex)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));
        vector.push_back(TypeParam(5u));

        typename capu::Vector<TypeParam>::Iterator iter = vector.begin();

        ++iter;
        ++iter;

        capu::status_t result = vector.erase(iter);

        EXPECT_TRUE(capu::CAPU_OK == result);
        EXPECT_EQ(4u, vector.size());
        EXPECT_EQ(TypeParam(1u), vector[0]);
        EXPECT_EQ(TypeParam(2u), vector[1]);
        EXPECT_EQ(TypeParam(4u), vector[2]);
        EXPECT_EQ(TypeParam(5u), vector[3]);

        result = vector.erase(vector.begin());

        EXPECT_TRUE(capu::CAPU_OK == result);
        EXPECT_EQ(3u, vector.size());
        EXPECT_EQ(TypeParam(2u), vector[0]);
        EXPECT_EQ(TypeParam(4u), vector[1]);
        EXPECT_EQ(TypeParam(5u), vector[2]);

        iter = vector.end();

        result = vector.erase(iter);

        EXPECT_FALSE(capu::CAPU_OK == result);
        EXPECT_EQ(3u, vector.size());
        EXPECT_EQ(TypeParam(2u), vector[0]);
        EXPECT_EQ(TypeParam(4u), vector[1]);
        EXPECT_EQ(TypeParam(5u), vector[2]);

        --iter;

        result = vector.erase(iter);

        EXPECT_TRUE(capu::CAPU_OK == result);
        EXPECT_EQ(2u, vector.size());
        EXPECT_EQ(TypeParam(2u), vector[0]);
        EXPECT_EQ(TypeParam(4u), vector[1]);

        result = vector.erase(vector.begin());

        EXPECT_TRUE(capu::CAPU_OK == result);

        result = vector.erase(vector.begin());

        EXPECT_TRUE(capu::CAPU_OK == result);
        EXPECT_EQ(0u, vector.size());
    }

    TYPED_TEST(TypedVectorTest, EraseWithElementOld)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));
        vector.push_back(TypeParam(5u));

        TypeParam old;

        vector.erase(vector.begin(), &old);
        EXPECT_EQ(old, TypeParam(1u));

        vector.erase(vector.begin()+2u, &old);
        EXPECT_EQ(old, TypeParam(4u));
    }

    TYPED_TEST(TypedVectorTest, EraseAdd)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(1u));
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(3u));
        vector.push_back(TypeParam(4u));
        vector.push_back(TypeParam(5u));

        vector.erase(3);
        vector.erase(1);
        vector.push_back(TypeParam(2u));
        vector.push_back(TypeParam(4u));

        EXPECT_EQ(5u, vector.size());
        EXPECT_EQ(TypeParam(1u), vector[0]);
        EXPECT_EQ(TypeParam(3u), vector[1]);
        EXPECT_EQ(TypeParam(5u), vector[2]);
        EXPECT_EQ(TypeParam(2u), vector[3]);
        EXPECT_EQ(TypeParam(4u), vector[4]);
    }

    TYPED_TEST(TypedVectorTest, ForEach)
    {
        capu::Vector<TypeParam> vector;

        vector.push_back(TypeParam(32));
        vector.push_back(TypeParam(43));
        vector.push_back(TypeParam(44));

        capu::Vector<TypeParam> testVector;

        capu_foreach(typename capu::Vector<TypeParam>, vector, iter)
        {
            testVector.push_back(*iter);
        }

        EXPECT_EQ(TypeParam(32), testVector[0]);
        EXPECT_EQ(TypeParam(43), testVector[1]);
        EXPECT_EQ(TypeParam(44), testVector[2]);

    }

    TYPED_TEST(TypedVectorTest, Compare)
    {
        capu::Vector<TypeParam> vector1;
        capu::Vector<TypeParam> vector2;
        capu::Vector<TypeParam> vector3;

        vector1.push_back(TypeParam(1));
        vector1.push_back(TypeParam(2));
        vector1.push_back(TypeParam(3));

        vector2.push_back(TypeParam(1));
        vector2.push_back(TypeParam(2));
        vector2.push_back(TypeParam(2));

        EXPECT_FALSE(vector1 == vector2);

        vector2[2] = TypeParam(3);

        EXPECT_TRUE(vector1 == vector2);

        EXPECT_FALSE(vector3 == vector2);

    }

    TYPED_TEST(TypedVectorTest, TestFrontBack)
    {
        capu::Vector<TypeParam> vec;
        vec.push_back(TypeParam(1));
        vec.push_back(TypeParam(2));
        vec.push_back(TypeParam(3));

        const capu::Vector<TypeParam>& constVec = vec;
        EXPECT_EQ(TypeParam(1), vec.front());
        EXPECT_EQ(TypeParam(1), constVec.front());

        EXPECT_EQ(TypeParam(3), vec.back());
        EXPECT_EQ(TypeParam(3), constVec.back());
    }

    TEST(VectorTest, CompareComplexType)
    {
        capu::Vector<capu::String> vector1;
        capu::Vector<capu::String> vector2;
        capu::Vector<capu::String> vector3;

        vector1.push_back("test");
        vector2.push_back("something");

        EXPECT_FALSE(vector1 == vector2);

        vector2.clear();
        vector2.push_back("test");

        EXPECT_TRUE(vector1 ==  vector2);

        EXPECT_FALSE(vector3 == vector2);
    }

    TEST(VectorTest, iteratorDifference)
    {
        capu::Vector<capu::uint_t> vec(3, 0);

        EXPECT_EQ(3, vec.end() - vec.begin());
        EXPECT_EQ(-3, vec.begin() - vec.end());
        EXPECT_EQ(0, vec.begin() - vec.begin());
    }

    TEST(VectorTest, IteratorAccessOperators)
    {
        capu::Vector<ComplexTestType> vec(3, 0);

        capu::Vector<ComplexTestType>::Iterator it = vec.begin();
        EXPECT_EQ(0u, (*it).value);
        EXPECT_EQ(0u, it->value);
    }

    TEST(VectorTest, ConstIteratorAccessOperators)
    {
        capu::Vector<ComplexTestType> vec(3, 0);
        const capu::Vector<ComplexTestType>& constVec = vec;

        capu::Vector<ComplexTestType>::ConstIterator it = constVec.begin();
        EXPECT_EQ(0u, (*it).value);
        EXPECT_EQ(0u, it->value);
    }

    TEST(VectorTest, relationalOpsOnIterators)
    {
        capu::Vector<capu::uint_t> vec(2, 0);

        EXPECT_TRUE(vec.begin() == vec.begin());
        EXPECT_TRUE(vec.begin() != vec.end());
        EXPECT_TRUE(vec.begin() < vec.begin() + 1u);
        EXPECT_TRUE(vec.begin() + 1u > vec.begin());
    }

    TEST(VectorTest, relationalOpsOnIteratorsForConstValues)
    {
        capu::Vector<const capu::uint_t*> vec(2, 0);

        EXPECT_TRUE(vec.begin() == vec.begin());
        EXPECT_TRUE(vec.begin() != vec.end());
        EXPECT_TRUE(vec.begin() < vec.begin() + 1u);
        EXPECT_TRUE(vec.begin() + 1u > vec.begin());
    }

    TEST(VectorTest, relationalOpsOnConstIterators)
    {
        capu::Vector<capu::uint_t> vec(2, 0);
        const capu::Vector<capu::uint_t>& vecConst = vec;

        EXPECT_TRUE(vecConst.begin() == vecConst.begin());
        EXPECT_TRUE(vecConst.begin() != vecConst.end());
        EXPECT_TRUE(vecConst.begin() < vecConst.begin() + 1u);
        EXPECT_TRUE(vecConst.begin() + 1u > vecConst.begin());
    }

    TEST(VectorTest, relationalOpsOnMixedteratorAndConstIterators)
    {
        capu::Vector<capu::uint_t> vec(2, 0);
        const capu::Vector<capu::uint_t>& vecConst = vec;

        EXPECT_TRUE(vecConst.begin() == vec.begin());
        EXPECT_TRUE(vecConst.begin() != vec.end());

        EXPECT_TRUE(vec.begin() == vecConst.begin());
        EXPECT_TRUE(vec.begin() != vecConst.end());
    }

    TEST(VectorTest, SwapVectorViaMemberFunction)
    {
        capu::Vector<capu::uint_t> vec(3);
        vec[0] = 1;
        vec[1] = 2;
        vec[2] = 3;
        vec.reserve(20);
        capu::Vector<capu::uint_t> other(1);
        other[0] = 11;

        vec.swap(other);
        EXPECT_EQ(1u, vec.capacity());
        EXPECT_EQ(20u, other.capacity());
        ASSERT_EQ(1u, vec.size());
        EXPECT_EQ(11u, vec[0]);
        ASSERT_EQ(3u, other.size());
        EXPECT_EQ(1u, other[0]);
        EXPECT_EQ(2u, other[1]);
        EXPECT_EQ(3u, other[2]);
    }

    TEST(VectorTest, SwapVectorViaSwapFreeFunction)
    {
        capu::Vector<capu::uint_t> vec(3);
        vec[0] = 1;
        vec[1] = 2;
        vec[2] = 3;
        vec.reserve(20);
        capu::Vector<capu::uint_t> other(1);
        other[0] = 11;

        using capu::swap;
        swap(vec, other);
        EXPECT_EQ(1u, vec.capacity());
        EXPECT_EQ(20u, other.capacity());
        ASSERT_EQ(1u, vec.size());
        EXPECT_EQ(11u, vec[0]);
        ASSERT_EQ(3u, other.size());
        EXPECT_EQ(1u, other[0]);
        EXPECT_EQ(2u, other[1]);
        EXPECT_EQ(3u, other[2]);
    }

    TEST(VectorTest, Shrink)
    {
        capu::Vector<capu::uint_t> vec(3, 0);
        vec[0] = 1;
        vec[1] = 2;
        vec[2] = 3;
        vec.reserve(20);
        EXPECT_EQ(20u, vec.capacity());

        vec.shrink_to_fit();
        EXPECT_EQ(3u, vec.capacity());
        EXPECT_EQ(3u, vec.size());

        vec.shrink_to_fit();
        EXPECT_EQ(3u, vec.capacity());
        EXPECT_EQ(3u, vec.size());

        EXPECT_EQ(1u, vec[0]);
        EXPECT_EQ(2u, vec[1]);
        EXPECT_EQ(3u, vec[2]);
    }

    TEST(VectorTest, ExponentialGrowOnPushBack)
    {
        capu::Vector<capu::uint_t> vec;
        EXPECT_EQ(0u, vec.capacity());
        vec.push_back(0);
        EXPECT_EQ(1u, vec.capacity());
        vec.push_back(0);
        EXPECT_EQ(2u, vec.capacity());
        vec.push_back(0);
        EXPECT_EQ(4u, vec.capacity());
        vec.push_back(0);
        vec.push_back(0);
        EXPECT_EQ(8u, vec.capacity());
    }

    TEST(VectorTest, ExponentialGrowOnSmallResize)
    {
        capu::Vector<capu::uint_t> vec;
        EXPECT_EQ(0u, vec.capacity());
        vec.resize(vec.size() + 1);
        EXPECT_EQ(1u, vec.capacity());
        vec.resize(vec.size() + 1);
        EXPECT_EQ(2u, vec.capacity());
        vec.resize(vec.size() + 1);
        EXPECT_EQ(4u, vec.capacity());
        vec.resize(vec.size() + 2);
        EXPECT_EQ(8u, vec.capacity());
    }

    TEST(VectorTest, ExponentialGrowOnInsertSingle)
    {
        capu::Vector<capu::uint_t> vec;
        EXPECT_EQ(0u, vec.capacity());
        vec.insert(vec.end(), 0);
        EXPECT_EQ(1u, vec.capacity());
        vec.insert(vec.end(), 0);
        EXPECT_EQ(2u, vec.capacity());
        vec.insert(vec.end(), 0);
        EXPECT_EQ(4u, vec.capacity());
        vec.insert(vec.end(), 0);
        vec.insert(vec.end(), 0);
        EXPECT_EQ(8u, vec.capacity());
    }

    TEST(VectorTest, ExponentialGrowOnInsertSmallRange)
    {
        capu::Vector<capu::uint_t> vec;
        capu::Vector<capu::uint_t> rangeVec(1);

        EXPECT_EQ(0u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.end());
        EXPECT_EQ(1u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.end());
        EXPECT_EQ(2u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.end());
        EXPECT_EQ(4u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.end());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.end());
        EXPECT_EQ(8u, vec.capacity());
    }

    TEST(VectorTest, MixedGrowthBehaviorOnResize)
    {
        capu::Vector<capu::uint_t> vec;
        EXPECT_EQ(0u, vec.capacity());
        vec.resize(1);
        EXPECT_EQ(1u, vec.capacity());
        vec.resize(3);
        EXPECT_EQ(3u, vec.capacity());
        vec.resize(4);
        EXPECT_EQ(6u, vec.capacity());
        vec.resize(15);
        EXPECT_EQ(15u, vec.capacity());
    }

    TEST(VectorTest, MixedGrowthBehaviorOnInsertRange)
    {
        capu::Vector<capu::uint_t> vec;
        capu::Vector<capu::uint_t> rangeVec(11);

        EXPECT_EQ(0u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.begin() + 1);
        EXPECT_EQ(1u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.begin() + 2);
        EXPECT_EQ(3u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.begin() + 1);
        EXPECT_EQ(6u, vec.capacity());
        vec.insert(vec.end(), rangeVec.begin(), rangeVec.end());
        EXPECT_EQ(15u, vec.capacity());
    }
