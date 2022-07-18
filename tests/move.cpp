/**
 * Copyright (c) 2022 Luca Erbetta
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

#include <cassert>
#include <memory>

#include "utils/collections/SyncCircularBuffer.h"

using namespace std;
int main()
{
    SyncCircularBuffer<unique_ptr<int>, 200> buf1;
    SyncCircularBuffer<int, 200> buf2;

    buf1.put(make_unique<int>(10));  // put(T&&) gets called
    buf2.put(10);

    unique_ptr<int> vpop = buf1.pop();  // Ok
    // unique_ptr<int> vget =
    //     buf1.get();  // Compilation error: SyncCircularBuffer<unique_ptr<int>,
    //                  // 200>::get() is not enabled!

    int vget2 =
        buf2.get();  // OK! SyncCircularBuffer<int, 200>::get() is enabled

    assert(*vpop == 10);
    assert(vget2 == 10);
}