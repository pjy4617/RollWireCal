cmake_minimum_required(VERSION 3.14)
project(RollWireCal)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 코드 커버리지 옵션
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" OFF)

if(ENABLE_COVERAGE)
    # GCC/Clang에서 커버리지 플래그 추가
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "Coverage enabled")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage -O0 -g")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage -O0 -g")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        
        # 커버리지 타겟 생성
        find_program(LCOV_PATH lcov)
        find_program(GENHTML_PATH genhtml)
        
        if(LCOV_PATH AND GENHTML_PATH)
            add_custom_target(coverage
                # 초기화
                COMMAND ${LCOV_PATH} --directory . --zerocounters
                # 테스트 실행
                COMMAND ${CMAKE_CTEST_COMMAND} --verbose
                # 커버리지 수집
                COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info
                # 불필요한 파일 제거
                COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/test/*' '*/tests/*' '*/_deps/*' --output-file coverage.info
                # HTML 리포트 생성
                COMMAND ${GENHTML_PATH} -o coverage_html coverage.info
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating code coverage report"
            )
        endif()
    endif()
endif()

# Google Test 설정
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Windows에서 gtest_force_shared_crt 설정
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# 소스 파일
add_library(rollwirecalculator
  src/RollWireCalculator.cpp
  src/SimMotor.cpp
)

target_include_directories(rollwirecalculator PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 테스트 실행 파일
add_executable(rollwirecalculator_test
  test/RollWireCalculatorTest.cpp
  test/MotorTest.cpp
  test/SimMotorTest.cpp
)

target_link_libraries(rollwirecalculator_test
  rollwirecalculator
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(rollwirecalculator_test)
