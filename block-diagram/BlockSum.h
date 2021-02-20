//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKSUM_H
#define BLOCKDIAGRAMBUILDER_BLOCKSUM_H

#include <array>
#include "BlockBase.h"
#include <numeric>

/// Block used to sum signals of type Signal Type
/// \tparam SignalType Type of signals to be summed up and also output
/// \tparam inputNumber Amount of input signals (to be summed up)
template <typename SignalType, int inputNumber>
class BlockSum: public BlockBase<SignalType, inputNumber, 1> {
    using LocalBlockBase=BlockBase<SignalType, inputNumber, 1>;
    using LocalInputSignal=InputSignal<SignalType>;
public:
    
    /// Simple constructor
    BlockSum() {
        helperFunction = [](SignalType s, InputSignal<SignalType>& is) {return s + is.get();};
        this->blockFunctions[0] = [this](){
#ifdef DEBUG
            std::cerr << "\t\tcalling function 0 on block " << this->getName() << std::endl;
#endif // DEBUG
            return std::accumulate(begin(this->inputs),
                                  end(this->inputs),
                                  SignalType{},
                                  this->helperFunction);
        };
        
        this->setupBlockFunctions();

#ifdef DEBUG
        printClassName();
#endif //DEBUG
    }
    
#ifdef DEBUG
    /// helper function to get the block's name
    void printClassName() {
        PRINT_CLASS_NAME(*this);
        std::cerr << "\t" << this->getName() << std::endl;
    };
#endif //DEBUG
private:
    /// helper lambda function to help on the accumulate call
    std::function<SignalType(SignalType, InputSignal<SignalType>&)> helperFunction;
};


#endif //BLOCKDIAGRAMBUILDER_BLOCKSUM_H
