//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKSUM_H
#define BLOCKDIAGRAMBUILDER_BLOCKSUM_H

#include <array>
#include "BlockBase.h"
#include <numeric>

template <typename SignalType, int inputNumber>
class BlockSum: public BlockBase<SignalType, 1, inputNumber, 1> {
    using LocalBlockBase=BlockBase<SignalType, 1, inputNumber, 1>;
    using LocalInputSignal=InputSignal<SignalType>;
public:
    
    BlockSum() {
        this->bfa = {};
    }
    
    OutputSignal<SignalType>& output() {
        return LocalBlockBase::output(0);
    }
};


#endif //BLOCKDIAGRAMBUILDER_BLOCKSUM_H
