//
// Created by Dmytro Babenko on 2019-06-04.
//

#include "IntegralImage.h"

#include "ThreadPool.h"

void IntegralImage::calcPixelsSingle(std::vector<uint8_t> &data, size_t width, size_t height)
{
    for (size_t row = 0; row < height; ++row)
    {
        IntegralImage::calcSumHorizontal(data, row, width);
    }

    for (size_t col = 0; col < width; ++col)
    {
        IntegralImage::calcSumVertical(data, col, width, height);
    }
}

void IntegralImage::calcPixelsMulti(std::vector<uint8_t> &data, size_t width, size_t height, size_t numThreads)
{

    ThreadPool threadPool(numThreads);

    std::vector<std::future<void>> futures;

    for (size_t row = 0; row < height; ++row)
    {
        auto future = threadPool.addTaskToAsyncExec(IntegralImage::calcSumHorizontal, std::ref(data), row, width);
        futures.push_back(std::move(future));
    }

    for (auto& future : futures)
    {
        future.get();
    }

    futures.clear();


    for (size_t col = 0; col < width; ++col)
    {
        auto future = threadPool.addTaskToAsyncExec(IntegralImage::calcSumVertical, std::ref(data), col, width, height);
        futures.push_back(std::move(future));
    }

    for (auto& future : futures)
    {
        future.get();
    }

}

void IntegralImage::calcSumHorizontal(std::vector<uint8_t> &data, size_t rowIndex, size_t width)
{
    for (size_t i = rowIndex * width + 1; i < (rowIndex + 1) * width; ++i)
    {
        data[i] += data[i - 1];
    }

}

void IntegralImage::calcSumVertical(std::vector<uint8_t> &data, size_t colIndex, size_t width, size_t height)
{

    for (size_t j = 1; j < height; ++j)
    {
        size_t prevIdx = (j - 1) * width + colIndex;
        size_t idx = j * width + colIndex;

        data[idx] += data[prevIdx];
    }

}
