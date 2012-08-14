PROJECT(lastjson-tests)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(
   ../include/
   )

ADD_EXECUTABLE(lastjson-test
               main.cpp
               stringescape.cpp
               json.cpp
               refcounting.cpp
              )
