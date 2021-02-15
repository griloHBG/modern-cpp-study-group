//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKBASE_H
#define BLOCKDIAGRAMBUILDER_BLOCKBASE_H

#include <array>
#include "Signal.h"
#include <numeric>
#include <functional>

template <typename SignalType, int inputNumber, int outputNumber>
class BlockBase {
    
    using BlockFunction = std::function<SignalType(void)>;
    
public:
    
    BlockBase() = default;
    
    template<int index>
    InputSignal<SignalType>& input() {
        return inputs.at(index);
    }
    
    template<int index>
    OutputSignal<SignalType>& output() {
        if(thisBlockIsReady) {
            return outputs.at(index);
        } else {
            throw std::runtime_error("function setupBlockFunctions was not called!");
        }
    }
    
protected:
    std::array<InputSignal<SignalType>, inputNumber> inputs;
    std::array<OutputSignal<SignalType>, outputNumber> outputs;
    
    std::array<BlockFunction, outputNumber> blockFunctions;
    
    void setupBlockFunctions(){
        for(int i = 0; i < outputNumber; ++i) {
            outputs[i] = OutputSignal<SignalType>(blockFunctions[i]);
        }
        thisBlockIsReady = true;
    }
    
private:
    bool thisBlockIsReady = false;
};

#endif //BLOCKDIAGRAMBUILDER_BLOCKBASE_H
