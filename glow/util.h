//
// OpenGL Object Wrapper
//
// Copyright 2016-2019 Sean Farrell <sean.farrell@rioki.org>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef _GLOW_UTIL_H_
#define _GLOW_UTIL_H_

#include <vector>
#include <string>

#include "defines.h"

namespace glow
{
	/*!
	 * Clear all pending OpenGL errors.
	 */
    GLOW_EXPORT
    void clear_errors();

	/*!
	 * Get the last OpenGL error as string.
	 *
	 * @return the last OpenGL error as string
	 */
    GLOW_EXPORT
    std::string get_error();

	/*!
	 * Initialize glow in the current OpenGL context.
	 *
	 * You need to call init in each OpenGL context you intend to use glow
	 * before calling anyhting else.
	 */
    GLOW_EXPORT
    void init();
}

#endif
