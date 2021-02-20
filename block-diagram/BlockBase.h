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
/// BlockBase class base for all blocks (untill now)
/// \tparam SignalType the variable type of signal that will go into and out of the Block
/// \tparam inputNumber amount of input signals on this block
/// \tparam outputNumber amount of output signals on this block
template <typename InputSignalType, typename OutputSignalType, int inputNumber, int outputNumber>
class BlockBase : private BlockIndexCtorCounter {
    
    using BlockFunction = std::function<OutputSignalType(void)>;
    
public:
    /// Basic Constructor for BlockBase.
    /// Takes no argument. Its name will be the current counter index from BlockIndexCtorCounter
    /// setupBlockFunction should be called afterwards.
    BlockBase(): blockIndex(BlockIndexCtorCounter::blockIndexCounter) {};
    
    /// Constructor for BlockBase.
    /// \param blockName Block's name. It's index will be appended to this.
    explicit BlockBase(std::string blockName) : blockIndex(BlockIndexCtorCounter::blockIndexCounter), blockName(std::move(blockName) + std::to_string(blockIndex)) {}
    
    /// Access inputs by reference //TODO: should this output function be const const?
    /// \tparam index Index of the input (starting at zero)
    /// \return
    template<int index>
    InputSignal<InputSignalType>& input() {
        static_assert(index >= 0 && index < inputNumber);
        return inputs.at(index);
    }
    
    /// Access outputs by reference //TODO: should this input function be const const?
    /// \tparam index Index of the output (starting at zero)
    /// \return
    template<int index>
    OutputSignal<OutputSignalType>& output() {
        static_assert(index >= 0 && index < outputNumber);
        if(outputBlockFunctionSetupDone) {
            return outputs.at(index);
        } else {
            throw std::runtime_error("function setupBlockFunctions was not called!");
        }
    }

protected:
    /// Array of input signals for this block. These input signals calls connect to the output signals
    std::array<InputSignal<InputSignalType>, inputNumber> inputs;
    
    /// Array of input signals for this block. These output signals are given to connect to input signals
    std::array<OutputSignal<OutputSignalType>, outputNumber> outputs;
    
    /// Array of block functions for this block. There is one and only one function block for each output.
    std::array<BlockFunction, outputNumber> blockFunctions;
    
    /// Function that connects each block function to each output, index by index. Also sets output and input names as its block name, appending "_i" for input and "_o" for output
    void setupBlockFunctions(){
        for(int i = 0; i < outputNumber; ++i) {
            outputs[i] = OutputSignal<OutputSignalType>(getName(), blockFunctions[i]);
            outputs[i].setName(getName() + "_o");
        }
        
        for(auto& is : inputs) {
            is.setName(getName() + "_i");
        }
    
        outputBlockFunctionSetupDone = true;
    }
    
    /// Function to get block's name. It's appended with block's index
    /// \return
    [[nodiscard]] std::string getName() const {
        if (blockName.empty()) {
            return std::to_string(blockIndex);
        }
        else {
            return blockName;
        }
    }
    
    ///
    /// \return
    
    /// Function that returns if this block was already evaluated
    /// \tparam outputIndex index of output
    /// \return returns if the output at outputIndex was already evaluated
    template<int outputIndex>
    [[nodiscard]] bool getAlreadyEvaluated() const {
        return alreadyEvaluated[outputIndex];
    }
    
    /// Function that sets already evaluated to true
    template<int outputIndex>
    void setAlreadyEvaluated() {
        alreadyEvaluated[outputIndex] = true;
    }
    
    /// Value that holds last block evaluation
    OutputSignalType outputValue;

private:
    
    /// variable to hold whether each output was connected to each block function
    bool outputBlockFunctionSetupDone = false;
    
    /// variable to hold block's name
    const std::string blockName;
    
    /// variable to hold blocks "historical" index
    const unsigned int blockIndex = 0;
    
    /// variable to hold if block was already evaluated
    std::array<bool, outputNumber> alreadyEvaluated;
};

#endif //BLOCKDIAGRAMBUILDER_BLOCKBASE_H
