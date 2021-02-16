//
// Created by grilo on 15/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_UTILS_DEBUG_H
    #define BLOCKDIAGRAMBUILDER_UTILS_DEBUG_H
    #ifdef DEBUG
        #include <iostream>
        #include <cxxabi.h>
        /// macro to print variable's name
        #define quote(x) #x
        
        //yes-op :p
        /// macro to print the variable's class name
        #define PRINT_CLASS_NAME(obj) { \
            int status;                                                                 \
            char * demangled = abi::__cxa_demangle(typeid(obj).name(), 0, 0, &status);  \
            std::cerr << demangled << ' ' << quote(obj) << '\t';                        \
            free(demangled);                                                            \
        }
    #else
        //no-op
        #define PRINT_CLASS_NAME(obj) (void)0
    #endif //DEBUG
#endif //BLOCKDIAGRAMBUILDER_UTILS_DEBUG_H
