#include <gtest/gtest.h>
#include <stdexcept>
#include <cmath>
#include "RollWireCalculator.h"

// Phase 1.1: RollWireCalculator 클래스 생성
TEST(RollWireCalculatorTest, CanCreateInstance) {
    // RollWireCalculator 클래스를 생성할 수 있다
    RollWireCalculator calculator(1.0, 50.0);

    // 객체가 생성되면 테스트 통과
    SUCCEED();
}

TEST(RollWireCalculatorTest, CanGetWireThickness) {
    // 생성자에 전달한 와이어 두께를 조회할 수 있다
    double expectedThickness = 1.5;
    RollWireCalculator calculator(expectedThickness, 50.0);

    EXPECT_DOUBLE_EQ(expectedThickness, calculator.getWireThickness());
}

TEST(RollWireCalculatorTest, CanGetInnerRadius) {
    // 생성자에 전달한 롤 내경 반지름을 조회할 수 있다
    double expectedInnerRadius = 50.0;
    RollWireCalculator calculator(1.5, expectedInnerRadius);

    EXPECT_DOUBLE_EQ(expectedInnerRadius, calculator.getInnerRadius());
}

// Phase 1.2: 생성자 입력 검증
TEST(RollWireCalculatorTest, ThrowsExceptionWhenWireThicknessIsZero) {
    // 와이어 두께가 0일 때 예외를 발생시킨다
    EXPECT_THROW({
        RollWireCalculator calculator(0.0, 50.0);
    }, std::invalid_argument);
}

TEST(RollWireCalculatorTest, ThrowsExceptionWhenWireThicknessIsNegative) {
    // 와이어 두께가 음수일 때 예외를 발생시킨다
    EXPECT_THROW({
        RollWireCalculator calculator(-1.0, 50.0);
    }, std::invalid_argument);
}

TEST(RollWireCalculatorTest, ThrowsExceptionWhenInnerRadiusIsZero) {
    // 롤 내경이 0일 때 예외를 발생시킨다
    EXPECT_THROW({
        RollWireCalculator calculator(1.0, 0.0);
    }, std::invalid_argument);
}

TEST(RollWireCalculatorTest, ThrowsExceptionWhenInnerRadiusIsNegative) {
    // 롤 내경이 음수일 때 예외를 발생시킨다
    EXPECT_THROW({
        RollWireCalculator calculator(1.0, -10.0);
    }, std::invalid_argument);
}

// Phase 1.4: 롤 내경 설정 및 조회
TEST(RollWireCalculatorTest, CanSetInnerRadius) {
    // setInnerRadius() 메서드로 롤 내경 반지름을 변경할 수 있다
    RollWireCalculator calculator(1.0, 50.0);
    double newInnerRadius = 75.0;

    calculator.setInnerRadius(newInnerRadius);

    EXPECT_DOUBLE_EQ(newInnerRadius, calculator.getInnerRadius());
}

TEST(RollWireCalculatorTest, SetInnerRadiusThrowsExceptionWhenZero) {
    // setInnerRadius()에 0을 전달하면 예외를 발생시킨다
    RollWireCalculator calculator(1.0, 50.0);

    EXPECT_THROW({
        calculator.setInnerRadius(0.0);
    }, std::invalid_argument);
}

TEST(RollWireCalculatorTest, SetInnerRadiusThrowsExceptionWhenNegative) {
    // setInnerRadius()에 음수를 전달하면 예외를 발생시킨다
    RollWireCalculator calculator(1.0, 50.0);

    EXPECT_THROW({
        calculator.setInnerRadius(-25.0);
    }, std::invalid_argument);
}

// Phase 2.1: 길이 → 회전량 변환 기본 동작
TEST(RollWireCalculatorTest, CalculateRotationFromLengthReturnsZeroWhenLengthIsZero) {
    // 길이가 0일 때 회전량 0을 반환한다
    RollWireCalculator calculator(1.0, 50.0);

    double rotation = calculator.calculateRotationFromLength(0.0);

    EXPECT_DOUBLE_EQ(0.0, rotation);
}

// Phase 2.2: 단순 케이스 (1바퀴)
TEST(RollWireCalculatorTest, CalculateRotationFromLengthReturns360ForOneRotation) {
    // 와이어가 롤 내경에 정확히 1바퀴 감길 때 360도를 반환한다
    // 연속 증가 모델: r(θ) = innerRadius + θ/360 × wireThickness
    // L = ∫[0→360] r(θ) × (2π/360) dθ
    //   = ∫[0→360] [innerRadius + θ/360 × wireThickness] × (2π/360) dθ
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]|[0→360]
    //   = (2π/360) × [innerRadius × 360 + wireThickness × 360²/(2×360)]
    //   = (2π/360) × [innerRadius × 360 + wireThickness × 360/2]
    //   = 2π × innerRadius + π × wireThickness

    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 1바퀴(360도)에 해당하는 와이어 길이 계산 (mm → m)
    double expectedLength = (2.0 * M_PI * innerRadius + M_PI * wireThickness) / 1000.0;

    double rotation = calculator.calculateRotationFromLength(expectedLength);

    EXPECT_NEAR(360.0, rotation, 0.01);  // 0.01도 오차 허용
}

TEST(RollWireCalculatorTest, CalculateRotationFromLengthReturns180ForHalfRotation) {
    // 와이어가 롤 내경에 0.5바퀴 감길 때 180도를 반환한다
    // 연속 증가 모델에서 180도 회전에 해당하는 길이:
    // L = ∫[0→180] r(θ) × (2π/360) dθ
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]|[0→180]
    //   = (2π/360) × [innerRadius × 180 + wireThickness × 180²/(2×360)]
    //   = (2π/360) × [innerRadius × 180 + wireThickness × 180/4]
    //   = π × innerRadius + π × wireThickness / 4

    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 0.5바퀴(180도)에 해당하는 와이어 길이 계산 (mm → m)
    double expectedLength = (M_PI * innerRadius + M_PI * wireThickness / 4.0) / 1000.0;

    double rotation = calculator.calculateRotationFromLength(expectedLength);

    EXPECT_NEAR(180.0, rotation, 0.01);  // 0.01도 오차 허용
}

// Phase 2.3: 복잡한 케이스 (연속 증가 모델)
TEST(RollWireCalculatorTest, CalculateRotationFromLengthReturnsLessThan720ForTwoRotationsWorthOfLength) {
    // 2바퀴 분량의 길이를 입력하면 720도보다 약간 작은 회전량을 반환한다
    // 반지름이 증가하므로, 같은 길이로 더 적은 회전을 하게 됨

    double wireThickness = 2.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 720도(2바퀴)에 해당하는 와이어 길이 계산
    // L = (2π/360) × [innerRadius × 720 + wireThickness × 720²/(2×360)]
    //   = (2π/360) × [50 × 720 + 2 × 720²/720]
    //   = (2π/360) × [36000 + 1440]
    //   = (2π/360) × 37440
    double length720 = (2.0 * M_PI / 360.0) * (innerRadius * 720.0 + wireThickness * 720.0 * 720.0 / 720.0) / 1000.0;

    double rotation = calculator.calculateRotationFromLength(length720);

    // 연속 증가 모델에서는 정확히 720도여야 함
    EXPECT_NEAR(720.0, rotation, 0.01);

    // 이제 고정 반지름 모델(innerRadius만 사용)로 2바퀴 길이를 계산
    // 이 길이는 연속 증가 모델에서 720도보다 적은 회전을 일으킴
    double lengthFixedRadius = (2.0 * M_PI * innerRadius * 2.0) / 1000.0;  // 2바퀴, 고정 반지름

    double rotationForFixedLength = calculator.calculateRotationFromLength(lengthFixedRadius);

    // 반지름이 증가하므로, 같은 길이로는 720도보다 적게 회전
    EXPECT_LT(rotationForFixedLength, 720.0);
    EXPECT_GT(rotationForFixedLength, 680.0);  // 합리적인 하한
}

// Phase 2.4: 입력 검증
TEST(RollWireCalculatorTest, CalculateRotationFromLengthThrowsExceptionWhenLengthIsNegative) {
    // 길이가 음수일 때 예외를 발생시킨다
    RollWireCalculator calculator(1.0, 50.0);

    EXPECT_THROW({
        calculator.calculateRotationFromLength(-1.0);
    }, std::invalid_argument);
}
