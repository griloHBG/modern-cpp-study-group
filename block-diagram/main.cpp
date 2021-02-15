#include <iostream>
#include "BlockSum.h"
#include "BlockConstant.h"

int main() {
    
    BlockConstant<int> bc1{1};
    BlockConstant<int> bc2{2};
    BlockConstant<int> bc5{5};
    
    BlockSum<int, 3> sum1;
    BlockSum<int, 2> sum2;
    
    sum1.input<0>().connect(bc1.output<0>());
    sum1.input<1>().connect(bc2.output<0>());
    
    sum2.input<0>().connect(sum1.output<0>());
    sum2.input<1>().connect(bc5.output<0>());
    
    BlockSum<int, 3> sum3;
    
    sum3.input<0>().connect(sum1.output<0>());
    sum3.input<1>().connect(sum2.output<0>());
    sum3.input<2>().connect(bc5.output<0>());
    
    std::cout << sum3.output().get() << std::endl;
    
    return 0;
}
