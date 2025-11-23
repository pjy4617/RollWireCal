#include <gtest/gtest.h>
#include "Motor.h"
#include <type_traits>

// Phase 1.1: Motor 인터페이스 (순수 가상 클래스)
TEST(MotorTest, MotorIsAbstractClass) {
    // Motor는 순수 가상 클래스이다 (인스턴스화 불가능)
    // C++11 type_traits를 사용하여 추상 클래스임을 확인
    EXPECT_TRUE(std::is_abstract<Motor>::value);
}

TEST(MotorTest, MotorHasVirtualDestructor) {
    // Motor는 가상 소멸자를 가지고 있다
    EXPECT_TRUE(std::has_virtual_destructor<Motor>::value);
}
