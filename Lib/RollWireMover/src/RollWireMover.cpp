#include "RollWireMover.h"
#include "RollWireCalculator.h"
#include <cmath>
#include <vector>

RollWireMover::RollWireMover(double wireThickness, double innerRadius,
                             Motor *motor, ErrorCode &outError)
    : calculator(nullptr), motor(motor),
      currentPosition(0.0),               // 초기 위치는 0 (완전히 올린 상태)
      currentState(MotionState::STOPPED), // 초기 상태는 STOPPED
      maxWireLength(5.0),                 // 초기 최대 와이어 길이는 5.0m
      innerRadius(innerRadius),           // 롤 내경 반지름 저장
      currentProfile(ProfileType::TRAPEZOID) { // 기본 프로파일은 TRAPEZOID

  // Motor 포인터 검증
  if (motor == nullptr) {
    outError = ErrorCode::INVALID_MOTOR_POINTER;
    return;
  }

  // wireThickness 검증
  if (wireThickness <= 0.0) {
    outError = ErrorCode::INVALID_WIRE_THICKNESS;
    return;
  }

  // innerRadius 검증
  if (innerRadius <= 0.0) {
    outError = ErrorCode::INVALID_INNER_RADIUS;
    return;
  }

  // Calculator 생성
  calculator = new RollWireCalculator(wireThickness, innerRadius);

  outError = ErrorCode::SUCCESS;
}

RollWireMover::~RollWireMover() {
  if (calculator != nullptr) {
    delete calculator;
    calculator = nullptr;
  }
}

double RollWireMover::getCurrentPosition() const { return currentPosition; }

RollWireMover::MotionState RollWireMover::getCurrentState() const {
  return currentState;
}

bool RollWireMover::isMoving() const {
  return currentState != MotionState::STOPPED;
}

RollWireMover::ErrorCode RollWireMover::setAccelerationTime(double time) {
  if (time <= 0.0) {
    return ErrorCode::INVALID_ACCELERATION_TIME;
  }
  accelerationTime = time;
  return ErrorCode::SUCCESS;
}

RollWireMover::ErrorCode RollWireMover::setConstantVelocity(double velocity) {
  if (velocity < MIN_VELOCITY || velocity > MAX_VELOCITY) {
    return ErrorCode::INVALID_VELOCITY;
  }
  constantVelocity = velocity;
  return ErrorCode::SUCCESS;
}

RollWireMover::ErrorCode RollWireMover::setDecelerationTime(double time) {
  if (time <= 0.0) {
    return ErrorCode::INVALID_DECELERATION_TIME;
  }
  decelerationTime = time;
  return ErrorCode::SUCCESS;
}

RollWireMover::ErrorCode RollWireMover::setMaxWireLength(double length) {
  if (length <= 0.0) {
    return ErrorCode::INVALID_MAX_LENGTH;
  }
  maxWireLength = length;
  return ErrorCode::SUCCESS;
}

RollWireMover::ErrorCode RollWireMover::setInnerRadius(double radius) {
  if (radius <= 0.0) {
    return ErrorCode::INVALID_INNER_RADIUS;
  }
  innerRadius = radius;
  return ErrorCode::SUCCESS;
}

void RollWireMover::setVelocityProfile(ProfileType type) {
  currentProfile = type;
}

RollWireMover::ErrorCode RollWireMover::moveTo(double targetPosition) {
  // 목표 위치 검증
  if (targetPosition < 0.0 || targetPosition > maxWireLength) {
    return ErrorCode::OUT_OF_RANGE;
  }

  // 이동 거리 계산
  double distance = targetPosition - currentPosition;

  // 이동 거리가 0이면 바로 성공
  if (std::abs(distance) < 0.000001) {
    return ErrorCode::SUCCESS;
  }

  bool isRetracting = (distance < 0); // 거리가 음수이면 감기(Retracting)

  // 속도 프로파일 생성 (절대값 거리 사용)
  std::vector<double> velocityProfile =
      generateVelocityProfile(std::abs(distance));

  // 회전량 프로파일로 변환
  std::vector<double> rotationProfile =
      convertToRotationProfile(velocityProfile, isRetracting);

  // 모터 실행
  motor->executeRotationProfile(rotationProfile);

  // 상태 업데이트 (시뮬레이션이므로 즉시 완료 처리)
  currentPosition = targetPosition;

  return ErrorCode::SUCCESS;
}

RollWireMover::ErrorCode RollWireMover::moveRelative(double distance) {
  return moveTo(currentPosition + distance);
}

std::vector<double> RollWireMover::generateVelocityProfile(double distance) {
  std::vector<double> profile;
  double dt = 0.001; // 1ms 샘플링

  // 가속 거리 = 0.5 * a * t^2 = 0.5 * (v/t_acc) * t_acc^2 = 0.5 * v * t_acc
  double accDist = 0.5 * constantVelocity * accelerationTime;
  double decDist = 0.5 * constantVelocity * decelerationTime;

  // 정속 구간이 존재하는지 확인
  if (distance >= accDist + decDist) {
    // 정속 구간 존재
    double constDist = distance - accDist - decDist;
    double constTime = constDist / constantVelocity;

    // 가속 구간 (반복 횟수 계산)
    int accSteps = static_cast<int>(accelerationTime / dt + 0.5);
    for (int i = 0; i < accSteps; i++) {
      double t = i * dt;
      double v = (t / accelerationTime) * constantVelocity;
      profile.push_back(v);
    }

    // 정속 구간 (반복 횟수 계산)
    int constSteps = static_cast<int>(constTime / dt + 0.5);
    for (int i = 0; i < constSteps; i++) {
      profile.push_back(constantVelocity);
    }

    // 감속 구간 (반복 횟수 계산)
    int decSteps = static_cast<int>(decelerationTime / dt + 0.5);
    for (int i = 0; i < decSteps; i++) {
      double t = i * dt;
      double v = constantVelocity * (1.0 - (t / decelerationTime));
      profile.push_back(v);
    }
    // 마지막 값을 명시적으로 0으로 설정
    profile.push_back(0.0);
  } else {
    // 정속 구간 없음 (삼각형 프로파일)
    double v_peak = std::sqrt((2 * distance * constantVelocity) /
                              (accelerationTime + decelerationTime));

    double t_acc_actual = accelerationTime * (v_peak / constantVelocity);
    double t_dec_actual = decelerationTime * (v_peak / constantVelocity);

    // 가속 구간 (반복 횟수 계산)
    int accSteps = static_cast<int>(t_acc_actual / dt + 0.5);
    for (int i = 0; i < accSteps; i++) {
      double t = i * dt;
      double v = (t / t_acc_actual) * v_peak;
      profile.push_back(v);
    }

    // 감속 구간 (반복 횟수 계산)
    int decSteps = static_cast<int>(t_dec_actual / dt + 0.5);
    for (int i = 0; i < decSteps; i++) {
      double t = i * dt;
      double v = v_peak * (1.0 - (t / t_dec_actual));
      profile.push_back(v);
    }
    // 마지막 값을 명시적으로 0으로 설정
    profile.push_back(0.0);
  }

  // 테스트용: 마지막 프로파일 저장
  lastVelocityProfile = profile;

  return profile;
}

std::vector<double> RollWireMover::convertToRotationProfile(
    const std::vector<double> &velocityProfile, bool isRetracting) {
  std::vector<double> rotationProfile;
  double currentRotation = motor->getCurrentRotation();
  double dt = 0.001;

  for (double v : velocityProfile) {
    double ds = v * dt; // 이동 거리

    // 거리를 회전량으로 변환 (Calculator 사용)
    double dRot = calculator->calculateRotation(ds);

    if (isRetracting) {
      currentRotation -= dRot;
    } else {
      currentRotation += dRot;
    }

    rotationProfile.push_back(currentRotation);
  }

  return rotationProfile;
}

const std::vector<double> &RollWireMover::getLastVelocityProfile() const {
  return lastVelocityProfile;
}
