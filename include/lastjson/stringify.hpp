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

#ifndef LASTJSON_STRINGIFY_HPP__
#define LASTJSON_STRINGIFY_HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "stringrep.hpp"
#include "value.hpp"

namespace lastjson {

namespace impl {

template<typename Iterator>
inline void write_json_object(std::ostream & stream, Iterator begin, Iterator end);

template<typename Iterator>
inline void write_json_array(std::ostream & stream, Iterator begin, Iterator end);

} // namespace impl

inline void write_json(std::ostream & stream, bool data)
{
    if (data)
    {
        stream << "true";
    }
    else
    {
        stream << "false";
    }
}

inline void write_json(std::ostream & stream, char data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, unsigned char data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, signed char data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, unsigned short data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, signed short data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, unsigned int data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, signed int data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, unsigned long data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, signed long data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, unsigned long long data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, signed long long data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, float data)
{
    stream << std::dec << data;
}
inline void write_json(std::ostream & stream, double data)
{
    stream << std::dec << data;
}

inline void write_json(std::ostream & stream, std::string const & data)
{
    escape_string(stream, data);
}

inline void write_json(std::ostream & stream, char const * data)
{
    escape_string(stream, data);
}

template<typename T>
inline void write_json(std::ostream & stream, std::map<std::string, T> const & data)
{
    impl::write_json_object(stream, data.begin(), data.end());
}

template<typename T>
inline void write_json(std::ostream & stream, std::vector< std::pair<std::string, T> > const & data)
{
    impl::write_json_object(stream, data.begin(), data.end());
}

template<typename T>
inline void write_json(std::ostream & stream, std::vector<T> const & data)
{
    impl::write_json_array(stream, data.begin(), data.end());
}

template<class V>
inline void write_json(std::ostream & stream, basic_value<V> const & data)
{
    data.write_json(stream);
}

template<typename T>
inline std::string stringify(T const & t)
{
    std::ostringstream ss;
    write_json(ss, t);
    return ss.str();
}

template<class V>
inline void basic_value<V>::write_json(std::ostream & stream) const
{
    switch (m_type)
    {
    case JSONNULL:
        stream << "null";
        break;
    case BOOL:
        ::lastjson::write_json(stream, m_data.boolean);
        break;
    case INT:
        ::lastjson::write_json(stream, m_data.integer);
        break;
    case FLOAT:
        ::lastjson::write_json(stream, m_data.floatingpoint);
        break;
    case STRING:
        ::lastjson::write_json(stream, string_ref_dangerous());
        break;
    case ARRAY:
        ::lastjson::write_json(stream, array_ref_dangerous());
        break;
    case OBJECT:
        ::lastjson::write_json(stream, object_ref_dangerous());
        break;
    default:
        throw std::logic_error("basic_value::write_json encountered invalid type");
    }
}

namespace impl {

template<typename Iterator>
inline void write_json_object(std::ostream & stream, Iterator begin, Iterator end)
{
    stream << '{';
    if (begin != end)
    {
        escape_string(stream, begin->first);
        stream << ':';
        write_json(stream, begin->second);

        for (++begin; begin != end; ++begin)
        {
            stream << ',';
            escape_string(stream, begin->first);
            stream << ':';
            write_json(stream, begin->second);
        }
    }

    stream << '}';
}

template<typename Iterator>
inline void write_json_array(std::ostream & stream, Iterator begin, Iterator end)
{
    stream << '[';
    if (begin != end)
    {
        write_json(stream, *begin);

        for (++begin; begin != end; ++begin)
        {
            stream << ',';
            write_json(stream, *begin);
        }
    }

    stream << ']';
}

} // namespace impl

} // namespace lastjson

#endif // ifndef LASTJSON_STRINGIFY_HPP__
