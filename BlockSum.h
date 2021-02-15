//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKSUM_H
#define BLOCKDIAGRAMBUILDER_BLOCKSUM_H

#include <array>
#include "BlockBase.h"
#include <numeric>

template <typename SignalType, int inputNumber>
class BlockSum: public BlockBase<SignalType, inputNumber, 1> {
    using LocalBlockBase=BlockBase<SignalType, inputNumber, 1>;
    using LocalInputSignal=InputSignal<SignalType>;
public:
    
    BlockSum() {
        helperFunction = [](SignalType s, InputSignal<SignalType>& is) {return s + is.get();};
        this->blockFunctions[0] = [this](){return std::accumulate(begin(this->inputs),
                                        end(this->inputs),
                                        0,
                                        this->helperFunction);};
    
        this->setupBlockFunctions();
    }
private:
    std::function<SignalType(SignalType, InputSignal<SignalType>&)> helperFunction;
};


#endif //BLOCKDIAGRAMBUILDER_BLOCKSUM_H
