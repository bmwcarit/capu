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

#ifndef CAPU_LIST_H
#define CAPU_LIST_H

#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/container/Comparator.h"
#include "capu/util/Swap.h"
#include "capu/util/Allocator.h"

namespace capu
{
    template<typename T>
    class GenericListNode
    {
    public:

        GenericListNode()
            : mNext(0)
            , mPrev(0)
        {
        }

        GenericListNode(const T& data)
            : mData(data)
            , mNext(0)
            , mPrev(0)
        {
        }

        T mData;
        GenericListNode<T>* mNext;
        GenericListNode<T>* mPrev;
    };

    /**
     * List of objects.
     */
    template <class T, class A = Allocator<GenericListNode<T> >, class C = Comparator>
    class List
    {
    private:
        typedef GenericListNode<T> ListNode;

        class ListIterator
        {
        public:
            friend class List<T, A , C>;

            /**
             * destructor
             */
            ~ListIterator();

            /**
             * Indirection
             * @return the current value referenced by the iterator
             */
            T& operator*();

            /**
             * Dereference
             * @return a pointer to the current object the iterator points to
             */
            T* operator->();

            /**
             * Compares two iterators
             * @return true if the iterators point to the same position
             */
            capu::bool_t operator==(const ListIterator& iter) const;

            /**
             * Compares two iterators
             * @return true if the iterators do not point to the same position
             */
            capu::bool_t operator!=(const ListIterator& iter) const;

            /**
             * Step the iterator forward to the next element (prefix operator)
             * @return the next iterator
             */
            const ListIterator& operator++() const;

            /**
             * Step the iterator forward to the next element (prefix operator)
             * @return the next iterator
             */
            ListIterator& operator++();

            /**
             * Step the iterator forward to the next element (postfix operator)
             * @return the next iterator
             */
            const ListIterator operator++(capu::int32_t) const;

            /**
             * Step the iterator forward to the next element (postfix operator)
             * @return the next iterator
             */
            ListIterator operator++(capu::int32_t);

            //TODO: Add other operators as --, +, -, ...

            /**
             * Returns the index of the current element.
             * @return The index of the current element. If there is no current element, the return value is undefined.
             */
            uint_t currentIndex() const;

        private:
            ListIterator(const ListNode* boundary); // creation only in list.begin()
            const ListNode* mInitialNode;
            mutable ListNode* mCurrentNode;
            mutable int_t mIndex;
        };

        ListNode mBoundary;
        uint_t mSize;
        const C mComparator;
        A mAllocator;

        status_t insertElement(ListNode* addPosition, const T& element);
        status_t deleteElement(ListNode* deletePosition);
        ListNode* findElement(const uint_t index) const;

    public:
        /**
         * An iterator for lists
         */
        typedef typename List<T, A , C>::ListIterator Iterator;

        /**
         * An iterator for const lists
         */
        typedef const typename List<T, A , C>::ListIterator ConstIterator;

        /**
         * Default Constructor
         */
        List();

        /**
         * Copy constructor
         */
        List(const List<T, A , C>& other);

        /**
         * Destructor
         */
        virtual ~List();

        /**
         * Swaps the list with another list and returns a reference to *this.
         * @param other the other list
         * @return this list
         */
        List<T, A , C>& swap(List<T, A , C>& other);

        /**
         * Adds all items of the given list to the current list.
         * @param other The list whose items will get copied into the current list.
         * @return This list.
         */
        List<T, A , C>& operator=(List<T, A , C> other);

        /**
         * Checks if two lists are equal
         * @param other list to compare
         * @return true if lists are equal false otherwise
         */
        bool_t operator==(const List<T, A, C>& other);

        /**
         * Checks if two lists are not equal
         * @param other list to compare
         * @return true if lists aren't equal false otherwise
         */
        bool_t operator!=(const List<T, A, C>& other);

        /**
         * Inserts element at the end of the list
         *
         * @param element element that will be added
         * @return CAPU_ENO_MEMORY if allocation of element is failed
         *         CAPU_OK if the element is successfully added
         */
        status_t push_back(const T& element);

        /**
         * Inserts element at the begin of the list
         *
         * @param element element that will be added
         * @return CAPU_ENO_MEMORY if allocation of element is failed
         *         CAPU_OK if the element is successfully added
         */
        status_t push_front(const T& element);

        /**
         * Inserts element at the end of list
         *
         * NOTE: Not STL compatible
         *
         * @param element element that will be added
         * @return CAPU_ENO_MEMORY if allocation of element is failed
         *         CAPU_OK if the element is successfully added
         */
        status_t insert(const T& element);

        /**
         * Inserts element before the specified position
         *
         * NOTE: Not STL compatible
         *
         * @param index index of element which will be inserted
         * @param element new value that will be inserted
         *
         * @return CAPU_EINVAL if given index is invalid.
         *         CAPU_ENO_MEMORY if allocation of element is failed
         *         CAPU_OK if the element is successfully added
         *         CAPU_ERROR otherwise
         */
        status_t insert(const uint_t index, const T& element);

        /**
         * Add element before the specified position
         *
         * @param iter iterator with the position to insert
         * @param element new value that will be inserted
         *
         * @return CAPU_ENO_MEMORY memory allocation failed.
         *         CAPU_OK otherwise
         */
        status_t insert(Iterator& iter, const T& element);

        /**
         * Sets the element at the specified index.
         *
         * NOTE: Not STL compatible
         *
         * @param index the index of element that will be replaced
         * @param element element that will be overwritten to existing place
         * @param elementOld the buffer to keep the existing
         * @return CAPU_EINVAL if the index is not valid
         *         CAPU_OK otherwise
         */
        status_t set(const uint_t index, const T& element, T* elementOld = 0);

        /**
         * remove the element in the specified index and if the element_old
         * parameter is not NULL, the removed element will be put to element_old
         *
         * NOTE: Not STL compatible
         *
         * @param index index of element that will be removed
         * @param elementOld the buffer which will keep the copy of the removed element
         * @return CAPU_EINVAL invalid index
         *         CAPU_OK if the element is successfully removed
         */
        status_t erase(const uint_t index, T* elementOld = 0);

        /**
         * Removes the element in the specified iterator position and if the element_old
         * parameter is not NULL, the removed element will be put to element_old
         * @param listIterator of element that will be removed
         * @param elementOld the buffer which will keep the copy of the removed element
         * @return CAPU_EINVAL invalid iterator
         *         CAPU_OK if the element is successfully removed
         *
         */
        status_t erase(Iterator& listIterator, T* elementOld = 0);

        /**
         * Removes every occurence of element in list.
         *
         * @param element The element that will get removed.
         * @return CAPU_EINVAL if element was not found
         *         CAPU_OK if element was removed.
         */
        status_t remove(const T& element);

        /**
         * Deletes element at the end of the list
         *
         * @return CAPU_ERANGE if list was empty
         *         CAPU_OK if the element is successfully deleted
         */
        status_t pop_back();

        /**
         * Deletes element at the begin of the list
         *
         * @return CAPU_ERANGE if list was empty
         *         CAPU_OK if the element is successfully deleted
         */
        status_t pop_front();

        /**
         * removes all elements from linked list
         */
        void clear();

        /**
         * Gets a single element on specified index
         *
         * NOTE: Not STL compatible
         *
         * @param index index of element that will be get
         * @param result status code which can be CAPU_EINVAL on invalid index
                                          CAPU_OK otherwise
         * @return a copy of the element or
                  NULL, if element does not exist
         */
        T get(const uint_t index, status_t* result = 0) const;

        /**
         * Return the number of elements in list
         *
         * @return return the number of elements in list
         */
        uint_t size() const;

        /**
         * Check the list is empty or not
         *
         * @return true if empty
         *         false otherwise
         */
        bool_t isEmpty() const;

        /**
         * Check whether the list is empty or not
         *
         * this method ensures the compability to the STL interface
         *
         * @return true if empty
         *         false otherwise
         */
        bool_t empty() const;

        /**
         * Returns an iterator pointing to the beginning of list.
         *
         * @return iterator
         */
        Iterator begin();

        /**
         * Returns a const iterator pointing to the beginning of the list
         *
         * @return iterator
         */
        const Iterator begin() const;

        /**
         * Returns an iterator pointing to the end of list.
         *
         * @return iterator
         */
        Iterator end();

        /**
         * Returns a const interator pointing after the last element of the list
         *
         * @return iterator
         */
        const Iterator end() const;

        /**
         * Returns a reference to the first element in the list
         *
         * @return reference to first element
         */
        T& front();

        /**
         * Returns a const reference to the first element in the list
         *
         * @return const reference to first element
         */
        const T& front() const;

        /**
         * Returns a reference to the last element in the list
         *
         * @return reference to the last element in the list
         */
        T& back();

        /**
         * Returns a const reference to the last element in the list
         *
         * @return const reference to the last element in the list
         */
        const T& back() const;

        /**
         * Finds the index of given element in the link list
         * if you are using an object you need to overload == operator
         *
         * NOTE: Not STL compatible
         *
         * @param element the value that will be searched
         * @return -1 if the value either does not exist or given value is NULL
         *          otherwise index of value on linked list
         */
        int_t find(const T& element) const;

        /**
         * Check that if the list contains the given parameter or not
         * if you are using an object you need to overload == operator
         *
         * NOTE: Not STL compatible
         *
         * @param element element that will be checked
         * @return true list contains it
         *         false otherwise
         */
        bool_t contains(const T& element) const;
    };

    /*
     * Implementation specialized swap for list
     */

    template <class T, class A, class C>
    void swap(List<T, A , C>& first, List<T, A , C>& second)
    {
        first.swap(second);
    }

    /*
     * Implementation
     */

    template <class T, class A, class C>
    List<T, A , C>::List()
        : mSize(0), mComparator()
    {
        mBoundary.mNext = &mBoundary;
        mBoundary.mPrev = &mBoundary;
    }

    template <class T, class A, class C>
    List<T, A , C>::List(const List<T, A , C>& other)
        : mSize(0), mComparator()
    {
        mBoundary.mNext = &mBoundary;
        mBoundary.mPrev = &mBoundary;

        // add all items from the other List
        Iterator it = other.begin();
        while (it != other.end())
        {
            insert(*it);
            it++;
        }
    }

    template <class T, class A, class C>
    List<T, A , C>::~List()
    {
        clear();
    }

    template <class T, class A, class C>
    void List<T, A , C>::clear()
    {
        ListNode* current = mBoundary.mNext;
        ListNode* toDelete = 0;
        while (current != &mBoundary)
        {
            toDelete = current;
            current = current->mNext;
            mAllocator.deallocate(toDelete);
        }
        mSize = 0;
        mBoundary.mNext = &mBoundary;
        mBoundary.mPrev = &mBoundary;
    }

    template <class T, class A, class C>
    List<T, A , C>& List<T, A , C>::operator=(List<T, A , C> other)
    {
        return swap(other);
    }

    template <class T, class A, class C>
    int_t List<T, A , C>::find(const T& element) const
    {
        int_t counter = 0;
        ListNode* current = mBoundary.mNext;
        while (current != &mBoundary)
        {
            if (mComparator(current->mData, element))
            {
                // element was found, return index
                return counter;
            }
            current = current->mNext;
            ++counter;
        }

        // not found
        return -1;
    }

    template <class T, class A, class C>
    inline List<T, A , C>& List<T, A , C>::swap(List< T, A, C >& other)
    {
        ListNode* thisNext = mBoundary.mNext;
        ListNode* thisPrev = mBoundary.mPrev;
        ListNode* otherNext = other.mBoundary.mNext;
        ListNode* otherPrev = other.mBoundary.mPrev;

        mBoundary.mNext = other.isEmpty() ? &mBoundary : otherNext;
        mBoundary.mPrev = other.isEmpty() ? &mBoundary : otherPrev;
        other.mBoundary.mNext = isEmpty() ? &other.mBoundary : thisNext;
        other.mBoundary.mPrev = isEmpty() ? &other.mBoundary : thisPrev;

        capu::swap(mSize, other.mSize);

        // correct next and first of elements next to the boundary
        mBoundary.mPrev->mNext = &mBoundary;
        other.mBoundary.mPrev->mNext = &(other.mBoundary);
        mBoundary.mNext->mPrev = &mBoundary;
        other.mBoundary.mNext->mPrev = &(other.mBoundary);

        // comparator is default constructed and type safe
        //  -> don't swap comparator
        return *this;
    }

    template <class T, class A, class C>
    inline bool_t List<T, A , C>::contains(const T& element) const
    {
        return find(element) != -1;
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::insert(Iterator& iter, const T& element)
    {
        status_t retVal = insertElement(iter.mCurrentNode->mPrev, element); // insert before found position (stl)
        if (retVal == CAPU_OK)
        {
            // adjust index if insert was successful
            iter.mIndex++;
        }
        return retVal;
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::insert(const uint_t index, const T& element)
    {
        if (index > mSize) // if index == mSize, element is added at the end
        {
            // invalid index
            return CAPU_EINVAL;
        }

        return insertElement(findElement(index)->mPrev, element); // insert before found position
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::insert(const T& element)
    {
        return insertElement(mBoundary.mPrev, element); // insert at list end (boundary->mPrev)
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::push_back(const T& element)
    {
        return insertElement(mBoundary.mPrev, element); // insert at list end (boundary->mPrev)
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::push_front(const T& element)
    {
        return insertElement(&mBoundary, element); // insert at list begin
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::insertElement(typename List<T, A , C>::ListNode* addPosition, const T& element)
    {
        ListNode* newNode = mAllocator.allocate();
        if (NULL == newNode)
        {
            return CAPU_ENO_MEMORY;
        }
        newNode->mData = element; // copy in
        newNode->mNext = addPosition->mNext;
        newNode->mPrev = addPosition;
        addPosition->mNext->mPrev = newNode;
        addPosition->mNext = newNode;
        ++mSize;
        return CAPU_OK;
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::deleteElement(typename List<T, A , C>::ListNode* deletePosition)
    {
        deletePosition->mPrev->mNext = deletePosition->mNext;
        deletePosition->mNext->mPrev = deletePosition->mPrev;
        mAllocator.deallocate(deletePosition);
        --mSize;
        return CAPU_OK;
    }

    template <class T, class A, class C>
    typename List<T, A , C>::ListNode* List<T, A , C>::findElement(const uint_t index) const
    {
        // search element by running through list from the first element
        uint_t counter = 0;
        ListNode* current = mBoundary.mNext;
        while (counter < index)
        {
            ++counter;
            current = current->mNext;
        }

        // current is the element that was requested
        return current;
    }

    template <class T, class A, class C>
    status_t List<T, A, C>::remove(const T& element)
    {
        ListNode* current = mBoundary.mNext;
        while (current != &mBoundary)
        {
            if (mComparator(current->mData, element))
            {
                // deletion element found
                deleteElement(current);
                return CAPU_OK;
            }
            current = current->mNext;
        }
        return CAPU_EINVAL;
    }

    template <class T, class A, class C>
    status_t List<T, A , C>::pop_back()
    {
        ListNode* current = mBoundary.mPrev;
        if (current != &mBoundary)
        {
            return deleteElement(current);
        }
        return CAPU_EINVAL;
    }

    template <class T, class A, class C>
    status_t List<T, A , C>::pop_front()
    {
        ListNode* current = mBoundary.mNext;
        if (current != &mBoundary)
        {
            return deleteElement(current);
        }
        return CAPU_EINVAL;
    }

    template <class T, class A, class C>
    status_t List<T, A , C>::erase(const uint_t index, T* elementOld)
    {
        if (index >= mSize)
        {
            // invalid index
            return CAPU_EINVAL;
        }

        ListNode* toDelete = findElement(index);
        if (elementOld)
        {
            *elementOld = toDelete->mData; // copy out
        }
        return deleteElement(toDelete);
    }

    template <class T, class A, class C>
    status_t List<T, A , C>::erase(Iterator& listIterator, T* elementOld)
    {
        if (listIterator != end())
        {
            if (elementOld)
            {
                *elementOld = listIterator.mCurrentNode->mData; // copy out
            }
            ListNode* node = listIterator.mCurrentNode;
            listIterator.mCurrentNode = listIterator.mCurrentNode->mNext;
            listIterator.mIndex--;
            return deleteElement(node);
        }
        return CAPU_EINVAL;
    }

    template <class T, class A, class C>
    inline status_t List<T, A , C>::set(const uint_t index, const T& element, T* elementOld)
    {
        if (index >= mSize)
        {
            // invalid index
            return CAPU_EINVAL;
        }

        ListNode* node = findElement(index);
        if (elementOld)
        {
            *elementOld = node->mData; // copy out
        }
        node->mData = element; // copy in
        return CAPU_OK;
    }

    template <class T, class A, class C>
    T List<T, A , C>::get(const uint_t index, status_t* result) const
    {
        if (index >= mSize)
        {
            // invalid index
            if (result)
            {
                *result = CAPU_EINVAL;
            }
            return T();
        }

        T element = findElement(index)->mData; // copy out
        if (result)
        {
            *result = CAPU_OK;
        }

        return element;
    }

    template <class T, class A, class C>
    inline uint_t List<T, A , C>::size() const
    {
        return mSize;
    }

    template <class T, class A, class C>
    inline bool_t List<T, A , C>::isEmpty() const
    {
        return mSize == 0;
    }

    template <class T, class A, class C>
    inline bool_t List<T, A , C>::empty() const
    {
        return isEmpty();
    }

    template <class T, class A, class C>
    typename List<T, A , C>::Iterator List<T, A , C>::begin()
    {
        return ListIterator(&mBoundary);
    }

    template <class T, class A, class C>
    const typename List<T, A , C>::Iterator List<T, A , C>::begin() const
    {
        const ListIterator result(&mBoundary);
        return result;
    }

    template <class T, class A, class C>
    typename List<T, A , C>::Iterator List<T, A , C>::end()
    {
        ListIterator result(&mBoundary);

        result.mCurrentNode = result.mCurrentNode->mPrev;
        result.mIndex = size();
        return result;
    }

    template <class T, class A, class C>
    const typename List<T, A , C>::Iterator List<T, A , C>::end() const
    {
        const ListIterator result(&mBoundary);

        // hack to get to a non const pointer to mBoundary without const cast
        result.mCurrentNode = result.mCurrentNode->mPrev;
        result.mIndex = size();
        return result;
    }

    template <class T, class A, class C>
    T& List<T, A , C>::front()
    {
        return mBoundary.mNext->mData;
    }

    template <class T, class A, class C>
    const T& List<T, A , C>::front() const
    {
        return mBoundary.mNext->mData;
    }

    template <class T, class A, class C>
    T& List<T, A , C>::back()
    {
        return mBoundary.mPrev->mData;
    }

    template <class T, class A, class C>
    const T& List<T, A , C>::back() const
    {
        return mBoundary.mPrev->mData;
    }

    template <class T, class A, class C>
    bool_t List<T, A, C>::operator!=(const List<T, A, C>& other)
    {
        return !(operator==(other));
    }

    template <class T, class A, class C>
    bool_t List<T, A, C>::operator==(const List<T, A, C>& other)
    {
        if (size() != other.size())
        {
            return false;
        }

        ListIterator thisIter = begin();
        ListIterator otherIter = other.begin();
        while (thisIter != end())
        {
            if ((*thisIter) != (*otherIter))
            {
                // if first element is different, we can stop equality check
                return false;
            }
            ++thisIter;
            ++otherIter;
        }

        // reached the end of both lists, this means they are equal
        return true;
    }

    template <class T, class A, class C>
    List<T, A , C>::ListIterator::ListIterator(const ListNode* boundary)
        : mInitialNode(boundary)
        , mCurrentNode(boundary->mNext)
        , mIndex(mCurrentNode == boundary ? -1 : 0) // if list is empty, the index is -1, otherwise 0.
    {
    }

    template <class T, class A, class C>
    List<T, A , C>::ListIterator::~ListIterator()
    {
    }

    template <class T, class A, class C>
    T& List<T, A , C>::ListIterator::operator*()
    {
        return mCurrentNode->mData;
    }

    template <class T, class A, class C>
    T* List<T, A , C>::ListIterator::operator->()
    {
        return &** this;
    }

    template <class T, class A, class C>
    capu::bool_t List<T, A , C>::ListIterator::operator==(const typename List<T, A , C>::ListIterator& other) const
    {
        return mCurrentNode == other.mCurrentNode;
    }

    template <class T, class A, class C>
    capu::bool_t List<T, A , C>::ListIterator::operator!=(const typename List<T, A , C>::ListIterator& other) const
    {
        return !(*this == other);
    }

    template <class T, class A, class C>
    const typename List<T, A , C>::ListIterator& List<T, A , C>::ListIterator::operator++() const
    {
        mCurrentNode = mCurrentNode->mNext;
        ++mIndex;
        return *this;
    }

    template <class T, class A, class C>
    typename List<T, A , C>::ListIterator& List<T, A , C>::ListIterator::operator++()
    {
        mCurrentNode = mCurrentNode->mNext;
        ++mIndex;
        return *this;
    }

    template <class T, class A, class C>
    const typename List<T, A , C>::ListIterator List<T, A , C>::ListIterator::operator++(capu::int32_t) const
    {
        typename List<T, A , C>::ListIterator oldValue(*this);
        ++(*this);
        return oldValue;
    }

    template <class T, class A, class C>
    typename List<T, A , C>::ListIterator List<T, A , C>::ListIterator::operator++(capu::int32_t)
    {
        typename List<T, A , C>::ListIterator oldValue(*this);
        ++(*this);
        return oldValue;
    }

    template <class T, class A, class C>
    uint_t List<T, A , C>::ListIterator::currentIndex() const
    {
        return mIndex;
    }
}

#endif /* CAPU_LIST_H */
