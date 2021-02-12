//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKBASE_H
#define BLOCKDIAGRAMBUILDER_BLOCKBASE_H

#include <array>
#include "Signal.h"
#include <numeric>
#include <functional>

template <typename SignalType, int functionNumber, int inputNumber, int outputNumber>
class BlockBase {

    friend class OutputSignal<SignalType>;
    
public:
    
    InputSignal<SignalType>& input(int index) {
        return inputs.at(index);
    }
    
    OutputSignal<SignalType>& output(int index) {
        return outputs.at(index);
    }
    
    virtual SignalType operator()() = 0;
    
protected:
    std::array<InputSignal<SignalType>, inputNumber> inputs{};
    std::array<OutputSignal<SignalType>, outputNumber> outputs{};
    std::array<std::function<OutputSignal<SignalType>(std::array<InputSignal<SignalType>,inputNumber>)>, functionNumber> bfa{};
};


#endif //BLOCKDIAGRAMBUILDER_BLOCKBASE_H
