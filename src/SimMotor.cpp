#include "SimMotor.h"

SimMotor::SimMotor()
    : currentRotation(0.0), running(false), currentIndex(0) {
}

void SimMotor::executeRotationProfile(const std::vector<double>& rotations) {
    // 빈 배열이면 아무것도 하지 않음
    if (rotations.empty()) {
        return;
    }

    // 프로파일 저장 및 마지막 회전 위치로 이동
    // 현재는 즉시 완료되는 단순 구현
    profile = rotations;
    currentRotation = rotations.back();
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
