# RollWireMover 서브프로젝트
# GoogleTest는 루트 CMakeLists.txt에서 이미 설정됨

# 소스 파일
set(SOURCES
    src/SimMotor.cpp
)

# 헤더 파일 디렉토리
include_directories(include)

# 라이브러리 생성
add_library(rollwiremover ${SOURCES})

# Coverage 플래그 추가
if(ENABLE_COVERAGE)
    target_compile_options(rollwiremover PRIVATE --coverage)
    target_link_options(rollwiremover PRIVATE --coverage)
endif()

# 테스트 실행 파일
add_executable(rollwiremover_test
    test/MotorTest.cpp
    test/SimMotorTest.cpp
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
