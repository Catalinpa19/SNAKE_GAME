# CMake generated Testfile for 
# Source directory: C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests
# Build directory: C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/build-vs
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[SnakeUiTests]=] "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/build-vs/Debug/SnakeTests.exe")
  set_tests_properties([=[SnakeUiTests]=] PROPERTIES  ENVIRONMENT "QT_QPA_PLATFORM=offscreen" ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:C:/Qt/6.12.0/msvc2022_64/bin" _BACKTRACE_TRIPLES "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;25;add_test;C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[SnakeUiTests]=] "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/build-vs/Release/SnakeTests.exe")
  set_tests_properties([=[SnakeUiTests]=] PROPERTIES  ENVIRONMENT "QT_QPA_PLATFORM=offscreen" ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:C:/Qt/6.12.0/msvc2022_64/bin" _BACKTRACE_TRIPLES "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;25;add_test;C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[SnakeUiTests]=] "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/build-vs/MinSizeRel/SnakeTests.exe")
  set_tests_properties([=[SnakeUiTests]=] PROPERTIES  ENVIRONMENT "QT_QPA_PLATFORM=offscreen" ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:C:/Qt/6.12.0/msvc2022_64/bin" _BACKTRACE_TRIPLES "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;25;add_test;C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[SnakeUiTests]=] "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/build-vs/RelWithDebInfo/SnakeTests.exe")
  set_tests_properties([=[SnakeUiTests]=] PROPERTIES  ENVIRONMENT "QT_QPA_PLATFORM=offscreen" ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:C:/Qt/6.12.0/msvc2022_64/bin" _BACKTRACE_TRIPLES "C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;25;add_test;C:/scl_anul_2/practica/SNAKE_GAME/Snake/SnakeTests/CMakeLists.txt;0;")
else()
  add_test([=[SnakeUiTests]=] NOT_AVAILABLE)
endif()
