//
// Created by Dmytro Babenko on 2019-06-04.
//

#include "Reducer.h"

#include <future>


namespace
{
    template <typename T>
    std::function<T(T, T)> getOperationByType(typename Reducer<T>::Type type)
    {
        switch (type)
        {
            case Reducer<T>::SUM:
                return [](T a, T b) {return a + b;};
            case Reducer<T>::MIN:
                return [](T a, T b) {return a ? a > b : b;};
        }
        return nullptr;
    }
}


template <typename T>
Reducer<T>::Reducer(Type iType, size_t numThreads)
: NUM_THREADS(numThreads)
, mThreadPool(numThreads)
, type(iType)
{
    operation = getOperationByType<T>(type);
}

template <typename T>
T Reducer<T>::reduce(std::vector<T> &data)
{
    if (data.empty())
        return 0;

    const size_t size = data.size();

    if (NUM_THREADS >= size){
        _binaryReduceSum(data);
        return data.front();
    }

    const size_t chunkSize = _calcOptimalChunkSize(size);

    std::vector<std::future<void>> futures;
    for (int i = 0; i < size; i += chunkSize)
    {
        auto future = mThreadPool.addTaskToAsyncExec(&Reducer::_reduceSingle, this, std::ref(data), i, 1, i + chunkSize - 1, size);
        futures.push_back(std::move(future));
    }

    //wait until thread pool did recently added tasks
    for (auto& future : futures)
    {
        future.get();
    }

    _binaryReduceSum(data, chunkSize);

    return data.front();
}

template <typename T>
void Reducer<T>::_reduceSingle(std::vector<T> &data, size_t start, size_t step, size_t end, size_t size)
{
    for (size_t i = start + step; i <= end && i < size; i+= step)
    {
        data[start] = operation(data[start], data[i]);
    }
}

template <typename T>
void Reducer<T>::_binaryReduceSum(std::vector<T >& data, int s0 )
{
    const size_t size = data.size();
    for (int s = s0; s < size; s*= 2)
    {
        std::vector<std::future<void>> futures;
        for (int i = 0; i < size; i += 2*s)
        {
            auto future = mThreadPool.addTaskToAsyncExec(&Reducer::_reduceSingle, this, std::ref(data), i, s, i + s, size);
            futures.push_back(std::move(future));
        }

        //wait until thread pool did recently added tasks
        for (auto& future : futures)
        {
            future.get();
        }
    }
}

template <typename T>
size_t Reducer<T>::_calcOptimalChunkSize(size_t size) {
    assert(size > NUM_THREADS);

    size_t chunkSize = size / NUM_THREADS;

    if (size % NUM_THREADS == 0)
        return chunkSize;

    return chunkSize + 1;
}

