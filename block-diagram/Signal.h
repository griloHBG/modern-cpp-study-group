//
// Created by grilo on 11/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_SIGNAL_H
#define BLOCKDIAGRAMBUILDER_SIGNAL_H

#include <vector>
#include <memory>
#include <functional>

/// Base class for Signals
/// \tparam SignalType variable type of the signal
template<typename SignalType>
class SignalBase {
public:
    /// Set signal's name
    /// \param name name of the signal
    void setName(const std::string& name) {
        signalName = name;
    };
    
    /// Set signal's name
    /// \param name name of the signal
    void setName(std::string&& name) {
        signalName = std::move(name);
    };
    
    /// Retrieve signal's name
    const std::string& getName() {
        return signalName;
    };
    
protected:
    
    /// Protected constructor for Signal Base so only derived classes can call this
    /// \param name Signal's name
    explicit SignalBase(const std::string& name) : signalName(name) {}
    
    /// Protected constructor for Signal Base so only derived classes can call this
    /// \param name Signal's name
    explicit SignalBase(std::string&& name) : signalName(std::move(name)) {}
    
    /// virtual destructor for avoid the "slicing" deletion of its derived classes
    virtual ~SignalBase() = default;
private:
    /// Signal's name
    std::string signalName;
    /// Virtual function get() should be implemented by its derived classes
    virtual SignalType get() = 0;
};

template<typename SignalType>
class InputSignal;

/// Output signal class. Its get function member triggers evaluation of everything that is connected directly or indirectly to this
/// \tparam SignalType variable type of the signal
template<typename SignalType>
class OutputSignal : public SignalBase<SignalType> {
    using BlockFunction = std::function<SignalType(void)>;
    using LocalInputSignal = InputSignal<SignalType>;
    
public:
    
    /// Basic constructor
    OutputSignal() :SignalBase<SignalType>(std::string{""}), blockFunctionPtr(nullptr) {}
    
    /// Constructor that receives a name for the OutputSignal
    /// \param name output signal's name
    explicit OutputSignal(const std::string& name) : SignalBase<SignalType>(name), blockFunctionPtr(nullptr) {};
    
    /// Constructor that receives a name and block function for the output
    /// \param name output signal's name
    /// \param blockFunction function to which this signal is connect
    explicit OutputSignal(std::string name, BlockFunction& blockFunction) : SignalBase<SignalType>(name), blockFunctionPtr(std::make_shared<BlockFunction>(blockFunction)) {}
    
    /// Function that makes everything come to life!
    /// \return output of the block function to which this signal is connect
    SignalType get() {
        return (*blockFunctionPtr)();
    }
    
    /// Conencting signals. The preferred way is to call connect from the input signal. This function was never tested!
    void connect(LocalInputSignal& is) {
        is.connect(this);
    }
    
    /// Local destructor
    ~OutputSignal() = default;
    
private:
    /// This guy hold the block function that was created inside the block and was assigned this this output signal
    std::shared_ptr<BlockFunction> blockFunctionPtr{};
};

/// Input signal class. Its get() member calls get() of the output signal to which it is connected. Input signals are really responsible for making connections
/// \tparam SignalType variable type of the signal
template<typename SignalType>
class InputSignal : public SignalBase<SignalType> {
    using LocalOutputSignal = OutputSignal<SignalType>;
public:
    /// Basic constructor
    InputSignal() : SignalBase<SignalType>(""), outputSignal(nullptr) {}
    
    /// Constructor that receives a name for the InputSignal
    /// \param name output signal's name
    explicit InputSignal(const std::string& name) : SignalBase<SignalType>(name), outputSignal(nullptr) {}
    
    /// Calls output signal's get() to which this is connect
    /// \return
    [[nodiscard]] SignalType get() {
#ifdef DEBUG
        std::cerr << "inputSignal " << this->getName() << " calling outputSignal " << getSignalConnectionName() << std::endl;
#endif //DEBUG
        return outputSignal->get();
    }
    
    /// Creates a connection
    /// \param os output signa to connect to.
    void connect(LocalOutputSignal& os) {
        outputSignal = std::make_shared<LocalOutputSignal>(os);
    }
    
    //TODO not so useful?
    /// Get name of the output connection
    /// \return
    [[nodiscard]] std::string getSignalConnectionName() const {
        return outputSignal->getName();
    }
    
    /// Local destructor
    ~InputSignal() = default;

private:
    /// Shared pointer to the output signal this input signal is connected to
    std::shared_ptr<LocalOutputSignal> outputSignal{};
};

#endif //BLOCKDIAGRAMBUILDER_SIGNAL_H
