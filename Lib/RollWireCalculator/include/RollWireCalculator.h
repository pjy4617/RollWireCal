#ifndef ROLLWIRECALCULATOR_H
#define ROLLWIRECALCULATOR_H

#include <stdexcept>

/**
 * @brief 롤에 감긴 와이어의 길이와 회전량 간의 변환을 계산하는 클래스
 *
 * RollWireCalculator는 와이어가 롤에 연속적으로 감길 때, 와이어의 길이와
 * 롤의 회전량(각도) 사이의 관계를 계산합니다. 와이어가 감기면서 롤의 유효
 * 반지름이 증가하는 것을 고려한 연속 증가 모델을 사용합니다.
 *
 * 주요 특징:
 * - 와이어 두께를 고려한 정확한 계산
 * - 연속 증가 모델: r(θ) = innerRadius + (θ/360) * wireThickness
 * - 양방향 변환 지원 (길이 ↔ 회전량)
 * - 역함수 관계 보장 (부동소수점 오차 범위 내)
 */
class RollWireCalculator {
private:
    double wireThickness;   // mm - 와이어 두께
    double innerRadius;     // mm - 롤의 내경 반지름

public:
    /**
     * @brief RollWireCalculator 생성자
     *
     * @param thickness 와이어의 두께 (mm, 0보다 커야 함)
     * @param radius 롤의 내경 반지름 (mm, 0보다 커야 함)
     * @throws std::invalid_argument thickness 또는 radius가 0 이하인 경우
     */
    RollWireCalculator(double thickness, double radius);

    /**
     * @brief 와이어 두께를 조회합니다
     *
     * @return double 와이어 두께 (mm)
     */
    double getWireThickness() const;

    /**
     * @brief 롤의 내경 반지름을 설정합니다
     *
     * @param radius 새로운 롤 내경 반지름 (mm, 0보다 커야 함)
     * @throws std::invalid_argument radius가 0 이하인 경우
     */
    void setInnerRadius(double radius);

    /**
     * @brief 롤의 내경 반지름을 조회합니다
     *
     * @return double 롤 내경 반지름 (mm)
     */
    double getInnerRadius() const;

    /**
     * @brief 주어진 와이어 길이에 해당하는 롤의 회전량을 계산합니다
     *
     * 와이어가 롤에 감기면서 유효 반지름이 증가하는 연속 증가 모델을 사용하여
     * 정확한 회전량을 계산합니다. 수치 적분 방법을 사용합니다.
     *
     * @param length 와이어 길이 (m, 미터, 0 이상이어야 함)
     * @return double 롤의 회전량 (도, degrees)
     * @throws std::invalid_argument length가 음수인 경우
     */
    double calculateRotationFromLength(double length) const;

    /**
     * @brief 주어진 롤의 회전량에 해당하는 와이어 길이를 계산합니다
     *
     * 와이어가 롤에 감기면서 유효 반지름이 증가하는 연속 증가 모델을 사용하여
     * 정확한 와이어 길이를 계산합니다. 적분 공식을 사용합니다.
     *
     * @param rotation 롤의 회전량 (도, degrees, 0 이상이어야 함)
     * @return double 와이어 길이 (m, 미터)
     * @throws std::invalid_argument rotation이 음수인 경우
     */
    double calculateLengthFromRotation(double rotation) const;
};

#endif // ROLLWIRECALCULATOR_H
