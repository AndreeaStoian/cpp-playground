#ifndef GUS_LIB_CUSTOM_MUTEX_H
#define GUS_LIB_CUSTOM_MUTEX_H

//   This file is part of the guslib library, licensed under the terms of the MIT License.
//
//   The MIT License
//   Copyright (C) 2010-2016  Augustin Preda (thegusty999@gmail.com)
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy
//   of this software and associated documentation files (the "Software"), to deal
//   in the Software without restriction, including without limitation the rights
//   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//   copies of the Software, and to permit persons to whom the Software is
//   furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in
//   all copies or substantial portions of the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//   THE SOFTWARE.

#include <guslib/guslibbuildopts.h>

namespace guslib
{
  class GUSLIB_EXPORT_SYMBOL CustomMutex
  {
  public:
    CustomMutex();
    virtual ~CustomMutex();

  public:
    void lock();
    void unlock();

  private:
    // Internal class to store the implementation.
    class Impl;

    // Pointer to implementation (pImpl).
    Impl* impl_;
  };

  class GUSLIB_EXPORT_SYMBOL CustomLock
  {
  public:
    CustomLock(CustomMutex& mutex);
    virtual ~CustomLock();

  private:
    CustomMutex& mutex_;
  };
}

#endif  // GUS_LIB_SMARTPTR_H
