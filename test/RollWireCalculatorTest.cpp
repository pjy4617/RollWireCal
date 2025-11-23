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

TEST(RollWireCalculatorTest, CalculatesFirstRotationLengthAccurately) {
    // 롤 내경 기준으로 첫 번째 바퀴의 길이를 정확히 계산한다
    // 연속 증가 모델: r(θ) = innerRadius + θ/360 × wireThickness
    // 첫 번째 바퀴(360도)의 길이:
    // L = ∫[0→360] r(θ) × (2π/360) dθ
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]|[0→360]
    //   = (2π/360) × [innerRadius × 360 + wireThickness × 360²/(2×360)]
    //   = 2π × innerRadius + π × wireThickness

    double wireThickness = 2.0;  // mm
    double innerRadius = 100.0;  // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 정확히 360도에 해당하는 길이 (mm → m)
    double lengthFor360 = (2.0 * M_PI * innerRadius + M_PI * wireThickness) / 1000.0;

    double rotation = calculator.calculateRotationFromLength(lengthFor360);

    // 첫 번째 바퀴의 길이를 입력하면 정확히 360도를 반환해야 함
    EXPECT_NEAR(360.0, rotation, 0.01);
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

TEST(RollWireCalculatorTest, CalculatesRotationForThreeOrMoreRotationsAccurately) {
    // 3바퀴 이상의 긴 길이에서도 정확한 회전량을 계산한다
    // 연속 증가 모델: r(θ) = innerRadius + θ/360 × wireThickness
    // L = ∫[0→θ] r(t) × (2π/360) dt
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]

    double wireThickness = 1.5;  // mm
    double innerRadius = 75.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 3바퀴(1080도)에 해당하는 정확한 길이 계산 (mm → m)
    double theta = 1080.0;  // degrees
    double lengthFor1080 = (2.0 * M_PI / 360.0) *
                           (innerRadius * theta + wireThickness * theta * theta / (2.0 * 360.0)) / 1000.0;

    double rotation = calculator.calculateRotationFromLength(lengthFor1080);

    // 3바퀴의 길이를 입력하면 정확히 1080도를 반환해야 함
    EXPECT_NEAR(1080.0, rotation, 0.1);  // 0.1도 오차 허용

    // 5바퀴(1800도)도 테스트
    theta = 1800.0;
    double lengthFor1800 = (2.0 * M_PI / 360.0) *
                           (innerRadius * theta + wireThickness * theta * theta / (2.0 * 360.0)) / 1000.0;

    rotation = calculator.calculateRotationFromLength(lengthFor1800);

    EXPECT_NEAR(1800.0, rotation, 0.1);  // 0.1도 오차 허용
}

// Phase 2.5: 경계값 테스트
TEST(RollWireCalculatorTest, CalculateRotationFromLengthWorksForVerySmallLength) {
    // 매우 작은 길이(0.001m)에서 올바르게 동작한다
    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    double verySmallLength = 0.001;  // meters (1mm)

    // 매우 작은 길이에 대해 회전량을 계산
    // 예상: 회전량은 매우 작아야 하며(1도 미만), 음수가 아니어야 함
    // L = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]
    // 매우 작은 θ에서는 θ² 항이 무시 가능하므로 근사적으로:
    // L ≈ (2π/360) × innerRadius × θ
    // θ ≈ L × 360 / (2π × innerRadius)
    // θ ≈ 0.001 * 1000 * 360 / (2π × 50) ≈ 1.146도

    double rotation = calculator.calculateRotationFromLength(verySmallLength);

    EXPECT_GT(rotation, 0.0);  // 양수여야 함
    EXPECT_LT(rotation, 2.0);  // 2도 미만이어야 함
    EXPECT_NEAR(1.146, rotation, 0.01);  // 예상 값과 근사
}

TEST(RollWireCalculatorTest, CalculateRotationFromLengthWorksForVeryLargeLength) {
    // 매우 큰 길이(1000m)에서 올바르게 동작한다
    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    double veryLargeLength = 1000.0;  // meters (1,000,000mm)

    // 매우 큰 길이에 대해 회전량을 계산
    // 수치적 안정성 검증: 양수이고 합리적인 범위 내에 있어야 함
    double rotation = calculator.calculateRotationFromLength(veryLargeLength);

    EXPECT_GT(rotation, 0.0);  // 양수여야 함
    EXPECT_GT(rotation, 100000.0);  // 최소 100,000도 이상 (많은 회전)

    // 결과가 유한한 값이어야 함 (수치적 안정성)
    EXPECT_TRUE(std::isfinite(rotation));

    // 합리적인 범위 검증 (대략 185,000~190,000도 예상)
    EXPECT_LT(rotation, 200000.0);

    // 역산 검증: 계산된 회전량을 다시 길이로 변환했을 때 원래 길이와 유사해야 함
    // (calculateLengthFromRotation이 구현되면 이 부분을 활성화)
    // double calculatedLength = calculator.calculateLengthFromRotation(rotation);
    // EXPECT_NEAR(veryLargeLength, calculatedLength, 0.1);  // 10cm 오차 허용
}

// Phase 3.1: 회전량 → 길이 변환 기본 동작
TEST(RollWireCalculatorTest, CalculateLengthFromRotationMethodExists) {
    // calculateLengthFromRotation 메서드가 존재한다
    RollWireCalculator calculator(1.0, 50.0);

    // 메서드를 호출할 수 있으면 테스트 통과
    double length = calculator.calculateLengthFromRotation(0.0);

    // 컴파일되고 실행되면 메서드가 존재하는 것
    SUCCEED();
}

TEST(RollWireCalculatorTest, CalculateLengthFromRotationReturnsZeroWhenRotationIsZero) {
    // 회전량이 0일 때 길이 0을 반환한다
    RollWireCalculator calculator(1.0, 50.0);

    double length = calculator.calculateLengthFromRotation(0.0);

    EXPECT_DOUBLE_EQ(0.0, length);
}

// Phase 3.2: 단순 케이스 (1바퀴)
TEST(RollWireCalculatorTest, CalculateLengthFromRotationReturnsLengthFor360Degrees) {
    // 회전량 360도일 때 1바퀴에 해당하는 길이를 반환한다
    // 연속 증가 모델: r(θ) = innerRadius + θ/360 × wireThickness
    // L = ∫[0→360] r(θ) × (2π/360) dθ
    //   = 2π × innerRadius + π × wireThickness

    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 360도 회전에 해당하는 와이어 길이 계산 (mm → m)
    double expectedLength = (2.0 * M_PI * innerRadius + M_PI * wireThickness) / 1000.0;

    double length = calculator.calculateLengthFromRotation(360.0);

    EXPECT_NEAR(expectedLength, length, 0.00001);  // 0.01mm 오차 허용
}

TEST(RollWireCalculatorTest, CalculateLengthFromRotationReturnsLengthFor180Degrees) {
    // 회전량 180도일 때 0.5바퀴에 해당하는 길이를 반환한다
    // 연속 증가 모델에서 180도 회전에 해당하는 길이:
    // L = ∫[0→180] r(θ) × (2π/360) dθ
    //   = (2π/360) × [innerRadius × 180 + wireThickness × 180²/(2×360)]
    //   = π × innerRadius + π × wireThickness / 4

    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 180도 회전에 해당하는 와이어 길이 계산 (mm → m)
    double expectedLength = (M_PI * innerRadius + M_PI * wireThickness / 4.0) / 1000.0;

    double length = calculator.calculateLengthFromRotation(180.0);

    EXPECT_NEAR(expectedLength, length, 0.00001);  // 0.01mm 오차 허용
}

// Phase 3.3: 복잡한 케이스 (연속 증가 모델)
TEST(RollWireCalculatorTest, CalculatesExactLengthFor180DegreesWithContinuousModel) {
    // 180도 회전에 해당하는 정확한 와이어 길이를 계산한다
    // 연속 증가 모델에서 반지름은 회전량에 따라 선형적으로 증가:
    // r(θ) = innerRadius + (θ/360) × wireThickness
    //
    // 180도 회전의 길이:
    // L = ∫[0→180] r(θ) × (2π/360) dθ
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]|[0→180]
    //   = (2π/360) × [innerRadius × 180 + wireThickness × 180²/(2×360)]
    //   = (2π/360) × [innerRadius × 180 + wireThickness × 32400/720]
    //   = (2π/360) × [innerRadius × 180 + wireThickness × 45]
    //   = π × innerRadius + π × wireThickness / 4

    // 테스트 1: 와이어 두께 2.0mm, 내경 75mm
    double wireThickness1 = 2.0;  // mm
    double innerRadius1 = 75.0;   // mm
    RollWireCalculator calculator1(wireThickness1, innerRadius1);

    // 예상 길이 계산 (mm → m)
    double expectedLength1 = (M_PI * innerRadius1 + M_PI * wireThickness1 / 4.0) / 1000.0;

    double length1 = calculator1.calculateLengthFromRotation(180.0);

    EXPECT_NEAR(expectedLength1, length1, 0.00001);  // 0.01mm 오차 허용

    // 테스트 2: 와이어 두께 0.5mm, 내경 25mm (더 작은 값)
    double wireThickness2 = 0.5;  // mm
    double innerRadius2 = 25.0;   // mm
    RollWireCalculator calculator2(wireThickness2, innerRadius2);

    // 예상 길이 계산 (mm → m)
    double expectedLength2 = (M_PI * innerRadius2 + M_PI * wireThickness2 / 4.0) / 1000.0;

    double length2 = calculator2.calculateLengthFromRotation(180.0);

    EXPECT_NEAR(expectedLength2, length2, 0.00001);  // 0.01mm 오차 허용

    // 테스트 3: 와이어 두께 3.0mm, 내경 120mm (더 큰 값)
    double wireThickness3 = 3.0;  // mm
    double innerRadius3 = 120.0;  // mm
    RollWireCalculator calculator3(wireThickness3, innerRadius3);

    // 예상 길이 계산 (mm → m)
    double expectedLength3 = (M_PI * innerRadius3 + M_PI * wireThickness3 / 4.0) / 1000.0;

    double length3 = calculator3.calculateLengthFromRotation(180.0);

    EXPECT_NEAR(expectedLength3, length3, 0.00001);  // 0.01mm 오차 허용
}

TEST(RollWireCalculatorTest, CalculateLengthFromRotationReturnsLengthFor720Degrees) {
    // 720도(2바퀴) 회전 시 연속적인 반지름 증가를 고려한 길이를 반환한다
    // 연속 증가 모델: r(θ) = innerRadius + θ/360 × wireThickness
    // L = ∫[0→720] r(θ) × (2π/360) dθ
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]|[0→720]
    //   = (2π/360) × [innerRadius × 720 + wireThickness × 720²/(2×360)]
    //   = (2π/360) × [innerRadius × 720 + wireThickness × 518400/720]
    //   = (2π/360) × [innerRadius × 720 + wireThickness × 720]
    //   = (2π/360) × 720 × (innerRadius + wireThickness)

    double wireThickness = 2.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 720도 회전에 해당하는 예상 길이 계산 (mm → m)
    double theta = 720.0;  // degrees
    double expectedLength = (2.0 * M_PI / 360.0) *
                           (innerRadius * theta + wireThickness * theta * theta / (2.0 * 360.0)) / 1000.0;

    double length = calculator.calculateLengthFromRotation(720.0);

    EXPECT_NEAR(expectedLength, length, 0.00001);  // 0.01mm 오차 허용
}

TEST(RollWireCalculatorTest, RadiusIncreaseModelIsAccuratelyApplied) {
    // 회전량에 비례한 반지름 증가 모델이 정확히 적용된다
    // 연속 증가 모델: r(θ) = innerRadius + (θ/360) × wireThickness
    //
    // 이 테스트는 모델이 정확히 적용되는지 여러 각도에서 검증:
    // 1. 0도에서 시작 반지름 = innerRadius
    // 2. 360도에서 반지름 = innerRadius + wireThickness
    // 3. 720도에서 반지름 = innerRadius + 2 × wireThickness
    //
    // 각 구간의 길이가 예상대로 계산되는지 확인

    double wireThickness = 2.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 첫 번째 360도(0→360)의 길이
    // L₁ = ∫[0→360] r(θ) × (2π/360) dθ
    //    = (2π/360) × [innerRadius × 360 + wireThickness × 360²/(2×360)]
    //    = 2π × innerRadius + π × wireThickness
    double length1 = calculator.calculateLengthFromRotation(360.0);
    double expectedLength1 = (2.0 * M_PI * innerRadius + M_PI * wireThickness) / 1000.0;
    EXPECT_NEAR(expectedLength1, length1, 0.00001);

    // 두 번째 360도(360→720)의 길이
    // L₂ = L(720) - L(360)
    // L(720) = (2π/360) × [innerRadius × 720 + wireThickness × 720²/(2×360)]
    // L₂ = L(720) - L(360)
    double length720 = calculator.calculateLengthFromRotation(720.0);
    double length2 = length720 - length1;

    // 두 번째 바퀴는 반지름이 더 크므로 첫 번째 바퀴보다 길어야 함
    EXPECT_GT(length2, length1);

    // 예상 두 번째 바퀴 길이 계산
    // L(720) - L(360) = (2π/360) × [innerRadius × 360 + wireThickness × (720² - 360²)/(2×360)]
    //                 = (2π/360) × [innerRadius × 360 + wireThickness × (518400 - 129600)/720]
    //                 = (2π/360) × [innerRadius × 360 + wireThickness × 540]
    //                 = 2π × innerRadius + 3π × wireThickness
    double expectedLength2 = (2.0 * M_PI * innerRadius + 3.0 * M_PI * wireThickness) / 1000.0;
    EXPECT_NEAR(expectedLength2, length2, 0.00001);

    // 세 번째 360도(720→1080)의 길이
    double length1080 = calculator.calculateLengthFromRotation(1080.0);
    double length3 = length1080 - length720;

    // 세 번째 바퀴는 두 번째 바퀴보다 더 길어야 함
    EXPECT_GT(length3, length2);

    // 예상 세 번째 바퀴 길이 계산
    // L(1080) - L(720) = (2π/360) × [innerRadius × 360 + wireThickness × (1080² - 720²)/(2×360)]
    //                  = (2π/360) × [innerRadius × 360 + wireThickness × (1166400 - 518400)/720]
    //                  = (2π/360) × [innerRadius × 360 + wireThickness × 900]
    //                  = 2π × innerRadius + 5π × wireThickness
    double expectedLength3 = (2.0 * M_PI * innerRadius + 5.0 * M_PI * wireThickness) / 1000.0;
    EXPECT_NEAR(expectedLength3, length3, 0.00001);

    // 반지름 증가 확인: 각 바퀴의 길이 증가량이 일정한 패턴을 따름
    // ΔL₂ - ΔL₁ = (L₂ - L₁) = 2π × wireThickness
    // ΔL₃ - ΔL₂ = (L₃ - L₂) = 2π × wireThickness
    double diff21 = length2 - length1;
    double diff32 = length3 - length2;
    double expectedDiff = (2.0 * M_PI * wireThickness) / 1000.0;
    EXPECT_NEAR(expectedDiff, diff21, 0.00001);
    EXPECT_NEAR(expectedDiff, diff32, 0.00001);
}

TEST(RollWireCalculatorTest, CalculateLengthFromRotationReturnsAccurateValuesForThreeOrMoreRotations) {
    // 3바퀴 이상의 복잡한 케이스에서 적분 또는 수치계산으로 정확한 값을 반환한다
    // 연속 증가 모델: r(θ) = innerRadius + (θ/360) × wireThickness
    // L = ∫[0→θ] r(t) × (2π/360) dt
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]
    //
    // 다양한 회전량(3, 5, 10바퀴)에 대해 정확도 검증

    double wireThickness = 1.5;  // mm
    double innerRadius = 60.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    // 테스트 1: 3바퀴 (1080도)
    double theta1 = 1080.0;  // degrees
    double expectedLength1 = (2.0 * M_PI / 360.0) *
                            (innerRadius * theta1 + wireThickness * theta1 * theta1 / (2.0 * 360.0)) / 1000.0;
    double actualLength1 = calculator.calculateLengthFromRotation(theta1);
    EXPECT_NEAR(expectedLength1, actualLength1, 0.00001);  // 0.01mm 오차 허용

    // 테스트 2: 5바퀴 (1800도)
    double theta2 = 1800.0;  // degrees
    double expectedLength2 = (2.0 * M_PI / 360.0) *
                            (innerRadius * theta2 + wireThickness * theta2 * theta2 / (2.0 * 360.0)) / 1000.0;
    double actualLength2 = calculator.calculateLengthFromRotation(theta2);
    EXPECT_NEAR(expectedLength2, actualLength2, 0.00001);  // 0.01mm 오차 허용

    // 테스트 3: 10바퀴 (3600도)
    double theta3 = 3600.0;  // degrees
    double expectedLength3 = (2.0 * M_PI / 360.0) *
                            (innerRadius * theta3 + wireThickness * theta3 * theta3 / (2.0 * 360.0)) / 1000.0;
    double actualLength3 = calculator.calculateLengthFromRotation(theta3);
    EXPECT_NEAR(expectedLength3, actualLength3, 0.00001);  // 0.01mm 오차 허용

    // 테스트 4: 더 큰 값들에 대한 상대 오차 검증
    // 큰 회전량에서도 적분 공식과 정확히 일치해야 함
    double theta4 = 7200.0;  // 20바퀴
    double expectedLength4 = (2.0 * M_PI / 360.0) *
                            (innerRadius * theta4 + wireThickness * theta4 * theta4 / (2.0 * 360.0)) / 1000.0;
    double actualLength4 = calculator.calculateLengthFromRotation(theta4);

    // 절대 오차 검증
    EXPECT_NEAR(expectedLength4, actualLength4, 0.00001);  // 0.01mm 오차 허용

    // 상대 오차 검증 (0.001% 이내)
    double relativeError = std::abs((actualLength4 - expectedLength4) / expectedLength4);
    EXPECT_LT(relativeError, 1e-5);

    // 테스트 5: 다른 파라미터로도 검증
    double wireThickness5 = 2.5;  // mm
    double innerRadius5 = 40.0;   // mm
    RollWireCalculator calculator2(wireThickness5, innerRadius5);

    double theta5 = 5400.0;  // 15바퀴
    double expectedLength5 = (2.0 * M_PI / 360.0) *
                            (innerRadius5 * theta5 + wireThickness5 * theta5 * theta5 / (2.0 * 360.0)) / 1000.0;
    double actualLength5 = calculator2.calculateLengthFromRotation(theta5);
    EXPECT_NEAR(expectedLength5, actualLength5, 0.00001);  // 0.01mm 오차 허용
}

// Phase 3.4: 입력 검증
TEST(RollWireCalculatorTest, CalculateLengthFromRotationThrowsExceptionWhenRotationIsNegative) {
    // 회전량이 음수일 때 예외를 발생시킨다
    RollWireCalculator calculator(1.0, 50.0);

    EXPECT_THROW({
        calculator.calculateLengthFromRotation(-90.0);
    }, std::invalid_argument);
}

// Phase 3.5: 경계값 테스트
TEST(RollWireCalculatorTest, CalculateLengthFromRotationWorksWithVerySmallRotation) {
    // 매우 작은 회전량(0.1도)에서 올바르게 동작한다
    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    double verySmallRotation = 0.1;  // degrees

    // 매우 작은 회전량에 대해 길이를 계산
    // 연속 증가 모델: r(θ) = innerRadius + θ/360 × wireThickness
    // L = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]
    // θ가 매우 작으므로 θ² 항은 무시 가능하며, 근사적으로:
    // L ≈ (2π/360) × innerRadius × θ
    // L ≈ (2π/360) × 50 × 0.1 = 0.0872665 mm = 0.0000872665 m

    double length = calculator.calculateLengthFromRotation(verySmallRotation);

    // 양수여야 함
    EXPECT_GT(length, 0.0);

    // 예상 길이 계산 (정확한 공식 사용)
    double theta = verySmallRotation;  // degrees
    double expectedLength = (2.0 * M_PI / 360.0) *
                           (innerRadius * theta + wireThickness * theta * theta / (2.0 * 360.0)) / 1000.0;

    // 정확한 값과 매우 가까워야 함 (0.000001m = 0.001mm 오차 허용)
    EXPECT_NEAR(expectedLength, length, 0.000001);

    // 결과가 유한한 값이어야 함 (수치적 안정성)
    EXPECT_TRUE(std::isfinite(length));
}

TEST(RollWireCalculatorTest, CalculateLengthFromRotationWorksWithVeryLargeRotation) {
    // 매우 큰 회전량(36000도)에서 올바르게 동작한다
    double wireThickness = 1.0;  // mm
    double innerRadius = 50.0;   // mm
    RollWireCalculator calculator(wireThickness, innerRadius);

    double veryLargeRotation = 36000.0;  // degrees (100바퀴)

    // 매우 큰 회전량에 대해 길이를 계산
    // 연속 증가 모델: r(θ) = innerRadius + θ/360 × wireThickness
    // L = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]
    // L = (2π/360) × [50 × 36000 + 1.0 × 36000²/(2×360)] / 1000
    // L = (2π/360) × [1,800,000 + 1,800,000] / 1000
    // L = (2π/360) × 3,600,000 / 1000 ≈ 62.83m

    double length = calculator.calculateLengthFromRotation(veryLargeRotation);

    // 양수여야 함
    EXPECT_GT(length, 0.0);

    // 예상 길이 계산 (정확한 공식 사용)
    double theta = veryLargeRotation;  // degrees
    double expectedLength = (2.0 * M_PI / 360.0) *
                           (innerRadius * theta + wireThickness * theta * theta / (2.0 * 360.0)) / 1000.0;

    // 정확한 값과 가까워야 함 (0.01m = 10mm 오차 허용)
    EXPECT_NEAR(expectedLength, length, 0.01);

    // 결과가 유한한 값이어야 함 (수치적 안정성)
    EXPECT_TRUE(std::isfinite(length));

    // 합리적인 범위 검증 (100바퀴, 반지름 50~149mm, 대략 62~63m 예상)
    EXPECT_GT(length, 60.0);  // 최소 60m 이상
    EXPECT_LT(length, 65.0);  // 최대 65m 미만

    // 상대 오차 검증 (0.01% 이내)
    double relativeError = std::abs((length - expectedLength) / expectedLength);
    EXPECT_LT(relativeError, 1e-4);
}
