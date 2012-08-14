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

#ifndef LASTJSON_STRINGREP_HPP__
#define LASTJSON_STRINGREP_HPP__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "impl_helpers.hpp"

namespace lastjson {

namespace impl {

template<class Iterator>
inline void unescape_string_inplace(Iterator & it, Iterator end, Iterator & a, Iterator & b)
{
    Iterator const str_begin = it;
    Iterator outit = it;

    while (it != end)
    {
        if (*it == '"')
        {
            ++it;
            a = str_begin;
            b = outit;
            return;
        }
        else if (*it == '\\')
        {
            ++it;
            if (it == end)
            {
                break;
            }

            char const esc = *(it++);
            switch (esc)
            {
            case 'b':
                *(outit++) = '\b';
                break;
            case 'f':
                *(outit++) = '\f';
                break;
            case 'n':
                *(outit++) = '\n';
                break;
            case 'r':
                *(outit++) = '\r';
                break;
            case 't':
                *(outit++) = '\t';
                break;
            case '/': case '\\': case '"':
                *(outit++) = esc;
                break;
            case 'u':
            {
                uint16_t codepoint = read4hex(it, end);
                if (codepoint < 0x80)
                {
                    *(outit++) = codepoint;
                }
                else if (codepoint < 0x800)
                {
                    *(outit++) = 0xc0 | ((codepoint >> 6) & 0x1f);
                    *(outit++) = 0x80 | (codepoint & 0x3f);
                }
                else if (codepoint < 0xd800 || codepoint >= 0xe000)
                {
                    *(outit++) = 0xe0 | ((codepoint >> 12) & 0x0f);
                    *(outit++) = 0x80 | ((codepoint >> 6) & 0x3f);
                    *(outit++) = 0x80 | (codepoint & 0x3f);
                }
                else if (codepoint < 0xdc00)
                {
                    if (it == end || *it != '\\' || ++it == end || *it != 'u')
                    {
                        throw parser_error("error decoding surrogate unicode escape sequence");
                    }

                    ++it;
                    uint16_t codepoint2 = read4hex(it, end);
                    if (codepoint2 < 0xdc00 || codepoint2 >= 0xe000)
                    {
                        throw parser_error("error decoding surrogate unicode escape sequence");
                    }

                    uint32_t cp = (((codepoint & 0x3ff) << 10) | (codepoint2 & 0x3ff)) + 0x10000;
                    *(outit++) = 0xf0 | ((cp >> 18) & 0x07);
                    *(outit++) = 0x80 | ((cp >> 12) & 0x3f);
                    *(outit++) = 0x80 | ((cp >> 6) & 0x3f);
                    *(outit++) = 0x80 | (cp & 0x3f);
                }
                else
                {
                    throw parser_error("error decoding surrogate unicode escape sequence");
                }
            }
            break;
            default:
                throw parser_error("error while parsing backslash escape sequence");
            }
        }
        else
        {
            *(outit++) = *(it++);
        }
    }

    throw parser_error("premature end of json data while parsing string");
}

template<class IntType>
inline void output_hex(std::ostream & out, IntType value, unsigned int digits)
{
    while (digits > 0)
    {
        --digits;
        out << ("0123456789abcdef"[(value >> (digits * 4)) & 0xf]);
    }
}

} // namespace impl

inline std::ostream & escape_string(std::ostream & out, std::string const & txt,
                                    bool const escape_utf8 = true, bool const escape_slash = false)
{
    out << '"';

    for (std::string::const_iterator it = txt.begin(); it != txt.end(); ++it)
    {
        unsigned char const c = *it;

        if (c < 0x20)   // control character
        {
            switch (c)
            {
            case '\b':
                out << "\\b";
                break;
            case '\f':
                out << "\\f";
                break;
            case '\n':
                out << "\\n";
                break;
            case '\r':
                out << "\\r";
                break;
            case '\t':
                out << "\\t";
                break;
            default:
                out << "\\u00";
                impl::output_hex(out, c, 2);
            }
        }
        else if (c >= 0x80)
        {
            if (escape_utf8)
            {
                if ((c & 0xe0) == 0xc0) // two byte sequence
                {
                    uint16_t codepoint = (c &  0x1f) << 6;
                    ++it;
                    if (it == txt.end() || (*it & 0xc0) != 0x80)
                    {
                        throw utf8_sequence_error();
                    }

                    codepoint |= (*it & 0x3f);

                    out << "\\u";
                    impl::output_hex(out, codepoint, 4);
                }
                else if ((c & 0xf0) == 0xe0) // three byte sequence
                {
                    uint16_t codepoint = (c &  0x0f) << 12;
                    ++it;
                    if (it == txt.end() || (*it & 0xc0) != 0x80)
                    {
                        throw utf8_sequence_error();
                    }

                    codepoint |= (*it & 0x3f) << 6;
                    ++it;
                    if (it == txt.end() || (*it & 0xc0) != 0x80)
                    {
                        throw utf8_sequence_error();
                    }

                    codepoint |= (*it & 0x3f);

                    out << "\\u";
                    impl::output_hex(out, codepoint, 4);
                }
                else if ((c & 0xf8) == 0xf0) // four byte sequence
                {
                    uint32_t codepoint = (c &  0x07) << 18;
                    ++it;
                    if (it == txt.end() || (*it & 0xc0) != 0x80)
                    {
                        throw utf8_sequence_error();
                    }

                    codepoint |= (*it & 0x3f) << 12;
                    ++it;
                    if (it == txt.end() || (*it & 0xc0) != 0x80)
                    {
                        throw utf8_sequence_error();
                    }

                    codepoint |= (*it & 0x3f) << 6;
                    ++it;
                    if (it == txt.end() || (*it & 0xc0) != 0x80)
                    {
                        throw utf8_sequence_error();
                    }

                    codepoint |= (*it & 0x3f);

                    if (codepoint < 0x10000)
                    {
                        if (codepoint >= 0xd800 && codepoint < 0xe000)
                        {
                            throw utf8_sequence_error();
                        }

                        out << "\\u";
                        impl::output_hex(out, codepoint, 4);
                    }
                    else if (codepoint <= 0x10ffff)
                    {
                        codepoint -= 0x10000;
                        out << "\\u";
                        impl::output_hex(out, ((codepoint & 0xffc00) >> 10) | 0xd800, 4);
                        out << "\\u";
                        impl::output_hex(out, (codepoint & 0x003ff) | 0xdc00, 4);
                    }
                    else
                    {
                        throw utf8_sequence_error();
                    }
                }
                else
                {
                    throw utf8_sequence_error();
                }
            }
            else
            {
                out << c;
            }
        }
        else
        {
            switch (c)
            {
            case '"':
                out << "\\\"";
                break;
            case '\\':
                out << "\\\\";
                break;
            case '/':
                if (escape_slash)
                {
                    out << "\\/";
                }
                else
                {
                    out << '/';
                }

                break;
            default:
                out << c;
            }
        }
    }

    out << '"';
    return out;
}

inline std::string escape_string(std::string const & txt,
                                 bool const escape_utf8 = true, bool const escape_slash = false)
{
    std::ostringstream ss;
    escape_string(ss, txt, escape_utf8, escape_slash);
    return ss.str();
}

} // namespace lastjson

#endif // ifndef LASTJSON_STRINGREP_HPP__
