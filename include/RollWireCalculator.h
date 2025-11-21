#ifndef ROLLWIRECALCULATOR_H
#define ROLLWIRECALCULATOR_H

#include <stdexcept>

class RollWireCalculator {
private:
    double wireThickness;   // mm - 와이어 두께
    double innerRadius;     // mm - 롤의 내경 반지름

public:
    // 생성자: 와이어 두께와 롤 내경 반지름을 받음
    RollWireCalculator(double thickness, double radius);

    // 와이어 두께 조회
    double getWireThickness() const;

    // 롤 내경 반지름 설정
    void setInnerRadius(double radius);

    // 롤 내경 반지름 조회
    double getInnerRadius() const;

    // 길이 → 회전량 변환
    double calculateRotationFromLength(double length) const;

    // 회전량 → 길이 변환
    double calculateLengthFromRotation(double rotation) const;
};

#endif // ROLLWIRECALCULATOR_H
