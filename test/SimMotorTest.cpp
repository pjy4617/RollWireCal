#include <gtest/gtest.h>
#include "SimMotor.h"

// Phase 1.2: SimMotor 기본 구조 (Motor 구현체)
TEST(SimMotorTest, CanCreateSimMotor) {
    // SimMotor 클래스를 생성할 수 있다
    SimMotor simMotor;

    // 객체가 생성되면 테스트 통과
    SUCCEED();
}

TEST(SimMotorTest, InitialRotationPositionIsZero) {
    // 기본 생성자로 SimMotor 객체를 생성하면 초기 회전 위치는 0이다
    SimMotor simMotor;

    EXPECT_DOUBLE_EQ(0.0, simMotor.getCurrentRotation());
}

TEST(SimMotorTest, InitialStateIsNotRunning) {
    // 기본 생성자로 SimMotor 객체를 생성하면 초기 상태는 정지(not running)이다
    SimMotor simMotor;

    EXPECT_FALSE(simMotor.isRunning());
}

// Phase 1.3: SimMotor 위치 조회 및 리셋
TEST(SimMotorTest, CanGetCurrentRotation) {
    // getCurrentRotation() 메서드로 현재 회전 위치를 조회할 수 있다
    SimMotor simMotor;

    // 초기 위치는 0이어야 함
    double rotation = simMotor.getCurrentRotation();
    EXPECT_DOUBLE_EQ(0.0, rotation);
}

TEST(SimMotorTest, CanCheckIfMotorIsRunning) {
    // isRunning() 메서드로 모터 동작 여부를 조회할 수 있다
    SimMotor simMotor;

    // 초기 상태는 정지 상태여야 함
    bool running = simMotor.isRunning();
    EXPECT_FALSE(running);
}

TEST(SimMotorTest, CanResetPosition) {
    // resetPosition() 메서드로 회전 위치를 0으로 초기화할 수 있다
    SimMotor simMotor;

    // 위치를 변경 (executeRotationProfile을 통해)
    std::vector<double> profile = {360.0};
    simMotor.executeRotationProfile(profile);

    // resetPosition 호출
    simMotor.resetPosition();

    // 위치가 0으로 초기화되었는지 확인
    EXPECT_DOUBLE_EQ(0.0, simMotor.getCurrentRotation());
}

TEST(SimMotorTest, GetCurrentRotationReturnsZeroAfterReset) {
    // resetPosition() 호출 후 getCurrentRotation()은 0을 반환한다
    SimMotor simMotor;

    // 위치를 변경
    std::vector<double> profile = {720.0};
    simMotor.executeRotationProfile(profile);

    // 리셋
    simMotor.resetPosition();

    // getCurrentRotation()이 0을 반환하는지 확인
    EXPECT_DOUBLE_EQ(0.0, simMotor.getCurrentRotation());
}

// Phase 1.4: SimMotor 회전량 배열 실행
TEST(SimMotorTest, ExecuteRotationProfileMethodExists) {
    // executeRotationProfile() 메서드가 존재한다
    SimMotor simMotor;

    // 메서드를 호출할 수 있으면 테스트 통과
    std::vector<double> profile = {360.0};
    simMotor.executeRotationProfile(profile);

    // 컴파일되고 실행되면 메서드가 존재하는 것
    SUCCEED();
}

TEST(SimMotorTest, ExecuteEmptyProfileDoesNothing) {
    // 빈 배열을 전달하면 아무 동작도 하지 않는다
    SimMotor simMotor;

    // 초기 상태 확인
    EXPECT_DOUBLE_EQ(0.0, simMotor.getCurrentRotation());
    EXPECT_FALSE(simMotor.isRunning());

    // 빈 배열 실행
    std::vector<double> emptyProfile;
    simMotor.executeRotationProfile(emptyProfile);

    // 상태가 변하지 않아야 함
    EXPECT_DOUBLE_EQ(0.0, simMotor.getCurrentRotation());
    EXPECT_FALSE(simMotor.isRunning());
}

TEST(SimMotorTest, ExecuteSingleValueProfileSetsRotationTo360) {
    // 단일 값 배열 [360.0]을 전달하면 현재 회전 위치가 360도가 된다
    SimMotor simMotor;

    // 단일 값 배열 실행
    std::vector<double> profile = {360.0};
    simMotor.executeRotationProfile(profile);

    // 현재 회전 위치가 360도가 되어야 함
    EXPECT_DOUBLE_EQ(360.0, simMotor.getCurrentRotation());
}

TEST(SimMotorTest, ExecuteMultiValueProfileSetsRotationToLastValue) {
    // 다중 값 배열 [90.0, 180.0, 270.0]을 전달하면 마지막 값(270도)이 현재 위치가 된다
    SimMotor simMotor;

    // 다중 값 배열 실행
    std::vector<double> profile = {90.0, 180.0, 270.0};
    simMotor.executeRotationProfile(profile);

    // 현재 회전 위치가 마지막 값인 270도가 되어야 함
    EXPECT_DOUBLE_EQ(270.0, simMotor.getCurrentRotation());
}

TEST(SimMotorTest, ProfileExecutionCompletedSetsRunningFalse) {
    // executeRotationProfile() 완료 후에는 isRunning()이 false를 반환한다
    SimMotor simMotor;

    // 프로파일 실행
    std::vector<double> profile = {180.0};
    simMotor.executeRotationProfile(profile);
    
    // 실행이 완료되었으므로 running은 false여야 함
    EXPECT_FALSE(simMotor.isRunning());
}

// Phase 1.5: 모터 정지 기능
TEST(SimMotorTest, StopMethodExists) {
    // stop() 메서드가 존재한다
    SimMotor simMotor;
    
    // stop() 메서드를 호출할 수 있어야 함
    simMotor.stop();
}

TEST(SimMotorTest, StopSetsRunningToFalse) {
    // stop() 호출 시 isRunning()이 false를 반환한다
    SimMotor simMotor;
    
    // stop() 호출
    simMotor.stop();
    
    // running이 false여야 함
    EXPECT_FALSE(simMotor.isRunning());
}
