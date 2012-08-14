#include <boost/test/unit_test.hpp>

#include <lastjson/stringify.hpp>
#include <lastjson/parse.hpp>

BOOST_AUTO_TEST_SUITE( refcounting_test )

namespace {
class Refcounting_TestSuite
{
public:

class mystring : public std::string
{
public:
  mystring()
  {
    ++instance_counter;
  }

  mystring(mystring const & x)
  : std::string(x)
  {
    ++instance_counter;
  }

  mystring(std::string const & x)
  : std::string(x)
  {
    ++instance_counter;
  }

  mystring(char const * x)
  : std::string(x)
  {
    ++instance_counter;
  }

  ~mystring()
  {
    --instance_counter;
  }

  static int instance_counter;
};

struct special_properties
{
    typedef lastjson::basic_value<special_properties> value;

    typedef bool bool_type;
    typedef int64_t int_type;
    typedef double float_type;
    typedef mystring string_type;
    typedef std::vector<value> array_type;
    typedef std::string object_key_type;
    typedef std::map<object_key_type, value> object_type;

    typedef lastjson::shared_ptr<string_type> string_pointer;
    typedef lastjson::shared_ptr<array_type> array_pointer;
    typedef lastjson::shared_ptr<object_type> object_pointer;
};

typedef lastjson::basic_value<special_properties> rct_value;
};
int Refcounting_TestSuite::mystring::instance_counter;

BOOST_FIXTURE_TEST_CASE(refcounting_, Refcounting_TestSuite)
{
  mystring::instance_counter = 0;

  {
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);
  rct_value val;
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);
  val = "hallo";
  BOOST_CHECK_EQUAL(mystring::instance_counter, 1);
  }
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);

  {
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);
  rct_value val;
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);
  val = rct_value::array_type(10);
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);
  val[0] = "foo"; // increases string count by one
  BOOST_CHECK_EQUAL(mystring::instance_counter, 1);
  val[1] = "bar"; // increases string count by one
  BOOST_CHECK_EQUAL(mystring::instance_counter, 2);
  val[2] = "baz"; // increases string count by one
  BOOST_CHECK_EQUAL(mystring::instance_counter, 3);
  val[2] = "whoop"; // overwrites old string - leaves count unchanged
  BOOST_CHECK_EQUAL(mystring::instance_counter, 3);
  val[1] = false; // reduces string count by one
  BOOST_CHECK_EQUAL(mystring::instance_counter, 2);
  val[1] = val[2]; // re-uses string in val[2] - leaves count untouched
  BOOST_CHECK_EQUAL(mystring::instance_counter, 2);
  val[2] = false; // overwrites val[2], but the string is still
  // referenced by val[1] - count stays as it is
  BOOST_CHECK_EQUAL(mystring::instance_counter, 2);
  }
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);

  {
    rct_value val = rct_value::object_type();
    BOOST_CHECK_EQUAL(mystring::instance_counter, 0);
    val["foo"] = "bar"; // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 1);
    val["foo2"] = "baz"; // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 2);
    val["foo3"] = rct_value::object_type();
    BOOST_CHECK_EQUAL(mystring::instance_counter, 2);
    val["foo3"]["1"] = "x1"; // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 3);
    val["foo3"]["2"] = "x2"; // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 4);
    val["foo3"]["3"] = "x3"; // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 5);
    val["foo4"] = rct_value::array_type();
    BOOST_CHECK_EQUAL(mystring::instance_counter, 5);
    val["foo4"].get_array_ref().push_back("y1"); // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 6);
    val["foo4"].get_array_ref().push_back("y2"); // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 7);
    val["foo4"].get_array_ref().push_back("y3"); // string count += 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 8);
    val["foo4_copy"] = val["foo4"]; // no change to string count
    BOOST_CHECK_EQUAL(mystring::instance_counter, 8);
    val["foo4"] = false; // no change to string count - still have copy
    BOOST_CHECK_EQUAL(mystring::instance_counter, 8);
    val["foo4_copy2"] = val["foo4_copy"]; // no change to string count
    BOOST_CHECK_EQUAL(mystring::instance_counter, 8);
    val["foo4_copy"] = false; // no change to string count - still have copy
    BOOST_CHECK_EQUAL(mystring::instance_counter, 8);
    val["foo4_copy2"] = val["foo4_copy2"]; // self assignment - no change to string count
    BOOST_CHECK_EQUAL(mystring::instance_counter, 8);
    val["foo4_copy2"] = val["foo3"]; // string count -= 3
    BOOST_CHECK_EQUAL(mystring::instance_counter, 5);
    val["foo3"] = val["foo2"]; // still have a copy of foo3 in foo4_copy2
    BOOST_CHECK_EQUAL(mystring::instance_counter, 5);
    val["foo3"] = true; // foo2 still has that string
    BOOST_CHECK_EQUAL(mystring::instance_counter, 5);
    val["foo2"] = true; // string count -= 1
    BOOST_CHECK_EQUAL(mystring::instance_counter, 4);
    val["bar"] = val["foo4_copy2"]; // no change
    BOOST_CHECK_EQUAL(mystring::instance_counter, 4);
    val["bar2"].deepcopy(val["foo4_copy2"]); // string count += 3
    BOOST_CHECK_EQUAL(mystring::instance_counter, 7);
    val["bar2"].deepcopy(val["bar2"]); // self assignment - no change
    BOOST_CHECK_EQUAL(mystring::instance_counter, 7);
    val["bar"].deepcopy(val["bar"]); // self assignment
    // this was a shallow copy and becomes a deep copy: += 3
    BOOST_CHECK_EQUAL(mystring::instance_counter, 10);
  }
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);

  rct_value("foo");
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);
  rct_value("foo") = "bar";
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);

  {
  rct_value::array_type tmp(10, "foo");
  // "foo" gets converted to value first, then put into the array ten times.
  BOOST_CHECK_EQUAL(mystring::instance_counter, 1);
  }
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);

  {
  std::vector<mystring> tmp(10, "foo");
  // "foo" gets converted to 10 separate mystrings.
  BOOST_CHECK_EQUAL(mystring::instance_counter, 10);
  }
  BOOST_CHECK_EQUAL(mystring::instance_counter, 0);

}

}
BOOST_AUTO_TEST_SUITE_END()
