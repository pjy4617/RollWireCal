cmake_minimum_required(VERSION 3.14)
project(RollWireCalculator)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest 찾기
find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest_main gtest)
include_directories(${GTEST_INCLUDE_DIRS})

# 라이브러리
add_library(rollwirecalculator
  src/RollWireCalculator.cpp
)

target_include_directories(rollwirecalculator PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Coverage 플래그 추가
if(ENABLE_COVERAGE)
    target_compile_options(rollwirecalculator PRIVATE --coverage)
    target_link_options(rollwirecalculator PRIVATE --coverage)
endif()

# 테스트 실행 파일
add_executable(rollwirecalculator_test
  test/RollWireCalculatorTest.cpp
)

target_link_libraries(rollwirecalculator_test
  rollwirecalculator
  ${GTEST_LDFLAGS}
  pthread
)

# Coverage 플래그 추가
if(ENABLE_COVERAGE)
    target_compile_options(rollwirecalculator_test PRIVATE --coverage)
    target_link_options(rollwirecalculator_test PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(rollwirecalculator_test)
