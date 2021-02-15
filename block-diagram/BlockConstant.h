//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
#define BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H

template <typename SignalType>
class BlockConstant: public BlockBase<SignalType, 0, 1> {
    
    using LocalBlockBase=BlockBase<SignalType, 0, 1>;
    
public:
    BlockConstant(SignalType value): LocalBlockBase{}, value{value}{
        this->blockFunctions[0] = [this](){return this->value;};
        
        this->setupBlockFunctions();
    }
    
private:
    SignalType value;
};
#endif //BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
