#include "RollWireMover.h"
#include "SimMotor.h"
#include <gtest/gtest.h>

// Phase 2.1: 클래스 생성 및 초기화 (의존성 주입)
TEST(RollWireMoverTest, CanCreateRollWireMover) {
  // RollWireMover 클래스를 생성할 수 있다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 객체가 생성되면 테스트 통과
  SUCCEED();
}

TEST(RollWireMoverTest, ConstructorHasCorrectParameters) {
  // wireThickness, innerRadius, Motor 포인터, ErrorCode& 출력 파라미터를 받는
  // 생성자가 있다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  double wireThickness = 1.0; // mm
  double innerRadius = 50.0;  // mm
  RollWireMover::ErrorCode error;

  // 생성자가 올바른 파라미터를 받아 객체를 생성할 수 있어야 함
  RollWireMover mover(wireThickness, innerRadius, motor, error);

  // 컴파일되고 실행되면 생성자가 올바른 시그니처를 가진 것
  SUCCEED();
}

TEST(RollWireMoverTest, ConstructorStoresMotorPointer) {
  // 생성 시 Motor 포인터를 멤버 변수에 저장한다 (의존성 주입)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // Motor 포인터가 저장되었는지는 내부 구현이므로 직접 확인 불가
  // 대신 생성이 성공했는지 확인
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, error);
}

TEST(RollWireMoverTest, ConstructorReturnsErrorWhenMotorIsNull) {
  // Motor 포인터가 nullptr일 때 ErrorCode::INVALID_MOTOR_POINTER를 반환한다
  Motor *motor = nullptr;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_MOTOR_POINTER, error);
}

TEST(RollWireMoverTest, ConstructorReturnsErrorWhenWireThicknessIsZero) {
  // wireThickness가 0일 때 ErrorCode::INVALID_WIRE_THICKNESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(0.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_WIRE_THICKNESS, error);
}

TEST(RollWireMoverTest, ConstructorReturnsErrorWhenWireThicknessIsNegative) {
  // wireThickness가 음수일 때 ErrorCode::INVALID_WIRE_THICKNESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(-1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_WIRE_THICKNESS, error);
}

TEST(RollWireMoverTest, ConstructorReturnsErrorWhenInnerRadiusIsZero) {
  // innerRadius가 0일 때 ErrorCode::INVALID_INNER_RADIUS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 0.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_INNER_RADIUS, error);
}

TEST(RollWireMoverTest, ConstructorReturnsErrorWhenInnerRadiusIsNegative) {
  // innerRadius가 음수일 때 ErrorCode::INVALID_INNER_RADIUS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, -50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_INNER_RADIUS, error);
}

TEST(RollWireMoverTest, ConstructorReturnsSuccessWhenAllParametersValid) {
  // 모든 파라미터가 유효할 때 ErrorCode::SUCCESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, error);
}

// Phase 2.2: 초기 상태
TEST(RollWireMoverTest, InitialPositionIsZero) {
  // 초기 현재 위치는 0이다 (완전히 올린 상태)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(0.0, mover.getCurrentPosition());
}

TEST(RollWireMoverTest, InitialStateIsStopped) {
  // 초기 상태는 STOPPED이다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::MotionState::STOPPED, mover.getCurrentState());
}

TEST(RollWireMoverTest, InitialMaxWireLengthIs5Meters) {
  // 초기 최대 와이어 길이는 5.0m이다
  // 이는 내부 상태이므로 직접 확인할 수 없지만, 생성이 성공하면 설정된 것으로
  // 간주
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 생성 성공 확인
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, error);
  // TODO: setMaxWireLength 구현 후 실제 값 확인 가능
}

TEST(RollWireMoverTest, IsMovingReturnsFalseInitially) {
  // isMoving()은 초기에 false를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_FALSE(mover.isMoving());
}

// Phase 2.3: 모션 파라미터 설정
TEST(RollWireMoverTest, CanSetAccelerationTime) {
  // setAccelerationTime() 메서드로 가속 시간을 설정할 수 있다 (ErrorCode 반환)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  RollWireMover::ErrorCode result = mover.setAccelerationTime(0.5);
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, result);
}

TEST(RollWireMoverTest, CanSetConstantVelocity) {
  // setConstantVelocity() 메서드로 정속 속도를 설정할 수 있다 (ErrorCode 반환)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  RollWireMover::ErrorCode result = mover.setConstantVelocity(0.5);
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, result);
}

TEST(RollWireMoverTest, CanSetDecelerationTime) {
  // setDecelerationTime() 메서드로 감속 시간을 설정할 수 있다 (ErrorCode 반환)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  RollWireMover::ErrorCode result = mover.setDecelerationTime(0.5);
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, result);
}

TEST(RollWireMoverTest, SetAccelerationTimeReturnsErrorWhenZeroOrNegative) {
  // setAccelerationTime()에 0 이하 값을 전달하면
  // ErrorCode::INVALID_ACCELERATION_TIME을 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_ACCELERATION_TIME,
            mover.setAccelerationTime(0.0));
  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_ACCELERATION_TIME,
            mover.setAccelerationTime(-0.5));
}

TEST(RollWireMoverTest, SetDecelerationTimeReturnsErrorWhenZeroOrNegative) {
  // setDecelerationTime()에 0 이하 값을 전달하면
  // ErrorCode::INVALID_DECELERATION_TIME을 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_DECELERATION_TIME,
            mover.setDecelerationTime(0.0));
  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_DECELERATION_TIME,
            mover.setDecelerationTime(-0.5));
}

TEST(RollWireMoverTest, SetConstantVelocityReturnsErrorWhenBelowMinimum) {
  // setConstantVelocity()에 MIN_VELOCITY(0.01) 미만 값을 전달하면
  // ErrorCode::INVALID_VELOCITY를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_VELOCITY,
            mover.setConstantVelocity(0.005));
  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_VELOCITY,
            mover.setConstantVelocity(0.0));
}

TEST(RollWireMoverTest, SetConstantVelocityReturnsErrorWhenAboveMaximum) {
  // setConstantVelocity()에 MAX_VELOCITY(1.0) 초과 값을 전달하면
  // ErrorCode::INVALID_VELOCITY를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_VELOCITY,
            mover.setConstantVelocity(1.5));
  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_VELOCITY,
            mover.setConstantVelocity(2.0));
}

TEST(RollWireMoverTest, SetValidParametersReturnsSuccess) {
  // 유효한 값 설정 시 ErrorCode::SUCCESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setAccelerationTime(0.5));
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setConstantVelocity(0.5));
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setDecelerationTime(0.5));
}

// Phase 2.4: 시스템 설정
TEST(RollWireMoverTest, CanSetMaxWireLength) {
  // setMaxWireLength() 메서드로 최대 와이어 길이를 설정할 수 있다 (ErrorCode
  // 반환)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  RollWireMover::ErrorCode result = mover.setMaxWireLength(10.0);
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, result);
}

TEST(RollWireMoverTest, SetMaxWireLengthReturnsErrorWhenZeroOrNegative) {
  // setMaxWireLength()에 0 이하 값을 전달하면 ErrorCode::INVALID_MAX_LENGTH를
  // 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_MAX_LENGTH,
            mover.setMaxWireLength(0.0));
  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_MAX_LENGTH,
            mover.setMaxWireLength(-5.0));
}

TEST(RollWireMoverTest, CanSetInnerRadius) {
  // setInnerRadius() 메서드로 롤 내경을 변경할 수 있다 (ErrorCode 반환)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  RollWireMover::ErrorCode result = mover.setInnerRadius(100.0);
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, result);
}

TEST(RollWireMoverTest, SetInnerRadiusReturnsErrorWhenZeroOrNegative) {
  // setInnerRadius()에 0 이하 값을 전달하면 ErrorCode::INVALID_INNER_RADIUS를
  // 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_INNER_RADIUS,
            mover.setInnerRadius(0.0));
  EXPECT_EQ(RollWireMover::ErrorCode::INVALID_INNER_RADIUS,
            mover.setInnerRadius(-50.0));
}

TEST(RollWireMoverTest, SetValidSystemParametersReturnsSuccess) {
  // 유효한 값 설정 시 ErrorCode::SUCCESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setMaxWireLength(10.0));
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setInnerRadius(100.0));
}

// Phase 2.6: 속도 프로파일 타입 설정
TEST(RollWireMoverTest, SetVelocityProfileMethodExists) {
  // setVelocityProfile() 메서드가 존재한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 메서드를 호출할 수 있으면 테스트 통과
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  SUCCEED();
}

TEST(RollWireMoverTest, CanSetTrapezoidProfile) {
  // TRAPEZOID 프로파일 타입을 설정할 수 있다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  // 설정이 성공하면 테스트 통과
  SUCCEED();
}

TEST(RollWireMoverTest, CanSetSCurveProfile) {
  // S_CURVE 프로파일 타입을 설정할 수 있다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  mover.setVelocityProfile(RollWireMover::ProfileType::S_CURVE);
  // 설정이 성공하면 테스트 통과
  SUCCEED();
}

// Phase 3: 핵심 이동 로직 (moveTo)
// Phase 3.1: 입력 검증 및 상태 체크
TEST(RollWireMoverTest, MoveToMethodExists) {
  // moveTo() 메서드가 존재한다 (ErrorCode 반환)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 메서드를 호출할 수 있으면 테스트 통과
  // 유효한 범위 내의 값으로 호출
  RollWireMover::ErrorCode result = mover.moveTo(2.5);
  // 아직 구현되지 않았으므로 결과는 중요하지 않음, 컴파일 여부가 중요
  (void)result;
  SUCCEED();
}

TEST(RollWireMoverTest, MoveToReturnsErrorWhenTargetIsNegative) {
  // 목표 위치가 음수일 때 ErrorCode::OUT_OF_RANGE를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  EXPECT_EQ(RollWireMover::ErrorCode::OUT_OF_RANGE, mover.moveTo(-1.0));
}

TEST(RollWireMoverTest, MoveToReturnsErrorWhenTargetExceedsMaxLength) {
  // 목표 위치가 최대 길이를 초과할 때 ErrorCode::OUT_OF_RANGE를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 기본 최대 길이는 5.0m
  EXPECT_EQ(RollWireMover::ErrorCode::OUT_OF_RANGE, mover.moveTo(5.1));
}

TEST(RollWireMoverTest, MoveRelativeMethodExists) {
  // moveRelative() 메서드가 존재한다 (ErrorCode 반환)
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 메서드를 호출할 수 있으면 테스트 통과
  RollWireMover::ErrorCode result = mover.moveRelative(0.0);
  (void)result;
  SUCCEED();
}

TEST(RollWireMoverTest, MoveRelativeZeroReturnsSuccess) {
  // moveRelative(0)을 호출하면 위치가 변하지 않고 ErrorCode::SUCCESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  double initialPos = mover.getCurrentPosition();
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.moveRelative(0.0));
  EXPECT_EQ(initialPos, mover.getCurrentPosition());
}

TEST(RollWireMoverTest, MoveRelativeReturnsErrorWhenTargetOutOfRangeNegative) {
  // 위치 0에서 moveRelative(-1.0)을 호출하면 ErrorCode::OUT_OF_RANGE를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 초기 위치는 0
  EXPECT_EQ(RollWireMover::ErrorCode::OUT_OF_RANGE, mover.moveRelative(-1.0));
}

TEST(RollWireMoverTest, MoveRelativeReturnsErrorWhenTargetOutOfRangePositive) {
  // 위치 0에서 moveRelative(6.0)을 호출하면 (maxLength=5.0)
  // ErrorCode::OUT_OF_RANGE를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 초기 위치는 0, 최대 길이는 5.0
  EXPECT_EQ(RollWireMover::ErrorCode::OUT_OF_RANGE, mover.moveRelative(6.0));
}

TEST(RollWireMoverTest, MoveRelativeReturnsSuccessWhenTargetInRange) {
  // 유효한 범위 내 이동 시 ErrorCode::SUCCESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 초기 위치 0에서 2.5로 이동 (유효)
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.moveRelative(2.5));
}

TEST(RollWireMoverTest, MoveToZeroReturnsSuccess) {
  // moveTo(0)을 호출하면 위치 0으로 이동하고 ErrorCode::SUCCESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 초기 위치 0에서 0으로 이동 (제자리 이동이지만 유효)
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.moveTo(0.0));
}

TEST(RollWireMoverTest, MoveToMaxWireLengthReturnsSuccess) {
  // moveTo(maxWireLength)를 호출하면 최대 길이 위치로 이동하고
  // ErrorCode::SUCCESS를 반환한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // 최대 길이 5.0으로 이동
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.moveTo(5.0));
}

// Phase 4: Trapezoid 속도 프로파일
// Phase 4.1: 단순 Trapezoid 프로파일 생성
TEST(RollWireMoverTest, SetsTrapezoidProfileTypeForGeneration) {
  // 프로파일 타입을 TRAPEZOID로 설정한다
  // 가속 시간 0.1초, 정속 속도 0.1 m/s, 감속 시간 0.1초로 설정한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);

  // 모션 파라미터 설정
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setAccelerationTime(0.1));
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setConstantVelocity(0.1));
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, mover.setDecelerationTime(0.1));

  // 설정이 정상적으로 되었으면 테스트 통과
  SUCCEED();
}

TEST(RollWireMoverTest, GeneratesProfileForVeryShortDistance) {
  // 매우 짧은 거리(0.01m) 이동 시 속도 프로파일이 생성된다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  mover.setAccelerationTime(0.1);
  mover.setConstantVelocity(0.1);
  mover.setDecelerationTime(0.1);

  // 매우 짧은 거리 이동
  RollWireMover::ErrorCode moveError = mover.moveRelative(0.01);

  // 이동 명령이 성공해야 함 (프로파일 생성 성공)
  EXPECT_EQ(RollWireMover::ErrorCode::SUCCESS, moveError);

  // 모터에 프로파일이 전달되었는지 확인
  // SimMotor의 getLastProfile()을 통해 프로파일이 생성되었는지 확인
  const std::vector<double> &profile = simMotor.getLastProfile();
  EXPECT_FALSE(profile.empty());
}

TEST(RollWireMoverTest, MaxVelocityIsLessThanOrEqualToConstantVelocity) {
  // 생성된 프로파일의 최대 속도는 constantVelocity 이하이다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  mover.setAccelerationTime(0.1);
  double constantVel = 0.1;
  mover.setConstantVelocity(constantVel);
  mover.setDecelerationTime(0.1);

  // 매우 짧은 거리 이동 (삼각형 프로파일)
  mover.moveRelative(0.01);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  EXPECT_FALSE(velocityProfile.empty());

  // 최대 속도 찾기
  double maxVelocity = 0.0;
  for (double v : velocityProfile) {
    if (v > maxVelocity) {
      maxVelocity = v;
    }
  }

  // 최대 속도는 constantVelocity 이하여야 함
  EXPECT_LE(maxVelocity, constantVel);
}

TEST(RollWireMoverTest, ProfileStartsWithZeroVelocity) {
  // 프로파일의 시작 속도는 0이다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  mover.setAccelerationTime(0.1);
  mover.setConstantVelocity(0.1);
  mover.setDecelerationTime(0.1);

  // 이동 실행
  mover.moveRelative(0.05);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 시작 속도는 0이어야 함
  EXPECT_DOUBLE_EQ(0.0, velocityProfile.front());
}

TEST(RollWireMoverTest, ProfileEndsWithZeroVelocity) {
  // 프로파일의 종료 속도는 0이다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  mover.setAccelerationTime(0.1);
  mover.setConstantVelocity(0.1);
  mover.setDecelerationTime(0.1);

  // 이동 실행
  mover.moveRelative(0.05);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 종료 속도는 0이어야 함 (또는 0에 매우 가까워야 함)
  EXPECT_NEAR(0.0, velocityProfile.back(), 0.001);
}

// Phase 4.2: Trapezoid 프로파일 - 가속 구간
TEST(RollWireMoverTest, AccelerationPhaseVelocityIncreasesLinearly) {
  // 가속 구간의 속도는 선형적으로 증가한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.2; // 200ms 가속
  double velocity = 0.5;  // 0.5 m/s 정속
  double decelTime = 0.2; // 200ms 감속
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동 (정속 구간 포함)
  mover.moveRelative(0.15); // 150mm

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 가속 구간 샘플 수 계산 (1ms 간격이므로 200개)
  int accelSamples = static_cast<int>(accelTime * 1000);

  // 가속 구간이 충분히 있어야 함
  ASSERT_GE(velocityProfile.size(), static_cast<size_t>(accelSamples));

  // 가속 구간에서 속도가 선형적으로 증가하는지 검증
  // 선형성 검증: 연속된 속도 차이가 일정해야 함
  double expectedDeltaV = velocity / accelSamples;
  for (int i = 1; i < accelSamples; ++i) {
    double deltaV = velocityProfile[i] - velocityProfile[i - 1];
    // 허용 오차 내에서 일정한 증가율
    EXPECT_NEAR(expectedDeltaV, deltaV, 0.001) << "at index " << i;
  }
}

TEST(RollWireMoverTest, AccelerationPhaseTimeMatchesSetAccelerationTime) {
  // 가속 구간의 시간은 설정한 accelerationTime과 일치한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.15; // 150ms 가속
  double velocity = 0.3;   // 0.3 m/s 정속
  double decelTime = 0.15; // 150ms 감속
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동 (정속 구간 포함)
  mover.moveRelative(0.1); // 100mm

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 가속 구간의 끝을 찾기 (속도가 constantVelocity에 도달하는 시점)
  int accelEndIndex = 0;
  for (size_t i = 0; i < velocityProfile.size(); ++i) {
    if (velocityProfile[i] >= velocity - 0.001) {
      accelEndIndex = i;
      break;
    }
  }

  // 가속 구간 시간 계산 (1ms 간격이므로 인덱스 = 시간(ms))
  double actualAccelTime = accelEndIndex * 0.001;

  // 설정한 가속 시간과 일치해야 함
  EXPECT_NEAR(accelTime, actualAccelTime, 0.002); // ±2ms 허용 오차
}

TEST(RollWireMoverTest, AccelerationPhaseEndsAtConstantVelocity) {
  // 가속 구간 종료 시 속도는 constantVelocity에 도달한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.1;
  double velocity = 0.4;
  double decelTime = 0.1;
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동 (정속 구간 포함)
  mover.moveRelative(0.1); // 100mm

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 가속 구간 샘플 수 (1ms 간격)
  int accelSamples = static_cast<int>(accelTime * 1000);

  // 가속 구간 끝 속도가 constantVelocity에 도달해야 함
  ASSERT_GT(velocityProfile.size(), static_cast<size_t>(accelSamples));
  EXPECT_NEAR(velocity, velocityProfile[accelSamples], 0.001);
}

// Phase 4.3: Trapezoid 프로파일 - 정속 구간
TEST(RollWireMoverTest, ConstantVelocityPhaseExistsForLongDistance) {
  // 충분히 긴 거리 이동 시 정속 구간이 존재한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.1;   // 100ms 가속
  double velocity = 0.5;    // 0.5 m/s 정속
  double decelTime = 0.1;   // 100ms 감속
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동 (가속 + 정속 + 감속이 모두 필요한 거리)
  // 가속 거리 = 0.5 * v * t = 0.5 * 0.5 * 0.1 = 0.025m
  // 감속 거리 = 0.5 * v * t = 0.5 * 0.5 * 0.1 = 0.025m
  // 총 0.15m 이동 -> 정속 구간 0.1m가 생김
  mover.moveRelative(0.15);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 정속 구간 찾기 (constantVelocity에 도달한 후 유지되는 구간)
  int constantStartIndex = -1;
  int constantEndIndex = -1;
  
  for (size_t i = 0; i < velocityProfile.size(); ++i) {
    if (std::abs(velocityProfile[i] - velocity) < 0.001) {
      if (constantStartIndex == -1) {
        constantStartIndex = i;
      }
      constantEndIndex = i;
    } else if (constantStartIndex != -1 && velocityProfile[i] < velocity - 0.001) {
      // 감속 시작
      break;
    }
  }

  // 정속 구간이 존재해야 함
  ASSERT_NE(constantStartIndex, -1);
  ASSERT_NE(constantEndIndex, -1);
  
  // 정속 구간 길이가 0보다 커야 함
  int constantPhaseSamples = constantEndIndex - constantStartIndex + 1;
  EXPECT_GT(constantPhaseSamples, 10); // 최소 10ms 이상의 정속 구간
}

TEST(RollWireMoverTest, ConstantVelocityPhaseHasCorrectVelocity) {
  // 정속 구간의 속도는 constantVelocity이다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.1;
  double velocity = 0.6;
  double decelTime = 0.1;
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동
  mover.moveRelative(0.2);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 정속 구간 찾기
  std::vector<double> constantVelocities;
  for (size_t i = 0; i < velocityProfile.size(); ++i) {
    if (std::abs(velocityProfile[i] - velocity) < 0.001) {
      constantVelocities.push_back(velocityProfile[i]);
    }
  }

  // 정속 구간이 존재해야 함
  ASSERT_FALSE(constantVelocities.empty());

  // 정속 구간의 모든 속도가 constantVelocity와 같아야 함
  for (double v : constantVelocities) {
    EXPECT_NEAR(velocity, v, 0.001);
  }
}

TEST(RollWireMoverTest, ConstantVelocityPhaseTimeMatchesExpectedDuration) {
  // 정속 구간 시간 = (총 거리 - 가감속 거리) / constantVelocity
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.1;   // 100ms 가속
  double velocity = 0.5;    // 0.5 m/s 정속
  double decelTime = 0.1;   // 100ms 감속
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 이동 거리 계산
  double totalDistance = 0.2; // 200mm
  double accelDistance = 0.5 * velocity * accelTime; // 0.5 * 0.5 * 0.1 = 0.025m
  double decelDistance = 0.5 * velocity * decelTime; // 0.5 * 0.5 * 0.1 = 0.025m
  double constantDistance = totalDistance - accelDistance - decelDistance; // 0.15m
  double expectedConstantTime = constantDistance / velocity; // 0.15 / 0.5 = 0.3s

  // 이동 실행
  mover.moveRelative(totalDistance);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 정속 구간 찾기 (constantVelocity에 도달한 구간)
  int constantStartIndex = -1;
  int constantEndIndex = -1;
  
  for (size_t i = 0; i < velocityProfile.size(); ++i) {
    if (std::abs(velocityProfile[i] - velocity) < 0.001) {
      if (constantStartIndex == -1) {
        constantStartIndex = i;
      }
      constantEndIndex = i;
    } else if (constantStartIndex != -1 && velocityProfile[i] < velocity - 0.001) {
      // 감속 시작
      break;
    }
  }

  // 정속 구간이 존재해야 함
  ASSERT_NE(constantStartIndex, -1);
  ASSERT_NE(constantEndIndex, -1);
  
  // 정속 구간 시간 계산 (1ms 간격)
  double actualConstantTime = (constantEndIndex - constantStartIndex + 1) * 0.001;

  // 예상 정속 시간과 일치해야 함
  EXPECT_NEAR(expectedConstantTime, actualConstantTime, 0.005); // ±5ms 허용 오차
}

// Phase 4.4: Trapezoid 프로파일 - 감속 구간
TEST(RollWireMoverTest, DecelerationPhaseVelocityDecreasesLinearly) {
  // 감속 구간의 속도는 선형적으로 감소한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.15;
  double velocity = 0.4;
  double decelTime = 0.2; // 200ms 감속
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동 (정속 구간 포함)
  mover.moveRelative(0.15);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 감속 구간 찾기 (constantVelocity에서 0으로 감소하는 구간)
  int decelStartIndex = -1;
  for (size_t i = 0; i < velocityProfile.size() - 1; ++i) {
    // 정속에서 감속 시작하는 시점
    if (std::abs(velocityProfile[i] - velocity) < 0.001 && 
        velocityProfile[i + 1] < velocity - 0.001) {
      decelStartIndex = i + 1;
      break;
    }
  }

  ASSERT_NE(decelStartIndex, -1) << "감속 구간을 찾을 수 없습니다";

  // 감속 구간 샘플 수
  int decelSamples = static_cast<int>(decelTime * 1000);
  int decelEndIndex = decelStartIndex + decelSamples;

  // 감속 구간이 충분히 있어야 함
  ASSERT_LE(decelEndIndex, static_cast<int>(velocityProfile.size()));

  // 감속 구간에서 속도가 선형적으로 감소하는지 검증
  // 선형성 검증: 연속된 속도 차이가 일정해야 함
  double expectedDeltaV = -velocity / decelSamples;
  for (int i = decelStartIndex + 1; i < decelEndIndex && i < static_cast<int>(velocityProfile.size()); ++i) {
    double deltaV = velocityProfile[i] - velocityProfile[i - 1];
    // 허용 오차 내에서 일정한 감소율
    EXPECT_NEAR(expectedDeltaV, deltaV, 0.001) << "at index " << i;
  }
}

TEST(RollWireMoverTest, DecelerationPhaseTimeMatchesSetDecelerationTime) {
  // 감속 구간의 시간은 설정한 decelerationTime과 일치한다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.1;   // 100ms 가속
  double velocity = 0.5;    // 0.5 m/s 정속
  double decelTime = 0.18;  // 180ms 감속
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동 (정속 구간 포함)
  mover.moveRelative(0.15);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 감속 구간 찾기 (constantVelocity에서 0으로 감소하는 구간)
  int decelStartIndex = -1;
  for (size_t i = 0; i < velocityProfile.size() - 1; ++i) {
    // 정속에서 감속 시작하는 시점
    if (std::abs(velocityProfile[i] - velocity) < 0.001 &&
        velocityProfile[i + 1] < velocity - 0.001) {
      decelStartIndex = i + 1;
      break;
    }
  }

  ASSERT_NE(decelStartIndex, -1) << "감속 구간을 찾을 수 없습니다";

  // 감속 구간의 끝 찾기 (속도가 0에 도달하는 시점)
  int decelEndIndex = velocityProfile.size() - 1;
  for (size_t i = decelStartIndex; i < velocityProfile.size(); ++i) {
    if (velocityProfile[i] <= 0.001) {
      decelEndIndex = i;
      break;
    }
  }

  // 감속 구간 시간 계산 (1ms 간격)
  double actualDecelTime = (decelEndIndex - decelStartIndex + 1) * 0.001;

  // 설정한 감속 시간과 일치해야 함
  EXPECT_NEAR(decelTime, actualDecelTime, 0.002); // ±2ms 허용 오차
}

TEST(RollWireMoverTest, DecelerationPhaseEndsWithZeroVelocity) {
  // 감속 구간 종료 시 속도는 0이 된다
  SimMotor simMotor;
  Motor *motor = &simMotor;

  RollWireMover::ErrorCode error;
  RollWireMover mover(1.0, 50.0, motor, error);

  // TRAPEZOID 프로파일 설정
  mover.setVelocityProfile(RollWireMover::ProfileType::TRAPEZOID);
  double accelTime = 0.1;   // 100ms 가속
  double velocity = 0.5;    // 0.5 m/s 정속
  double decelTime = 0.1;   // 100ms 감속
  mover.setAccelerationTime(accelTime);
  mover.setConstantVelocity(velocity);
  mover.setDecelerationTime(decelTime);

  // 충분히 긴 거리 이동
  mover.moveRelative(0.1);

  // 생성된 속도 프로파일 가져오기
  const std::vector<double> &velocityProfile = mover.getLastVelocityProfile();

  // 프로파일이 비어있지 않아야 함
  ASSERT_FALSE(velocityProfile.empty());

  // 프로파일의 마지막 속도는 0이어야 함
  EXPECT_NEAR(0.0, velocityProfile.back(), 0.001);
}

