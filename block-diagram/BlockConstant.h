//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
#define BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H

/// A block that represents a constant signal of type SignalType
/// \tparam SignalType variable type of the constant signal
template <typename SignalType>
class BlockConstant: public BlockBase<SignalType, 0, 1> {
    
    using LocalBlockBase=BlockBase<SignalType, 0, 1>;
    
public:
    /// Constructor for a block that outputs a constant signal of type SignalType
    /// \param value
    explicit BlockConstant(SignalType value): value{value}{
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
    /// helper function to print block's name
    void printClassName() {
        PRINT_CLASS_NAME(*this);
        std::cerr << "\t" << this->getName() << std::endl;
    };
#endif //DEBUG

    /// Constant value that this block outputs in it's output signal
    SignalType value;
};
#endif //BLOCKDIAGRAMBUILDER_BLOCKCONSTANT_H
