# Roll Wire Calculator (RollWireCal)

![CI](https://github.com/pjy4617/RollWireCal/actions/workflows/ci.yml/badge.svg?branch=master)
[![codecov](https://codecov.io/gh/pjy4617/RollWireCal/branch/master/graph/badge.svg?token=CODECOV_TOKEN)](https://codecov.io/gh/pjy4617/RollWireCal)

롤에 감긴 와이어의 정밀한 위치 제어와 모션 계산을 위한 C++ 라이브러리입니다. 와이어 길이와 롤 회전량 간의 변환부터 정밀한 속도 프로파일 생성까지 제공합니다.

## 개요

이 프로젝트는 두 개의 핵심 라이브러리로 구성되어 있습니다:

- **RollWireCalculator**: 와이어 길이와 롤 회전량 간의 기하학적 변환을 수행하는 기본 계산 라이브러리
- **RollWireMover**: 실시간 모션 제어를 위한 속도 프로파일 생성 및 모터 제어 라이브러리

### 주요 특징

- 나선형(Spiral) 모델 기반의 정확한 기하학적 계산
- 다양한 속도 프로파일 지원 (S-curve, Trapezoid)
- 정밀한 1ms 간격 모션 계획
- 의존성 주입 기반 모터 인터페이스
- 에러 코드 시스템을 통한 안전한 에러 처리
- C++11/17 표준 지원
- CMake 빌드 시스템
- Google Test 기반 단위 테스트

## 시스템 아키텍처

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
│  │       RollWireCalculator (기하학적 변환)          │  │
│  │  - 길이 ↔ 회전량 변환                              │  │
│  │  - 나선형 모델 기반 계산                           │  │
│  └──────────────────────────────────────────────────┘  │
│                          ↓                              │
│  ┌──────────────────────────────────────────────────┐  │
│  │          Motor* (인터페이스 포인터)                │  │
│  │  - 의존성 주입 (Dependency Injection)              │  │
│  └──────────────┬───────────────────────────────────┘  │
└────────────────┼────────────────────────────────────────┘
                 │
     ┌───────────┴───────────┐
     │                       │
     ▼                       ▼
┌─────────────┐      ┌─────────────┐
│  SimMotor   │      │ Real Motor  │
│  (테스트용)  │      │  (실제 HW)  │
└─────────────┘      └─────────────┘
 implements Motor     implements Motor
```

## 라이브러리 상세

### 1. RollWireCalculator

와이어 길이와 롤 회전량 간의 기하학적 변환을 수행하는 기본 계산 라이브러리입니다.

#### 수학적 모델

**기본 가정**:
- 와이어의 두께는 일정함
- 와이어는 롤에 겹치지 않고 순차적으로 감김
- 롤의 내경: 와이어가 하나도 감기지 않은 상태의 반지름
- 회전량 0도는 초기 상태 (와이어가 완전히 감긴 상태)
- 와이어를 풀 때의 회전량은 양수

**반지름 증가 모델** (연속 증가):
```
r(θ) = innerRadius + (θ / 360) × wireThickness
```

**길이 계산** (적분 모델):
```
L = ∫[0→Θ] r(θ) × (2π/360) dθ
  = ∫[0→Θ] [innerRadius + (θ/360) × wireThickness] × (2π/360) dθ
```

#### API 개요

```cpp
class RollWireCalculator {
public:
    // 생성자: 와이어 두께(mm), 롤 내경 반지름(mm)
    RollWireCalculator(double thickness, double innerRadius);

    // 길이(m) → 회전량(degree) 변환
    double calculateRotationFromLength(double length) const;

    // 회전량(degree) → 길이(m) 변환
    double calculateLengthFromRotation(double rotation) const;

    // 설정 메서드
    void setInnerRadius(double radius);
    double getInnerRadius() const;
    double getWireThickness() const;
};
```

#### 에러 코드

```cpp
enum class ErrorCode {
    SUCCESS = 0,
    INVALID_WIRE_THICKNESS,    // 와이어 두께가 0 이하
    INVALID_INNER_RADIUS,      // 롤 내경 반지름이 0 이하
    INVALID_LENGTH,            // 와이어 길이가 음수
    INVALID_ROTATION           // 회전량이 음수
};
```

### 2. RollWireMover

RollWireCalculator를 활용하여 정밀한 모션 프로파일을 생성하고 모터를 제어하는 라이브러리입니다.

#### 주요 기능

- **이동 제어**: 상대/절대 이동 명령 처리
- **속도 프로파일**: Trapezoid, S-Curve 프로파일 생성
- **모션 계획**: 1ms 간격 회전량 배열 계산
- **상태 관리**: 실시간 상태 모니터링 (STOPPED, ACCELERATING, CONSTANT_VELOCITY, DECELERATING)
- **안전 제어**: 범위 검증 및 즉시 정지 기능

#### API 개요

```cpp
class RollWireMover {
public:
    // 생성자 (의존성 주입): 와이어 두께(mm), 롤 내경(mm), Motor*, 에러 코드
    RollWireMover(double wireThickness, double innerRadius,
                  Motor* motor, ErrorCode& outError);

    // 모션 파라미터 설정
    ErrorCode setAccelerationTime(double time);       // 가속 시간(초)
    ErrorCode setConstantVelocity(double velocity);   // 정속 속도(m/s)
    ErrorCode setDecelerationTime(double time);       // 감속 시간(초)
    ErrorCode setVelocityProfile(ProfileType type);   // TRAPEZOID or S_CURVE

    // 이동 명령
    ErrorCode moveRelative(double distance);  // 상대 이동: 음수(올림), 양수(내림)
    ErrorCode moveAbsolute(double position);  // 절대 위치 이동
    ErrorCode stop();                         // 즉시 정지

    // 상태 조회
    double getCurrentPosition() const;
    MotionState getCurrentState() const;
    bool isMoving() const;

    // 콜백 등록
    void setCompletionCallback(std::function<void()> callback);
};
```

#### Motor 인터페이스

```cpp
class Motor {
public:
    virtual ~Motor() = default;
    virtual void executeRotationProfile(const std::vector<double>& rotations) = 0;
    virtual void stop() = 0;
    virtual double getCurrentRotation() const = 0;
    virtual bool isRunning() const = 0;
    virtual void resetPosition() = 0;
};
```

**SimMotor**: Motor 인터페이스의 시뮬레이션 구현체 (테스트 및 개발용)

#### 속도 프로파일

**Trapezoid 프로파일**: 선형 가속/감속
```
속도
 ^
 │      ┌─────────┐
 │     /│         │\
 │    / │         │ \
 │   /  │         │  \
 └─────────────────────→ 시간
   가속   정속      감속
```

**S-Curve 프로파일**: 부드러운 가속/감속 (저크 최소화)
```
속도
 ^
 │      ╭─────────╮
 │    ╱ │         │ ╲
 │   ╱  │         │  ╲
 │  ╱   │         │   ╲
 └─────────────────────→ 시간
   가속   정속      감속
```

#### 에러 코드

```cpp
enum class ErrorCode {
    SUCCESS = 0,
    INVALID_MOTOR_POINTER,         // Motor 포인터가 nullptr
    INVALID_WIRE_THICKNESS,
    INVALID_INNER_RADIUS,
    INVALID_ACCELERATION_TIME,
    INVALID_DECELERATION_TIME,
    INVALID_VELOCITY,              // 속도가 범위 밖 (0.01~1.0 m/s)
    INVALID_MAX_LENGTH,
    OUT_OF_RANGE,                  // 이동 범위 초과
    MOTOR_BUSY                     // 모터가 이미 동작 중
};
```

## 단위 체계

| 파라미터 | 단위 |
|---------|------|
| 와이어 두께 | mm (밀리미터) |
| 와이어 길이 | m (미터) |
| 회전량 | degree (도) |
| 롤 내경 반지름 | mm (밀리미터) |
| 속도 | m/s (미터/초) |
| 시간 | s (초) |

## 빌드 방법

### 요구사항

- CMake 3.14 이상
- C++11 지원 컴파일러
- Google Test (자동으로 다운로드됨)

### 빌드 단계

```bash
# 빌드 디렉토리 생성
mkdir build
cd build

# CMake 구성
cmake ..

# 빌드 실행
cmake --build .

# 테스트 실행
./rollwirecalculator_test
```

## 사용 예제

### 예제 1: RollWireCalculator 기본 사용

```cpp
#include "RollWireCalculator.h"

int main() {
    // 와이어 두께 2.0mm, 롤 내경 반지름 50.0mm로 계산기 생성
    RollWireCalculator calc(2.0, 50.0);

    // 와이어 길이 10m를 풀 때 필요한 회전량 계산
    double rotation = calc.calculateRotationFromLength(10.0);
    std::cout << "10m 이동에 필요한 회전량: " << rotation << "도" << std::endl;

    // 회전량 → 길이 변환
    double length = calc.calculateLengthFromRotation(360.0);
    std::cout << "360도 회전 시 와이어 길이: " << length << "m" << std::endl;

    // 롤 내경 반지름 변경
    calc.setInnerRadius(60.0);

    // 현재 설정 조회
    double thickness = calc.getWireThickness();  // 2.0mm
    double radius = calc.getInnerRadius();       // 60.0mm

    return 0;
}
```

### 예제 2: RollWireMover를 이용한 모션 제어

```cpp
#include "RollWireMover.h"
#include "SimMotor.h"
#include <iostream>

int main() {
    // 1. 모터 객체 생성 (의존성 주입)
    SimMotor* simMotor = new SimMotor();

    // 2. RollWireMover 객체 생성
    RollWireMover::ErrorCode initError;
    RollWireMover mover(1.0, 50.0, simMotor, initError);  // 와이어 1mm, 롤 내경 50mm

    if (initError != RollWireMover::ErrorCode::SUCCESS) {
        std::cerr << "초기화 실패: "
                  << RollWireMover::getErrorMessage(initError) << std::endl;
        return -1;
    }

    // 3. 모션 파라미터 설정
    mover.setAccelerationTime(0.5);        // 가속 시간 0.5초
    mover.setConstantVelocity(0.5);        // 정속 속도 0.5 m/s
    mover.setDecelerationTime(0.5);        // 감속 시간 0.5초
    mover.setVelocityProfile(RollWireMover::S_CURVE);  // S-curve 프로파일
    mover.setMaxWireLength(5.0);           // 최대 길이 5m

    // 4. 완료 콜백 등록
    mover.setCompletionCallback([]() {
        std::cout << "이동 완료!" << std::endl;
    });

    // 5. 이동 명령
    auto result = mover.moveRelative(1.0);  // 1m 내림
    if (result != RollWireMover::ErrorCode::SUCCESS) {
        std::cerr << "이동 실패: "
                  << RollWireMover::getErrorMessage(result) << std::endl;
        return -1;
    }

    // 6. 상태 모니터링
    while (mover.isMoving()) {
        double pos = mover.getCurrentPosition();
        auto state = mover.getCurrentState();
        std::cout << "현재 위치: " << pos << "m" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 7. 절대 위치로 이동
    mover.moveAbsolute(2.5);  // 2.5m 위치로 이동

    // 8. 리소스 정리
    delete simMotor;

    return 0;
}
```

### 예제 3: 에러 처리

```cpp
#include "RollWireMover.h"

void safeMove(RollWireMover& mover, double distance) {
    auto result = mover.moveRelative(distance);

    switch (result) {
        case RollWireMover::ErrorCode::SUCCESS:
            std::cout << "이동 시작: " << distance << "m" << std::endl;
            break;
        case RollWireMover::ErrorCode::OUT_OF_RANGE:
            std::cerr << "범위 초과: 이동 불가" << std::endl;
            break;
        case RollWireMover::ErrorCode::MOTOR_BUSY:
            std::cerr << "모터 동작 중: 대기 필요" << std::endl;
            break;
        default:
            std::cerr << "에러: "
                      << RollWireMover::getErrorMessage(result) << std::endl;
            break;
    }
}
```

## 프로젝트 구조

```
RollWireCal/
├── CMakeLists.txt                      # 최상위 CMake 설정
├── README.md
├── CLAUDE.md                           # 개발 가이드라인 (TDD)
├── plan.md                             # 개발 계획 및 체크리스트
│
├── Lib/
│   ├── RollWireCalculator/             # 기하학적 계산 라이브러리
│   │   ├── CMakeLists.txt
│   │   ├── PRD.md                      # 제품 요구사항 문서
│   │   ├── include/
│   │   │   └── RollWireCalculator.h
│   │   ├── src/
│   │   │   └── RollWireCalculator.cpp
│   │   └── test/
│   │       └── RollWireCalculatorTest.cpp
│   │
│   └── RollWireMover/                  # 모션 제어 라이브러리
│       ├── CMakeLists.txt
│       ├── PRD.md                      # 제품 요구사항 문서
│       ├── include/
│       │   ├── RollWireMover.h
│       │   ├── Motor.h                 # 모터 인터페이스
│       │   └── SimMotor.h              # 시뮬레이션 모터
│       ├── src/
│       │   ├── RollWireMover.cpp
│       │   └── SimMotor.cpp
│       └── test/
│           ├── RollWireMoverTest.cpp
│           └── SimMotorTest.cpp
│
└── build/                              # 빌드 출력 디렉토리 (생성됨)
    ├── Lib/
    │   ├── RollWireCalculator/
    │   │   └── rollwirecalculator_test
    │   └── RollWireMover/
    │       └── rollwiremover_test
    └── ...
```

## 테스트

이 프로젝트는 Google Test를 사용하여 단위 테스트를 제공합니다.

```bash
# 빌드 후 테스트 실행
cd build

# RollWireCalculator 테스트
./Lib/RollWireCalculator/rollwirecalculator_test

# RollWireMover 테스트
./Lib/RollWireMover/rollwiremover_test
```

### RollWireCalculator 테스트 항목

- 생성자 파라미터 검증
- 와이어 두께 조회 기능
- 롤 내경 반지름 설정 및 조회
- 길이-회전량 변환 정확성
- 회전량-길이 변환 정확성
- 경계값 테스트
- 에러 코드 처리 테스트

### RollWireMover 테스트 항목

- **Motor 인터페이스**: 순수 가상 클래스 검증
- **SimMotor**: 회전량 배열 실행, 위치 피드백, 정지 기능
- **기본 기능**: 객체 생성, 의존성 주입, 파라미터 설정
- **이동 제어**: 상대/절대 이동, 범위 검증, 즉시 정지
- **속도 프로파일**: Trapezoid/S-Curve 프로파일 생성
- **상태 관리**: 상태 천이, 완료 콜백
- **통합 테스트**: RollWireMover + RollWireCalculator + Motor

## 제약사항 및 성능

### 물리적 제약사항

1. 와이어 두께는 일정하다고 가정
2. 와이어는 겹치지 않고 순차적으로 감김
3. 롤의 내경은 고정값 (와이어가 감기지 않은 상태)
4. 롤의 외경: 와이어가 모두 감긴 상태의 반지름

### 수치 제약사항

**RollWireCalculator**:
- 와이어 두께 > 0
- 롤 내경 반지름 > 0
- 와이어 길이 ≥ 0
- 회전량 ≥ 0

**RollWireMover**:
- 가속/감속 시간 > 0
- 속도 범위: 0.01 m/s ≤ velocity ≤ 1.0 m/s
- 위치 범위: 0 ≤ position ≤ maxWireLength (기본값 5m)
- 모터 포인터는 nullptr이 아니어야 함

### 성능 요구사항

- **샘플링 주기**: 1ms (±0.1ms)
- **위치 정밀도**: ±0.1mm
- **회전량 정밀도**: ±0.01도
- **실시간 계산**: 모션 프로파일 생성 및 실행

## 개발 방법론

이 프로젝트는 **Kent Beck의 TDD(Test-Driven Development)** 원칙을 따릅니다:

- **Red → Green → Refactor** 사이클 준수
- 모든 기능은 테스트 우선 작성
- 최소한의 코드로 테스트 통과
- **Tidy First** 접근: 구조 변경과 행동 변경 분리
- 모든 커밋 전 테스트 통과 필수

자세한 개발 가이드라인은 [CLAUDE.md](CLAUDE.md)를 참조하세요.

## 문서

### 전체 프로젝트
- [개발 가이드라인 (TDD)](CLAUDE.md)
- [개발 계획 문서](plan.md)

### RollWireCalculator
- [PRD (제품 요구사항 문서)](Lib/RollWireCalculator/PRD.md)

### RollWireMover
- [PRD (제품 요구사항 문서)](Lib/RollWireMover/PRD.md)

## 라이선스

이 프로젝트는 교육 및 연구 목적으로 제공됩니다.

## 기여 및 지원

이슈, 버그 리포트, 기능 요청은 GitHub Issues를 통해 제출해주세요.

## 버전

- **현재 버전**: v1.0
- **최종 업데이트**: 2025-11-24
