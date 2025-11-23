#include "SimMotor.h"

SimMotor::SimMotor() : currentRotation(0.0), running(false), currentIndex(0) {}

void SimMotor::executeRotationProfile(const std::vector<double> &rotations) {
  // 빈 배열이면 아무것도 하지 않음
  if (rotations.empty()) {
    return;
  }

  // 프로파일 저장 및 실행 시작
  profile = rotations;
  running = true; // 실행 중 상태로 설정

  // 프로파일 실행 (현재는 동기적으로 즉시 완료)
  // TODO: 향후 비동기 또는 단계별 실행으로 변경 필요
  currentRotation = rotations.back();

  // 실행 완료 후 정지 상태로 변경
  running = false;
}

void SimMotor::stop() {
  // 실행 중단 - 현재 위치는 유지
  running = false;
}

double SimMotor::getCurrentRotation() const { return currentRotation; }

bool SimMotor::isRunning() const { return running; }

void SimMotor::resetPosition() { currentRotation = 0.0; }

void SimMotor::loadProfile(const std::vector<double> &rotations) {
  // 프로파일을 로드하지만 실행하지는 않음
  profile = rotations;
  currentIndex = 0;
}

void SimMotor::startExecution() {
  // 실행 시작
  if (!profile.empty()) {
    running = true;
    currentIndex = 0;
  }
}

void SimMotor::step() {
  // 한 스텝 실행
  if (!running || currentIndex >= profile.size()) {
    return;
  }

  // 현재 인덱스의 회전량 적용
  currentRotation = profile[currentIndex];
  currentIndex++;

  // 마지막 스텝이었다면 실행 종료
  if (currentIndex >= profile.size()) {
    running = false;
  }
}

const std::vector<double> &SimMotor::getLastProfile() const { return profile; }
