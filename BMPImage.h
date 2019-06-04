//
// Created by Dmytro Babenko on 2019-06-02.
//

#ifndef HW3_BMPIMAGE_H
#define HW3_BMPIMAGE_H

#include <string>
#include <vector>
#include <fstream>

template <typename T = uint8_t>
class BMPImage
{
public:
    explicit BMPImage(const std::string& file)
            : loaded(false)
            , width(0)
            , height(0)
    {
        _loadImage(file);
    }


    bool imageLoaded() const {return loaded; }

    std::vector<T>& R() {return r; }
    std::vector<T>& G() {return g; }
    std::vector<T>& B() {return b; }


    std::vector<T> RCopy() {return r; }
    std::vector<T> GCopy() {return g; }
    std::vector<T> BCopy() {return b; }

    uint32_t channelSize() const {return width * height; }

    uint32_t Width() const {return width;}
    uint32_t Height() const {return height;}


private:
    void _loadImage(const std::string& file)
    {
        loaded = false;
        std::ifstream input;
        input.open(file.c_str(), std::ios::binary);

        std::istreambuf_iterator<char > begin = std::istreambuf_iterator<char >(input);
        std::istreambuf_iterator<char > end = std::istreambuf_iterator<char>();

        std::vector<uint8_t> buffer(begin, end);

        if (buffer.size() <= HEADER_SIZE)
            return;


        width = *(int*)&buffer[18];
        height = *(int*)&buffer[22];

        const int channelSize  = width * height;

        const int size = CHANNELS * channelSize;
        if (buffer.size() != size + HEADER_SIZE)
            return;

        r.resize(channelSize);
        g.resize(channelSize);
        b.resize(channelSize);

        for (int i = HEADER_SIZE, j = 0; i < size && j < channelSize; i += 3, ++j)
        {
            r[j] = buffer[i + 2];
            g[j] = buffer[i+1];
            b[j] = buffer[i];
        }

        loaded = true;
    }

private:

    bool loaded;
    int width;
    int height;

    std::vector<T> r;
    std::vector<T> g;
    std::vector<T> b;

private:
    static const int HEADER_SIZE = 54;
    static const int CHANNELS = 3;
};

#endif //HW3_BMPIMAGE_H
