all:

test: test/lastjson-test
	cd test && ./lastjson-test

test/lastjson-test: test/Makefile $(wildcard test/*.cpp) $(wildcard include/lastjson/*.hpp)
	$(MAKE) -C test lastjson-test

test/Makefile: test/CMakeLists.txt
	cd test && cmake .

clean:
	cd test && rm -Rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile lastjson-test
