
#include "BMPImage.h"
#include "Reducer.hpp"
#include "ThreadPool.h"
#include "IntegralImage.h"


#include <iostream>
#include <vector>
#include <thread>
#include <numeric>


static const char* const imageFile = "../Lenna.bmp";
static constexpr size_t NUM_THREADS = 8;


void taskA()
{
    std::cout<<"Task a"<<std::endl;

    BMPImage<uint64_t> image(imageFile);
    if (!image.imageLoaded())
    {
        std::cerr<<"Image is not loaded"<<std::endl;
        return;
    }

    std::vector<uint64_t> rChannel = image.RCopy();

    auto start = std::chrono::system_clock::now();

    uint64_t singleSum = std::accumulate(rChannel.begin(), rChannel.end(), 0L);

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout<<"Single sum = "<<singleSum<<std::endl;
    std::cout<<"Single sum time calculating "<<elapsed.count()<<" μs"<<std::endl;
    std::cout<<std::endl;

    Reducer<uint64_t > reducer(Reducer<uint64_t >::SUM, NUM_THREADS);

    start = std::chrono::system_clock::now();

    uint64_t multSum = reducer.reduce(image.R());

    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout<<"Multithreading sum = "<<multSum<<std::endl;
    std::cout<<"Multithreading sum time calculating "<<elapsed.count()<<" μs"<<std::endl;
    std::cout<<std::endl;

    std::cout<<"Task a is completed successfully"<<std::endl;
    std::cout<<std::endl;

}


void taskB()
{
    std::cout<<"Task b"<<std::endl;

    BMPImage<uint8_t > image(imageFile);
    if (!image.imageLoaded())
    {
        std::cerr<<"Image is not loaded"<<std::endl;
        return;
    }

    std::vector<uint8_t> rChannel = image.RCopy();

    auto start = std::chrono::system_clock::now();

    auto minIter = std::min_element(rChannel.begin(), rChannel.end());
    uint8_t singleMin = *minIter;

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout<<"Single min = "<<(int)singleMin<<std::endl;
    std::cout<<"Single min time calculating "<<elapsed.count()<<" μs"<<std::endl;
    std::cout<<std::endl;

    Reducer<uint8_t > reducer(Reducer<uint8_t >::MIN, NUM_THREADS);

    start = std::chrono::system_clock::now();

    uint8_t multMin = reducer.reduce(image.R());

    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout<<"Multithreading min = "<<(int)multMin<<std::endl;
    std::cout<<"Multithreading min time calculating "<<elapsed.count()<<" μs"<<std::endl;
    std::cout<<std::endl;

    std::cout<<"Task b is completed successfully"<<std::endl;
    std::cout<<std::endl;

}


void taskC()
{
    std::cout<<"Task c"<<std::endl;

    BMPImage<uint8_t> image(imageFile);
    if (!image.imageLoaded())
    {
        std::cerr<<"Image is not loaded"<<std::endl;
        return;
    }

    const size_t channleSize = image.channelSize();

    std::vector<uint8_t> rChannel = image.RCopy();

    auto start = std::chrono::system_clock::now();

    IntegralImage::calcPixelsSingle(rChannel, image.Width(), image.Height());

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout<<"Single integrated pixels = "<<(int)rChannel[0]<<", "<<(int)rChannel[1]<<", ..., ";
    std::cout<<(int)rChannel[channleSize / 2]<<", "<<(int)rChannel[channleSize / 2 + 1]<<", ... ,";
    std::cout<<(int)rChannel[channleSize - 2]<<", "<<(int)rChannel[channleSize - 1]<<std::endl;
    std::cout<<"Single time calculating "<<elapsed.count()<<" μs"<<std::endl;
    std::cout<<std::endl;


    rChannel = image.RCopy();

    start = std::chrono::system_clock::now();

    IntegralImage::calcPixelsMulti(rChannel, image.Width(), image.Height(), NUM_THREADS);


    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout<<"Multithreading integrated pixels = "<<(int)rChannel[0]<<", "<<(int)rChannel[1]<<", ..., ";
    std::cout<<(int)rChannel[channleSize / 2]<<", "<<(int)rChannel[channleSize / 2 + 1]<<", ... ,";
    std::cout<<(int)rChannel[channleSize - 2]<<", "<<(int)rChannel[channleSize - 1]<<std::endl;
    std::cout<<"Multithreading time calculating "<<elapsed.count()<<" μs"<<std::endl;
    std::cout<<std::endl;

    std::cout<<"Task c is completed successfully"<<std::endl;
    std::cout<<std::endl;



}

int main()
{
    taskA();
    taskB();
    taskC();
    return 0;
}

