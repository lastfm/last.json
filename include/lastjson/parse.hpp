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

#ifndef LASTJSON_PARSER_HPP__
#define LASTJSON_PARSER_HPP__

#ifdef LASTJSON_CXX11
# include <string>
#else
# include <boost/lexical_cast.hpp>
# define stoll(x) boost::lexical_cast<value::int_type>(x)
# define stod(x) boost::lexical_cast<value::float_type>(x)
#endif

#include "value.hpp"
#include "stringrep.hpp"
#include "impl_helpers.hpp"

namespace lastjson {

namespace impl {

inline value parse_fragment(std::string::iterator & it, std::string::iterator end)
{
    if (it == end)
    {
        throw parser_error("premature end of json data");
    }

    switch (*it)
    {
    case 'n':
        ++it;
        if (it != end && *it == 'u')
        {
            ++it;
            if (it != end && *it == 'l')
            {
                ++it;
                if (it != end && *it == 'l')
                {
                    ++it;
                    return value();
                }
            }
        }

        throw parser_error("invalid json data");

    case 'f':
        ++it;
        if (it != end && *it == 'a')
        {
            ++it;
            if (it != end && *it == 'l')
            {
                ++it;
                if (it != end && *it == 's')
                {
                    ++it;
                    if (it != end && *it == 'e')
                    {
                        ++it;
                        return value(false);
                    }
                }
            }
        }

        throw parser_error("invalid json data");

    case 't':
        ++it;
        if (it != end && *it == 'r')
        {
            ++it;
            if (it != end && *it == 'u')
            {
                ++it;
                if (it != end && *it == 'e')
                {
                    ++it;
                    return value(true);
                }
            }
        }

        throw parser_error("invalid json data");

    case '"':
    {
        ++it;
        std::string::iterator a, b;
        unescape_string_inplace(it, end, a, b);
        return value(a, b);
    }

    case '[':
    {
        ++it;
        skipws(it, end);
        if (it == end)
        {
            throw parser_error("premature end of json data while parsing array");
        }

        value::array_pointer array_ptr(new value::array_type);
        value::array_type & array = *array_ptr;
        if (*it != ']')
        {
            while (true)
            {
                array.push_back(parse_fragment(it, end));
                skipws(it, end);
                if (it == end)
                {
                    throw parser_error("premature end of json data while parsing array");
                }

                if (*it == ']')
                {
                    break;
                }

                if (*it != ',')
                {
                    throw parser_error("error parsing json array");
                }

                ++it;
                skipws(it, end);
            }
        }

        ++it;
        return value(array_ptr);
    }

    case '{':
    {
        ++it;
        skipws(it, end);
        if (it == end)
        {
            throw parser_error("premature end of json data while parsing object");
        }

        value::object_pointer object_ptr(new value::object_type);
        value::object_type & object = *object_ptr;
        if (*it != '}')
        {
            while (true)
            {
                if (*it != '"')
                {
                    throw parser_error("error parsing json object");
                }

                ++it;
                std::string::iterator key_a, key_b;
                unescape_string_inplace(it, end, key_a, key_b);
                skipws(it, end);
                if (it == end)
                {
                    throw parser_error("premature end of json data while parsing object");
                }

                if (*it != ':')
                {
                    throw parser_error("error parsing json object");
                }

                ++it;
                skipws(it, end);
                if (it == end)
                {
                    throw parser_error("premature end of json data while parsing object");
                }

                object[std::string(key_a, key_b)] = parse_fragment(it, end);
                skipws(it, end);
                if (it == end)
                {
                    throw parser_error("premature end of json data while parsing object");
                }

                if (*it == '}')
                {
                    break;
                }

                if (*it != ',')
                {
                    throw parser_error("premature end of json data while parsing object");
                }

                ++it;
                skipws(it, end);
            }
        }

        ++it;
        return value(object_ptr);
    }

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '+': case '-': case '.':
    {
        int sign = 1;
        if (*it == '+')
        {
            ++it;
        }
        else if  (*it == '-')
        {
            sign = -1;
            ++it;
        }

        std::string::iterator const digits_begin = it;
        while (it != end && *it >= '0' && *it <= '9')
        {
            ++it;
        }

        if (it != end && (*it == '.' || *it == 'e' || *it == 'E'))
        {
            // this is a floating point number
            if (*it == '.')
            {
                ++it;
                while (it != end && *it >= '0' && *it <= '9')
                {
                    ++it;
                }

                if (it-1 == digits_begin)
                {
                    // only a period, no digits
                    throw parser_error("invalid json data");
                }
            }

            if (it != end && (*it == 'e' || *it == 'E'))
            {
                ++it;
                if (it == end)
                {
                    throw parser_error("premature end of json data while parsing float");
                }

                if (*it == '+' || *it == '-')
                {
                    ++it;
                }

                if (it == end)
                {
                    throw parser_error("premature end of json data while parsing float");
                }

                if (*it < '0' || *it > '9')
                {
                    throw parser_error("invalid json data");
                }

                ++it;
                while (it != end && *it >= '0' && *it <= '9')
                {
                    ++it;
                }
            }

            return value(sign * stod(std::string(digits_begin, it)));
        }
        else
        {
            if (it == digits_begin)
            {
                throw parser_error("invalid json data");
            }

            return value(sign * stoll(std::string(digits_begin, it)));
        }
    }
    default:
        break;
    }

    throw parser_error("invalid json data");
};

inline value parse(std::string::iterator begin, std::string::iterator end)
{
    value result = parse_fragment(begin, end);
    skipws(begin, end);
    if (begin != end)
    {
        std::cerr << "additional data: [[" << std::string(begin, end) << "]]" << std::endl;
        throw parser_error("additional data at the end of json data");
    }

    return result;
}

} // namespace impl

inline value parse(std::string::const_iterator begin, std::string::const_iterator end)
{
    impl::skipws(begin, end);
    std::string copy(begin, end);
    std::string::iterator it = copy.begin();
    return impl::parse(it, copy.end());
}

inline value parse(std::string const & str)
{
    return parse(str.begin(), str.end());
}

inline value parse_destructive(std::string::iterator begin, std::string::iterator end)
{
    impl::skipws(begin, end);
    return impl::parse(begin, end);
}

inline value parse_destructive(std::string str)
{
    std::string::iterator it = str.begin();
    return parse_destructive(it, str.end());
}

} // namespace lastjson

#endif // ifndef LASTJSON_PARSER_HPP__
