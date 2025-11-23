#ifndef ROLLWIREMOVER_H
#define ROLLWIREMOVER_H

#include "Motor.h"

// RollWireCalculator 전방 선언 (실제 구현은 나중에)
class RollWireCalculator;

/**
 * @brief RollWireMover 클래스
 *
 * 롤에 감긴 와이어의 이동을 제어하는 클래스입니다.
 */
class RollWireMover {
public:
  // 에러 코드
  enum class ErrorCode {
    SUCCESS = 0,
    INVALID_MOTOR_POINTER,
    INVALID_WIRE_THICKNESS,
    INVALID_INNER_RADIUS,
    INVALID_ACCELERATION_TIME,
    INVALID_DECELERATION_TIME,
    INVALID_VELOCITY,
    INVALID_MAX_LENGTH,
    OUT_OF_RANGE,
    MOTOR_BUSY
  };

  // 생성자
  RollWireMover(double wireThickness, double innerRadius, Motor *motor,
                ErrorCode &outError);
  ~RollWireMover(); // 소멸자 추가

  // 모션 상태
  enum class MotionState {
    STOPPED,           // 정지
    ACCELERATING,      // 가속 중
    CONSTANT_VELOCITY, // 정속 중
    DECELERATING       // 감속 중
  };

  // 속도 프로파일 타입
  enum class ProfileType {
    TRAPEZOID, // 사다리꼴 프로파일
    S_CURVE    // S자 곡선 프로파일
  };

  // 상태 조회
  double getCurrentPosition() const;   // 현재 위치 조회 (m)
  MotionState getCurrentState() const; // 현재 상태 조회
  bool isMoving() const;               // 이동 중 여부

  // 모션 파라미터 설정
  ErrorCode setAccelerationTime(double time);     // 가속 시간 설정 (초)
  ErrorCode setConstantVelocity(double velocity); // 정속 속도 설정 (m/s)
  ErrorCode setDecelerationTime(double time);     // 감속 시간 설정 (초)

  // 시스템 설정
  ErrorCode setMaxWireLength(double length); // 최대 와이어 길이 설정 (m)
  ErrorCode setInnerRadius(double radius);   // 롤 내경 변경 (mm)

  // 속도 프로파일 설정
  void setVelocityProfile(ProfileType type); // 속도 프로파일 타입 설정

  // 이동 명령
  ErrorCode moveTo(double targetPosition); // 목표 위치로 이동 (m)
  ErrorCode moveRelative(double distance); // 상대 거리 이동 (m)

  // 테스트용 메서드
  const std::vector<double> &getLastVelocityProfile() const;

private:
  RollWireCalculator *calculator; // 길이-회전량 변환기
  Motor *motor;                   // 모터 제어 객체 (의존성 주입)

  // 상태 변수
  double currentPosition;   // 현재 와이어 위치 (m, 0 = 완전히 올림)
  MotionState currentState; // 현재 모션 상태
  double maxWireLength;     // 최대 와이어 길이 (m, 기본값 5.0)

  // 모션 파라미터
  double accelerationTime; // 가속 시간 (초)
  double constantVelocity; // 정속 속도 (m/s)
  double decelerationTime; // 감속 시간 (초)

  // 시스템 파라미터
  double innerRadius;         // 롤 내경 반지름 (mm)
  ProfileType currentProfile; // 현재 속도 프로파일 타입

  // 테스트용 변수
  std::vector<double> lastVelocityProfile; // 마지막으로 생성된 속도 프로파일

  // 속도 제한 상수
  static constexpr double MAX_VELOCITY = 1.0;  // 최대 속도 (m/s)
  static constexpr double MIN_VELOCITY = 0.01; // 최소 속도 (m/s)

  // 내부 헬퍼 메서드
  std::vector<double> generateVelocityProfile(double distance);
  std::vector<double>
  convertToRotationProfile(const std::vector<double> &velocityProfile,
                           bool isRetracting);
};

#endif // ROLLWIREMOVER_H
