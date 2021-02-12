//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
#define BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H

template <typename SignalType>
class BlockConstant: public BlockBase<SignalType, 1, 0, 1> {
    
    using LocalBlockBase=BlockBase<SignalType, 1, 0, 1>;
    
public:
    BlockConstant(SignalType value): value{value}{}
    
    const OutputSignal<SignalType>& output() {
        return LocalBlockBase::output(0);
    }
    
    virtual SignalType operator()() {
        return value;
    }
    
private:
    SignalType value;
};
#endif //BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
