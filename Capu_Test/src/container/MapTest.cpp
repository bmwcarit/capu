/*
 * Copyright (C) 2015 BMW Car IT GmbH
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
#include "capu/container/Map.h"
#include "capu/container/Vector.h"
#include "capu/container/List.h"
#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/container/String.h"

namespace capu
{

    /// Value class for the test map "Map1".
    class Value
    {
    public:

        /// Constructor.
        /** @param value The value to store. */
        Value(uint32_t value = 99) :
            m_value(value)
        {

        }

        /// Copy-constructor.
        /** @param other The value to copy. */
        Value(const Value& other) :
            m_value(other.m_value)
        {

        }

        /// Return the stored value.
        /** @return The value. */
        uint32_t get()
        {
            return m_value;
        }

    protected:

        /// The stored value.
        uint32_t m_value;
    };

    /// Key/value pair.
    /** Used to store elements in a comparison list. */
    class KeyValue
    {
    public:

        /// Constructor.
        /** @param key The key.
         *  @param value The value. */
        KeyValue(int32_t key = 0, const Value& value = Value()) :
            m_key(key),
            m_value(value)
        {
        }

        /// Copy-constructor.
        /** @param other The key/value pair to copy. */
        KeyValue(const KeyValue& other) :
            m_key(other.m_key),
            m_value(other.m_value)
        {

        }

        /// The key.
        int32_t m_key;

        /// The value.
        Value m_value;
    };

    /// First test map class.
    typedef Map<int32_t, Value> Map1;

    /// Second test map class.
    typedef Map<String, float> Map2;

    /// Test class for the map container class.
    class MapTest : public testing::Test
    {
    public:

        /// Constructor.
        MapTest();

        /// Destructor.
        ~MapTest();

    protected:

        /// Checks the consistency of m_map1.
        /** @return "true", when m_map is consistent and "false" otherwise. */
        bool checkConsistency1();

        /// Checks the consistency of m_map2.
        /** @return "true", when m_map2 is consistent and "false" otherwise. */
        bool checkConsistency2();

        /// Returns the lower bound element from m_map with respect to a given key.
        /** The lower bound element is the smallest element, with a greater or equal key than the given key.
         *  The complexity is O(log(n)). This is also an example, how to use the Map::traverse function to implement
         *  such a custom search function.
         *  @param key The key for comparison.
         *  @return Iterator to the found element. */
        Map1::Iterator lower_bound(int32_t key);

        /// Iterator for iterating over m_map1.
        Map1::Iterator m_i;

        /// The first test map.
        Map1 m_map1;

        /// The second test map.
        Map2 m_map2;
    };

    MapTest::MapTest()
    {
    }

    MapTest::~MapTest()
    {
    }

    bool MapTest::checkConsistency1()
    {
        return m_map1.checkConsistency();
    }

    bool MapTest::checkConsistency2()
    {
        return m_map2.checkConsistency();
    }

    /// Finds the insertion position for a new element.
    /** @param list The list of elements, where the new element shall be inserted.
     *  @param element The new element.
     *  @return Iterator to the element, that goes directly after the new element.
     *          end(), when the new element shall be inserted at the end of the list.
     */
    List<KeyValue>::Iterator findInsertPos(List<KeyValue>& list, const KeyValue& element)
    {
        List<KeyValue>::Iterator i = list.begin();
        for (; i != list.end(); ++i)
        {
            if (i->m_key >= element.m_key)
            {
                return i;
            }
        }
        return i;
    }

    /// Inserts an element to a list, when no element with the same key already exists.
    /** @param list The list to insert the new element to.
     *  @param element The new element to insert.
     *  @return "true", when the element was inserted. */
    bool insert(List<KeyValue>& list, const KeyValue& element)
    {
        List<KeyValue>::Iterator i = findInsertPos(list, element);
        if (i != list.end())
        {
            if (i->m_key == element.m_key)
            {
                return false;
            }
            list.insert(i, element);
        }
        else
        {
            list.insert(element);
        }
        return true;
    }

    /// Inserts an element to a list or changes an already existing element with the same key.
    /** @param list The list to insert or change the element.
     *  @param element The new element to insert. */
    void insertOrChange(List<KeyValue>& list, const KeyValue& element)
    {
        List<KeyValue>::Iterator i = findInsertPos(list, element);
        if (i != list.end())
        {
            if (i->m_key == element.m_key)
            {
                i->m_value = element.m_value;
            }
            else
            {
                list.insert(i, element);
            }
        }
        else
        {
            list.insert(element);
        }
    }

#define BEGIN_CHECK()\
{\
    ASSERT_EQ(true, checkConsistency1());\
}

#define END_CHECK()\
{\
    uint32_t i(0);\
    for (m_i = m_map1.begin(); m_i != m_map1.end(); ++m_i)\
    {\
        ASSERT_EQ(keys[i], m_i->first);\
        ASSERT_EQ(values[i], m_i->second.get());\
        i++;\
    }\
    i = uint32_t(keys.size()) - 1;\
    for (m_i = m_map1.back(); m_i != m_map1.end(); --m_i)\
    {\
        ASSERT_EQ(keys[i], m_i->first);\
        ASSERT_EQ(values[i], m_i->second.get());\
        i--;\
    }\
    keys.clear();\
    values.clear();\
}

#define CHECK(KEY, VALUE)\
{\
    keys.push_back(KEY);\
    values.push_back(VALUE);\
}

    TEST_F(MapTest, insertTest)
    {
        m_i = m_map1.begin();

        Vector<int32_t> keys;
        Vector<uint32_t> values;

        BEGIN_CHECK()
        END_CHECK()

        ASSERT_EQ(m_i, m_map1.end());

        ASSERT_TRUE(m_map1.insert(5, Value(15)));

        BEGIN_CHECK()

        CHECK(5, 15);

        END_CHECK()

        ASSERT_TRUE(m_map1.insert(9, Value(17)));

        BEGIN_CHECK()
        CHECK(5, 15)
        CHECK(9, 17)
        END_CHECK()

        ASSERT_TRUE(m_map1.insert(7, Value(19)));

        BEGIN_CHECK()
        CHECK(5, 15)
        CHECK(7, 19)
        CHECK(9, 17)
        END_CHECK()

        ASSERT_FALSE(m_map1.insert(7, Value(18)));

        BEGIN_CHECK()
        CHECK(5, 15)
        CHECK(7, 19)
        CHECK(9, 17)
        END_CHECK()

    }

    TEST_F(MapTest, insertTestBig)
    {

        List<KeyValue> m_list;

        for (uint32_t i = 0; i < 1000; i++)
        {
            KeyValue element(rand(), Value(i));

            bool insertedMap = m_map1.insert(element.m_key, element.m_value);

            bool insertedList = insert(m_list, element);

            ASSERT_EQ(insertedMap, insertedList);

            ASSERT_TRUE(checkConsistency1());

            m_i = m_map1.begin();
            for (List<KeyValue>::Iterator listI = m_list.begin(); listI != m_list.end(); ++listI)
            {
                EXPECT_EQ(listI->m_key, m_i->first);
                EXPECT_EQ(listI->m_value.get(), m_i->second.get());

                Map<int32_t, Value>::Iterator ifind = m_map1.find(listI->m_key);

                EXPECT_EQ(ifind, m_i);

                ++m_i;
            }
            EXPECT_EQ(m_i, m_map1.end());

        }
    }

    TEST_F(MapTest, insertOperatorTestBig)
    {
        List<KeyValue> m_list;

        for (uint32_t i = 0; i < 1000; i++)
        {
            KeyValue element(rand() % 1000, Value(i));

            m_map1[element.m_key] = element.m_value;

            insertOrChange(m_list, element);

            ASSERT_TRUE(checkConsistency1());

            m_i = m_map1.begin();
            for (List<KeyValue>::Iterator listI = m_list.begin(); listI != m_list.end(); ++listI)
            {
                EXPECT_EQ(listI->m_key, m_i->first);
                EXPECT_EQ(listI->m_value.get(), m_i->second.get());

                Map1::Iterator ifind = m_map1.find(listI->m_key);

                EXPECT_EQ(ifind, m_i);

                ++m_i;
            }
            EXPECT_EQ(m_i, m_map1.end());
        }

    }

    TEST_F(MapTest, insertKeyStringValueFloat)
    {
        m_map2["AWESOME_TESTSTRING"] = 123.0;
        m_map2["Alex"] = 8.5;
        m_map2["Map"] = 456.0;

        EXPECT_EQ(8.5, m_map2["Alex"]);
        EXPECT_EQ(456.0, m_map2["Map"]);
        EXPECT_EQ(123.0, m_map2["AWESOME_TESTSTRING"]);
        EXPECT_EQ(0.0, m_map2["ANOTHER_KEY"]);

        m_map2["ANOTHER_KEY"] = 69.0;
        EXPECT_EQ(69.0, m_map2["ANOTHER_KEY"]);

        checkConsistency2();
    }

    TEST_F(MapTest, findKeyStringValueFloat)
    {
        m_map2["AWESOME_TESTSTRING"] = 123.0;
        m_map2["Alex"] = 8.5;
        m_map2["Map"] = 456.0;

        Map<String, float>::Iterator i;

        i = m_map2.find("AWESOME_TESTSTRING");
        EXPECT_EQ(i->first, "AWESOME_TESTSTRING");
        EXPECT_EQ(i->second, 123.0);

        i = m_map2.find("Alex");
        EXPECT_EQ(i->first, "Alex");
        EXPECT_EQ(i->second, 8.5);

        i = m_map2.find("Map");
        EXPECT_EQ(i->first, "Map");
        EXPECT_EQ(i->second, 456.0);

        i = m_map2.find("ANOTHER_KEY");
        EXPECT_EQ(i, m_map2.end());

        m_map2["ANOTHER_KEY"] = 69.0;

        i = m_map2.find("ANOTHER_KEY");

        EXPECT_EQ(i->first, "ANOTHER_KEY");
        EXPECT_EQ(i->second, 69.0);

        checkConsistency2();
    }

    TEST_F(MapTest, lower_bound)
    {
        m_map1[16] = 0;
        m_map1[2] = 1;
        m_map1[12] = 2;
        m_map1[6] = 3;
        m_map1[8] = 4;
        m_map1[4] = 5;
        m_map1[14] = 6;

        checkConsistency1();

        Map1::Iterator i = m_map1.lower_bound(10);
        EXPECT_EQ(i->first, 12);

        i = m_map1.lower_bound(12);
        EXPECT_EQ(i->first, 12);

        i = m_map1.lower_bound(1);
        EXPECT_EQ(i->first, 2);

        i = m_map1.lower_bound(2);
        EXPECT_EQ(i->first, 2);

        i = m_map1.lower_bound(16);
        EXPECT_EQ(i->first, 16);

        i = m_map1.lower_bound(17);
        EXPECT_EQ(i, m_map1.end());

        i = m_map1.lower_bound(-1);
        EXPECT_EQ(i->first, 2);
    }

    TEST_F(MapTest, upper_bound)
    {
        m_map1[16] = 0;
        m_map1[2] = 1;
        m_map1[12] = 2;
        m_map1[6] = 3;
        m_map1[8] = 4;
        m_map1[4] = 5;
        m_map1[14] = 6;

        checkConsistency1();

        Map1::Iterator i = m_map1.upper_bound(10);
        EXPECT_EQ(i->first, 12);

        i = m_map1.upper_bound(12);
        EXPECT_EQ(i->first, 14);

        i = m_map1.upper_bound(1);
        EXPECT_EQ(i->first, 2);

        i = m_map1.upper_bound(2);
        EXPECT_EQ(i->first, 4);

        i = m_map1.upper_bound(16);
        EXPECT_EQ(i, m_map1.end());

        i = m_map1.upper_bound(17);
        EXPECT_EQ(i, m_map1.end());

        i = m_map1.upper_bound(-1);
        EXPECT_EQ(i->first, 2);
    }

    class VisitAlways : public Map1::UnaryFunction
    {
    public:
        VisitAlways()
        {
        }
        bool operator()(const Map1::Iterator&)
        {
            return true;
        }
    };

    TEST_F(MapTest, traverse_inorder)
    {
        m_map1[16] = 6;
        m_map1[2] = 0;
        m_map1[12] = 4;
        m_map1[6] = 2;
        m_map1[8] = 3;
        m_map1[4] = 1;
        m_map1[14] = 5;

        class Visit : public Map1::UnaryFunction
        {
        public:
            Visit() :
                m_i(0)
            {
            }
            bool operator()(const Map1::Iterator& element)
            {
                EXPECT_EQ(element->second.get(), m_i++);
                return true;
            }
        protected:
            uint32_t m_i;
        };

        Visit visit;
        VisitAlways visitAlways;

        m_map1.traverse(visit, visitAlways, visitAlways, Map1::IN_ORDER);
    }

    TEST_F(MapTest, traverse_preorder)
    {
        m_map1[16] = 5;
        m_map1[2] = 1;
        m_map1[12] = 3;
        m_map1[6] = 0;
        m_map1[8] = 4;
        m_map1[4] = 2;
        m_map1[14] = 6;

        class Visit : public Map1::UnaryFunction
        {
        public:
            Visit() :
                m_i(0)
            {
            }
            bool operator()(const Map1::Iterator& element)
            {
                EXPECT_EQ(element->second.get(), m_i++);
                return true;
            }
        protected:
            uint32_t m_i;
        };

        Visit visit;
        VisitAlways visitAlways;

        m_map1.traverse(visit, visitAlways, visitAlways, Map1::PRE_ORDER);
    }

    TEST_F(MapTest, traverse_postorder)
    {
        m_map1[16] = 4;
        m_map1[2] = 1;
        m_map1[12] = 5;
        m_map1[6] = 6;
        m_map1[8] = 2;
        m_map1[4] = 0;
        m_map1[14] = 3;

        class Visit : public Map1::UnaryFunction
        {
        public:
            Visit() :
                m_i(0)
            {
            }
            bool operator()(const Map1::Iterator& element)
            {
                EXPECT_EQ(element->second.get(), m_i++);
                return true;
            }
        protected:
            uint32_t m_i;
        };

        Visit visit;
        VisitAlways visitAlways;

        m_map1.traverse(visit, visitAlways, visitAlways, Map1::POST_ORDER);
    }

    TEST_F(MapTest, traverse_inorderMirrored)
    {
        m_map1[16] = 0;
        m_map1[2] = 6;
        m_map1[12] = 2;
        m_map1[6] = 4;
        m_map1[8] = 3;
        m_map1[4] = 5;
        m_map1[14] = 1;

        class Visit : public Map1::UnaryFunction
        {
        public:
            Visit(MapTest& test) :
                m_test(test),
                m_i(0)
            {
            }
            bool operator()(const Map1::Iterator& element)
            {
                EXPECT_EQ(element->second.get(), m_i++);
                return true;
            }
        protected:
            MapTest& m_test;
            uint32_t m_i;
        };

        Visit visit(*this);
        VisitAlways visitAlways;

        m_map1.traverse(visit, visitAlways, visitAlways, Map1::IN_ORDER_MIRRORED);
    }

    TEST_F(MapTest, traverse_preorderMirrored)
    {
        m_map1[16] = 2;
        m_map1[2] = 5;
        m_map1[12] = 1;
        m_map1[6] = 0;
        m_map1[8] = 4;
        m_map1[4] = 6;
        m_map1[14] = 3;

        class Visit : public Map1::UnaryFunction
        {
        public:
            Visit(MapTest& test) :
                m_test(test),
                m_i(0)
            {
            }
            bool operator()(const Map1::Iterator& element)
            {
                EXPECT_EQ(element->second.get(), m_i++);
                return true;
            }
        protected:
            MapTest& m_test;
            uint32_t m_i;
        };

        Visit visit(*this);
        VisitAlways visitAlways;

        m_map1.traverse(visit, visitAlways, visitAlways, Map1::PRE_ORDER_MIRRORED);
    }

    TEST_F(MapTest, traverse_postorderMirrored)
    {
        m_map1[16] = 1;
        m_map1[2] = 5;
        m_map1[12] = 3;
        m_map1[6] = 6;
        m_map1[8] = 2;
        m_map1[4] = 4;
        m_map1[14] = 0;

        class Visit : public Map1::UnaryFunction
        {
        public:
            Visit(MapTest& test) :
                m_test(test),
                m_i(0)
            {
            }
            bool operator()(const Map1::Iterator& element)
            {
                EXPECT_EQ(element->second.get(), m_i++);
                return true;
            }
        protected:
            MapTest& m_test;
            uint32_t m_i;
        };

        Visit visit(*this);
        VisitAlways visitAlways;

        m_map1.traverse(visit, visitAlways, visitAlways, Map1::POST_ORDER_MIRRORED);
    }

    TEST_F(MapTest, eraseTestBig)
    {
        List<KeyValue> m_list;

        uint32_t nElements = 1000;
        for (uint32_t i = 0; i < nElements; i++)
        {
            KeyValue element(rand(), Value(i));

            m_map1[element.m_key] = element.m_value;

            insertOrChange(m_list, element);
        }

        ASSERT_TRUE(checkConsistency1());

        while (!m_list.empty())
        {
            uint32_t n = uint32_t(m_list.size());

            uint32_t i = rand() % n;
            int32_t key = m_list.get(i).m_key;

            m_list.erase(i);

            EXPECT_TRUE(m_map1.erase(key));

            ASSERT_TRUE(checkConsistency1());

            m_i = m_map1.begin();
            for (List<KeyValue>::Iterator listI = m_list.begin(); listI != m_list.end(); ++listI)
            {
                EXPECT_EQ(listI->m_key, m_i->first);
                EXPECT_EQ(listI->m_value.get(), m_i->second.get());

                Map1::Iterator ifind = m_map1.find(listI->m_key);

                EXPECT_EQ(ifind, m_i);

                ++m_i;
            }
            EXPECT_EQ(m_i, m_map1.end());
        }

    }

    TEST_F(MapTest, eraseByIteratorTest)
    {
        List<KeyValue> m_list;

        for (uint32_t i = 0; i < 1000; i++)
        {
            KeyValue element(rand(), Value(i));

            m_map1[element.m_key] = element.m_value;

            insertOrChange(m_list, element);
        }

        ASSERT_TRUE(checkConsistency1());

        Map1::Iterator it = m_map1.begin();

        while (!m_list.empty())
        {
            m_list.erase(0);

            Map<int32_t, Value>::Iterator itNext = it;
            ++itNext;
            m_map1.erase(it);
            it = itNext;

            ASSERT_TRUE(checkConsistency1());

            m_i = m_map1.begin();
            for (List<KeyValue>::Iterator listI = m_list.begin(); listI != m_list.end(); ++listI)
            {
                EXPECT_EQ(listI->m_key, m_i->first);
                EXPECT_EQ(listI->m_value.get(), m_i->second.get());

                Map1::Iterator ifind = m_map1.find(listI->m_key);

                EXPECT_EQ(ifind, m_i);

                ++m_i;
            }
            EXPECT_EQ(m_i, m_map1.end());

        }
    }

    TEST_F(MapTest, eraseByKeyTest)
    {
        Vector<int32_t> keys;
        Vector<uint32_t> values;

        m_map1[16] = 1;
        m_map1[2] = 5;
        m_map1[12] = 3;
        m_map1[6] = 6;
        m_map1[8] = 2;
        m_map1[4] = 4;
        m_map1[14] = 0;

        ASSERT_TRUE(checkConsistency1());
        EXPECT_TRUE(m_map1.erase(8));

        BEGIN_CHECK()
        CHECK(2, 5)
        CHECK(4, 4)
        CHECK(6, 6)
        CHECK(12, 3)
        CHECK(14, 0)
        CHECK(16, 1)
        END_CHECK()

        EXPECT_TRUE(m_map1.erase(14));

        BEGIN_CHECK()
        CHECK(2, 5)
        CHECK(4, 4)
        CHECK(6, 6)
        CHECK(12, 3)
        CHECK(16, 1)
        END_CHECK()

        EXPECT_FALSE(m_map1.erase(7));

        BEGIN_CHECK()
        CHECK(2, 5)
        CHECK(4, 4)
        CHECK(6, 6)
        CHECK(12, 3)
        CHECK(16, 1)
        END_CHECK()

        EXPECT_FALSE(m_map1.erase(14));

        BEGIN_CHECK()
        CHECK(2, 5)
        CHECK(4, 4)
        CHECK(6, 6)
        CHECK(12, 3)
        CHECK(16, 1)
        END_CHECK()

        EXPECT_TRUE(m_map1.erase(16));

        BEGIN_CHECK()
        CHECK(2, 5)
        CHECK(4, 4)
        CHECK(6, 6)
        CHECK(12, 3)
        END_CHECK()

        EXPECT_TRUE(m_map1.erase(2));

        BEGIN_CHECK()
        CHECK(4, 4)
        CHECK(6, 6)
        CHECK(12, 3)
        END_CHECK()

        EXPECT_TRUE(m_map1.erase(4));

        BEGIN_CHECK()
        CHECK(6, 6)
        CHECK(12, 3)
        END_CHECK()

        EXPECT_TRUE(m_map1.erase(12));

        BEGIN_CHECK()
        CHECK(6, 6)
        END_CHECK()

        EXPECT_TRUE(m_map1.erase(6));

        BEGIN_CHECK()
        END_CHECK()

        EXPECT_FALSE(m_map1.erase(6));

        BEGIN_CHECK()
        END_CHECK()

    }

    TEST_F(MapTest, clearTest)
    {
        m_map1[16] = 1;
        m_map1[2] = 5;
        m_map1[12] = 3;
        m_map1[6] = 6;
        m_map1[8] = 2;
        m_map1[4] = 4;
        m_map1[14] = 0;

        m_map1.clear();

        ASSERT_TRUE(checkConsistency1());
        ASSERT_TRUE(m_map1.begin() == m_map1.end());
        ASSERT_TRUE(m_map1.find(16) == m_map1.end());
        ASSERT_TRUE(m_map1.find(2) == m_map1.end());
        ASSERT_TRUE(m_map1.find(12) == m_map1.end());
        ASSERT_TRUE(m_map1.find(6) == m_map1.end());
        ASSERT_TRUE(m_map1.find(8) == m_map1.end());
        ASSERT_TRUE(m_map1.find(4) == m_map1.end());
        ASSERT_TRUE(m_map1.find(14) == m_map1.end());
    }

}

