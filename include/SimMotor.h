#ifndef SIMMOTOR_H
#define SIMMOTOR_H

#include "Motor.h"
#include <cstddef>

/**
 * @brief SimMotor 클래스 - Motor 인터페이스의 시뮬레이션 구현체
 *
 * 실제 모터처럼 동작하는 시뮬레이션 클래스입니다.
 * 테스트 및 개발 단계에서 사용되며, 추후 실제 모터 드라이버로 교체 가능합니다.
 */
class SimMotor : public Motor {
public:
    // 생성자
    SimMotor();

    // Motor 인터페이스 구현
    void executeRotationProfile(const std::vector<double>& rotations) override;
    void stop() override;
    double getCurrentRotation() const override;
    bool isRunning() const override;
    void resetPosition() override;

private:
    double currentRotation;                 // 현재 회전 각도 (도)
    bool running;                           // 동작 상태
    std::vector<double> profile;            // 실행 중인 프로파일
    size_t currentIndex;                    // 현재 실행 인덱스
};

#endif // SIMMOTOR_H
