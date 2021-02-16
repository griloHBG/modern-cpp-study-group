//
// Created by grilo on 16/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKMUX_H
#define BLOCKDIAGRAMBUILDER_BLOCKMUX_H


#include "BlockBase.h"
template <typename InputSignalType, typename OutputSignalType, int inputNumber>
class BlockMux : public BlockBase<InputSignalType, OutputSignalType, inputNumber, 1> {
    //using LocalBlockBase=BlockBase<InputSignalType, OutputSignalType, inputNumber, 1>;
    //using LocalInputSignal=InputSignal<InputSignalType>;

public:
    BlockMux() {
        this->blockFunctions[0] = [this](){
            for(int i = 0; i < inputNumber; ++i) {
                outputValue[i] = this->inputs[i].get();
            }
            
            return outputValue;
        };
        
        this->setupBlockFunctions();
    }

private:
    OutputSignalType outputValue;
};


#endif //BLOCKDIAGRAMBUILDER_BLOCKMUX_H
