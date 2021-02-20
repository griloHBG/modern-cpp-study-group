#include <iostream>
#include "BlockSum.h"
#include "BlockConstant.h"

#ifdef DEBUG
    #include "utils_debug.h"
#endif //DEBUG

int main() {
    
    BlockConstant<int> bc1{1};
    BlockConstant<int> bc2{2};
    BlockConstant<int> bc5{5};
    
    BlockSum<int, 2> sum1;
    BlockSum<int, 2> sum2;
    
    sum1.input<0>().connect(bc1.output<0>());
    sum1.input<1>().connect(bc2.output<0>());
    
    sum2.input<0>().connect(sum1.output<0>());
    sum2.input<1>().connect(bc5.output<0>());
    
    BlockSum<int, 3> sum3;
    
    sum3.input<0>().connect(sum1.output<0>());
    sum3.input<1>().connect(sum2.output<0>());
    sum3.input<2>().connect(bc5.output<0>());
    
    std::cout << sum3.output<0>().get() << std::endl; //Ok : there is no loop to evaluate sum3's output
    
    //Uncomment the following comment block to see the infinity loop occuring
    /*
    BlockSum<int, 2> sum4;
    BlockSum<int, 2> sum5;
    
    sum4.input<0>().connect(sum3.output<0>());
    sum4.input<1>().connect(sum5.output<0>());
    
    sum5.input<0>().connect(sum4.output<0>());
    sum5.input<1>().connect(bc5.output<0>());
    
    std::cout << sum5.output<0>().get() << std::endl;
    */
    
    BlockConstant<std::string> bcEae{"eae"};
    BlockConstant<std::string> bcXablau{" xablau"};
    
    BlockSum<std::string, 2> ssum;
    
    ssum.input<0>().connect(bcEae.output<0>());
    ssum.input<1>().connect(bcXablau.output<0>());
    
    std::cout << ssum.output<0>().get() << std::endl;
    
    return 0;
}
