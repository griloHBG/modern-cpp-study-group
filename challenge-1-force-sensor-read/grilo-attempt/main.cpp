#include <iostream> // cerr, cout, endl
#include <fstream> // ifstream
#include <string> // string, getline
#include <iomanip> // setw
#include <vector> // vector
#include <array> // array
#include "../force_torque_to_file.h" // force_toque_to_file

enum ERROR_CODE {
    NO_SENSOR_LOG_FILE=0,
    COULD_NOT_OPEN_FILE=1,
};

struct Span {
    int start;
    int length;
};

/// Hold sensor relevant information
struct SensorData {
    
    /// each force data:    Fx     Fy     Fz
    std::vector<std::array<float, 3>> force{};
    
    /// each torque data:   Tx     Ty     Tz
    std::vector<std::array<float, 3>> torque{};
    
    /// holds calibration matrix (ROW MAJOR FORMAT), used for force and torque calculation from sensor's strain gage data
    std::array<float, 6*6> calibrationMatrix{};

};

enum SensorOpCode {
    SGData1 = 0x0,
    SGData2 = 0x1,
    MatrixData1 = 0x2,
    MatrixData2 = 0x3,
    MatrixData3 = 0x4,
    CountsPer = 0x7,
    Unit = 0x8,
};

enum MsgType {
    FROM_SENSOR,
    TO_SENSOR
};

/// Axis enumeration
enum Axis {
    /// Force in x direction
    Fx = 0x0,
    /// Force in y direction
    Fy = 0x1,
    /// Force in z direction
    Fz = 0x2,
    /// Torque in x direction
    Tx = 0x3,
    /// Torque in y direction
    Ty = 0x4,
    /// Torque in z direction
    Tz = 0x5,
};


/// Force Unit code enumeration
enum ForceUnitCode {
    /// libra-force
    lbf=1,
    /// Newton
    N=2,
    /// kilo-libra force
    klbf=3,
    /// kilo-Newton
    kN=4,
    /// kilo-gram force
    kgf=5,
    /// gram force
    gf=6,
};

/// Torque Unit code enumeration
enum TorqueUnitCode {
    /// libra-force-inch
    lbf_in=1,
    /// libra-force-feet
    lbf_ft=2,
    /// Newton-meter
    N_m=3,
    /// Newton-millimeter
    N_mm=4,
    /// kilo-gram-centimeter
    kgf_cm=5,
    /// kiilo-Newton-meter
    kN_m=6,
};

struct Unit {
    struct ForceUnit {
        // didn't work! but: https://en.cppreference.com/w/cpp/language/enum
        // using enum ForceUnitCode;
        enum ForceUnitCode force;
    
        ForceUnit& operator=(const enum ForceUnitCode fuc) {
            force = fuc;
            return *this;
        }
        
        std::string to_string() const {
            switch (force) {
                case ForceUnitCode::lbf:
                    return "lbf";
                    break;
                case ForceUnitCode::N:
                    return "N";
                    break;
                case ForceUnitCode::klbf:
                    return "klbf";
                    break;
                case ForceUnitCode::kN:
                    return "kN";
                    break;
                case ForceUnitCode::kgf:
                    return "kgf";
                    break;
                case ForceUnitCode::gf:
                    return "gf";
                    break;
            }
            
            return "";
        }
    } forceUnit;
    struct TorqueUnit {
        
        enum TorqueUnitCode toque;
    
        TorqueUnit& operator=(const enum TorqueUnitCode tuc) {
            toque = tuc;
            return *this;
        }
        
        std::string to_string() const {
            switch (toque) {
                case TorqueUnitCode::lbf_in:
                    return "lbf_in";
                    break;
                case TorqueUnitCode::lbf_ft:
                    return "lbf_ft";
                    break;
                case TorqueUnitCode::N_m:
                    return "N_m";
                    break;
                case TorqueUnitCode::N_mm:
                    return "N_mm";
                    break;
                case TorqueUnitCode::kgf_cm:
                    return "kgf_cm";
                    break;
                case TorqueUnitCode::kN_m:
                    return "kN_m";
                    break;
            }
    
            return "";
        }
        
    } torqueUnit;
};

//std::tuple<float, float, float, float, float, float> matrixTimesSgData(std::array<float, 6*6> matrix, std::array<int, 6> sgData)
std::array<float, 6> matrixTimesSgData(std::array<float, 6*6> matrix, std::array<int16_t, 6> sgData) {
    
    std::array<float, 6> ft{};
    
    float accumulate = 0;
    
    int row, col;
    
    for (row = 0; row < 6; ++row) {
        for(col = 0; col < 6; ++col) {
            accumulate += matrix[row * 6 + col] * sgData[col];
        }
        ft[row] = accumulate;
    }
    
    return ft;
}

int main(int argc, char **argv) {
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sensor-log.txt>" << std::endl;
        return ERROR_CODE::NO_SENSOR_LOG_FILE;
    }
    
    std::ifstream sensorLog{argv[1]};
    
    if (!sensorLog) {
        std::cerr << "File " << argv[1] << " could not e opened!" << std::endl;
        return ERROR_CODE::COULD_NOT_OPEN_FILE;
    }
    
    int lineCounter = 1;
    /*
    // post-increment
    for (std::string line; std::getline(sensorLog, line) && lineCounter++ < 5; ) {
        
        std::cout << std::setw(3) << lineCounter << ": " << line << std::endl;
        
    }
    
    sensorLog.seekg(0);
    
    lineCounter = 0;
    
    std::cout << std::endl;
    
    //pre-increment
    
    for (std::string line; std::getline(sensorLog, line) && ++lineCounter < 5; ) {
        
        std::cout << std::setw(3) << lineCounter << ": " << line << std::endl;
        
    }
     
     return 0;
     */
    
    /// Base Op Code for log reading
    constexpr int baseOpCode = 0x7F;
    
    /// Interface string span
    struct Span interfaceSpan{2, 4};
    
    /// CAN ID span
    struct Span canIdSpan{8, 3};
    
    /// Payload size span
    struct Span payloadSpan{15, 1};
    
    /// CAN message payloadSize span (the length is the MAXIMUM possible)
    struct Span canMessageSpan{19, 23};
    
    /// indicates if the current message was sent sensor -> BBB or BBB -> sensor. sensor is reactive so everything starts with a message from the BBB :)
    enum MsgType msgType{MsgType::TO_SENSOR};
    
    ///Data struct to hold all
    struct SensorData sensorData;
    
    /// CAN ID of the current message
    uint16_t canID;
    
    /// payloadSize size (amount of bytes)
    uint8_t payloadSize;
    
    /// operation opCode of a CAN message (sent or received from sensor)
    uint8_t opCode;
    
    /// strain gage data received from sensor (MUST be assembled from two subsequent messages)
    std::array<int16_t, 6> sgData{};
    
    /// auxiliary variable to store where I'm in the payloadSize part of the string
    int byteStringPos;
    
    /// auxiliary varible to store all bytes from the payloadSize of the message
    std::array<uint8_t, 8> byte{};
    
    /// unit
    struct Unit unit{};
    
    /// based on message from BBB, store the axis that was asked to assembly the calibration matrix
    enum Axis axis;
    
    /// counts per force and counts per torque
    unsigned int cpF, cpT;
    
    /// sorry.. I need i...
    int i;
    
    /// counters to run over the calibration matrix :)
    int row, col;
    
    /// an auxiliary variable to reinterpret matrix's float elements from the CAN messages
    int aux;
    
    /// auxiliary array to store force and torque after the matrix multiplication
    std::array<float, 6> ftAux;
    
    for (std::string line; std::getline(sensorLog, line); ++lineCounter) {
        
        canID = std::stoul(line.substr(canIdSpan.start, canIdSpan.length), nullptr, 16);
        payloadSize = std::stoul(line.substr(payloadSpan.start, payloadSpan.length), nullptr, 16);
        
        if( canID >> 4 != baseOpCode)
        {
            // not interested in it :)
            continue;
        }
        /*
        std::cout << std::setw(7) << std::right << lineCounter  << " | " << line.substr(interfaceSpan.start, interfaceSpan.length)
                  << " | " << line.substr(canIdSpan.start, canIdSpan.length)
                  << " | " << line.substr(payloadSpan.start, payloadSpan.length);
    
        if (line.substr(payloadSpan.start, payloadSpan.length) != "0") {
            std::cout << " | " << line.substr(canMessageSpan.start) << " | " << std::endl;
        }
        else {
            std::cout << " | " <<  std::endl;
        }
        */
        
        opCode = canID & 0xF;
        
        switch (opCode) {
            case SensorOpCode::SGData1:
                payloadSize == 0 ? msgType = MsgType::TO_SENSOR : msgType = MsgType::FROM_SENSOR; // TODO whatif error in message reception?
                
                if (msgType == MsgType::FROM_SENSOR) {
                    i=0;
                    //TODO make a function from it!
                    for(byteStringPos = canMessageSpan.start;
                        i < payloadSize;
                        byteStringPos += 3)
                    {
                        //TODO we do not need to care about the 2 first bytes tat are STATUS CODE! (at least not for the challenge :)
                        byte[i++] = std::stoul(line.substr(byteStringPos, 2), nullptr, 16);
                    }
    
                    sgData[0] = (byte[2] << 8) | byte[3];
                    sgData[2] = (byte[4] << 8) | byte[5];
                    sgData[4] = (byte[6] << 8) | byte[7];
                    // other way
                    // std::tie(sgData[0], sgData[2], sgData[4]) = std::tuple<int16_t, int16_t, int16_t>{(byte[2] << 8) & byte[3], (byte[4] << 8) & byte[5], (byte[6] << 8) & byte[7]};
                } else {
                    //do nothing!
                }
                break;
                
            case SensorOpCode::SGData2: // this is ALWAYS from the sensor
                msgType = MsgType::FROM_SENSOR;
                i=0;
                for(byteStringPos = canMessageSpan.start;
                    i < payloadSize;
                    byteStringPos += 3)
                {
                    //TODO we do not need to care about the 2 first bytes tat are STATUS CODE! (at least not for the challenge :)
                    byte[i++] = std::stoul(line.substr(byteStringPos, 2), nullptr, 16);
                }
        
                // my error: using & instead of |
                byte[6] = byte[7] = 0;
                sgData[1] = (byte[0] << 8) | byte[1];
                sgData[3] = (byte[2] << 8) | byte[3];
                sgData[5] = (byte[4] << 8) | byte[5];
        
                /*std::cout << std::setw(7) << std::right << lineCounter  << " | ";
                
                for(auto &e : sgData)
                {
                    std::cout << std::setw(8) << static_cast<int>(e);
                }
        
                std::cout << std::endl;*/
                
                sensorData.force.emplace_back();
                sensorData.torque.emplace_back();
                
                ftAux = matrixTimesSgData(sensorData.calibrationMatrix, sgData);
        
                sensorData.force[sensorData.force.size()-1][0] = ftAux[0];
                sensorData.force[sensorData.force.size()-1][1] = ftAux[1];
                sensorData.force[sensorData.force.size()-1][2] = ftAux[2];
                sensorData.torque[sensorData.torque.size()-1][0] = ftAux[3];
                sensorData.torque[sensorData.torque.size()-1][1] = ftAux[4];
                sensorData.torque[sensorData.torque.size()-1][2] = ftAux[5];
                /*
                std::cout << std::setw(7) << std::right << lineCounter  << " | ";
        
                for(auto& f: sensorData.force[sensorData.force.size()-1])
                {
                    std::cout << std::setw(12) << f;
                }
        
                for(auto& t: sensorData.torque[sensorData.torque.size() - 1])
                {
                    std::cout << std::setw(12) << t;
                }
                
                std::cout << std::endl;
                */
                break;
            case SensorOpCode::MatrixData1:
            case SensorOpCode::MatrixData2:
            case SensorOpCode::MatrixData3:
                payloadSize == 1 ? msgType = MsgType::TO_SENSOR : msgType = MsgType::FROM_SENSOR; // TODO whatif error in message reception?
                if (msgType == MsgType::FROM_SENSOR) {
                    i=0;
                    for(byteStringPos = canMessageSpan.start;
                        i < payloadSize;
                        byteStringPos += 3)
                    {
                        //TODO we do not need to care about the 2 first bytes tat are STATUS CODE! (at least not for the challenge :)
                        byte[i++] = std::stoul(line.substr(byteStringPos, 2), nullptr, 16);
                    }
                    
                    //CAN'T!
                    //std::copy(byte.cbegin(), byte.cend(), sensorData.calibrationMatrix[axis*6]);
                    
                    // if opcode is 0b0010 (2), then I will store byte[0,3] on matrix[axis][0] and byte[4,7] on matrix[axis][1]
                    // if opcode is 0b0011 (3), then I will store byte[0,3] on matrix[axis][2] and byte[4,7] on matrix[axis][3]
                    // if opcode is 0b0100 (4), then I will store byte[0,3] on matrix[axis][4] and byte[4,7] on matrix[axis][5]
                    
                    // each matrix response message, gives me 8 bytes (4 bytes for SG[i] and 4 bytes for SG[i+1])
                    // Each message containing a 4 bytes pair, gives 2 elements of the calibration matrix (each element is a 32 bits - 4 bytes - float(ing point) number)
                    
                    aux = byte[0] << 24u | byte[1] << 16u | byte[2] << 8u | byte[3];
                    sensorData.calibrationMatrix[axis*6 + (opCode - SensorOpCode::MatrixData1)*2 + 0] = reinterpret_cast<float&>(aux);

                    aux = byte[4] << 24u | byte[5] << 16u | byte[6] << 8u | byte[7];
                    sensorData.calibrationMatrix[axis*6 + (opCode - SensorOpCode::MatrixData1)*2 + 1] = reinterpret_cast<float&>(aux);
                    
                    if(axis == Axis::Fx ||axis == Axis::Fy || axis == Axis::Fz)
                    {
                        sensorData.calibrationMatrix[axis*6 + (opCode - SensorOpCode::MatrixData1)*2 + 0] *= 1.f/cpF;
                        sensorData.calibrationMatrix[axis*6 + (opCode - SensorOpCode::MatrixData1)*2 + 1] *= 1.f/cpF;
                    }
                    else if((axis == Axis::Tx ||axis == Axis::Ty || axis == Axis::Tz))
                    {
                        sensorData.calibrationMatrix[axis*6 + (opCode - SensorOpCode::MatrixData1)*2 + 0] *= 1.f/cpT;
                        sensorData.calibrationMatrix[axis*6 + (opCode - SensorOpCode::MatrixData1)*2 + 1] *= 1.f/cpT;
                    }
                    
                    if(axis == Axis::Tz && opCode == 0x4)
                    {
                        std::cout << std::setw(7) << std::right << "calibration matrix" << std::endl;
    
                        for(row = 0; row < 6; row++) {
                            
                            std::cout << std::setw(7) << " " <<  " | ";
                            
                            for (col = 0; col < 6; col++) {
                                std::cout << std::setw(12) << sensorData.calibrationMatrix[row*6+col] << " ";
                            }
                            
                            std::cout << std::endl;
                        }
                    }
                    
                } else {
                    byteStringPos = canMessageSpan.start;
                    axis = static_cast<enum Axis>(std::stoul(line.substr(byteStringPos, 2), nullptr, 16));
                }
                break;
            case SensorOpCode::Unit:
                payloadSize == 0 ? msgType = MsgType::TO_SENSOR : msgType = MsgType::FROM_SENSOR;
                if (msgType == MsgType::FROM_SENSOR) {
                    
                    byteStringPos = canMessageSpan.start;
                    unit.forceUnit = static_cast<enum ForceUnitCode>(std::stoul(line.substr(byteStringPos, 2), nullptr, 16));
                    
                    std::cout << "Force unit: " << unit.forceUnit.to_string() << std::endl;
                    
                    byteStringPos += 3;
                    unit.torqueUnit = static_cast<enum TorqueUnitCode>(std::stoul(line.substr(byteStringPos, 2), nullptr, 16));
    
                    std::cout << "Torque unit: " << unit.torqueUnit.to_string() << std::endl;
                    
                } else {
                    //do nothing!
                }
                break;
            case SensorOpCode::CountsPer:
                payloadSize == 0 ? msgType = MsgType::TO_SENSOR : msgType = MsgType::FROM_SENSOR;
                if (msgType == MsgType::FROM_SENSOR) {
                    i=0;
                    for(byteStringPos = canMessageSpan.start;
                        i < payloadSize;
                        byteStringPos += 3)
                    {
                        //TODO we do not need to care about the 2 first bytes tat are STATUS CODE! (at least not for the challenge :)
                        byte[i++] = std::stoul(line.substr(byteStringPos, 2), nullptr, 16);
                    }
                    
                    cpF = byte[0] << 24u | byte[1] << 16u | byte[2] << 8u | byte[3];
                    cpT = byte[4] << 24u | byte[5] << 16u | byte[6] << 8u | byte[7];
                    
                    std::cout << "Counts per Force: "  << cpF << std::endl;
                    std::cout << "Counts per Torque: " << cpT << std::endl;
    
                } else {
                    // do nothing!
                }
                break;
            default:
                continue;
        }
    }
    
    to_file::forceTorqueToFile(sensorData.calibrationMatrix, sensorData.force, sensorData.torque);
}
