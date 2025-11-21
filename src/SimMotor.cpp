#include "SimMotor.h"

SimMotor::SimMotor()
    : currentRotation(0.0), running(false), currentIndex(0) {
}

void SimMotor::executeRotationProfile(const std::vector<double>& rotations) {
    // 최소 구현 - 추후 완성 예정
    profile = rotations;
    running = true;
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
