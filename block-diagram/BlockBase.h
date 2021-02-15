//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKBASE_H
#define BLOCKDIAGRAMBUILDER_BLOCKBASE_H

#include <array>
#include "Signal.h"
#include <numeric>
#include <functional>
#include <utility>
#include "BlockIndexCtorCounter.h"

#ifdef DEBUG
    #include "utils_debug.h"
#endif //DEBUG

template <typename SignalType, int inputNumber, int outputNumber>
class BlockBase : private BlockIndexCtorCounter {
    
    using BlockFunction = std::function<SignalType(void)>;
    
public:
    //TODO allow Block to have different SignalType between input and output
    BlockBase(): blockIndex(BlockIndexCtorCounter::blockIndexCounter) {};
    
    explicit BlockBase(std::string blockName) : blockName(std::move(blockName)), blockIndex(BlockIndexCtorCounter::blockIndexCounter) {}
    
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
            outputs[i] = OutputSignal<SignalType>(getName(), blockFunctions[i]);
            outputs[i].setName(getName());
        }
        
        for(auto& is : inputs) {
            is.setName(getName());
        }
        
        thisBlockIsReady = true;
    }
    
    [[nodiscard]] std::string getName() const {
        if (blockName.empty()) {
            return std::to_string(blockIndex);
        }
        else {
            return blockName;
        }
    }

private:
    bool thisBlockIsReady = false;
    
    const std::string blockName;
    
    const unsigned int blockIndex = 0;
};

#endif //BLOCKDIAGRAMBUILDER_BLOCKBASE_H
