# RollWireCalculator 서브프로젝트
# GoogleTest는 루트 CMakeLists.txt에서 이미 설정됨

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
  GTest::gtest_main
)

# Coverage 플래그 추가
if(ENABLE_COVERAGE)
    target_compile_options(rollwirecalculator_test PRIVATE --coverage)
    target_link_options(rollwirecalculator_test PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(rollwirecalculator_test)
