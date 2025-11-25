# RollWireMover 서브프로젝트
# GoogleTest는 루트 CMakeLists.txt에서 이미 설정됨

cmake_minimum_required(VERSION 3.14)
project(RollWireMover)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일
set(SOURCES
    src/SimMotor.cpp
    src/RollWireMover.cpp
)

# 헤더 파일 디렉토리
include_directories(include)

# 라이브러리 생성
add_library(rollwiremover ${SOURCES})

# RollWireCalculator 라이브러리 링크
target_link_libraries(rollwiremover PUBLIC rollwirecalculator)

# RollWireCalculator의 헤더 포함 경로 추가
target_include_directories(rollwiremover PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../RollWireCalculator/include
)

# Coverage 플래그 추가
if(ENABLE_COVERAGE)
    target_compile_options(rollwiremover PRIVATE --coverage)
    target_link_options(rollwiremover PRIVATE --coverage)
endif()

# 테스트 실행 파일
add_executable(rollwiremover_test
    test/MotorTest.cpp
    test/SimMotorTest.cpp
    test/RollWireMoverTest.cpp
)

# 테스트 링크
target_link_libraries(rollwiremover_test
    rollwiremover
    GTest::gtest_main
)

# Coverage 플래그 추가
if(ENABLE_COVERAGE)
    target_compile_options(rollwiremover_test PRIVATE --coverage)
    target_link_options(rollwiremover_test PRIVATE --coverage)
endif()

# 테스트 등록
include(GoogleTest)
gtest_discover_tests(rollwiremover_test)
