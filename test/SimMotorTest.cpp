#include <gtest/gtest.h>
#include "SimMotor.h"

// Phase 1.2: SimMotor 기본 구조 (Motor 구현체)
TEST(SimMotorTest, CanCreateSimMotor) {
    // SimMotor 클래스를 생성할 수 있다
    SimMotor simMotor;

    // 객체가 생성되면 테스트 통과
    SUCCEED();
}

TEST(SimMotorTest, InitialRotationPositionIsZero) {
    // 기본 생성자로 SimMotor 객체를 생성하면 초기 회전 위치는 0이다
    SimMotor simMotor;

    EXPECT_DOUBLE_EQ(0.0, simMotor.getCurrentRotation());
}
