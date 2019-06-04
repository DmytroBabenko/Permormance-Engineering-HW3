//
// Created by Dmytro Babenko on 2019-06-04.
//

#ifndef HW3_INTEGRALIMAGE_H
#define HW3_INTEGRALIMAGE_H


#include <cstdint>
#include <vector>

class IntegralImage {

public:

    static void calcPixelsSingle(std::vector<uint8_t>& data, size_t width, size_t height);
    static void calcPixelsMulti(std::vector<uint8_t>& data, size_t width, size_t height, size_t numThreads);

private:
    static void calcSumHorizontal(std::vector<uint8_t>& data, size_t rowIndex, size_t width);
    static void calcSumVertical(std::vector<uint8_t>& data, size_t colIndex, size_t width, size_t height);



};


#endif //HW3_INTEGRALIMAGE_H
