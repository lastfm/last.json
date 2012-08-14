#include <boost/test/unit_test.hpp>

#include <lastjson/stringify.hpp>
#include <lastjson/parse.hpp>

BOOST_AUTO_TEST_SUITE( json_test )

namespace {
class JSON_TestSuite
{
public:
};

BOOST_FIXTURE_TEST_CASE(parse_primitives, JSON_TestSuite)
{
  lastjson::value val;

  BOOST_CHECK(val.is_null());

  val = lastjson::parse("true");
  BOOST_CHECK(val.is_bool());
  BOOST_CHECK(val.get_bool() == true);

  val = lastjson::parse("false");
  BOOST_CHECK(val.is_bool());
  BOOST_CHECK(val.get_bool() == false);

  val = lastjson::parse("null");
  BOOST_CHECK(val.is_null());
  BOOST_CHECK(val.get_bool() == false);

  val = lastjson::parse("\"\"");
  BOOST_CHECK(val.is_string());
  BOOST_CHECK(val.get_string() == "");
  BOOST_CHECK(val.get_bool() == false);

  val = lastjson::parse("\"test\"");
  BOOST_CHECK(val.is_string());
  BOOST_CHECK(val.get_string() == "test");
  BOOST_CHECK(val.get_bool() == true);

  val = lastjson::parse("0");
  BOOST_CHECK(val.is_int());
  BOOST_CHECK(val.get_int() == 0);
  BOOST_CHECK(val.get_float() == 0.);
  BOOST_CHECK(val.get_bool() == false);

  val = lastjson::parse("1");
  BOOST_CHECK(val.is_int());
  BOOST_CHECK(val.get_int() == 1);
  BOOST_CHECK(val.get_float() == 1.);
  BOOST_CHECK(val.get_bool() == true);

  val = lastjson::parse("0.0");
  BOOST_CHECK(val.is_float());
  BOOST_CHECK(val.get_int() == 0);
  BOOST_CHECK(val.get_float() == 0.);
  BOOST_CHECK(val.get_bool() == false);

  val = lastjson::parse("1.0");
  BOOST_CHECK(val.is_float());
  BOOST_CHECK(val.get_int() == 1);
  BOOST_CHECK(val.get_float() == 1.);
  BOOST_CHECK(val.get_bool() == true);

  val = lastjson::parse("[]");
  BOOST_CHECK(val.is_array());
  BOOST_CHECK(val.get_bool() == false);

  val = lastjson::parse("[null]");
  BOOST_CHECK(val.is_array());
  BOOST_CHECK(val.get_bool() == true);

  val = lastjson::parse("{}");
  BOOST_CHECK(val.is_object());
  BOOST_CHECK(val.get_bool() == false);

  val = lastjson::parse("{ \"\" : null }");
  BOOST_CHECK(val.is_object());
  BOOST_CHECK(val.get_bool() == true);
}

BOOST_FIXTURE_TEST_CASE(stringify_primitives, JSON_TestSuite)
{
  lastjson::value val;

  BOOST_CHECK(val.is_null());

  val = true;
  BOOST_CHECK(val.is_bool());
  BOOST_CHECK(val.get_bool() == true);
  BOOST_CHECK(lastjson::stringify(val) == "true");

  val = false;
  BOOST_CHECK(val.is_bool());
  BOOST_CHECK(val.get_bool() == false);
  BOOST_CHECK(lastjson::stringify(val) == "false");

  val = lastjson::value();
  BOOST_CHECK(val.is_null());
  BOOST_CHECK(val.get_bool() == false);
  BOOST_CHECK(lastjson::stringify(val) == "null");

  val = "";
  BOOST_CHECK(val.is_string());
  BOOST_CHECK(val.get_string() == "");
  BOOST_CHECK(val.get_bool() == false);
  BOOST_CHECK(lastjson::stringify(val) == "\"\"");

  val = "test";
  BOOST_CHECK(val.is_string());
  BOOST_CHECK(val.get_string() == "test");
  BOOST_CHECK(val.get_bool() == true);
  BOOST_CHECK(lastjson::stringify(val) == "\"test\"");

  val = 0;
  BOOST_CHECK(val.is_int());
  BOOST_CHECK(val.get_int() == 0);
  BOOST_CHECK(val.get_float() == 0.);
  BOOST_CHECK(val.get_bool() == false);
  BOOST_CHECK(lastjson::stringify(val) == "0");

  val = 1;
  BOOST_CHECK(val.is_int());
  BOOST_CHECK(val.get_int() == 1);
  BOOST_CHECK(val.get_float() == 1.);
  BOOST_CHECK(val.get_bool() == true);
  BOOST_CHECK(lastjson::stringify(val) == "1");

  val = 0.1;
  BOOST_CHECK(val.is_float());
  BOOST_CHECK(val.get_int() == 0);
  BOOST_CHECK(val.get_float() == 0.1);
  BOOST_CHECK(val.get_bool() == true);
  BOOST_CHECK(lastjson::stringify(val) == "0.1");

  val = 1.1;
  BOOST_CHECK(val.is_float());
  BOOST_CHECK(val.get_int() == 1);
  BOOST_CHECK(val.get_float() == 1.1);
  BOOST_CHECK(val.get_bool() == true);
  BOOST_CHECK(lastjson::stringify(val) == "1.1");

  val = lastjson::value::array_type();
  BOOST_CHECK(val.is_array());
  BOOST_CHECK(val.get_bool() == false);
  BOOST_CHECK(lastjson::stringify(val) == "[]");

  val.get_array_ref().push_back(lastjson::value());
  BOOST_CHECK(val.is_array());
  BOOST_CHECK(val.get_bool() == true);
  BOOST_CHECK(lastjson::stringify(val) == "[null]");

  val = lastjson::value::object_type();
  BOOST_CHECK(val.is_object());
  BOOST_CHECK(val.get_bool() == false);
  BOOST_CHECK(lastjson::stringify(val) == "{}");

  val[""] = lastjson::value();
  BOOST_CHECK(val.is_object());
  BOOST_CHECK(val.get_bool() == true);
  BOOST_CHECK(lastjson::stringify(val) == "{\"\":null}");
}

BOOST_FIXTURE_TEST_CASE(stringify_strictly_typed_data, JSON_TestSuite)
{
  BOOST_CHECK(lastjson::stringify(false) == "false");
  BOOST_CHECK(lastjson::stringify(true) == "true");
  BOOST_CHECK(lastjson::stringify(0) == "0");
  BOOST_CHECK(lastjson::stringify(0.1) == "0.1");
  BOOST_CHECK(lastjson::stringify(1) == "1");
  BOOST_CHECK(lastjson::stringify(1.1) == "1.1");
  BOOST_CHECK(lastjson::stringify("") == "\"\"");
  BOOST_CHECK(lastjson::stringify(std::string("")) == "\"\"");
  BOOST_CHECK(lastjson::stringify("test") == "\"test\"");
  BOOST_CHECK(lastjson::stringify(std::string("test")) == "\"test\"");
  BOOST_CHECK(lastjson::stringify(std::vector<bool>()) == "[]");
  BOOST_CHECK(lastjson::stringify(std::map<std::string,bool>()) == "{}");

  std::vector<std::map<std::string, std::vector<std::string> > > test;
  test.resize(2);
  test[0]["test1_1"].resize(3);
  test[0]["test1_1"][0] = "a";
  test[0]["test1_1"][1] = "b";
  test[0]["test1_1"][2] = "c";
  test[1]["test2_1"];
  test[1]["test2_2"].resize(2);
  test[1]["test2_2"][0] = "";
  test[1]["test2_2"][1] = "X";
  BOOST_CHECK(lastjson::stringify(test) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");

  lastjson::value testcopy = test;
  BOOST_CHECK(lastjson::stringify(testcopy) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");
}

BOOST_FIXTURE_TEST_CASE(stringify_loosely_typed_data, JSON_TestSuite)
{
  std::vector<std::map<std::string, std::vector<lastjson::value> > > test;
  test.resize(2);
  test[0]["test1_1"].resize(3);
  test[0]["test1_1"][0] = "a";
  test[0]["test1_1"][1] = "b";
  test[0]["test1_1"][2] = "c";
  test[1]["test2_1"];
  test[1]["test2_2"].resize(2);
  test[1]["test2_2"][0] = "";
  test[1]["test2_2"][1] = "X";
  BOOST_CHECK(lastjson::stringify(test) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");

  lastjson::value testcopy = test;
  BOOST_CHECK(lastjson::stringify(testcopy) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");
}

BOOST_FIXTURE_TEST_CASE(stringify_mixedly_typed_data, JSON_TestSuite)
{
  {
  std::vector<std::map<std::string, lastjson::value> > test;
  test.resize(2);
  test[0]["test1_1"]=lastjson::value::array_type(3);
  test[0]["test1_1"][0] = "a";
  test[0]["test1_1"][1] = "b";
  test[0]["test1_1"][2] = "c";
  test[1]["test2_1"]=lastjson::value::array_type(0);
  test[1]["test2_2"]=lastjson::value::array_type(2);
  test[1]["test2_2"][0] = "";
  test[1]["test2_2"][1] = "X";
  BOOST_CHECK(lastjson::stringify(test) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");

  lastjson::value testcopy = test;
  BOOST_CHECK(lastjson::stringify(testcopy) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");
  }

  {
  std::vector<lastjson::value> test;
  test.resize(2);
  test[0]=lastjson::value::object_type();
  test[0]["test1_1"]=lastjson::value::array_type(3);
  test[0]["test1_1"][0] = "a";
  test[0]["test1_1"][1] = "b";
  test[0]["test1_1"][2] = "c";
  test[1]=lastjson::value::object_type();
  test[1]["test2_1"]=lastjson::value::array_type(0);
  test[1]["test2_2"]=lastjson::value::array_type(2);
  test[1]["test2_2"][0] = "";
  test[1]["test2_2"][1] = "X";
  BOOST_CHECK(lastjson::stringify(test) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");

  lastjson::value testcopy = test;
  BOOST_CHECK(lastjson::stringify(testcopy) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");
  }

  {
  lastjson::value test = lastjson::value::array_type(2);
  test[0]=lastjson::value::object_type();
  test[0]["test1_1"]=lastjson::value::array_type(3);
  test[0]["test1_1"][0] = "a";
  test[0]["test1_1"][1] = "b";
  test[0]["test1_1"][2] = "c";
  test[1]=lastjson::value::object_type();
  test[1]["test2_1"]=lastjson::value::array_type(0);
  test[1]["test2_2"]=lastjson::value::array_type(2);
  test[1]["test2_2"][0] = "";
  test[1]["test2_2"][1] = "X";
  BOOST_CHECK(lastjson::stringify(test) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");
  }

  {
    // This is basically going one step back, to where the first layer is strongly typed.
  lastjson::value::array_type test(2);
  test[0]=lastjson::value::object_type();
  test[0]["test1_1"]=lastjson::value::array_type(3);
  test[0]["test1_1"][0] = "a";
  test[0]["test1_1"][1] = "b";
  test[0]["test1_1"][2] = "c";
  test[1]=lastjson::value::object_type();
  test[1]["test2_1"]=lastjson::value::array_type(0);
  test[1]["test2_2"]=lastjson::value::array_type(2);
  test[1]["test2_2"][0] = "";
  test[1]["test2_2"][1] = "X";
  BOOST_CHECK(lastjson::stringify(test) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");

  lastjson::value testcopy = test;
  BOOST_CHECK(lastjson::stringify(testcopy) == "[{\"test1_1\":[\"a\",\"b\",\"c\"]},{\"test2_1\":[],\"test2_2\":[\"\",\"X\"]}]");
  }

}

}
BOOST_AUTO_TEST_SUITE_END()

