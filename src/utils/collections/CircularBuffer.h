/* Copyright (c) 2015-2018 Skyward Experimental Rocketry
 * Author: Luca Erbetta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <utility>

using std::range_error;

/**
 * Implementation of an non-synchronized circular buffer.
 */
template <typename T, unsigned int Size>
class CircularBuffer
{
    static_assert(Size > 0, "Circular buffer size must be greater than 0!");

public:
    CircularBuffer() {}

    ~CircularBuffer() {}

    /**
     * @brief Puts a copy of the element in the buffer.
     *
     * @param elem Element to be added to the queue.
     * @return The element added.
     */
    template <typename T1 = T,
              typename    = std::enable_if_t<std::is_copy_assignable_v<T1>>>
    T& put(const T& elem)
    {
        buffer[writePtr] = elem;
        return put();
    }

    /**
     * @brief Puts a copy of the element in the buffer.
     *
     * @param elem Element to be added to the queue.
     * @return The element added.
     */
    T& put(T&& elem)
    {
        buffer[writePtr] = std::move(elem);
        return put();
    }

    /**
     * @brief Gets an element from the buffer, without removing it.
     *
     * Index starts from the oldest element in the buffer.
     * get() returns the first element.
     *
     * @warning Remember to catch the exception!
     * @throw range_error if index >= count().
     * @param i Index of the element to get, starting from the oldest.
     * @return The element.
     */
    T& get(unsigned int i = 0)
    {
        if (i < CircularBuffer<T, Size>::count())
        {
            int ptr = (readPtr + i) % Size;
            return buffer[ptr];
        }
        else
            throw range_error("Index out of range");
    }

    /**
     * @brief Returns the last element added in the buffer.
     *
     * @warning Remember to catch the exception!
     * @throw range_error if buffer is empty.
     * @return The element.
     */
    T& last() { return get(count() - 1); }

    /**
     * @brief Pops the first element in the buffer.
     *
     * @warning Remember to catch the exception!
     * @throw range_error if buffer is empty.
     * @return The element that has been popped.
     */
    T& pop()
    {
        if (!empty)
        {
            size_t ptr = readPtr;
            readPtr    = (readPtr + 1) % Size;

            empty = readPtr == writePtr;

            return buffer[ptr];
        }
        else
            throw range_error("CircularBuffer is empty!");
    }

    /**
     * @brief Counts the elements in the buffer.
     *
     * @return Number of elements in the buffer.
     */
    size_t count() const
    {
        if (empty)
            return 0;
        if (writePtr > readPtr)
        {
            return writePtr - readPtr;
        }
        else
        {
            return Size - readPtr + writePtr;
        }
    }

    bool isEmpty() const { return empty; }

    bool isFull() const { return CircularBuffer<T, Size>::count() == Size; }
    /**
     * @brief Returns the maximum number of elements that can be stored in the
     * buffer.
     *
     * @return Buffer size.
     */
    size_t getSize() const { return Size; }

private:
    T& put()
    {
        T& added = buffer[writePtr];

        // Advance the read pointer if the two pointers match
        if (!empty && writePtr == readPtr)
            readPtr = (readPtr + 1) % Size;

        // Advance the write pointer
        writePtr = (writePtr + 1) % Size;

        empty = false;

        return added;
    }

    T buffer[Size];

    size_t writePtr = 0;
    size_t readPtr  = 0;
    bool empty      = true;
};
