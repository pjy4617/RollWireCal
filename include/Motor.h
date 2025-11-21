#ifndef MOTOR_H
#define MOTOR_H

#include <vector>

/**
 * @brief Motor 인터페이스 (순수 가상 클래스)
 *
 * 모터 제어를 위한 추상 인터페이스입니다.
 * 실제 모터 구현체는 이 인터페이스를 상속받아 구현해야 합니다.
 * 예: SimMotor (시뮬레이션용), RealMotor (실제 하드웨어용)
 */
class Motor {
public:
    // 가상 소멸자
    virtual ~Motor() = default;

    // 모션 실행 (순수 가상 함수)
    virtual void executeRotationProfile(const std::vector<double>& rotations) = 0;
    virtual void stop() = 0;

    // 상태 조회 (순수 가상 함수)
    virtual double getCurrentRotation() const = 0;
    virtual bool isRunning() const = 0;

    // 리셋 (순수 가상 함수)
    virtual void resetPosition() = 0;
};

#endif // MOTOR_H
