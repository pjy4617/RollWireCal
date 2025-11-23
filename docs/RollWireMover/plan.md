# RollWireMover - TDD 구현 계획

> Kent Beck의 TDD 원칙에 따라 작은 단계로 나누어 구현합니다.
> 각 테스트를 완료한 후 체크마크(✓)를 표시하세요.

## 테스트 작성 가이드
- 한 번에 하나의 테스트만 작성
- 가장 단순한 것부터 시작
- 작은 증분으로 기능 추가
- Red → Green → Refactor 사이클 준수
- 구조적 변경(Tidy First)과 행동적 변경 분리

---

## Phase 1: Motor 인터페이스 및 SimMotor 구현

### 1.1 Motor 인터페이스 (순수 가상 클래스)
- [✓] Motor 클래스를 생성할 수 있다 (인터페이스로 정의)
- [✓] Motor는 순수 가상 클래스이다 (인스턴스화 불가능)
- [✓] getCurrentRotation() 순수 가상 함수가 선언되어 있다
- [✓] isRunning() 순수 가상 함수가 선언되어 있다
- [✓] executeRotationProfile() 순수 가상 함수가 선언되어 있다
- [✓] stop() 순수 가상 함수가 선언되어 있다
- [✓] resetPosition() 순수 가상 함수가 선언되어 있다
- [✓] 가상 소멸자가 정의되어 있다

### 1.2 SimMotor 기본 구조 (Motor 구현체)
- [✓] SimMotor 클래스를 생성할 수 있다
- [✓] SimMotor는 Motor 인터페이스를 상속받는다
- [✓] 기본 생성자로 SimMotor 객체를 생성하면 초기 회전 위치는 0이다
- [✓] 기본 생성자로 SimMotor 객체를 생성하면 초기 상태는 정지(not running)이다

### 1.3 SimMotor 위치 조회 및 리셋
- [✓] getCurrentRotation() 메서드로 현재 회전 위치를 조회할 수 있다
- [✓] isRunning() 메서드로 모터 동작 여부를 조회할 수 있다
- [✓] resetPosition() 메서드로 회전 위치를 0으로 초기화할 수 있다
- [✓] resetPosition() 호출 후 getCurrentRotation()은 0을 반환한다

### 1.4 SimMotor 회전량 배열 실행
- [✓] executeRotationProfile() 메서드가 존재한다
- [✓] 빈 배열을 전달하면 아무 동작도 하지 않는다
- [✓] 단일 값 배열 [360.0]을 전달하면 현재 회전 위치가 360도가 된다
- [ ] 다중 값 배열 [90.0, 180.0, 270.0]을 전달하면 마지막 값(270도)이 현재 위치가 된다
- [ ] executeRotationProfile() 실행 중에는 isRunning()이 true를 반환한다
- [ ] executeRotationProfile() 완료 후에는 isRunning()이 false를 반환한다

### 1.5 SimMotor 모터 정지
- [ ] stop() 메서드가 존재한다
- [ ] stop() 호출 시 isRunning()이 false를 반환한다
- [ ] 실행 중인 프로파일을 stop()으로 중단할 수 있다
- [ ] stop() 호출 후 현재 위치는 중단 시점의 위치를 유지한다

---

## Phase 2: RollWireMover 기본 구조

### 2.1 클래스 생성 및 초기화 (의존성 주입)
- [ ] RollWireMover 클래스를 생성할 수 있다
- [ ] wireThickness, innerRadius, Motor 포인터, ErrorCode& 출력 파라미터를 받는 생성자가 있다
- [ ] 생성 시 내부적으로 RollWireCalculator 객체가 생성된다
- [ ] 생성 시 Motor 포인터를 멤버 변수에 저장한다 (의존성 주입)
- [ ] Motor 포인터가 nullptr일 때 ErrorCode::INVALID_MOTOR_POINTER를 반환한다
- [ ] wireThickness가 0일 때 ErrorCode::INVALID_WIRE_THICKNESS를 반환한다
- [ ] wireThickness가 음수일 때 ErrorCode::INVALID_WIRE_THICKNESS를 반환한다
- [ ] innerRadius가 0일 때 ErrorCode::INVALID_INNER_RADIUS를 반환한다
- [ ] innerRadius가 음수일 때 ErrorCode::INVALID_INNER_RADIUS를 반환한다
- [ ] 모든 파라미터가 유효할 때 ErrorCode::SUCCESS를 반환한다

### 2.2 초기 상태
- [ ] 초기 현재 위치는 0이다 (완전히 올린 상태)
- [ ] 초기 상태는 STOPPED이다
- [ ] 초기 최대 와이어 길이는 5.0m이다
- [ ] isMoving()은 초기에 false를 반환한다

### 2.3 모션 파라미터 설정
- [ ] setAccelerationTime() 메서드로 가속 시간을 설정할 수 있다 (ErrorCode 반환)
- [ ] setConstantVelocity() 메서드로 정속 속도를 설정할 수 있다 (ErrorCode 반환)
- [ ] setDecelerationTime() 메서드로 감속 시간을 설정할 수 있다 (ErrorCode 반환)
- [ ] setAccelerationTime()에 0 이하 값을 전달하면 ErrorCode::INVALID_ACCELERATION_TIME을 반환한다
- [ ] setDecelerationTime()에 0 이하 값을 전달하면 ErrorCode::INVALID_DECELERATION_TIME을 반환한다
- [ ] setConstantVelocity()에 MIN_VELOCITY(0.01) 미만 값을 전달하면 ErrorCode::INVALID_VELOCITY를 반환한다
- [ ] setConstantVelocity()에 MAX_VELOCITY(1.0) 초과 값을 전달하면 ErrorCode::INVALID_VELOCITY를 반환한다
- [ ] 유효한 값 설정 시 ErrorCode::SUCCESS를 반환한다

### 2.4 시스템 설정
- [ ] setMaxWireLength() 메서드로 최대 와이어 길이를 설정할 수 있다 (ErrorCode 반환)
- [ ] setMaxWireLength()에 0 이하 값을 전달하면 ErrorCode::INVALID_MAX_LENGTH를 반환한다
- [ ] setInnerRadius() 메서드로 롤 내경을 변경할 수 있다 (ErrorCode 반환)
- [ ] setInnerRadius()에 0 이하 값을 전달하면 ErrorCode::INVALID_INNER_RADIUS를 반환한다
- [ ] 유효한 값 설정 시 ErrorCode::SUCCESS를 반환한다

### 2.5 상태 조회
- [ ] getCurrentPosition() 메서드로 현재 와이어 위치를 조회할 수 있다
- [ ] getCurrentState() 메서드로 현재 모션 상태를 조회할 수 있다
- [ ] isMoving() 메서드로 이동 중 여부를 조회할 수 있다

### 2.6 속도 프로파일 타입 설정
- [ ] setVelocityProfile() 메서드가 존재한다
- [ ] TRAPEZOID 프로파일 타입을 설정할 수 있다
- [ ] S_CURVE 프로파일 타입을 설정할 수 있다

---

## Phase 3: 이동 제어 - 기본 동작

### 3.1 상대 이동 - 범위 검증
- [ ] moveRelative() 메서드가 존재한다 (ErrorCode 반환)
- [ ] moveRelative(0)을 호출하면 위치가 변하지 않고 ErrorCode::SUCCESS를 반환한다
- [ ] moveRelative()로 음수 거리를 전달하면 올리는 동작을 시도한다 (현재 위치 감소)
- [ ] moveRelative()로 양수 거리를 전달하면 내리는 동작을 시도한다 (현재 위치 증가)
- [ ] 위치 0에서 moveRelative(-1.0)을 호출하면 ErrorCode::OUT_OF_RANGE를 반환한다
- [ ] 위치 4.0에서 moveRelative(2.0)을 호출하면 (maxLength=5.0) ErrorCode::OUT_OF_RANGE를 반환한다
- [ ] 유효한 범위 내 이동 시 ErrorCode::SUCCESS를 반환한다

### 3.2 절대 이동 - 범위 검증
- [ ] moveAbsolute() 메서드가 존재한다 (ErrorCode 반환)
- [ ] moveAbsolute(0)을 호출하면 위치 0으로 이동하고 ErrorCode::SUCCESS를 반환한다 (완전히 올림)
- [ ] moveAbsolute(maxWireLength)를 호출하면 최대 길이 위치로 이동하고 ErrorCode::SUCCESS를 반환한다
- [ ] moveAbsolute()에 음수를 전달하면 ErrorCode::OUT_OF_RANGE를 반환한다
- [ ] moveAbsolute()에 maxWireLength를 초과하는 값을 전달하면 ErrorCode::OUT_OF_RANGE를 반환한다
- [ ] moveAbsolute()는 내부적으로 moveRelative()를 호출한다

---

## Phase 4: Trapezoid 속도 프로파일

### 4.1 단순 Trapezoid 프로파일 생성
- [ ] 프로파일 타입을 TRAPEZOID로 설정한다
- [ ] 가속 시간 0.1초, 정속 속도 0.1 m/s, 감속 시간 0.1초로 설정한다
- [ ] 매우 짧은 거리(0.01m) 이동 시 속도 프로파일이 생성된다
- [ ] 생성된 프로파일의 최대 속도는 constantVelocity 이하이다
- [ ] 프로파일의 시작 속도는 0이다
- [ ] 프로파일의 종료 속도는 0이다

### 4.2 Trapezoid 프로파일 - 가속 구간
- [ ] 가속 구간의 속도는 선형적으로 증가한다
- [ ] 가속 구간의 시간은 설정한 accelerationTime과 일치한다
- [ ] 가속 구간 종료 시 속도는 constantVelocity에 도달한다

### 4.3 Trapezoid 프로파일 - 정속 구간
- [ ] 충분히 긴 거리 이동 시 정속 구간이 존재한다
- [ ] 정속 구간의 속도는 constantVelocity이다
- [ ] 정속 구간 시간 = (총 거리 - 가감속 거리) / constantVelocity

### 4.4 Trapezoid 프로파일 - 감속 구간
- [ ] 감속 구간의 속도는 선형적으로 감소한다
- [ ] 감속 구간의 시간은 설정한 decelerationTime과 일치한다
- [ ] 감속 구간 종료 시 속도는 0이 된다

### 4.5 Trapezoid 프로파일 - 짧은 거리 처리
- [ ] 이동 거리가 너무 짧아 정속 구간이 없는 경우를 처리한다
- [ ] 삼각형 프로파일이 생성된다 (정속 구간 없음)
- [ ] 최대 속도는 constantVelocity보다 낮다
- [ ] 총 이동 거리는 요청한 거리와 일치한다

### 4.6 속도 → 회전량 변환
- [ ] 속도 프로파일을 회전량 배열로 변환하는 내부 메서드가 있다
- [ ] 1ms 간격으로 샘플링된다
- [ ] 각 시간 단계의 이동 거리 Δs = v × 0.001 (m)
- [ ] RollWireCalculator를 사용하여 각 단계의 회전량을 계산한다
- [ ] 회전량 배열의 길이는 (총 시간 / 0.001)과 일치한다
- [ ] 회전량 배열의 값은 누적 회전량이다

### 4.7 모터 실행 연동
- [ ] moveRelative() 호출 시 내부 Motor 객체의 executeRotationProfile()이 호출된다
- [ ] 생성된 회전량 배열이 Motor에 전달된다
- [ ] 이동 중 isMoving()이 true를 반환한다
- [ ] 이동 중 getCurrentState()가 적절한 상태를 반환한다
- [ ] 이동 완료 후 isMoving()이 false를 반환한다
- [ ] 이동 완료 후 getCurrentState()가 STOPPED를 반환한다

### 4.8 위치 업데이트
- [ ] moveRelative(1.0) 호출 후 currentPosition이 1.0 증가한다
- [ ] moveRelative(-0.5) 호출 후 currentPosition이 0.5 감소한다
- [ ] moveAbsolute(2.5) 호출 후 currentPosition이 2.5가 된다

---

## Phase 5: S-Curve 속도 프로파일

### 5.1 S-Curve 프로파일 생성
- [ ] 프로파일 타입을 S_CURVE로 설정한다
- [ ] S-Curve 프로파일 생성 메서드가 있다
- [ ] 생성된 프로파일의 시작 속도는 0이다
- [ ] 생성된 프로파일의 종료 속도는 0이다
- [ ] 생성된 프로파일의 최대 속도는 constantVelocity 이하이다

### 5.2 S-Curve 프로파일 - 가속 구간
- [ ] 가속 구간의 속도 변화는 부드럽다 (사인 함수 또는 3차 다항식)
- [ ] 가속 시작 시 가속도는 0이다 (저크 최소화)
- [ ] 가속 종료 시 가속도는 0이다
- [ ] 가속 구간 종료 시 속도는 constantVelocity에 도달한다

### 5.3 S-Curve 프로파일 - 정속 구간
- [ ] 충분히 긴 거리 이동 시 정속 구간이 존재한다
- [ ] 정속 구간의 속도는 constantVelocity이다

### 5.4 S-Curve 프로파일 - 감속 구간
- [ ] 감속 구간의 속도 변화는 부드럽다
- [ ] 감속 시작 시 가속도는 0이다
- [ ] 감속 종료 시 가속도는 0이다
- [ ] 감속 구간 종료 시 속도는 0이 된다

### 5.5 S-Curve vs Trapezoid 비교
- [ ] S-Curve 프로파일이 Trapezoid보다 부드럽다
- [ ] 같은 거리를 이동할 때 S-Curve의 가속도 변화율이 작다
- [ ] 두 프로파일 모두 같은 거리를 정확히 이동한다

---

## Phase 6: 상태 관리

### 6.1 상태 천이 - STOPPED → ACCELERATING
- [ ] 이동 시작 시 상태가 STOPPED에서 ACCELERATING으로 변경된다
- [ ] ACCELERATING 상태에서 isMoving()은 true를 반환한다

### 6.2 상태 천이 - ACCELERATING → CONSTANT_VELOCITY
- [ ] 가속 완료 후 상태가 ACCELERATING에서 CONSTANT_VELOCITY로 변경된다
- [ ] CONSTANT_VELOCITY 상태에서 isMoving()은 true를 반환한다

### 6.3 상태 천이 - CONSTANT_VELOCITY → DECELERATING
- [ ] 감속 시작 시 상태가 CONSTANT_VELOCITY에서 DECELERATING으로 변경된다
- [ ] DECELERATING 상태에서 isMoving()은 true를 반환한다

### 6.4 상태 천이 - DECELERATING → STOPPED
- [ ] 감속 완료 후 상태가 DECELERATING에서 STOPPED로 변경된다
- [ ] STOPPED 상태에서 isMoving()은 false를 반환한다

### 6.5 상태 천이 - 짧은 거리 이동
- [ ] 정속 구간이 없는 짧은 이동 시 CONSTANT_VELOCITY 상태를 건너뛴다
- [ ] ACCELERATING → DECELERATING → STOPPED 순서로 천이한다

---

## Phase 7: 고급 기능

### 7.1 즉시 정지
- [ ] stop() 메서드가 존재한다
- [ ] 이동 중 stop() 호출 시 즉시 정지한다
- [ ] stop() 호출 후 상태가 STOPPED로 변경된다
- [ ] stop() 호출 후 현재 위치는 정지 시점의 위치를 유지한다
- [ ] stop() 호출 후 isMoving()은 false를 반환한다

### 7.2 완료 콜백
- [ ] setCompletionCallback() 메서드가 존재한다
- [ ] 콜백 함수를 등록할 수 있다
- [ ] 이동 완료 시 (상태가 STOPPED로 전환) 등록된 콜백이 호출된다
- [ ] 콜백이 등록되지 않은 경우 예외가 발생하지 않는다
- [ ] stop()으로 중단된 경우 콜백이 호출되지 않는다

### 7.3 연속 이동
- [ ] 이동 완료 후 새로운 moveRelative()를 호출할 수 있다
- [ ] 이동 완료 후 새로운 moveAbsolute()를 호출할 수 있다
- [ ] 연속 이동 시 각 이동이 독립적으로 실행된다

### 7.4 이동 중 명령 처리
- [ ] 이동 중(isMoving() == true) moveRelative() 호출 시 ErrorCode::MOTOR_BUSY를 반환한다
- [ ] 이동 중(isMoving() == true) moveAbsolute() 호출 시 ErrorCode::MOTOR_BUSY를 반환한다
- [ ] 이동 중 파라미터 설정(setAccelerationTime 등)은 허용되며 ErrorCode::SUCCESS를 반환한다

---

## Phase 8: 통합 및 시나리오 테스트

### 8.1 RollWireCalculator 통합
- [ ] RollWireMover가 RollWireCalculator를 올바르게 사용한다
- [ ] 와이어 두께와 롤 내경에 따라 회전량이 정확히 계산된다
- [ ] 롤 반지름이 실시간으로 변하는 것을 반영한다
- [ ] 정속 구간에서도 회전 속도가 변한다

### 8.2 왕복 이동 시나리오
- [ ] 1m 내리기 → 완료 대기 → 1m 올리기를 실행할 수 있다
- [ ] 왕복 이동 후 최종 위치는 초기 위치와 동일하다
- [ ] 각 이동에서 콜백이 정상적으로 호출된다

### 8.3 연속 이동 시나리오
- [ ] 0 → 1m → 2m → 3m 순차 이동을 실행할 수 있다
- [ ] 각 단계에서 위치가 정확히 업데이트된다
- [ ] 최종 위치는 3m이다

### 8.4 긴급 정지 시나리오
- [ ] 이동 중 stop() 호출 시 즉시 정지한다
- [ ] 정지 후 현재 위치를 확인할 수 있다
- [ ] 정지 후 새로운 이동 명령을 실행할 수 있다
- [ ] 정지 위치에서 moveAbsolute(0)으로 원점 복귀할 수 있다

### 8.5 롤 내경 변경 시나리오
- [ ] setInnerRadius()로 롤 내경을 변경한다
- [ ] 변경 후 같은 거리 이동 시 다른 회전량이 계산된다
- [ ] 내경 변경 후에도 위치 제어가 정확하다

### 8.6 경계값 테스트
- [ ] 최소 속도(0.01 m/s)로 이동한다
- [ ] 최대 속도(1.0 m/s)로 이동한다
- [ ] 최대 거리(5.0m)를 이동한다
- [ ] 매우 짧은 거리(0.001m)를 이동한다
- [ ] 매우 긴 가속 시간(5초)으로 이동한다
- [ ] 매우 짧은 가속 시간(0.01초)으로 이동한다

### 8.7 성능 테스트
- [ ] 1ms 샘플링 주기가 정확하다
- [ ] 회전량 배열 계산이 합리적인 시간 내에 완료된다
- [ ] 1m 이동에 대한 회전량 배열 생성이 10ms 이내에 완료된다

---

## Phase 9: 예외 처리 및 에지 케이스

### 9.1 에러 코드 시스템
- [ ] ErrorCode enum이 정의되어 있다
- [ ] getErrorMessage() 정적 메서드가 존재한다
- [ ] SUCCESS 에러 코드에 대한 메시지를 반환한다
- [ ] INVALID_MOTOR_POINTER 에러 코드에 대한 메시지를 반환한다
- [ ] INVALID_WIRE_THICKNESS 에러 코드에 대한 메시지를 반환한다
- [ ] INVALID_INNER_RADIUS 에러 코드에 대한 메시지를 반환한다
- [ ] INVALID_ACCELERATION_TIME 에러 코드에 대한 메시지를 반환한다
- [ ] INVALID_DECELERATION_TIME 에러 코드에 대한 메시지를 반환한다
- [ ] INVALID_VELOCITY 에러 코드에 대한 메시지를 반환한다
- [ ] INVALID_MAX_LENGTH 에러 코드에 대한 메시지를 반환한다
- [ ] OUT_OF_RANGE 에러 코드에 대한 메시지를 반환한다
- [ ] MOTOR_BUSY 에러 코드에 대한 메시지를 반환한다
- [ ] 모든 에러 메시지는 명확하고 유용하다

### 9.2 파라미터 유효성 검증
- [ ] 모든 setter 메서드에서 유효하지 않은 값에 대해 예외를 발생시킨다
- [ ] 예외 메시지가 명확하고 유용하다
- [ ] 예외 발생 시 객체 상태가 변경되지 않는다

### 9.3 에지 케이스
- [ ] 가속 시간 = 감속 시간 = 0.001초 (최소값)으로 이동한다
- [ ] 이동 거리 = 최대 길이(5.0m)로 이동한다
- [ ] 속도 프로파일 생성 중 수치 오버플로우가 발생하지 않는다
- [ ] 회전량 배열이 너무 크지 않다 (메모리 효율성)

### 9.4 스레드 안전성 (선택 사항)
- [ ] 여러 스레드에서 상태 조회를 할 수 있다
- [ ] 이동 중 다른 스레드에서 stop()을 호출할 수 있다
- [ ] 경쟁 조건(race condition)이 발생하지 않는다

---

## Phase 10: 문서화 및 예제

### 10.1 코드 문서화
- [ ] 모든 public 메서드에 Doxygen 스타일 주석이 있다
- [ ] 클래스에 설명 주석이 있다
- [ ] 파라미터와 반환값이 문서화되어 있다
- [ ] 예외 발생 조건이 문서화되어 있다

### 10.2 예제 코드
- [ ] 기본 사용 예제 코드가 작성되어 있다
- [ ] Trapezoid 프로파일 사용 예제가 있다
- [ ] S-Curve 프로파일 사용 예제가 있다
- [ ] 콜백 사용 예제가 있다
- [ ] 긴급 정지 예제가 있다
- [ ] 예제 코드가 컴파일되고 실행된다

---

## 완료 체크리스트

- [ ] 모든 단위 테스트가 통과한다
- [ ] 모든 통합 테스트가 통과한다
- [ ] 컴파일 경고가 없다
- [ ] 코드 커버리지가 90% 이상이다
- [ ] 메모리 누수가 없다
- [ ] 문서화가 완료되었다
- [ ] CMakeLists.txt가 올바르게 설정되었다
- [ ] README.md에 사용법이 설명되어 있다

---

**참고사항:**
- 각 테스트는 독립적으로 실행 가능해야 함
- 테스트 우선, 구현은 최소한으로
- 리팩토링은 테스트 통과 후에만
- 작은 단계로 커밋하기
- 구조적 변경과 행동적 변경을 분리하여 커밋

**TDD 사이클:**
1. **RED**: 실패하는 테스트 작성
2. **GREEN**: 테스트를 통과시킬 최소한의 코드 작성
3. **REFACTOR**: 테스트가 통과하는 상태에서 코드 개선
4. 다음 테스트로 이동

**기반 문서:**
- CLAUDE.md (TDD 원칙)
- PRD_RollWireMover.md (기능 요구사항)

**작성일**: 2025-11-21
**작성자**: Claude Code
