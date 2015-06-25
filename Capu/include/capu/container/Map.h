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

#ifndef CAPU_Map_H
#define CAPU_Map_H

#include "capu/container/Pair.h"

namespace capu
{
    /// Implementation of a map that stores elements of type T sorted by their keys of type Key.
    /** NOTE: Not STL compatible.
     *  Each element must have a key, that is unique in the map.
     *  For key comparison the "<" operator must be defined for the type Key.
     *  Elements can be inserted, searched and removed from the map in O(log(n)).
     *  The iterator can be used to navigate forward and backward to the next, previous element
     *  in the sorting order (O(log(n)).
     */
    template<class Key, class T>
        class Map
        {
        private:
            class Node;

        public:

            class Iterator;
            class UnaryFunction;

            /// Traversal orders of the binary search tree, used in the "traverse" function.
            enum EOrder
            {
                PRE_ORDER, IN_ORDER, POST_ORDER, PRE_ORDER_MIRRORED, IN_ORDER_MIRRORED, POST_ORDER_MIRRORED
            };

            /// Constuctor.
            /** Creates an empty map. */
            Map();

            /// Destructor.
            ~Map();

            /// Inserts an element by it's unique key into the map.
            /** If an element with the key already exists the new element is not inserted.
             *  @param key Key of the new element to insert.
             *  @param value The value to insert.
             *  @return "true", when the element was inserted and "false" otherwise. */
            bool_t insert(const Key& key, const T& value);

            /// Returns an existing element by it's key, if it already exists, or creates and inserts a new one.
            /** New elements are created by the default constructor of T.
             *  @param key Key for searching the element.
             *  @return The found or newly created element. */
            T& operator[](const Key& key);

            /// Returns the element by it's key.
            /** The returned iterator will remain valid all time, until the element is deleted.
             *  @param key Key for searching the element.
             *  @return Iterator to the element, when found. Equal to Map::end() otherwise. */
            Iterator find(const Key& key);

            /// Traverse the elements in the binary search tree in a specified traversal order.
            /** The elements of the left subtree are smaller than the current element, while all elements in the
             *  right subtree are greater than the current element.
             *  Three unary function objects are given for visiting elements and controlling the traversing.
             *  Use derived classes to implement the desired behavior.
             *  @param visit The visit function object is called for each element to traverse. Traversing is stopped, when this
             *  function returns "false".
             *  @param visitSmaller The visitSmaller function object is called to check if elements that are smaller than the
             *  given element shall be traversed or not. When "false" is returned, smaller elements are not traversed.
             *  @param visitGreater The visitGreater function object is called to check if elements that are greater than the
             *  given element shall be traversed or not. When "false" is returned, greater elements are not traversed.
             *  @param order The traversal order. */
            void traverse(UnaryFunction& visit, UnaryFunction& visitSmaller, UnaryFunction& visitGreater, EOrder order);

            /// Erases an element from the map given by it's iterator.
            /** The iterator will become invalid afterwards.
             *  @param element Iterator to the element to erase. */
            void erase(Iterator& element);

            /// Erases an element from the map given by it's key.
            /** @param key Key for searching the element.
             *  @return "true", when the element was erased and "false" otherwise. */
            bool_t erase(const Key& key);

            /// Returns an iterator to the first element in the map.
            /** The first element has the smallest key in the map.
             *  The returned iterator will remain valid all time, until the element is deleted.
             *  @return Iterator to the first element. Equal to Map::end() when the map is empty. */
            Iterator begin();

            /** Returns an iterator to the past-the-end element of the map.
             *  @return Iterator to the past-the-end element (not a valid element). */
            Iterator end();

            /// Returns an iterator to the last element in the map.
            /** The last element has the greatest key in the map.
             *  The returned iterator will remain valid all time, until the element is deleted.
             *  @return Iterator to the last element. Equal to Map::end() when the map is empty. */
            Iterator back();

            /// Removes all elements from the map.
            void clear();

            /// Returns the lower bound element from the map with respect to a given key.
            /** The lower bound element is the smallest element, with a greater or equal key than the given key.
             *  The complexity is O(log(n)).
             *  @param key The key for comparison.
             *  @return Iterator to the found element or Map::end() if none found. */
            Iterator lower_bound(const Key& key);

            /// Returns the upper bound element from the map with respect to a given key.
            /** The upper bound element is the smallest element, with a greater key than the given key.
             *  The complexity is O(log(n)).
             *  @param key The key for comparison.
             *  @return Iterator to the found element or Map::end() if none found. */
            Iterator upper_bound(const Key& key);

            /// Iterator class for elements in the map.
            /** Points to an element in the map or to the dummy past-the-end element, returned by
             *  Map::end().
             *  The referenced element can be accessed as a pair of key, element:
             *  iterator->first or (*iterator).first accesses the element key.
             *  iterator->second or (*iterator).second accesses the element value. */
            class Iterator
            {
            public:
                friend class Map<Key, T> ;

                /// Constructor.
                /** Points to the past-the-end element Map::end(). */
                Iterator();

                /// Sets the current position to the next element.
                /** Points to Map::end(), when there is no next element.
                 *  @return The iterator itself. */
                Iterator& operator++();

                /// Sets the current position to the previous element.
                /** Points to Map::end(), when there is no previous element.
                 *  When the iterator points to Map::end() before this operation,
                 *  it is unchanged.
                 *  @return The iterator itself. */
                Iterator& operator--();

                /// Compares if two iterators point to the same element.
                /** @param other The second iterator to compare with.
                 *  @return "true", when both iterators point to the same element. */
                bool_t operator==(const Iterator& other) const;

                /// Compares if two iterators point to different elements.
                /** @param other The second iterator to compare with.
                 *  @return "true", when both iterators point to different elements. */
                bool_t operator!=(const Iterator& other) const;

                /// Dereferences the iterator to access the element as key, value pair.
                /** @return A reference to a pair holding the key and value. */
                Pair<const Key, T>& operator*() const;

                /// Dereferences the iterator to access the element as key, value pair.
                /** @return A pointer to a pair holding the key and value. */
                Pair<const Key, T>* operator->() const;

            private:

                /// Constructor.
                /** @param node The map node to reference. */
                Iterator(Node* node);

                /// Pointer to the map node, that holds the element.
                Node* m_current;

            };

            /// Base class for a unary function object, used in the "traverse" method.
            class UnaryFunction
            {
            public:
                /// Evaluates the function for an element in the map.
                virtual bool_t operator()(const Iterator& element) = 0;
            };

            /// Base class for a unary function object, which has a key for comparison.
            class UnaryFunctionKey : public UnaryFunction
            {
            public:

                /// Constructor.
                /** @param key Key to store for comparison. */
                UnaryFunctionKey(const Key& key) :
                    m_key(key)
                {
                }

            protected:

                /// Key for comparison.
                const Key& m_key;
            };

        private:

            /// Enum for a node's insert position
            enum EPosition
            {
                LEFT, EQUAL, RIGHT
            };

            /// Internal class for holding a key / value pair as a node in an AVL tree.
            class Node : public Pair<const Key, T>
            {
            public:
                friend class Map;

            private:

                /// Default constructor.
                /** Uses the default constructors for Key and T. */
                Node();

                /// Constructor.
                /** @param key The key of the element.
                 *  @param value Value of the element. */
                Node(const Key& key, const T& value);

                /// Destructor.
                ~Node();

                /// Traverses the node's subtree in pre-order sequence.
                /** @param visit This function object is called for each node, until it returns false.
                 *  @param visitSmaller Function object, that evaluates if the left subtree shall be traversed.
                 *  @param visitGreater Function object, that evaluates if the right subtree shall be traversed. */
                bool_t traversePreOrder(UnaryFunction& visit, UnaryFunction& visitSmaller, UnaryFunction& visitGreater);

                /// Traverses the node's subtree in post-order sequence.
                /** @param visit This function object is called for each node, until it returns false.
                 *  @param visitSmaller Function object, that evaluates if the left subtree shall be traversed.
                 *  @param visitGreater Function object, that evaluates if the right subtree shall be traversed. */
                bool_t traversePostOrder(UnaryFunction& visit, UnaryFunction& visitSmaller,
                UnaryFunction& visitGreater);

                /// Traverses the node's subtree in in-order sequence.
                /** @param visit This function object is called for each node, until it returns false.
                 *  @param visitSmaller Function object, that evaluates if the left subtree shall be traversed.
                 *  @param visitGreater Function object, that evaluates if the right subtree shall be traversed. */
                bool_t traverseInOrder(UnaryFunction& visit, UnaryFunction& visitSmaller, UnaryFunction& visitGreater);

                /// Traverses the node's subtree in mirrored pre-order sequence.
                /** @param visit This function object is called for each node, until it returns false.
                 *  @param visitSmaller Function object, that evaluates if the left subtree shall be traversed.
                 *  @param visitGreater Function object, that evaluates if the right subtree shall be traversed. */
                bool_t traversePreOrderMirrored(UnaryFunction& visit, UnaryFunction& visitSmaller,
                UnaryFunction& visitGreater);

                /// Traverses the node's subtree in mirrored post-order sequence.
                /** @param visit This function object is called for each node, until it returns false.
                 *  @param visitSmaller Function object, that evaluates if the left subtree shall be traversed.
                 *  @param visitGreater Function object, that evaluates if the right subtree shall be traversed. */
                bool_t traversePostOrderMirrored(UnaryFunction& visit, UnaryFunction& visitSmaller,
                UnaryFunction& visitGreater);

                /// Traverses the node's subtree in mirrored in-order sequence.
                /** @param visit This function object is called for each node, until it returns false.
                 *  @param visitSmaller Function object, that evaluates if the left subtree shall be traversed.
                 *  @param visitGreater Function object, that evaluates if the right subtree shall be traversed. */
                bool_t traverseInOrderMirrored(UnaryFunction& visit, UnaryFunction& visitSmaller,
                UnaryFunction& visitGreater);

                /// Inserts a new node as child to this node and rebalances the tree, if necessary.
                /** @param node Node to insert.
                 *  @param position Insertion position (must be LEFT or RIGHT).
                 *  @param root Pointer to the root node, that is updated, when the root changes due
                 *  to AVL rotation operations. */
                void insert(Node* node, EPosition position, Node*& root);

                /// Removes this node from the tree and rebalances the tree, if necessary.
                /** @param root Pointer to the root node, that is updated, when the root changes due
                 *  to AVL rotation operations. */
                void remove(Node*& root);

                /// Called after an insertion of a new node for each node, those subtree's height has increased by one.
                /** Updates the balance factor and rebalances the subtree of the node, when necessary.
                 *  @param subtree Flag, if the height of the left or right subtree has increased.
                 *  @param root Pointer to the root node, that is updated, when the root changes due
                 *  to AVL rotation operations.*/
                void rebalanceInsert(EPosition subtree, Node*& root);

                /// Called after a removal of a node for each node, those subtree's height has decreased by one.
                /** Updates the balance factor and rebalances the subtree of the node, when necessary.
                 *  @param subtree Flag, if the height of the left or right subtree has decreased.
                 *  @param root Pointer to the root node, that is updated, when the root changes due
                 *  to AVL rotation operations.*/
                void rebalanceRemove(EPosition subtree, Node*& root);

                /// Replaces this node by another node at it's parent.
                /** @param replacement The replacement node for this node.
                 *  @param root Pointer to the root node of the tree, that is updated. */
                void replaceWith(Node* replacement, Node*& root);

                /// Does a left AVL rotation of this node.
                /** @param root Pointer to the root node of the tree, that is updated. */
                void rotatLeft(Node*& root);

                /// Does a right AVL rotation of this node.
                /** @param root Pointer to the root node of the tree, that is updated. */
                void rotateRight(Node*& root);

                /// Sets the left child node.
                /** Also, updates the parent of the child node.
                 *  @param node Node to be set as left child node of this node. */
                void setLeft(Node* node);

                /// Sets the right child node.
                /** Also, updates the parent of the child node.
                 *  @param node Node to be set as right child node of this node. */
                void setRight(Node* node);

                /// Returns the in-order predecessor of this node.
                /** @return the predecessor, or 0 if it does not exist. */
                Node* predecessor();

                /// Returns the in-order successor of this node.
                /** @return the successor, or 0 if it does not exist. */
                Node* successor();

                /// Returns whether this node is a left child of it's parent, or not.
                /** @return "true", when this node is a left child of it's parent. */
                bool_t isLeftChild() const;

                /// Returns whether this node is a right child of it's parent, or not.
                /** @return "true", when this node is a right child of it's parent. */
                bool_t isRightChild() const;

                /// Searches the top most node that can be reached by only climbing up the left child links.
                /** @return Top most node, which is a parent of this node, and where all intermediate child nodes
                 *  are left childs. */
                Node* climbLeftUp();

                /// Searches the top most node that can be reached by only climbing up the right child links.
                /** @return Top most node, which is a parent of this node, and where all intermediate child nodes
                 *  are right childs. */
                Node* climbRightUp();

                /// Returns the smallest element in the subtree of this node.
                /** @return The smallest element. */
                Node* min();

                /// Returns the greatest element in the subtree of this node.
                /** @return The greatest element. */
                Node* max();

                /// Checks the consistency of the subtree of this node.
                /** Only used for testing.
                 *  Checks if the balance value is correct and returns the height of the subtree of this node.
                 *  @param height The computed subtree height is returned here.
                 *  @return "true", when the subtree is consistent in all nodes, or "false" otherwise. */
                bool_t checkConsistency(int32_t& height);

                /// Replaces the left or right child node by a new node.
                /** @param child Child to be replaced.
                 *  @param newChild New child node, that replaces child. */
                void replaceChild(Node* child, Node* newChild);

                /// The parent of this node.
                Node* m_parent;

                /// The child node of the left subtree.
                Node* m_left;

                /// The child node of the right subtree.
                Node* m_right;

                /// Balance value of this subtree.
                /** Difference of the right subtree height and the left subtree height.
                 * Possible values:
                 *  -2 : The tree has heavy overweight to the left (only temporary state).
                 *  -1 : The tree has overweight to the left.
                 *   0 : The tree is balanced.
                 *  +1:  The tree has overweight to the right.
                 *  +2 : The tree has heavy overweight to the right (only temporary state). */
                int8_t m_balance;

            };

            /// Checks the consistency of the map.
            /** Only used for testing.
             *  Checks if the tree is a valid AVL tree and if all balance values are correct.
             *  @return "true", when all nodes are successfully verified and "false" otherwise. */
            bool_t checkConsistency();

            /// Finds the insertion position for an element with a given key or the node itself, when the key already exists.
            /** @param key The key to search for.
             *  @param node The parent node for insertion is returned here.
             *  @param position Position where to insert the new node with the given key to the node returned in "node".
             *  Can be LEFT or RIGHT, when no element with the key already exists or EQUAL, otherwise. */
            void findInsertPos(const Key& key, Node*& node, EPosition& position);

            /// End-of-list node element, returned by map::end().
            /** This node represents an invalid element. */
            static Node sEndNode;

            /// The root node of the AVL tree that holds all elements.
            Node* m_root;

            /// Declare the MapTest as a friend, so it can access the internal checkConsistency method.
            friend class MapTest;
        };

    /// Map:

    template<class Key, class T>
        typename Map<Key, T>::Node Map<Key, T>::sEndNode;

    template<class Key, class T>
        Map<Key, T>::Map() :
            m_root(0)
        {

        }

    template<class Key, class T>
        Map<Key, T>::~Map()
        {
            clear();
        }

    template<class Key, class T>
        bool_t Map<Key, T>::insert(const Key& key, const T& value)
        {
            if (m_root)
            {
                Node* node;
                EPosition position;
                findInsertPos(key, node, position);

                if (position != EQUAL)
                {
                    node->insert(new Node(key, value), position, m_root);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                m_root = new Node(key, value);
                return true;
            }
        }

    template<class Key, class T>
        void Map<Key, T>::findInsertPos(const Key& key, Node*& node, EPosition& position)
        {
            class Visit : public UnaryFunction
            {
            public:
                Visit()
                {
                }
                bool_t operator()(const Iterator& element)
                {
                    m_i = element;
                    return false;
                }
                Iterator getIterator()
                {
                    return m_i;
                }
            protected:
                Iterator m_i;

            };

            class VisitSmaller : public UnaryFunctionKey
            {
            public:
                VisitSmaller(const Key& key) :
                    UnaryFunctionKey(key)
                {
                }
                bool_t operator()(const Iterator& element)
                {
                    return UnaryFunctionKey::m_key < element->first;
                }
            };

            class VisitGreater : public UnaryFunctionKey
            {
            public:
                VisitGreater(const Key& key) :
                    UnaryFunctionKey(key)
                {
                }
                bool_t operator()(const Iterator& element)
                {
                    return element->first < UnaryFunctionKey::m_key;
                }
            };

            Visit visit;
            VisitSmaller visitSmaller(key);
            VisitGreater visitGreater(key);

            traverse(visit, visitSmaller, visitGreater, POST_ORDER);
            node = visit.getIterator().m_current;
            if (key < node->first)
            {
                position = LEFT;
            }
            else if (key == node->first)
            {
                position = EQUAL;
            }
            else
            {
                position = RIGHT;
            }
        }

    template<class Key, class T>
        T& Map<Key, T>::operator[](const Key& key)
        {
            Node* node;
            if (m_root)
            {

                EPosition position;
                findInsertPos(key, node, position);

                if (position != EQUAL)
                {
                    Node* newNode = new Node(key, T());
                    node->insert(newNode, position, m_root);
                    node = newNode;
                }
            }
            else
            {
                node = new Node(key, T());
                m_root = node;

            }
            return node->second;
        }

    template<class Key, class T>
        void Map<Key, T>::traverse(UnaryFunction& visit, UnaryFunction& visitSmaller, UnaryFunction& visitGreater,
        EOrder order)
        {
            if (m_root)
            {
                switch (order)
                {
                    case PRE_ORDER:
                        m_root->traversePreOrder(visit, visitSmaller, visitGreater);
                    break;

                    case IN_ORDER:
                        m_root->traverseInOrder(visit, visitSmaller, visitGreater);
                    break;

                    case POST_ORDER:
                        m_root->traversePostOrder(visit, visitSmaller, visitGreater);
                    break;

                    case PRE_ORDER_MIRRORED:
                        m_root->traversePreOrderMirrored(visit, visitSmaller, visitGreater);
                    break;

                    case IN_ORDER_MIRRORED:
                        m_root->traverseInOrderMirrored(visit, visitSmaller, visitGreater);
                    break;

                    case POST_ORDER_MIRRORED:
                        m_root->traversePostOrderMirrored(visit, visitSmaller, visitGreater);
                    break;
                }
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator Map<Key, T>::find(const Key& key)
        {
            if (m_root)
            {
                Node* node;
                EPosition position;
                findInsertPos(key, node, position);
                if (position == EQUAL)
                {
                    return Iterator(node);
                }
            }
            return end();
        }

    template<class Key, class T>
        void Map<Key, T>::erase(Iterator& element)
        {
            Node* nodeToBeDeleted = element.m_current;
            nodeToBeDeleted->remove(m_root);
            delete nodeToBeDeleted;

            element.m_current = 0;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::erase(const Key& key)
        {
            Iterator i = find(key);
            if (i != end())
            {
                erase(i);
                return true;
            }
            else
            {
                return false;
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator Map<Key, T>::begin()
        {
            if (m_root)
            {
                return Iterator(m_root->min());
            }
            else
            {
                return end();
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator Map<Key, T>::back()
        {
            if (m_root)
            {
                return Iterator(m_root->max());
            }
            else
            {
                return end();
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator Map<Key, T>::end()
        {
            return Iterator(&sEndNode);
        }

    template<class Key, class T>
        bool_t Map<Key, T>::checkConsistency()
        {
            if (m_root)
            {
                int32_t height(0);
                return m_root->checkConsistency(height);
            }
            else
            {
                return true;
            }
        }

    template<class Key, class T>
        void Map<Key, T>::clear()
        {
            if (m_root)
            {
                delete m_root;
                m_root = 0;
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator Map<Key, T>::lower_bound(const Key& key)
        {
            class Visit : public UnaryFunctionKey
            {
            public:
                Visit(const Key& key) :
                    UnaryFunctionKey(key)
                {
                }
                bool operator()(const Iterator& element)
                {
                    if (element->first >= UnaryFunctionKey::m_key)
                    {
                        m_i = element;
                        return false;
                    }
                    return true;
                }
                Iterator i()
                {
                    return m_i;
                }
            protected:
                Iterator m_i;

            };

            class VisitSmaller : public UnaryFunctionKey
            {
            public:
                VisitSmaller(const Key& key) :
                    UnaryFunctionKey(key)
                {
                }
                bool operator()(const Iterator& element)
                {
                    return UnaryFunctionKey::m_key < element->first;
                }
            };

            class VisitGreater : public UnaryFunction
            {
            public:
                VisitGreater()
                {
                }
                bool operator()(const Iterator&)
                {
                    return true;
                }
            };

            Visit visit(key);
            VisitSmaller visitSmaller(key);
            VisitGreater visitGreater;

            traverse(visit, visitSmaller, visitGreater, IN_ORDER);
            return visit.i();
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator Map<Key, T>::upper_bound(const Key& key)
        {
            class Visit : public UnaryFunctionKey
            {
            public:
                Visit(const Key& key) :
                    UnaryFunctionKey(key)
                {
                }
                bool operator()(const Iterator& element)
                {
                    if (element->first > UnaryFunctionKey::m_key)
                    {
                        m_i = element;
                        return false;
                    }
                    return true;
                }
                Iterator i()
                {
                    return m_i;
                }
            protected:
                Iterator m_i;

            };

            class VisitSmaller : public UnaryFunctionKey
            {
            public:
                VisitSmaller(const Key& key) :
                    UnaryFunctionKey(key)
                {
                }
                bool operator()(const Iterator& element)
                {
                    return UnaryFunctionKey::m_key < element->first;
                }
            };

            class VisitGreater : public UnaryFunction
            {
            public:
                VisitGreater()
                {
                }
                bool operator()(const Iterator&)
                {
                    return true;
                }
            };

            Visit visit(key);
            VisitSmaller visitSmaller(key);
            VisitGreater visitGreater;

            traverse(visit, visitSmaller, visitGreater, IN_ORDER);
            return visit.i();
        }

    /// Node:

    template<class Key, class T>
        Map<Key, T>::Node::Node() :
            Pair<const Key, T>(Key(), T()),
            m_parent(0),
            m_left(0),
            m_right(0),
            m_balance(0)
        {

        }

    template<class Key, class T>
        Map<Key, T>::Node::Node(const Key& key, const T& value) :
            Pair<const Key, T>(key, value),
            m_parent(0),
            m_left(0),
            m_right(0),
            m_balance(0)
        {

        }

    template<class Key, class T>
        void Map<Key, T>::Node::setLeft(Node* node)
        {
            m_left = node;
            if (node)
            {
                node->m_parent = this;
            }
        }

    template<class Key, class T>
        void Map<Key, T>::Node::setRight(Node* node)
        {
            m_right = node;
            if (node)
            {
                node->m_parent = this;
            }
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::traversePreOrder(UnaryFunction& visit, UnaryFunction& visitSmaller,
        UnaryFunction& visitGreater)
        {
            Iterator i(this);

            if (!visit(i))
            {
                return false;
            }

            if (m_left && visitSmaller(i))
            {
                if (!m_left->traversePreOrder(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (m_right && visitGreater(i))
            {
                if (!m_right->traversePreOrder(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            return true;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::traversePostOrder(UnaryFunction& visit, UnaryFunction& visitSmaller,
        UnaryFunction& visitGreater)
        {
            Iterator i(this);

            if (m_left && visitSmaller(i))
            {
                if (!m_left->traversePostOrder(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (m_right && visitGreater(i))
            {
                if (!m_right->traversePostOrder(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (!visit(i))
            {
                return false;
            }

            return true;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::traverseInOrder(UnaryFunction& visit, UnaryFunction& visitSmaller,
        UnaryFunction& visitGreater)
        {
            Iterator i(this);

            if (m_left && visitSmaller(i))
            {
                if (!m_left->traverseInOrder(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (!visit(i))
            {
                return false;
            }

            if (m_right && visitGreater(i))
            {
                if (!m_right->traverseInOrder(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            return true;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::traversePreOrderMirrored(UnaryFunction& visit, UnaryFunction& visitSmaller,
        UnaryFunction& visitGreater)
        {
            Iterator i(this);

            if (!visit(i))
            {
                return false;
            }

            if (m_right && visitGreater(i))
            {
                if (!m_right->traversePreOrderMirrored(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (m_left && visitSmaller(i))
            {
                if (!m_left->traversePreOrderMirrored(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            return true;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::traversePostOrderMirrored(UnaryFunction& visit, UnaryFunction& visitSmaller,
        UnaryFunction& visitGreater)
        {
            Iterator i(this);

            if (m_right && visitGreater(i))
            {
                if (!m_right->traversePostOrderMirrored(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (m_left && visitSmaller(i))
            {
                if (!m_left->traversePostOrderMirrored(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (!visit(i))
            {
                return false;
            }

            return true;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::traverseInOrderMirrored(UnaryFunction& visit, UnaryFunction& visitSmaller,
        UnaryFunction& visitGreater)
        {
            Iterator i(this);

            if (m_right && visitGreater(i))
            {
                if (!m_right->traverseInOrderMirrored(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            if (!visit(i))
            {
                return false;
            }

            if (m_left && visitSmaller(i))
            {
                if (!m_left->traverseInOrderMirrored(visit, visitSmaller, visitGreater))
                {
                    return false;
                }
            }

            return true;
        }

    template<class Key, class T>
        void Map<Key, T>::Node::insert(Node* node, EPosition position, Node*& root)
        {
            if (position == LEFT)
            {
                setLeft(node);
            }
            else
            {
                setRight(node);
            }
            rebalanceInsert(position, root);
        }

    template<class Key, class T>
        void Map<Key, T>::Node::replaceChild(Node* child, Node* newChild)
        {
            if (m_left == child)
            {
                setLeft(newChild);
            }
            else
            {
                setRight(newChild);
            }
        }

    template<class Key, class T>
        void Map<Key, T>::Node::remove(Node*& root)
        {
            Node* replacement;
            Node* subtreeWithReducedHeight;
            EPosition subtree;
            if (m_left && m_right)
            {
                replacement = m_left->max();

                if (replacement != m_left)
                {
                    Node* replacementLeftChild = replacement->m_left;
                    subtreeWithReducedHeight = replacement->m_parent;

                    subtreeWithReducedHeight->setRight(replacementLeftChild);
                    subtree = RIGHT;

                    replacement->setLeft(m_left);
                    replacement->setRight(m_right);
                    replacement->m_balance = m_balance;
                }
                else
                {
                    replacement->setRight(m_right);
                    replacement->m_balance = m_balance;
                    subtreeWithReducedHeight = replacement;
                    subtree = LEFT;
                }
            }
            else
            {
                replacement = m_left ? m_left : m_right;
                subtree = isLeftChild() ? LEFT : RIGHT;
                subtreeWithReducedHeight = m_parent;
            }

            if (m_parent)
            {
                m_parent->replaceChild(this, replacement);
            }
            else
            {
                root = replacement;
                if (replacement)
                {
                    replacement->m_parent = 0;
                }
            }

            setLeft(0);
            setRight(0);

            if (subtreeWithReducedHeight)
            {
                subtreeWithReducedHeight->rebalanceRemove(subtree, root);
            }
        }

    template<class Key, class T>
        void Map<Key, T>::Node::rebalanceInsert(EPosition subtree, Node*& root)
        {
            if (subtree == LEFT)
            {
                m_balance--;
                if (m_balance == -2)
                {
                    if (m_left->m_balance == 1)
                    {
                        m_left->rotatLeft(root);
                    }
                    rotateRight(root);
                    return;
                }

            }
            else
            {
                /// subtree == RIGHT
                m_balance++;
                if (m_balance == 2)
                {
                    if (m_right->m_balance == -1)
                    {
                        m_right->rotateRight(root);
                    }
                    rotatLeft(root);
                    return;
                }

            }

            if ((m_balance != 0) && m_parent)
            {
                m_parent->rebalanceInsert(isLeftChild() ? LEFT : RIGHT, root);
            }
        }

    template<class Key, class T>
        void Map<Key, T>::Node::rebalanceRemove(EPosition subtree, Node*& root)
        {
            EPosition parentSubtree = isLeftChild() ? LEFT : RIGHT;
            Node* parent = m_parent;
            if (!m_left && !m_right)
            {
                m_balance = 0;
            }
            else
            {
                if (subtree == LEFT)
                {
                    m_balance++;
                    if (m_balance == 2)
                    {
                        int32_t balanceOppositeSubtree = m_right ? m_right->m_balance : 0;
                        if (balanceOppositeSubtree == -1)
                        {
                            m_right->rotateRight(root);
                        }
                        rotatLeft(root);
                        if (balanceOppositeSubtree == 0)
                        {
                            return;
                        }
                    }
                    if (m_balance == 1)
                    {
                        return;
                    }
                }
                else
                {
                    /// subtree == RIGHT
                    m_balance--;
                    if (m_balance == -2)
                    {
                        int32_t balanceOppositeSubtree = m_left ? m_left->m_balance : 0;
                        if (balanceOppositeSubtree == 1)
                        {
                            m_left->rotatLeft(root);
                        }
                        rotateRight(root);
                        if (balanceOppositeSubtree == 0)
                        {
                            return;
                        }
                    }
                    if (m_balance == -1)
                    {
                        return;
                    }
                }
            }

            if (parent)
            {
                parent->rebalanceRemove(parentSubtree, root);
            }
        }

    template<class Key, class T>
        void Map<Key, T>::Node::replaceWith(Node* replacement, Node*& root)
        {
            if (m_parent)
            {
                if (m_parent->m_left == this)
                {
                    m_parent->setLeft(replacement);
                }
                else
                {
                    m_parent->setRight(replacement);
                }
            }
            else
            {
                root = replacement;
                replacement->m_parent = 0;
            }
        }

    template<class Key, class T>
        void Map<Key, T>::Node::rotatLeft(Node*& root)
        {
            Node* n = m_right;

            replaceWith(n, root);

            setRight(n->m_left);
            n->setLeft(this);

            m_balance--;
            if (n->m_balance > 0)
            {
                m_balance -= n->m_balance;
            }
            n->m_balance--;
            if (m_balance < 0)
            {
                n->m_balance += m_balance;
            }
        }

    template<class Key, class T>
        void Map<Key, T>::Node::rotateRight(Node*& root)
        {
            Node* n = m_left;

            replaceWith(n, root);

            setLeft(n->m_right);
            n->setRight(this);

            m_balance++;
            if (n->m_balance < 0)
            {
                m_balance -= n->m_balance;
            }
            n->m_balance++;
            if (m_balance > 0)
            {
                n->m_balance += m_balance;
            }
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::isLeftChild() const
        {
            return m_parent && m_parent->m_left == this;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::isRightChild() const
        {
            return m_parent && m_parent->m_right == this;
        }

    template<class Key, class T>
        typename Map<Key, T>::Node* Map<Key, T>::Node::climbLeftUp()
        {
            if (isLeftChild())
            {
                return m_parent->climbLeftUp();
            }
            else
            {
                return this;
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Node* Map<Key, T>::Node::climbRightUp()
        {
            if (isRightChild())
            {
                return m_parent->climbRightUp();
            }
            else
            {
                return this;
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Node* Map<Key, T>::Node::predecessor()
        {
            if (m_left)
            {
                return m_left->max();
            }
            else
            {
                return climbLeftUp()->m_parent;
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Node* Map<Key, T>::Node::successor()
        {
            if (m_right)
            {
                return m_right->min();
            }
            else
            {
                return climbRightUp()->m_parent;
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Node* Map<Key, T>::Node::min()
        {
            if (m_left)
            {
                return m_left->min();
            }
            else
            {
                return this;
            }
        }

    template<class Key, class T>
        typename Map<Key, T>::Node* Map<Key, T>::Node::max()
        {
            if (m_right)
            {
                return m_right->max();
            }
            else
            {
                return this;
            }
        }

    template<class Key, class T>
        Map<Key, T>::Node::~Node()
        {
            if (m_left)
            {
                delete m_left;
            }
            if (m_right)
            {
                delete m_right;
            }
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Node::checkConsistency(int32_t& height)
        {
            int32_t leftHeight(0);
            if (m_left)
            {
                if (!m_left->checkConsistency(leftHeight))
                {
                    return false;
                }
            }
            int32_t rightHeight(0);
            if (m_right)
            {
                if (!m_right->checkConsistency(rightHeight))
                {
                    return false;
                }
            }
            height = ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
            int32_t balance = rightHeight - leftHeight;

            EXPECT_EQ(balance, m_balance);
            EXPECT_TRUE(balance >= -1 && balance <= 1);

            return (balance == m_balance) && (balance >= -1) && (balance <= 1);
        }

/// Iterator:

    template<class Key, class T>
        Map<Key, T>::Iterator::Iterator() :
            m_current(&Map<Key, T>::sEndNode)
        {

        }

    template<class Key, class T>
        Map<Key, T>::Iterator::Iterator(Node* node) :
            m_current(node)
        {

        }

    template<class Key, class T>
        Pair<const Key, T>& Map<Key, T>::Iterator::operator*() const
        {
            return *m_current;
        }

    template<class Key, class T>
        Pair<const Key, T>* Map<Key, T>::Iterator::operator->() const
        {
            return m_current;
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator& Map<Key, T>::Iterator::operator++()
        {
            if (m_current != &Map<Key, T>::sEndNode)
            {
                m_current = m_current->successor();
                if (!m_current)
                {
                    m_current = &Map<Key, T>::sEndNode;
                }
            }
            return *this;
        }

    template<class Key, class T>
        typename Map<Key, T>::Iterator& Map<Key, T>::Iterator::operator--()
        {
            if (m_current != &Map<Key, T>::sEndNode)
            {
                m_current = m_current->predecessor();
                if (!m_current)
                {
                    m_current = &Map<Key, T>::sEndNode;
                }
            }
            return *this;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Iterator::operator==(const Iterator& other) const
        {
            return m_current == other.m_current;
        }

    template<class Key, class T>
        bool_t Map<Key, T>::Iterator::operator!=(const Iterator& other) const
        {
            return m_current != other.m_current;
        }
}

#endif
