//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_SIGNAL_H
#define BLOCKDIAGRAMBUILDER_SIGNAL_H

#include <vector>
#include <memory>
#include <functional>

template<typename SignalType>
class SignalBase {
private:
    virtual SignalType get() = 0;
};

template<typename SignalType>
class InputSignal;

template<typename SignalType>
class OutputSignal : public SignalBase<SignalType> {
    using BlockFunction = std::function<SignalType(void)>;
    using LocalInputSignal = InputSignal<SignalType>;
    
public:
    
    OutputSignal() = default;
    
    OutputSignal(BlockFunction& bf) : bf(std::make_shared<BlockFunction>(bf)) {}
    
    SignalType get() {
        return (*bf)();
    }
    
    void connect(LocalInputSignal& is) {
        is.connect(this);
    }
    
private:
    std::shared_ptr<BlockFunction> bf{};
};


template<typename SignalType>
class InputSignal : public SignalBase<SignalType> {
    using LocalOutputSignal = OutputSignal<SignalType>;
public:
    
    [[nodiscard]] SignalType get() {
        return outputSignal->get();
    }
    
    void connect(LocalOutputSignal& os) {
        outputSignal = std::make_shared<LocalOutputSignal>(os);
    }

private:
    std::shared_ptr<LocalOutputSignal> outputSignal{};
};

#endif //BLOCKDIAGRAMBUILDER_SIGNAL_H
