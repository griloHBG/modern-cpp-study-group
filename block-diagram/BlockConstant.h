//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
#define BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H

template <typename SignalType>
class BlockConstant: public BlockBase<SignalType, 0, 1> {
    
    using LocalBlockBase=BlockBase<SignalType, 0, 1>;
    
public:
    BlockConstant(SignalType value): value{value}{
        this->blockFunctions[0] = [this](){
#ifdef DEBUG
            std::cerr << "\t\tcalling function 0 on block " << this->getName() << std::endl;
#endif // DEBUG
            return this->value;
        };
        
        this->setupBlockFunctions();

#ifdef DEBUG
        printClassName();
#endif //DEBUG
    }
private:

#ifdef DEBUG
    void printClassName() {
        PRINT_CLASS_NAME(*this);
        std::cerr << "\t" << this->getName() << std::endl;
    };
#endif //DEBUG

    
    SignalType value;
};
#endif //BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
