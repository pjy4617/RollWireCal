#include "RollWireCalculator.h"
#include <cmath>

RollWireCalculator::RollWireCalculator(double thickness, double radius)
    : wireThickness(thickness), innerRadius(radius) {
    if (thickness <= 0.0) {
        throw std::invalid_argument("Wire thickness must be positive");
    }
    if (radius <= 0.0) {
        throw std::invalid_argument("Inner radius must be positive");
    }
}

double RollWireCalculator::getWireThickness() const {
    return wireThickness;
}

void RollWireCalculator::setInnerRadius(double radius) {
    if (radius <= 0.0) {
        throw std::invalid_argument("Inner radius must be positive");
    }
    innerRadius = radius;
}

double RollWireCalculator::getInnerRadius() const {
    return innerRadius;
}

double RollWireCalculator::calculateRotationFromLength(double length) const {
    if (length < 0.0) {
        throw std::invalid_argument("Length must be non-negative");
    }

    if (length == 0.0) {
        return 0.0;
    }

    // 연속 증가 모델: r(θ) = innerRadius + (θ/360) × wireThickness
    // L = ∫[0→θ] r(t) × (2π/360) dt
    //   = (2π/360) × [innerRadius × θ + wireThickness × θ²/(2×360)]
    //
    // 이를 θ에 대해 정리하면 2차 방정식:
    // (wireThickness/720) × θ² + innerRadius × θ - L × (180/π) = 0
    //
    // 단위 변환: length는 m 단위, 내부 계산은 mm 단위
    double lengthMm = length * 1000.0;  // m → mm

    // 2차 방정식 계수: a×θ² + b×θ + c = 0
    double a = wireThickness / 720.0;
    double b = innerRadius;
    double c = -lengthMm * (180.0 / M_PI);

    // 판별식
    double discriminant = b * b - 4.0 * a * c;

    // 양수 해 선택 (물리적으로 의미있는 해)
    double theta = (-b + std::sqrt(discriminant)) / (2.0 * a);

    return theta;
}

double RollWireCalculator::calculateLengthFromRotation(double rotation) const {
    // 회전량이 0이면 길이도 0
    return 0.0;
}
