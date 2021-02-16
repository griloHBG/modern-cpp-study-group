//
// Created by grilo on 15/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKINDEXCTORCOUNTER_H
#define BLOCKDIAGRAMBUILDER_BLOCKINDEXCTORCOUNTER_H

/// Simple global block counter (and used as index generator)
class BlockIndexCtorCounter {
protected:
    /// Constructor that increments the counter
    BlockIndexCtorCounter() {
        ++BlockIndexCtorCounter::blockIndexCounter;
    }
    
    /// Destructor that decrements the counter
    ~BlockIndexCtorCounter() {
        --BlockIndexCtorCounter::blockIndexCounter;
    }
protected:
    /// The (static) block counter (declaration)
    static unsigned int blockIndexCounter;
};
/// The (static) block counter (initialisation)
unsigned int BlockIndexCtorCounter::blockIndexCounter = 0;

#endif //BLOCKDIAGRAMBUILDER_BLOCKINDEXCTORCOUNTER_H
