//
// Created by grilo on 23/01/2021.
//

#ifndef FT_SENSOR_READ_FORCE_TORQUE_OUTPUT_H
#define FT_SENSOR_READ_FORCE_TORQUE_OUTPUT_H

#include <fstream> // ofstream
#include <vector> // vector
#include <array> // array
#include <iomanip> // setw, setprecision
#include <sstream> // stringstream
#include <type_traits> // is_same

namespace to_file {
    
    // stuff in this anonymous namespace is not accessible from outside of to_file :)
    namespace {
        const int precision{10};
        const int width{15};
    
        const std::stringstream
        headerSS() {
        
            std::stringstream header{};
        
            header << std::setw(width) << "Fx"
                   << std::setw(width) << "Fy"
                   << std::setw(width) << "Fz"
                   << std::setw(width) << "Tx"
                   << std::setw(width) << "Ty"
                   << std::setw(width) << "Tz" << std::endl;
        
            return header;
        }
    
        template<typename M>
        std::stringstream
        matrix2sstream(const M &matrix) {
        
            std::stringstream ss{};
        
            if constexpr(std::is_same<M, std::array<std::array<float, 6>, 6>>::value) {
                // an actual matrix
            
                for (const auto &r: matrix) {
                    for (const auto &e: r) {
                        ss << std::setw(width) << std::setprecision(precision) << e;
                    }
                
                    ss << std::endl;
                }
            
            
            }
            else if constexpr(std::is_same<M, std::array<float, 6 * 6>>::value) {
                // one-dimensional-ROW_MAJOR matrix representation
            
                int row, col;
            
                for (row = 0; row < 6; ++row) {
                    for (col = 0; col < 6; ++col) {
                        ss << std::setw(width) << std::setprecision(precision) << matrix[row * 6 + col];
                    }
                
                    ss << std::endl;
                }
            }
        
            return ss;
        }
    
        std::stringstream
        ft2sstream(const std::vector<std::array<float, 6>> &ft) {
        
            std::stringstream ss{};
        
            for (const auto &r: ft) {
                for (const auto &e: r) {
                    ss << std::setw(width) << std::setprecision(precision) << e;
                }
                ss << std::endl;
            }
        
            return ss;
        }
    
        std::stringstream
        ft2sstream(const std::vector<std::array<float, 3>> &f,
                   const std::vector<std::array<float, 3>> &t) {
        
            std::stringstream ss{};
        
            int i, j;
        
            for (i = 0; i < (int) f.size(); ++i) {
            
                for (j = 0; j < 3; ++j) {
                    ss << std::setw(width) << std::setprecision(precision) << f[i][j];
                }
            
                for (j = 0; j < 3; ++j) {
                    ss << std::setw(width) << std::setprecision(precision) << t[i][j];
                }
            
                ss << std::endl;
            }
        
            return ss;
        }
    
        std::ofstream createOutputFile() {
            std::ofstream outputFile{"output.txt"};
        
            if (!outputFile) {
                throw std::runtime_error("Couldn't open output.txt file!");
            }
        
            return outputFile;
        }
    
        void forceToqueToFile_internal(std::stringstream& ftSS, std::stringstream& matrixSS) {
            
            std::ofstream outputFile{createOutputFile()};
            
            outputFile << "calibration matrix" << std::endl << matrixSS.rdbuf() << headerSS().rdbuf() <<  ftSS.rdbuf();
            matrixSS.seekp(0);
            ftSS.seekp(0);
            std::cout << "calibration matrix" << std::endl << matrixSS.rdbuf() << headerSS().rdbuf() <<  ftSS.rdbuf();
        }
        
    }
    
    ///
    /// \param matrix 6 arrays with 6 floats each
    /// \param ft a vector of Fx, Fy, Fz, Tx, Ty, Tz
    template<typename M, typename FT>
    void forceTorqueToFile(const M& matrix, FT& ft) {
        
        auto ftSS = ft2sstream(ft);
        
        auto matrixSS = matrix2sstream(matrix);
        
        forceToqueToFile_internal(ftSS, matrixSS);
    }
    
    ///
    /// \param matrix in row-major order
    /// \param ft a vector of Fx, Fy, Fz, Tx, Ty, Tz
    template<typename M, typename F, typename T>
    void forceTorqueToFile(const M& matrix, const F& f, const T& t) {
        
        auto ftSS = ft2sstream(f, t);
        
        auto matrixSS = matrix2sstream(matrix);
        
        forceToqueToFile_internal(ftSS, matrixSS);
    }
    
}

#endif //FT_SENSOR_READ_FORCE_TORQUE_OUTPUT_H
