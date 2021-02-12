#include <iostream>
#include "BlockSum.h"
#include "BlockConstant.h"

int main() {
    
    BlockConstant<int> bc1{1};
    BlockConstant<int> bc2{2};
    BlockConstant<int> bc5{5};
    
    BlockSum<int, 2> sum1{};
    BlockSum<int, 2> sum2{};
    
    sum1.input(0).connect(bc1.output());
    sum1.input(1).connect(bc2.output());
    
    sum2.input(0).connect(sum1.output());
    sum2.input(1).connect(bc5.output());
    
    std::cout << sum2.output().get() << std::endl;
    
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
