//
// Created by Dmytro Babenko on 2019-06-04.
//

#ifndef HW3_REDUCER_H
#define HW3_REDUCER_H

#include "ThreadPool.h"

#include <cstdint>
#include <vector>

template <typename T>
class Reducer {
public:
    enum Type
    {
        SUM,
        MIN,
        UNDEFINED
    };

public:

    explicit Reducer(Type iType, size_t numThreads);

    T reduce(std::vector<T>& data);


private:
    void _reduceSingle(std::vector<T> &data, size_t start, size_t step, size_t end, size_t size);
    void _binaryReduceSum(std::vector<T >& data, int s0 = 1);
    size_t _calcOptimalChunkSize(size_t size);

private:
    const size_t NUM_THREADS;
    ThreadPool mThreadPool;
    Type type;
    std::function<T(T, T)> operation;

};


#endif //HW3_REDUCER_H
