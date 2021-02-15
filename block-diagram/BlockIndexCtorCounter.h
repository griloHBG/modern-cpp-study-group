//
// Created by grilo on 15/02/2021.
//

#ifndef BLOCKDIAGRAMBUILDER_BLOCKINDEXCTORCOUNTER_H
#define BLOCKDIAGRAMBUILDER_BLOCKINDEXCTORCOUNTER_H


class BlockIndexCtorCounter {
protected:
    BlockIndexCtorCounter() {
        ++BlockIndexCtorCounter::blockIndexCounter;
    }
    ~BlockIndexCtorCounter() {
        --BlockIndexCtorCounter::blockIndexCounter;
    }
protected:
    static unsigned int blockIndexCounter;
};

unsigned int BlockIndexCtorCounter::blockIndexCounter = 0;

#endif //BLOCKDIAGRAMBUILDER_BLOCKINDEXCTORCOUNTER_H
