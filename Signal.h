//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_SIGNAL_H
#define BLOCKDIAGRAMBUILDER_SIGNAL_H

#include <vector>
#include <memory>

template<typename SignalType>
class Signal {

private:
    [[nodiscard]] virtual SignalType get() const = 0;

};

template<typename SignalType>
class OutputSignal : public Signal<SignalType> {
public:
    [[nodiscard]] SignalType get() const {
        return this->value;
    }
};


template<typename SignalType>
class InputSignal : public Signal<SignalType> {
    using LocalOutputSignal = OutputSignal<SignalType>;
    
public:
    void connect(const LocalOutputSignal& os) {
        outputSignal = std::make_shared<LocalOutputSignal>(os);
    }
    
    [[nodiscard]] virtual SignalType get() const {
        return outputSignal->get();
    }

private:
    std::shared_ptr<const LocalOutputSignal> outputSignal{};
};
#endif //BLOCKDIAGRAMBUILDER_SIGNAL_H
