#include "SimMotor.h"

SimMotor::SimMotor()
    : currentRotation(0.0), running(false), currentIndex(0) {
}

void SimMotor::executeRotationProfile(const std::vector<double>& rotations) {
    // 빈 배열이면 아무것도 하지 않음
    if (rotations.empty()) {
        return;
    }

    // 프로파일 저장 및 실행 시작
    profile = rotations;
    running = true;  // 실행 중 상태로 설정
    
    // 프로파일 실행 (현재는 동기적으로 즉시 완료)
    // TODO: 향후 비동기 또는 단계별 실행으로 변경 필요
    currentRotation = rotations.back();
    
    // 실행 완료 후 정지 상태로 변경
    running = false;
}

void SimMotor::stop() {
    // 최소 구현 - 추후 완성 예정
    running = false;
}

double SimMotor::getCurrentRotation() const {
    return currentRotation;
}

bool SimMotor::isRunning() const {
    return running;
}

void SimMotor::resetPosition() {
    currentRotation = 0.0;
}
