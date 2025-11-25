#include "RollWireCalculator.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== 롤 내경 변경 예제 ===" << std::endl << std::endl;

    // 초기 설정: 와이어 두께 1mm, 롤 내경 반지름 50mm
    RollWireCalculator calculator(1.0, 50.0);

    double testLength = 1.0;  // 1m

    std::cout << "테스트 와이어 길이: " << testLength << " m" << std::endl << std::endl;

    // 1. 첫 번째 내경에서 계산
    std::cout << "1. 롤 내경 반지름: " << calculator.getInnerRadius() << " mm" << std::endl;
    double rotation1 = calculator.calculateRotationFromLength(testLength);
    std::cout << "   회전량: " << std::fixed << std::setprecision(2) << rotation1 << " 도" << std::endl;
    std::cout << std::endl;

    // 2. 내경을 30mm로 변경
    calculator.setInnerRadius(30.0);
    std::cout << "2. 롤 내경 반지름을 " << calculator.getInnerRadius() << " mm로 변경" << std::endl;
    double rotation2 = calculator.calculateRotationFromLength(testLength);
    std::cout << "   회전량: " << std::fixed << std::setprecision(2) << rotation2 << " 도" << std::endl;
    std::cout << "   증가량: " << std::fixed << std::setprecision(2) << (rotation2 - rotation1) << " 도" << std::endl;
    std::cout << std::endl;

    // 3. 내경을 100mm로 변경
    calculator.setInnerRadius(100.0);
    std::cout << "3. 롤 내경 반지름을 " << calculator.getInnerRadius() << " mm로 변경" << std::endl;
    double rotation3 = calculator.calculateRotationFromLength(testLength);
    std::cout << "   회전량: " << std::fixed << std::setprecision(2) << rotation3 << " 도" << std::endl;
    std::cout << "   감소량: " << std::fixed << std::setprecision(2) << (rotation1 - rotation3) << " 도" << std::endl;
    std::cout << std::endl;

    // 4. 결론
    std::cout << "결론:" << std::endl;
    std::cout << "   내경이 작을수록 같은 길이를 감을 때 더 많이 회전합니다." << std::endl;
    std::cout << "   내경 30mm: " << std::fixed << std::setprecision(2) << rotation2 << " 도" << std::endl;
    std::cout << "   내경 50mm: " << std::fixed << std::setprecision(2) << rotation1 << " 도" << std::endl;
    std::cout << "   내경 100mm: " << std::fixed << std::setprecision(2) << rotation3 << " 도" << std::endl;
    std::cout << std::endl;

    return 0;
}
