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

#ifndef LASTJSON_VALUE_HPP__
#define LASTJSON_VALUE_HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "impl_helpers.hpp"

namespace lastjson {

/**
 * \brief Primitive JSON data types
 *
 * This enum lists the primitive data types in JSON. Instead of a single
 * 'number' type it contains an integer and a float type.
 */
enum jsontype
{
    JSONNULL = 0,
    BOOL     = 1,
    INT      = 2,
    FLOAT    = 3,
    STRING   = 4,
    ARRAY    = 5,
    OBJECT   = 6
};

/**
 * \brief Convert jsontype into a name string constant
 *
 * \param t a jsontype
 * \return a string description of the given type, or "(unknown)" if an invalid
 * type was given.
 */
inline char const * jsontype_name(jsontype t)
{
    switch (t)
    {
    case JSONNULL:
        return "null";

    case BOOL:
        return "bool";

    case INT:
        return "int";

    case FLOAT:
        return "float";

    case STRING:
        return "string";

    case ARRAY:
        return "array";

    case OBJECT:
        return "object";
    }

    return "(unknown)";
}

template<class>
class basic_value;

/**
 * \brief Standard properties for the value type
 *
 * The commonly used lastjson::value is a typedef of
 * lastjson::basic_value<standard_properties>, so this struct defines the
 * properties of the standard lastjson::value type.
 */
struct standard_properties
{
    typedef basic_value<standard_properties> value;

    typedef bool bool_type;
    typedef int64_t int_type;
    typedef double float_type;
    typedef std::string string_type;
    typedef std::vector<value> array_type;
    typedef string_type object_key_type;
    typedef std::map<object_key_type, value> object_type;

    typedef shared_ptr<string_type> string_pointer;
    typedef shared_ptr<array_type> array_pointer;
    typedef shared_ptr<object_type> object_pointer;
};

/**
 * \brief JSON value class with runtime typing.
 *
 * An object of this class can hold a value which can be of any of the
 * JSON data types. It offers methods to check its actual type at runtime,
 * and to access the value in typesafe ways.
 */
template<class Properties>
class basic_value : public Properties
{
public:

    /// The properties struct of this basic_value<> template instance
    typedef Properties properties;

    /// The type of this basic_value<> template instance itself
    typedef basic_value<properties> this_type;

    /// Type used for storing JSON booleans
    typedef typename properties::bool_type bool_type;

    /// Type used for storing JSON integers
    typedef typename properties::int_type int_type;

    /// Type used for storing JSON floating point numbers
    typedef typename properties::float_type float_type;

    /// Type used for storing JSON strings
    typedef typename properties::string_type string_type;

    /// Type used for storing JSON arrays
    typedef typename properties::array_type array_type;

    /// Type used for storing JSON keys within JSON objects
    typedef typename properties::object_key_type object_key_type;

    /// Type used for storing JSON objects (maps/dictionaries)
    typedef typename properties::object_type object_type;

    /// Pointer type used for storing a reference to string data
    typedef typename properties::string_pointer string_pointer;

    /// Pointer type used for storing a reference to array data
    typedef typename properties::array_pointer array_pointer;

    /// Pointer type used for storing a reference to object data
    typedef typename properties::object_pointer object_pointer;

/* ***************************************************************************
 *
 *  Constructors
 *
 * ************************************************************************ */

    /// Default constructor, evaluates as JSON null value
    basic_value()
        : m_type(JSONNULL)
    {
    }

    /// Constructor for JSON booleans (from bool_type)
    basic_value(bool_type v)
        : m_data((simple_data){ boolean : v }), m_type(BOOL)
    {
    }

    /// Constructor for JSON integers (from unsigned char)
    basic_value(unsigned char v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from signed char)
    basic_value(signed char v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from unsigned short)
    basic_value(unsigned short v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from signed short)
    basic_value(signed short v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from unsigned int)
    basic_value(unsigned int v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from signed int)
    basic_value(signed int v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from unsigned long)
    basic_value(unsigned long v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from signed long)
    basic_value(signed long v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from unsigned long long)
    basic_value(unsigned long long v)
        : m_data((simple_data){ integer : v })
        , m_type(INT)
    {
    }

    /// Constructor for JSON integers (from signed long long)
    basic_value(signed long long v)
        : m_data((simple_data){ integer : v })
        , m_type(INT) {
    }

    /// Constructor for JSON floating point numbers
    basic_value(float_type v)
        : m_data((simple_data){ floatingpoint : v })
        , m_type(FLOAT)
    {
    }

    /// Constructor for JSON strings
    basic_value(string_type const & v)
        : m_ptr(new string_type(v))
        , m_type(STRING)
    {
    }

    /// Constructor for JSON strings (from string_type::const_iterator pair)
    basic_value(typename string_type::const_iterator begin, typename string_type::const_iterator end)
        : m_ptr(new string_type(begin, end))
        , m_type(STRING)
    {
    }

    /// Constructor for JSON strings (from char const pointer)
    basic_value(char const * v)
        : m_ptr(new string_type(v))
        , m_type(STRING)
    {
    }

    /// Constructor for JSON strings (referencing existing string object)
    basic_value(string_pointer const & v)
        : m_ptr(v)
        , m_type(STRING)
    {
        if (!m_ptr)
        {
            m_ptr.reset(new string_type);
        }
    }

    /// Constructor for JSON arrays
    basic_value(array_type const & v)
        : m_ptr(new array_type(v))
        , m_type(ARRAY)
    {
    }

    /// Constructor for JSON arrays (from std::vector)
    template<typename Value>
    basic_value(std::vector<Value> const & v)
        : m_ptr(new array_type(v.begin(), v.end()))
        , m_type(ARRAY)
    {
    }

    /// Constructor for JSON arrays (referencing existing array object)
    basic_value(array_pointer const & v)
        : m_ptr(v), m_type(ARRAY)
    {
        if (!m_ptr)
        {
            m_ptr.reset(new array_type);
        }
    }

    /// Constructor for JSON objects
    basic_value(object_type const & v)
        : m_ptr(new object_type(v)), m_type(OBJECT)
    {
    }

    /// Constructor for JSON objects (from std::map)
    template<typename Key, typename Value>
    basic_value(std::map<Key, Value> const & v)
        : m_ptr(new object_type(v.begin(), v.end())), m_type(OBJECT)
    {
    }

    /// Constructor for JSON objects (referencing existing array object)
    basic_value(object_pointer const & v)
        : m_ptr(v), m_type(OBJECT)
    {
        if (!m_ptr)
        {
            m_ptr.reset(new object_type);
        }
    }

/* ***************************************************************************
 *
 *  JSON formatting
 *
 * ************************************************************************ */

    /**
     * \brief Output this object as JSON formatted data
     *
     * \param stream The stream to write the JSON formatted data to
     */
    void write_json(std::ostream & stream) const;

    /**
     * \brief Stringify this object (as JSON formatted string)
     *
     * \param out Reference to a string to assign the JSON string to
     */
    void stringify(std::string & out) const
    {
        std::ostringstream ss;
        write_json(ss);
        out = ss.str();
    }

    /**
     * \brief Stringify this object (as JSON formatted string)
     *
     * \return The JSON string
     */
    std::string stringify() const
    {
        std::ostringstream ss;
        write_json(ss);
        return ss.str();
    }

/* ***************************************************************************
 *
 *  Swap method
 *
 * ************************************************************************ */

    /**
     * \brief Swap the JSON type and value of this object with another one
     *
     * \param other The other object
     */
    void swap(this_type & other)
    {
        m_ptr.swap(other.m_ptr);
        std::swap(m_data, other.m_data);
        std::swap(m_type, other.m_type);
    }

/* ***************************************************************************
 *
 *  Type classification methods
 *
 * ************************************************************************ */

    /**
     * \brief Get the type of the JSON value stored in this object
     *
     * \return The type as a jsontype enum
     */
    jsontype get_type() const
    {
        return m_type;
    }

    /**
     * \brief Check whether this JSON value is null
     *
     * \return Boolean indicating whether this is null
     */
    bool is_null() const
    {
        return m_type == JSONNULL;
    }

    /**
     * \brief Check whether this JSON value is of type boolean
     *
     * \return Boolean indicating whether this is a boolean
     */
    bool is_bool() const
    {
        return m_type == BOOL;
    }

    /**
     * \brief Check whether this JSON value is of type integer
     *
     * \return Boolean indicating whether this is an integer
     */
    bool is_int() const
    {
        return m_type == INT;
    }

    /**
     * \brief Check whether this JSON value is of type float
     *
     * \return Boolean indicating whether this is a float
     */
    bool is_float() const
    {
        return m_type == FLOAT;
    }

    /**
     * \brief Check whether this JSON value is of type string
     *
     * \return Boolean indicating whether this is a string
     */
    bool is_string() const
    {
        return m_type == STRING;
    }

    /**
     * \brief Check whether this JSON value is of type array
     *
     * \return Boolean indicating whether this is an array
     */
    bool is_array() const
    {
        return m_type == ARRAY;
    }

    /**
     * \brief Check whether this JSON value is of type object
     *
     * \return Boolean indicating whether this is an object
     */
    bool is_object() const
    {
        return m_type == OBJECT;
    }

/* ***************************************************************************
 *
 *  Basic getter methods
 *
 * ************************************************************************ */

    /**
     * \brief Evaluate this JSON value as a boolean
     *
     * \return The boolean value of this JSON value. Strings, arrays and
     * objects are true if not empty. Numbers are true if not equal zero. Null
     * is always false.
     */
    bool_type get_bool() const
    {
        switch (m_type)
        {
        case JSONNULL:
            return false;

        case BOOL:
            return m_data.boolean;

        case INT:
            return m_data.integer;

        case FLOAT:
            return m_data.floatingpoint;

        case STRING:
            return !string_ref_dangerous().empty();

        case ARRAY:
            return !array_ref_dangerous().empty();

        case OBJECT:
            return !object_ref_dangerous().empty();

        default:
            throw std::logic_error("value object carries unknown type");
        }
    }

    /**
     * \brief Return reference to boolean
     *
     * This method throws if this value object is not of type boolean.
     *
     * \return Reference to the boolean data member
     */
    bool_type & get_bool_ref()
    {
        if (m_type == BOOL)
        {
            return m_data.boolean;
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to bool");
        }
    }

    /**
     * \brief Evaluate this JSON value as an integer
     *
     * This method throws if this value object is not of type boolean, integer
     * or float.
     *
     * \return The integer value of this JSON value.
     */
    int_type get_int() const
    {
        switch (m_type)
        {
        case BOOL:
            return m_data.boolean;

        case INT:
            return m_data.integer;

        case FLOAT:
            return m_data.floatingpoint;

        default:
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to int");
        }
    }

    /**
     * \brief Return reference to integer
     *
     * This method throws if this value object is not of type integer.
     *
     * \return Reference to the integer data member
     */
    int_type & get_int_ref()
    {
        if (m_type == INT)
        {
            return m_data.integer;
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to int");
        }
    }

    /**
     * \brief Evaluate this JSON value as a float
     *
     * This method throws if this value object is not of type boolean, integer
     * or float.
     *
     * \return The floating point value of this JSON value.
     */
    float_type get_float() const
    {
        switch (m_type)
        {
        case BOOL:
            return m_data.boolean;

        case INT:
            return m_data.integer;

        case FLOAT:
            return m_data.floatingpoint;

        default:
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to float");
        }
    }

    /**
     * \brief Return reference to float
     *
     * This method throws if this value object is not of type float.
     *
     * \return Reference to the float data member
     */
    float_type & get_float_ref()
    {
        if (m_type == FLOAT)
        {
            return m_data.floatingpoint;
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to float");
        }
    }

    /**
     * \brief Return const-reference to string
     *
     * This method throws if this value object is not of type string.
     *
     * \return Reference to the string data member. The referred to string
     * object may be shared with other instances of lastjson::value.
     */
    string_type const & get_string() const
    {
        if (m_type == STRING)
        {
            return string_ref_dangerous();
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to string");
        }
    }

    /**
     * \brief Return reference to string
     *
     * This method throws if this value object is not of type string.
     *
     * \return Reference to the string data member. The referred to string
     * object may be shared with other instances of lastjson::value.
     */
    string_type & get_string_ref()
    {
        if (m_type == STRING)
        {
            return string_ref_dangerous();
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to string");
        }
    }

    /**
     * \brief Return shared_ptr to string
     *
     * This method throws if this value object is not of type string.
     *
     * \return The shared_ptr to the string data member. The referred to string
     * object may be shared with other instances of lastjson::value.
     */
    string_pointer get_string_pointer()
    {
        if (m_type == STRING)
        {
            return static_pointer_cast<string_type>(m_ptr);
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to string");
        }
    }

    /**
     * \brief Return const-reference to array
     *
     * This method throws if this value object is not of type array.
     *
     * \return Reference to the array data member. The referred to array object
     * may be shared with other instances of lastjson::value.
     */
    array_type const & get_array() const
    {
        if (m_type == ARRAY)
        {
            return array_ref_dangerous();
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to array");
        }
    }

    /**
     * \brief Return reference to array
     *
     * This method throws if this value object is not of type array.
     *
     * \return Reference to the array data member. The referred to array object
     * may be shared with other instances of lastjson::value.
     */
    array_type & get_array_ref()
    {
        if (m_type == ARRAY)
        {
            return array_ref_dangerous();
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to array");
        }
    }

    /**
     * \brief Return shared_ptr to array
     *
     * This method throws if this value object is not of type array.
     *
     * \return The shared_ptr to the array data member. The referred to array
     * object may be shared with other instances of lastjson::value.
     */
    array_pointer get_array_pointer()
    {
        if (m_type == ARRAY)
        {
            return static_pointer_cast<array_type>(m_ptr);
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to array");
        }
    }

    /**
     * \brief Return const-reference to object
     *
     * This method throws if this value object is not of type object.
     *
     * \return Reference to the object data member. The referred to object
     * object may be shared with other instances of lastjson::value.
     */
    object_type const & get_object() const
    {
        if (m_type == OBJECT)
        {
            return object_ref_dangerous();
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to object");
        }
    }

    /**
     * \brief Return reference to object
     *
     * This method throws if this value object is not of type object.
     *
     * \return Reference to the object data member. The referred to object
     * object may be shared with other instances of lastjson::value.
     */
    object_type & get_object_ref()
    {
        if (m_type == OBJECT)
        {
            return object_ref_dangerous();
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to object");
        }
    }

    /**
     * \brief Return shared_ptr to object
     *
     * This method throws if this value object is not of type object.
     *
     * \return The shared_ptr to the object data member. The referred to object
     * object may be shared with other instances of lastjson::value.
     */
    object_pointer get_object_pointer()
    {
        if (m_type == OBJECT)
        {
            return static_pointer_cast<object_type>(m_ptr);
        }
        else
        {
            throw type_error("Cannot convert "+std::string(jsontype_name(m_type))+" to object");
        }
    }

/* ***************************************************************************
 *
 *  Array/object accessor operators
 *
 * ************************************************************************ */

    /**
     * \brief Return reference to array member
     *
     * This method throws if this value object is not of type array or if the
     * index is out of range.
     *
     * \return The value object representing the array member
     */
    this_type & operator[](typename array_type::size_type i)
    {
        array_type & ref = get_array_ref();
        if (i < ref.size())
        {
            return ref[i];
        }
        else
        {
            throw range_error("array index out of range");
        }
    }

    /**
     * \brief Return const-reference to array member
     *
     * This method throws if this value object is not of type array or if the
     * index is out of range.
     *
     * \return The value object representing the array member
     */
    this_type const & operator[](typename array_type::size_type i) const
    {
        array_type const & ref = get_array();
        if (i < ref.size())
        {
            return ref[i];
        }
        else
        {
            throw range_error("array index out of range");
        }
    }

    /**
     * \brief Add member to array
     *
     * This method throws if this value object is not of type array.
     */
    void push_back(typename array_type::value_type const & v)
    {
        get_array().push_back(v);
    }

    /**
     * \brief Return reference to object member
     *
     * This method throws if this value object is not of type object. If the
     * object does not hold a member with the given key, it is added to the
     * object (default-constructed, thus the value is null) and the reference
     * to the newly created value is returned.
     *
     * \return The value object representing the object member
     */
    this_type & operator[](object_key_type const & i)
    {
        return get_object_ref()[i];
    }

    /**
     * \brief Return const-reference to object member
     *
     * This method throws if this value object is not of type object or if the
     * object does not hold a member with the given key.
     *
     * \return The value object representing the object member
     */
    this_type const & operator[](object_key_type const & i) const
    {
        object_type const & ref = get_object();
        typename object_type::const_iterator it = ref.find(i);

        if (it != ref.end())
        {
            return it->second;
        }
        else
        {
            throw range_error("object has no field "+std::string(i));
        }
    }

/* ***************************************************************************
 *
 *  Deep copy
 *
 * ************************************************************************ */

    /**
     * \brief Assign this value object with a deep copy of another
     *
     * This method assigns the current value of this value object with a deep
     * copy of the given value
     *
     * \param other The json value to copy
     *
     * \return A reference to this value
     */
    basic_value & deepcopy(this_type const & other)
    {
        switch (other.get_type())
        {
        case JSONNULL:
            m_type = JSONNULL;
            m_ptr.reset();
            return *this;

        case BOOL:
            m_type = BOOL;
            m_data.boolean = other.m_data.boolean;
            return *this;

        case INT:
            m_type = INT;
            m_data.integer = other.m_data.integer;
            return *this;

        case FLOAT:
            m_type = FLOAT;
            m_data.floatingpoint = other.m_data.floatingpoint;
            return *this;

        case STRING:
        {
            basic_value tmp(other.string_ref_dangerous());
            swap(tmp);
        }
            return *this;

        case ARRAY:
            return deepcopy(other.array_ref_dangerous());

        case OBJECT:
            return deepcopy(other.object_ref_dangerous());
        }

        throw std::logic_error("value object carries unknown type");
    }

    /**
     * \brief Assign this value object with a deep copy of an array
     *
     * This method assigns the current value of this value object with a deep
     * copy of the given array
     *
     * \param source The array to copy
     *
     * \return A reference to this value
     */
    basic_value & deepcopy(array_type const & source)
    {
        array_pointer destination_ptr(new array_type(source.size()));
        array_type & destination = *destination_ptr;

        typename array_type::const_iterator sit = source.begin();
        typename array_type::iterator dit = destination.begin();
        while (sit != source.end())
        {
            (dit++)->deepcopy(*(sit++));
        }

        *this = destination_ptr;
        return *this;
    }

    /**
     * \brief Assign this value object with a deep copy of a json object
     *
     * This method assigns the current value of this value object with a deep
     * copy of the object
     *
     * \param source The json object to copy
     *
     * \return A reference to this value
     */
    basic_value & deepcopy(object_type const & source)
    {
        object_pointer destination_ptr(new object_type(source));
        object_type & destination = *destination_ptr;

        for (typename object_type::iterator it = destination.begin(); it != destination.end(); ++it)
        {
            it->second.deepcopy_self();
        }

        *this = destination_ptr;
        return *this;
    }

    /**
     * \brief Assign this value object with a deep copy of itself
     *
     * This method re-assigns this value with a deep copy of itself.
     * If this value or any member of arrays and objects therein is a shallow
     * copy of another value, this method will break this dependency.
     *
     * \return A reference to this value
     */
    basic_value & deepcopy_self()
    {
        switch (get_type())
        {
        case JSONNULL:
        case BOOL:
        case INT:
        case FLOAT:
            return *this;

        case STRING:
        {
            basic_value tmp(string_ref_dangerous());
            swap(tmp);
        }
            return *this;

        case ARRAY:
            return deepcopy(array_ref_dangerous());

        case OBJECT:
            return deepcopy(object_ref_dangerous());
        }

        throw std::logic_error("value object carries unknown type");
    }

/* ***************************************************************************
 *
 *  Convenience getter functions
 *
 * ************************************************************************ */

    /**
     * \brief Get a member of an object
     *
     * This function finds a member in an object and assigns it to a value that
     * is passed in by reference.
     *
     * \param object Const-reference to a json object
     * \param key The key to look up
     * \param returnvalue Reference to a json value that shall be assigned
     * \return Boolean indicating whether the given key is present in the
     * object. If false is returned, nothing has been written to the
     * returnvalue reference.
     */
    static bool object_get(object_type const & object, object_key_type const & key, this_type & returnvalue)
    {
        typename object_type::const_iterator const it = object.find(key);

        if (it != object.end())
        {
            returnvalue = it->second;
            return true;
        }

        return false;
    }

    /**
     * \brief Get boolean representation of a member of an object
     *
     * This function finds a member in an object and assigns its boolean
     * representation to a bool that is passed in by reference.
     *
     * \param object Const-reference to a json object
     * \param key The key to look up
     * \param returnvalue Reference to a bool that shall be assigned
     * \return Boolean indicating whether the given key is present in the
     * object. If false is returned, nothing has been written to the
     * returnvalue reference.
     */
    static bool object_get_bool(object_type const & object, object_key_type const & key, bool_type & returnvalue)
    {
        typename object_type::const_iterator const it = object.find(key);

        if (it != object.end())
        {
            returnvalue = it->second.get_bool();
            return true;
        }

        return false;
    }

    /**
     * \brief Get boolean representation of a member of an object
     *
     * This function finds a member in an object and returns its boolean
     * representation, or false if the object has no member with the given
     * key.
     *
     * \param object Const-reference to a json object
     * \param key The key to look up
     * \return Boolean representation of the value for the given key, or false
     * if the object does not contain a value for the given key.
     */
    static bool object_get_bool(object_type const & object, object_key_type const & key)
    {
        bool value = false;
        bool const returnvalue = object_get_bool(object, key, value);
        return returnvalue && value;
    }

    /**
     * \brief Get numeric value of a member of an object
     *
     * This template function finds a member in an object and assigns its
     * numeric value to a variable that is passed in by reference.
     *
     * \param object Const-reference to a json object
     * \param key The key to look up
     * \param returnvalue The variable to assign with the numeric value
     * \return Boolean indicating whether the given key is present in the
     * object. If false is returned, nothing has been written to the
     * returnvalue reference.
     */
    template<typename T>
    static bool object_get_number(object_type const & object, object_key_type const & key, T & returnvalue)
    {
        typename object_type::const_iterator const it = object.find(key);

        if (it != object.end())
        {
            this_type const & v = it->second;

            switch (v.m_type)
            {
            case BOOL:
                returnvalue = v.m_data.boolean;
                return true;

            case INT:
                returnvalue = v.m_data.integer;
                return true;

            case FLOAT:
                returnvalue = v.m_data.floatingpoint;
                return true;

            default:
                break;
            }
        }

        return false;
    }

    /**
     * \brief Get string member of an object
     *
     * This function finds a string member in an object and assigns its
     * value to a string that is passed in by reference.
     *
     * \param object Const-reference to a json object
     * \param key The key to look up
     * \param returnvalue The string to assign with the value found
     * \return Boolean indicating whether the given key is present in the
     * object and refers to a string. If false is returned, nothing has been
     * written to the returnvalue reference.
     */
    static bool object_get_string(object_type const & object, object_key_type const & key, string_type & returnvalue)
    {
        typename object_type::const_iterator const it = object.find(key);

        if (it != object.end() && it->second.is_string())
        {
            returnvalue = it->second.string_ref_dangerous();
            return true;
        }

        return false;
    }

/* ***************************************************************************
 *
 *  Convenience getter methods
 *
 * ************************************************************************ */

    /**
     * \brief Get value of a given key in object
     *
     * If this method is called on an object-type value, it assign the value
     * for a given key to a json value passed in by reference. Throws if this
     * is not an object-type value.
     *
     * \param key The key to look up
     * \param returnvalue The json value to assign with the value found
     * \return Boolean indicating whether the given key is present in the
     * object. If false is returned, nothing has been written to the
     * returnvalue reference.
     */
    bool get(object_key_type const & key, this_type & returnvalue) const
    {
        return object_get(this->get_object(), key, returnvalue);
    }

    /**
     * \brief Get boolean representation of key in object
     *
     * If this method is called on an object-type value, it returns the boolean
     * representation of the value of a given key. Throws if this is not an
     * object-type value.
     *
     * \param key The key to look up
     * \return Boolean indicating whether the given key is present in the
     * object. If the key is not present in the object, false is returned.
     */
    bool get_bool(object_key_type const & key) const
    {
        return object_get_bool(this->get_object(), key);
    }

    /**
     * \brief Get numeric value of key in object
     *
     * If this method is called on an object-type value, it fetches the numeric
     * value of a given key. Throws if this is not an object-type value.
     *
     * \param key The key to look up
     * \param returnvalue Reference to variable that shall be assigned with
     * the numeric value
     * \return Boolean indicating whether the given key is present in the
     * object. If the key is not present in the object, false is returned. If
     * the value cannot be expressed as number, false is returned.
     */
    template<typename T>
    bool get_number(object_key_type const & key, T & returnvalue) const
    {
        return object_get_number<T>(this->get_object(), key, returnvalue);
    }

    /**
     * \brief Get string value of key in object
     *
     * If this method is called on an object-type value, it fetches the string
     * value of a given key. Throws if this is not an object-type value.
     *
     * \param key The key to look up
     * \param returnvalue Reference to string that shall be assigned with the
     * value
     * \return Boolean indicating whether the given key is present in the
     * object. If the key is not present in the object, false is returned. If
     * the value is not of type string, false is returned.
     */
    bool get_string(object_key_type const & key, string_type & returnvalue) const
    {
        return object_get_string(this->get_object(), key, returnvalue);
    }


/* ************************************************************************ */

private:
    typedef shared_ptr<void> void_pointer;

    void_pointer m_ptr;
    union simple_data
    {
        int_type integer;
        float_type floatingpoint;
        bool_type boolean;
    } m_data;
    jsontype m_type;

    /**
     * \brief Return string const-reference
     *
     * This is dangerous, as it does not check the value type. Hence, it is a
     * private method. Only use this method when you know you are dealing with
     * a string type value.
     */
    string_type const & string_ref_dangerous() const
    {
        return *static_cast<string_type const *>(m_ptr.get());
    }

    /**
     * \brief Return string reference
     *
     * This is dangerous, as it does not check the value type. Hence, it is a
     * private method. Only use this method when you know you are dealing with
     * a string type value.
     */
    string_type & string_ref_dangerous()
    {
        return *static_cast<string_type *>(m_ptr.get());
    }

    /**
     * \brief Return array const-reference
     *
     * This is dangerous, as it does not check the value type. Hence, it is a
     * private method. Only use this method when you know you are dealing with
     * a array type value.
     */
    array_type const & array_ref_dangerous() const
    {
        return *static_cast<array_type const *>(m_ptr.get());
    }

    /**
     * \brief Return array reference
     *
     * This is dangerous, as it does not check the value type. Hence, it is a
     * private method. Only use this method when you know you are dealing with
     * a array type value.
     */
    array_type & array_ref_dangerous()
    {
        return *static_cast<array_type *>(m_ptr.get());
    }

    /**
     * \brief Return object const-reference
     *
     * This is dangerous, as it does not check the value type. Hence, it is a
     * private method. Only use this method when you know you are dealing with
     * a object type value.
     */
    object_type const & object_ref_dangerous() const
    {
        return *static_cast<object_type const *>(m_ptr.get());
    }

    /**
     * \brief Return object reference
     *
     * This is dangerous, as it does not check the value type. Hence, it is a
     * private method. Only use this method when you know you are dealing with
     * a object type value.
     */
    object_type & object_ref_dangerous()
    {
        return *static_cast<object_type *>(m_ptr.get());
    }
};

typedef basic_value<standard_properties> value;

}

#endif // ifndef LASTJSON_VALUE_HPP__
