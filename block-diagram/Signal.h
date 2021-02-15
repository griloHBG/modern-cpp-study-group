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
public:
    void setName(const std::string& name) {
        signalName = name;
    };
    std::string getName() {
        return signalName;
    };
protected:
    explicit SignalBase(std::string name) : signalName(std::move(name)) {}
    virtual ~SignalBase() = default;
private:
    std::string signalName;
    virtual SignalType get() = 0;
};

template<typename SignalType>
class InputSignal;

template<typename SignalType>
class OutputSignal : public SignalBase<SignalType> {
    using BlockFunction = std::function<SignalType(void)>;
    using LocalInputSignal = InputSignal<SignalType>;
    
public:
    
    OutputSignal() :SignalBase<SignalType>(""), blockFunctionPtr(nullptr) {}
    
    explicit OutputSignal(const std::string& name) : SignalBase<SignalType>(name), blockFunctionPtr(nullptr) {};
    
    explicit OutputSignal(const std::string& name, BlockFunction& blockFunction) : SignalBase<SignalType>(name), blockFunctionPtr(std::make_shared<BlockFunction>(blockFunction)) {}
    
    SignalType get() {
        return (*blockFunctionPtr)();
    }
    
    void connect(LocalInputSignal& is) {
        is.connect(this);
    }
    
    ~OutputSignal() = default;
    
private:
    std::shared_ptr<BlockFunction> blockFunctionPtr{};
};


template<typename SignalType>
class InputSignal : public SignalBase<SignalType> {
    using LocalOutputSignal = OutputSignal<SignalType>;
public:
    InputSignal() : SignalBase<SignalType>(""), outputSignal(nullptr) {}
    InputSignal(const std::string& name) : SignalBase<SignalType>(name), outputSignal(nullptr) {}
    
    [[nodiscard]] SignalType get() {
#ifdef DEBUG
        std::cerr << "inputSignal " << this->getName() << " calling outputSignal " << getSignalConnectionName() << std::endl;
#endif //DEBUG
        return outputSignal->get();
    }
    
    void connect(LocalOutputSignal& os) {
        outputSignal = std::make_shared<LocalOutputSignal>(os);
    }
    
    //TODO not so useful?
    std::string getSignalConnectionName() const {
        return outputSignal->getName();
    }
    
    ~InputSignal() = default;

private:
    std::shared_ptr<LocalOutputSignal> outputSignal{};
};

#endif //BLOCKDIAGRAMBUILDER_SIGNAL_H
