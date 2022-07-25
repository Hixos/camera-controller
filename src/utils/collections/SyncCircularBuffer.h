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

#include <condition_variable>
#include <mutex>
#include <type_traits>

#include "CircularBuffer.h"

using std::condition_variable;
using std::lock_guard;
using std::mutex;
using std::unique_lock;

/**
 * Implementation of an non-synchronized circular buffer.
 */
template <typename T, unsigned int Size>
class SyncCircularBuffer
{
    static_assert(Size > 0, "Circular buffer size must be greater than 0!");
    using Me = SyncCircularBuffer<T, Size>;

public:
    SyncCircularBuffer() : buf() {}

    ~SyncCircularBuffer() {}

    /**
     * @brief Puts a copy of the element in the buffer.
     *
     * @param elem Element to be added to the queue.
     * @return The element added.
     */
    template <typename T1 = T,
              typename    = std::enable_if_t<std::is_copy_assignable_v<T1>>>
    Me& put(const T& elem)
    {
        unique_lock<mutex> lock(mut);
        buf.put(elem);

        lock.unlock();
        cv.notify_one();
        return *this;
    }

    /**
     * @brief Moves the element in the buffer.
     *
     * @param elem Element to be added to the queue.
     * @return The element added.
     */
    Me& put(T&& elem)
    {
        unique_lock<mutex> lock(mut);
        buf.put(std::move(elem));

        lock.unlock();
        cv.notify_one();
        return *this;
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
    template <typename T1 = T,
              typename    = std::enable_if_t<std::is_copy_assignable_v<T1>>>
    T get(unsigned int i = 0)
    {
        lock_guard<mutex> lock(mut);
        return buf.get(i);
    }

    /**
     * @brief Gets the first element in the buffer. This call block until an
     * element is available.
     *
     * @warning Remember to catch the exception!
     * @throw range_error if buffer is empty.
     * @return The element that has been popped.
     */
    template <typename T1 = T,
              typename    = std::enable_if_t<std::is_copy_assignable_v<T1>>>
    T getBlocking()
    {
        unique_lock<mutex> lock(mut);
        cv.wait(lock, [&] { return buf.isEmpty() == false; });
        return buf.get();
    }

    /**
     * @brief Returns the last element added in the buffer.
     *
     * @warning Remember to catch the exception!
     * @throw range_error if buffer is empty.
     * @return The element.
     */
    template <typename T1 = T,
              typename    = std::enable_if_t<std::is_copy_assignable_v<T1>>>
    T last()
    {
        lock_guard<mutex> lock(mut);
        return buf.last();
    }

    /**
     * @brief Pops the first element in the buffer.
     *
     * @warning Remember to catch the exception!
     * @throw range_error if buffer is empty.
     * @return The element that has been popped.
     */
    T pop()
    {
        lock_guard<mutex> lock(mut);
        return std::move(buf.pop());
    }

    /**
     * @brief Pops the first element in the buffer. This call block until an
     * element is available.
     *
     *
     * @warning Remember to catch the exception!
     * @throw range_error if buffer is empty.
     * @return The element that has been popped.
     */
    T popBlocking()
    {
        unique_lock<mutex> lock(mut);
        cv.wait(lock, [&] { return buf.isEmpty() == false; });
        return std::move(buf.pop());
    }

    /**
     * @brief Waits until the buffer contains at least one element.
     */
    void waitUntilNotEmpty()
    {
        unique_lock<mutex> lock(mut);
        cv.wait(lock, [&] { return buf.isEmpty() == false; });
    }

    /**
     * @brief Counts the elements in the buffer.
     *
     * @return Number of elements in the buffer.
     */
    size_t count() const
    {
        lock_guard<mutex> lock(mut);
        return buf.count();
    }

    bool isEmpty() const
    {
        lock_guard<mutex> lock(mut);
        return buf.isEmpty();
    }

    bool isFull() const
    {
        lock_guard<mutex> lock(mut);
        return buf.isFull();
    }

    /**
     * @brief Returns the maximum number of elements that can be stored in the
     * buffer.
     *
     * @return Buffer size.
     */
    size_t getSize() const { return Size; }

private:
    CircularBuffer<T, Size> buf;

    mutable mutex mut;
    mutable condition_variable cv;
};
