#include "RollWireCalculator.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== RollWireCalculator 기본 사용 예제 ===" << std::endl << std::endl;

    // 1. RollWireCalculator 객체 생성
    // 와이어 두께: 1mm, 롤 내경 반지름: 50mm
    RollWireCalculator calculator(1.0, 50.0);

    std::cout << "1. 초기 설정" << std::endl;
    std::cout << "   와이어 두께: " << calculator.getWireThickness() << " mm" << std::endl;
    std::cout << "   롤 내경 반지름: " << calculator.getInnerRadius() << " mm" << std::endl;
    std::cout << std::endl;

    // 2. 길이 → 회전량 변환
    std::cout << "2. 길이 → 회전량 변환" << std::endl;
    double length = 1.0;  // 1m
    double rotation = calculator.calculateRotationFromLength(length);
    std::cout << "   와이어 길이 " << length << " m를 감으면" << std::endl;
    std::cout << "   롤이 " << std::fixed << std::setprecision(2) << rotation << " 도 회전합니다" << std::endl;
    std::cout << std::endl;

    // 3. 회전량 → 길이 변환
    std::cout << "3. 회전량 → 길이 변환" << std::endl;
    double targetRotation = 720.0;  // 2바퀴
    double calculatedLength = calculator.calculateLengthFromRotation(targetRotation);
    std::cout << "   롤을 " << targetRotation << " 도 회전시키면" << std::endl;
    std::cout << "   와이어가 " << std::fixed << std::setprecision(3) << calculatedLength << " m 감깁니다" << std::endl;
    std::cout << std::endl;

    // 4. 역변환 검증
    std::cout << "4. 역변환 검증" << std::endl;
    double testLength = 0.5;  // 0.5m
    double convertedRotation = calculator.calculateRotationFromLength(testLength);
    double backToLength = calculator.calculateLengthFromRotation(convertedRotation);
    std::cout << "   원래 길이: " << testLength << " m" << std::endl;
    std::cout << "   변환된 회전량: " << std::fixed << std::setprecision(2) << convertedRotation << " 도" << std::endl;
    std::cout << "   다시 변환된 길이: " << std::fixed << std::setprecision(6) << backToLength << " m" << std::endl;
    std::cout << "   오차: " << std::scientific << std::setprecision(2) << (testLength - backToLength) << " m" << std::endl;
    std::cout << std::endl;

    return 0;
}
