/* Last.json (c) 2012 Sven Over <sven@last.fm>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef LASTJSON_IMPL_HELPERS_HPP__
#define LASTJSON_IMPL_HELPERS_HPP__

#include <string>
#include <stdexcept>

#ifdef LASTJSON_CXX11
# include <memory>
# include <cstdint>
#else
# include <boost/shared_ptr.hpp>
# include <boost/integer.hpp>
#endif

#include "exceptions.hpp"

namespace lastjson {

#ifdef LASTJSON_CXX11
using std::shared_ptr;
using std::static_pointer_cast;
using std::int8_t;
using std::uint8_t;
using std::int16_t;
using std::uint16_t;
using std::int32_t;
using std::uint32_t;
using std::int64_t;
using std::uint64_t;
#else
using boost::shared_ptr;
using boost::static_pointer_cast;
using boost::int8_t;
using boost::uint8_t;
using boost::int16_t;
using boost::uint16_t;
using boost::int32_t;
using boost::uint32_t;
using boost::int64_t;
using boost::uint64_t;
#endif

namespace impl {

template<class Iterator>
inline void skipws(Iterator & begin, Iterator end)
{
    // The JSON standard defines whitespace characters as any of these four:
    //     x20  Space
    //     x09  Horizontal tab
    //     x0A  Line feed or New line
    //     x0D  Carriage return

    while (begin != end && (*begin == 0x20 || *begin == 0x09 || *begin == 0x0a || *begin== 0x0d))
    {
        ++begin;
    }
}

/**
 * \brief Consumes 4 characters as a hexadecimal literal and converts them to a number
 */
template<class Iterator>
inline uint16_t read4hex(Iterator & it, Iterator end)
{
    uint16_t rv = 0;

    for (int i = 0; i < 4; ++i)
    {
        if (it == end)
        {
            // we reached the end of the string before we got 4 characters
            throw parser_error("premature end of json string");
        }

        char const c = *it;

        // left-shift return value by 4 bits (one hex digit) before
        // we OR the value of this character to it
        rv <<= 4;

        if (c >= '0' && c <= '9')
        {
            rv |= c-'0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            rv |= c-'a' + 0xa;
        }
        else if (c >= 'A' && c <= 'F')
        {
            rv |= c-'A' + 0xa;
        }
        else
        {
            throw parser_error("error decoding unicode escape sequence");
        }

        ++it;
    }

    return rv;
}

} // namespace impl
} // namespace lastjson

#endif // ifndef LASTJSON_IMPL_HELPERS_HPP__

