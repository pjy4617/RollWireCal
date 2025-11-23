#ifndef ROLLWIRECALCULATOR_H
#define ROLLWIRECALCULATOR_H

class RollWireCalculator {
public:
  RollWireCalculator(double wireThickness, double innerRadius)
      : wireThickness(wireThickness), currentRadius(innerRadius) {}

  // 거리를 회전량(도)으로 변환
  // 단순화를 위해 현재 반지름을 기준으로 계산한다고 가정 (실제로는 적분 필요)
  double calculateRotation(double distance) {
    // 원주 = 2 * pi * r
    // 회전수 = 거리 / 원주
    // 회전각 = 회전수 * 360
    double circumference =
        2.0 * 3.14159265358979323846 * (currentRadius / 1000.0); // mm -> m 변환
    return (distance / circumference) * 360.0;
  }

  // 현재 반지름 조회 (mm)
  double getCurrentRadius() const { return currentRadius; }

private:
  double wireThickness;
  double currentRadius;
};

#endif // ROLLWIRECALCULATOR_H
