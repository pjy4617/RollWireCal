# PRD: RollWireMover

## 1. 프로젝트 개요

RollWireMover는 롤에 감긴 와이어의 이동을 제어하는 시스템입니다. RollWireCalculator 클래스를 활용하여 와이어 길이와 롤 회전량 간의 변환을 수행하며, 정밀한 모션 프로파일을 생성하여 모터를 제어합니다.

### 1.1 프로젝트 목표
- 와이어의 정밀한 위치 제어
- 다양한 속도 프로파일 지원 (S-curve, Trapezoid)
- 실시간 모션 계산 및 실행
- 안전한 동작 범위 보장

---

## 2. 시스템 아키텍처

```
┌─────────────────────────────────────────────────────────┐
│                    RollWireMover                        │
│  ┌──────────────────────────────────────────────────┐  │
│  │  Motion Planning & Control                       │  │
│  │  - 상대/절대 이동 명령 처리                        │  │
│  │  - 속도 프로파일 생성 (S-curve/Trapezoid)          │  │
│  │  - 회전량 배열 계산 (1ms 간격)                      │  │
│  └──────────────────────────────────────────────────┘  │
│                          ↓                              │
│  ┌──────────────────────────────────────────────────┐  │
│  │       RollWireCalculator (기존 클래스)            │  │
│  │  - 길이 ↔ 회전량 변환                              │  │
│  └──────────────────────────────────────────────────┘  │
│                          ↓                              │
│  ┌──────────────────────────────────────────────────┐  │
│  │          Motor* (인터페이스 포인터)                │  │
│  │  - 의존성 주입 (Dependency Injection)              │  │
│  └──────────────────┬───────────────────────────────┘  │
└────────────────────┼────────────────────────────────────┘
                     │
        ┌────────────┴────────────┐
        │                         │
        ▼                         ▼
┌───────────────┐         ┌───────────────┐
│   SimMotor    │         │  Real Motor   │
│   (테스트용)   │         │  (실제 HW)    │
└───────────────┘         └───────────────┘
  implements Motor          implements Motor
```

**의존성 주입 패턴**: RollWireMover는 Motor 인터페이스에만 의존하며, 실제 구현체(SimMotor, 실제 모터 등)는 외부에서 주입됩니다. 이를 통해 테스트 용이성과 확장성을 확보합니다.

---

## 3. 클래스 설계

### 3.1 RollWireMover 클래스

#### 3.1.1 책임
- 와이어 이동 명령 처리 (상대/절대 이동)
- 모션 프로파일 생성 및 실행
- 안전 범위 검증
- 상태 관리 및 모니터링

#### 3.1.2 주요 멤버 변수

```cpp
class RollWireMover {
private:
    // 핵심 객체
    RollWireCalculator calculator;  // 길이-회전량 변환기
    Motor* motor;                    // 모터 제어 객체 (의존성 주입)

    // 모션 파라미터
    double accelerationTime;         // 가속 시간 (초)
    double constantVelocity;         // 정속 속도 (m/s)
    double decelerationTime;         // 감속 시간 (초)

    // 속도 프로파일 타입
    enum ProfileType {
        TRAPEZOID,                   // 사다리꼴 프로파일
        S_CURVE                      // S자 곡선 프로파일
    };
    ProfileType currentProfile;

    // 시스템 상태
    enum MotionState {
        STOPPED,                     // 정지
        ACCELERATING,                // 가속 중
        CONSTANT_VELOCITY,           // 정속 중
        DECELERATING                 // 감속 중
    };
    MotionState currentState;

    // 위치 및 제한
    double currentPosition;          // 현재 와이어 위치 (m, 0 = 완전히 올림)
    double maxWireLength;            // 최대 와이어 길이 (m, 기본값 5.0)

    // 속도 제한
    static constexpr double MAX_VELOCITY = 1.0;    // 최대 속도 (m/s)
    static constexpr double MIN_VELOCITY = 0.01;   // 최소 속도 (m/s)

    // 모션 데이터
    std::vector<double> rotationProfile;  // 1ms 간격 회전량 배열 (도)

    // 콜백
    std::function<void()> completionCallback;  // 이동 완료 콜백
};
```

#### 3.1.3 공개 인터페이스

```cpp
// 생성자 (의존성 주입)
RollWireMover(double wireThickness, double innerRadius, Motor* motor);

// 모션 파라미터 설정
void setAccelerationTime(double time);      // 가속 시간 설정 (초)
void setConstantVelocity(double velocity);  // 정속 속도 설정 (m/s)
void setDecelerationTime(double time);      // 감속 시간 설정 (초)
void setVelocityProfile(ProfileType type);  // 속도 프로파일 타입 설정

// 시스템 설정
void setMaxWireLength(double length);       // 최대 와이어 길이 설정 (m)
void setInnerRadius(double radius);         // 롤 내경 변경

// 이동 명령
void moveRelative(double distance);         // 상대 이동 (음수: 올림, 양수: 내림)
void moveAbsolute(double position);         // 절대 위치 이동 (0: 완전히 올림)
void stop();                                // 즉시 정지

// 상태 조회
double getCurrentPosition() const;          // 현재 위치 조회 (m)
MotionState getCurrentState() const;        // 현재 상태 조회
bool isMoving() const;                      // 이동 중 여부

// 콜백 등록
void setCompletionCallback(std::function<void()> callback);  // 완료 콜백 등록
```

#### 3.1.4 내부 메서드

```cpp
private:
    // 모션 프로파일 생성
    void generateTrapezoidProfile(double targetDistance);
    void generateSCurveProfile(double targetDistance);

    // 속도 → 회전량 변환
    std::vector<double> convertVelocityToRotation(
        const std::vector<double>& velocityProfile);

    // 안전성 검증
    bool validateMove(double targetPosition);

    // 모션 실행
    void executeMotion();
```

---

### 3.2 Motor 클래스 (순수 가상 클래스 / 인터페이스)

#### 3.2.1 책임
- 모터 제어 인터페이스 정의
- 실제 모터 구현의 추상화 제공
- 의존성 역전 원칙(DIP) 적용

#### 3.2.2 인터페이스

```cpp
class Motor {
public:
    // 가상 소멸자
    virtual ~Motor() = default;

    // 모션 실행 (순수 가상 함수)
    virtual void executeRotationProfile(const std::vector<double>& rotations) = 0;  // 회전량 배열 실행
    virtual void stop() = 0;                                                         // 모터 정지

    // 상태 조회 (순수 가상 함수)
    virtual double getCurrentRotation() const = 0;      // 현재 회전 위치 (도)
    virtual bool isRunning() const = 0;                 // 동작 중 여부

    // 리셋 (순수 가상 함수)
    virtual void resetPosition() = 0;                   // 위치 초기화
};
```

**설명**: Motor는 순수 가상 클래스(인터페이스)로, 실제 모터 제어를 위한 추상화를 제공합니다. 이를 통해 실제 하드웨어 모터, 시뮬레이션 모터 등 다양한 구현체로 교체 가능합니다.

---

### 3.3 SimMotor 클래스 (Motor 구현체)

#### 3.3.1 책임
- Motor 인터페이스의 구체적인 구현
- 모터 동작 시뮬레이션
- 회전량 프로파일 실행 및 위치 피드백

#### 3.3.2 인터페이스

```cpp
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
```

**설명**: SimMotor는 Motor 인터페이스를 구현하여 실제 모터처럼 동작하는 시뮬레이션 클래스입니다. 테스트 및 개발 단계에서 사용되며, 추후 실제 모터 드라이버로 교체 가능합니다.

---

## 4. 기능 요구사항

### 4.1 이동 제어

#### FR-1: 상대 이동 (moveRelative)
- **설명**: 현재 위치에서 상대적으로 이동
- **입력**: 이동 거리 (m)
  - 음수: 와이어를 올림 (롤에 감음)
  - 양수: 와이어를 내림 (롤에서 풂)
- **동작**:
  1. 목표 위치 계산: `targetPosition = currentPosition + distance`
  2. 범위 검증: `0 ≤ targetPosition ≤ maxWireLength`
  3. 속도 프로파일 생성
  4. 회전량 배열 계산
  5. 모터 실행
- **예외**: 범위 초과 시 `std::out_of_range` 예외 발생

#### FR-2: 절대 이동 (moveAbsolute)
- **설명**: 절대 위치로 이동
- **입력**: 목표 위치 (m)
  - 0: 완전히 올린 상태
  - maxWireLength: 완전히 내린 상태
- **동작**:
  1. 이동 거리 계산: `distance = position - currentPosition`
  2. `moveRelative(distance)` 호출
- **예외**: 범위 초과 시 `std::out_of_range` 예외 발생

#### FR-3: 즉시 정지 (stop)
- **설명**: 현재 동작을 즉시 중단
- **동작**:
  1. 모터 정지 명령
  2. 상태를 STOPPED로 변경
  3. 현재 위치 업데이트

---

### 4.2 속도 프로파일

#### FR-4: Trapezoid 프로파일
```
속도
 ^
 │      ┌─────────┐
 │     /│         │\
 │    / │         │ \
 │   /  │         │  \
 │  /   │         │   \
 └─────────────────────→ 시간
   가속   정속      감속
```

- **특징**: 선형 가속/감속
- **단계**:
  1. 가속: `v(t) = (constantVelocity / accelerationTime) × t`
  2. 정속: `v(t) = constantVelocity`
  3. 감속: `v(t) = constantVelocity - (constantVelocity / decelerationTime) × (t - t_decel_start)`

#### FR-5: S-Curve 프로파일
```
속도
 ^
 │      ╭─────────╮
 │    ╱ │         │ ╲
 │   ╱  │         │  ╲
 │  ╱   │         │   ╲
 │ ╱    │         │    ╲
 └─────────────────────→ 시간
   가속   정속      감속
```

- **특징**: 부드러운 가속/감속 (저크 최소화)
- **공식**: 3차 다항식 또는 사인 함수 기반
  - 가속: `v(t) = constantVelocity × (1 - cos(π × t / accelerationTime)) / 2`

---

### 4.3 모션 계산

#### FR-6: 회전량 배열 생성
- **샘플링 주기**: 1ms
- **계산 과정**:
  1. 속도 프로파일 생성 → `v[i]` (m/s, 1ms 간격)
  2. 각 시간 단계에서:
     - 이동 거리: `Δs[i] = v[i] × 0.001` (m)
     - 누적 위치: `s[i] = s[i-1] + Δs[i]`
     - 현재 롤 반지름: RollWireCalculator로 계산
     - 회전량 증분: `Δθ[i] = RollWireCalculator.calculateRotationFromLength(Δs[i])`
     - 누적 회전량: `θ[i] = θ[i-1] + Δθ[i]`
  3. 회전량 배열 반환: `[θ[0], θ[1], ..., θ[n]]`

**중요**: 정속 구간에서도 롤 반지름이 실시간으로 변하므로, 회전 속도는 일정하지 않습니다.

---

### 4.4 상태 관리

#### FR-7: 상태 천이
```
    ┌─────────┐
    │ STOPPED │ ◄────────────┐
    └────┬────┘              │
         │                   │
   move 명령                stop
         │                   │
         ▼                   │
  ┌──────────────┐           │
  │ ACCELERATING │───────────┤
  └──────┬───────┘           │
         │                   │
   가속 완료                  │
         │                   │
         ▼                   │
  ┌─────────────────┐        │
  │ CONSTANT_       │────────┤
  │ VELOCITY        │        │
  └────────┬────────┘        │
           │                 │
   감속 시작                  │
           │                 │
           ▼                 │
  ┌──────────────┐           │
  │ DECELERATING │───────────┤
  └──────┬───────┘           │
         │                   │
   감속 완료                  │
         │                   │
         └───────────────────┘
```

#### FR-8: 완료 콜백
- 이동이 완료되면 (상태가 STOPPED로 전환) 등록된 콜백 함수 호출
- 콜백은 비동기로 실행될 수 있음

---

## 5. 성능 요구사항

### PR-1: 속도 제한
- 최대 속도: 1.0 m/s (정속 구간)
- 최소 속도: 0.01 m/s (정속 구간)
- 설정 범위 검증 시 예외 발생

### PR-2: 타이밍 정밀도
- 회전량 배열 샘플링: 1ms (±0.1ms)
- 모터 제어 주기: 1ms

### PR-3: 계산 정밀도
- 위치 정밀도: ±0.1mm
- 회전량 정밀도: ±0.01도

---

## 6. 제약사항

### CS-1: 동작 범위
- 와이어 최대 길이: 설정 가능 (기본값 5m)
- 최소 위치: 0m (완전히 올린 상태)
- 최대 위치: maxWireLength (완전히 내린 상태)
- 범위 초과 시 동작 거부 및 예외 발생

### CS-2: 모션 파라미터
- 가속 시간 > 0
- 감속 시간 > 0
- MIN_VELOCITY ≤ constantVelocity ≤ MAX_VELOCITY

### CS-3: 롤 특성
- 와이어 두께 > 0
- 롤 내경 반지름 > 0
- RollWireCalculator의 제약사항 준수

---

## 7. 인터페이스 명세

### 7.1 사용 예시

```cpp
// 1. 모터 객체 생성 (의존성 주입)
SimMotor* simMotor = new SimMotor();  // 시뮬레이션 모터 생성

// 2. RollWireMover 객체 생성 (모터 주입)
RollWireMover mover(1.0, 50.0, simMotor);  // 와이어 두께 1mm, 롤 내경 반지름 50mm

// 3. 파라미터 설정
mover.setAccelerationTime(0.5);    // 가속 시간 0.5초
mover.setConstantVelocity(0.5);    // 정속 속도 0.5 m/s
mover.setDecelerationTime(0.5);    // 감속 시간 0.5초
mover.setVelocityProfile(RollWireMover::S_CURVE);  // S-curve 프로파일
mover.setMaxWireLength(5.0);       // 최대 길이 5m

// 4. 콜백 등록
mover.setCompletionCallback([]() {
    std::cout << "이동 완료!" << std::endl;
});

// 5. 이동 명령
mover.moveRelative(1.0);   // 1m 내림
// 또는
mover.moveAbsolute(2.5);   // 절대 위치 2.5m로 이동

// 6. 상태 모니터링
while (mover.isMoving()) {
    double pos = mover.getCurrentPosition();
    auto state = mover.getCurrentState();
    std::cout << "위치: " << pos << "m, 상태: " << state << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// 7. 긴급 정지
if (emergency) {
    mover.stop();
}

// 8. 리소스 정리
delete simMotor;  // 모터 객체 메모리 해제
```

---

## 8. 빌드 시스템

### 8.1 CMakeLists.txt 구조

```cmake
cmake_minimum_required(VERSION 3.14)
project(RollWireMover VERSION 1.0.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일
set(ROLLWIREMOVER_SOURCES
    src/RollWireMover.cpp
    src/SimMotor.cpp
)

set(ROLLWIREMOVER_HEADERS
    include/RollWireMover.h
    include/Motor.h
    include/SimMotor.h
)

# 라이브러리 생성
add_library(rollwiremover STATIC
    ${ROLLWIREMOVER_SOURCES}
    ${ROLLWIREMOVER_HEADERS}
)

# 기존 RollWireCalculator 라이브러리 링크
target_link_libraries(rollwiremover PUBLIC rollwirecalculator)

# 인클루드 디렉토리
target_include_directories(rollwiremover
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 테스트
if(BUILD_TESTING)
    enable_testing()

    # Google Test 다운로드
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
    )
    FetchContent_MakeAvailable(googletest)

    # 테스트 실행 파일
    add_executable(rollwiremover_test
        test/RollWireMoverTest.cpp
        test/MotorTest.cpp
    )

    target_link_libraries(rollwiremover_test
        rollwiremover
        gtest
        gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(rollwiremover_test)
endif()
```

### 8.2 디렉토리 구조

```
RollWireCal/
├── CMakeLists.txt
├── include/
│   ├── RollWireCalculator.h   (기존)
│   ├── RollWireMover.h         (신규)
│   ├── Motor.h                 (신규 - 인터페이스)
│   └── SimMotor.h              (신규 - Motor 구현체)
├── src/
│   ├── RollWireCalculator.cpp  (기존)
│   ├── RollWireMover.cpp       (신규)
│   └── SimMotor.cpp            (신규)
└── test/
    ├── RollWireCalculatorTest.cpp  (기존)
    ├── RollWireMoverTest.cpp       (신규)
    ├── MotorTest.cpp               (신규 - 인터페이스 테스트)
    └── SimMotorTest.cpp            (신규 - 구현체 테스트)
```

---

## 9. 테스트 계획

### 9.1 단위 테스트

#### Motor 클래스 (인터페이스)
- [ ] 순수 가상 클래스로 정의됨
- [ ] 인스턴스화 불가능 확인

#### SimMotor 클래스 (Motor 구현체)
- [ ] Motor 인터페이스 구현 확인
- [ ] 회전량 배열 실행
- [ ] 현재 위치 피드백
- [ ] 정지 기능
- [ ] 위치 리셋

#### RollWireMover 클래스 - 기본 기능
- [ ] 객체 생성 및 초기화 (의존성 주입)
- [ ] Motor 포인터가 nullptr일 때 예외 처리
- [ ] 파라미터 설정 (가속/정속/감속)
- [ ] 최대 길이 설정

#### RollWireMover 클래스 - 이동 제어
- [ ] 상대 이동 (양수/음수)
- [ ] 절대 이동
- [ ] 범위 초과 예외 처리
- [ ] 즉시 정지

#### RollWireMover 클래스 - 속도 프로파일
- [ ] Trapezoid 프로파일 생성
- [ ] S-Curve 프로파일 생성
- [ ] 속도 제한 검증

#### RollWireMover 클래스 - 상태 관리
- [ ] 상태 천이 (STOPPED → ACCELERATING → CONSTANT_VELOCITY → DECELERATING → STOPPED)
- [ ] 현재 상태 조회
- [ ] 완료 콜백 호출

### 9.2 통합 테스트
- [ ] RollWireMover + RollWireCalculator 통합
- [ ] RollWireMover + Motor 통합
- [ ] 전체 이동 시나리오

### 9.3 시나리오 테스트
- [ ] 1m 내리기 → 1m 올리기 (왕복 이동)
- [ ] 연속 이동 (0 → 1m → 2m → 3m)
- [ ] 긴급 정지 후 재시작
- [ ] 롤 내경 변경 후 이동

---

## 10. 구현 우선순위

### Phase 1: 기본 인프라
1. Motor 인터페이스 정의 (순수 가상 클래스)
2. SimMotor 클래스 구현 (Motor 구현체)
3. RollWireMover 기본 구조 및 생성자 (의존성 주입)
4. 파라미터 설정 메서드

### Phase 2: 모션 생성
5. Trapezoid 프로파일 생성
6. 회전량 배열 계산
7. 모터 실행 연동

### Phase 3: 고급 기능
8. S-Curve 프로파일 생성
9. 상태 관리 및 조회
10. 즉시 정지 기능

### Phase 4: 완성도 향상
11. 완료 콜백
12. 예외 처리 강화
13. 통합 테스트

---

## 11. 참고사항

### 11.1 물리적 특성
- 롤에 와이어가 감기면 반지름이 증가
- 정속으로 와이어를 이동하더라도 롤 회전 속도는 변화
- RollWireCalculator로 정확한 변환 필요

### 11.2 안전성
- 모든 이동 명령 전에 범위 검증
- 파라미터 유효성 검증
- 예외 처리를 통한 안전 보장

### 11.3 확장성
- 다양한 속도 프로파일 추가 가능
- 실제 모터 드라이버로 교체 가능 (인터페이스 동일)
- 콜백을 통한 외부 시스템 연동

---

**문서 버전**: 1.0
**작성일**: 2025-11-21
**작성자**: Claude Code
**기반 문서**: Note_RollWireMover.md
