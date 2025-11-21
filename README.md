# Roll Wire Calculator (RollWireCal)

두께가 있는 와이어를 롤에 감을 때, 와이어 길이와 롤의 회전량 간의 상관관계를 계산하는 C++ 유틸리티 라이브러리입니다.

## 개요

이 라이브러리는 와이어를 롤에 감을 때 발생하는 기하학적 관계를 수학적으로 모델링하여, 와이어 길이와 롤 회전량 간의 변환을 제공합니다.

### 주요 특징

- 와이어 길이 → 롤 회전량 변환
- 롤 회전량 → 와이어 길이 변환
- 나선형(Spiral) 모델 기반의 정확한 계산
- 롤 내경 반지름 설정 기능
- C++11 표준 지원
- CMake 빌드 시스템

## 수학적 모델

### 기본 가정

- 와이어의 두께는 일정함
- 와이어는 롤에 겹치지 않고 순차적으로 감김
- 롤의 내경: 와이어가 하나도 감기지 않은 상태의 반지름
- 롤의 외경: 와이어가 모두 감긴 상태의 반지름
- 회전량 0도는 초기 상태 (와이어가 완전히 감긴 상태)
- 와이어를 풀 때의 회전량은 양수

### 반지름 증가 모델 (연속 증가)

```
r(θ) = innerRadius + (θ / 360) × wireThickness
```

- 360도 회전(1바퀴) 시: 반지름이 와이어 두께만큼 증가
- 반지름은 회전량에 비례하여 연속적으로 증가 (나선형 모델)

### 길이 계산 (적분 모델)

```
L = ∫[0→Θ] r(θ) × (2π/360) dθ
  = ∫[0→Θ] [innerRadius + (θ/360) × wireThickness] × (2π/360) dθ
```

## 단위 체계

| 파라미터 | 단위 |
|---------|------|
| 와이어 두께 | mm (밀리미터) |
| 와이어 길이 | m (미터) |
| 회전량 | degree (도) |
| 롤 내경 반지름 | mm (밀리미터) |

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

```cpp
#include "RollWireCalculator.h"

int main() {
    // 와이어 두께 2.0mm, 롤 내경 반지름 50.0mm로 계산기 생성
    RollWireCalculator calc(2.0, 50.0);

    // 와이어 길이 10m를 풀 때 필요한 회전량 계산
    double rotation = calc.calculateRotationFromLength(10.0);

    // 롤 내경 반지름 변경
    calc.setInnerRadius(60.0);

    // 현재 설정 조회
    double thickness = calc.getWireThickness();  // 2.0mm
    double radius = calc.getInnerRadius();       // 60.0mm

    return 0;
}
```

## API 문서

### 클래스: `RollWireCalculator`

#### 생성자

```cpp
RollWireCalculator(double thickness, double radius)
```

- `thickness`: 와이어 두께 (mm 단위)
- `radius`: 롤 내경 반지름 (mm 단위)

#### 메서드

##### `double getWireThickness() const`

와이어 두께를 반환합니다.

- **반환값**: 와이어 두께 (mm)

##### `void setInnerRadius(double radius)`

롤의 내경 반지름을 설정합니다.

- **파라미터**: `radius` - 롤 내경 반지름 (mm)
- **예외**: `std::invalid_argument` - 반지름이 0 이하일 경우

##### `double getInnerRadius() const`

롤의 내경 반지름을 반환합니다.

- **반환값**: 롤 내경 반지름 (mm)

##### `double calculateRotationFromLength(double length) const`

와이어 길이를 입력받아 롤의 회전량을 계산합니다.

- **파라미터**: `length` - 와이어 길이 (m)
- **반환값**: 회전량 (degree)
- **예외**: `std::invalid_argument` - 길이가 음수일 경우

## 테스트

이 프로젝트는 Google Test를 사용하여 단위 테스트를 제공합니다.

```bash
# 빌드 후 테스트 실행
cd build
./rollwirecalculator_test
```

### 테스트 항목

- 생성자 파라미터 검증
- 와이어 두께 조회 기능
- 롤 내경 반지름 설정 및 조회
- 길이-회전량 변환 정확성
- 경계값 테스트
- 예외 처리 테스트

## 제약사항

### 물리적 제약사항

1. 와이어 두께는 일정하다고 가정
2. 와이어는 겹치지 않고 순차적으로 감김
3. 롤의 내경은 고정값

### 수치 제약사항

- 음수 길이는 허용하지 않음
- 롤의 내경 반지름은 양수여야 함
- 와이어 두께는 양수여야 함

## 라이선스

이 프로젝트는 교육 및 연구 목적으로 제공됩니다.

## 문서

- [PRD (제품 요구사항 문서)](PRD.md)
- [개발 노트](Note.md)
- [계획 문서](plan.md)

## 버전

- **현재 버전**: v1.0
- **작성일**: 2025-11-21
