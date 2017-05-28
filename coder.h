#ifndef CODER_H
#define CODER_H

#include <cstdint>
#include <vector>
#include <string>

class Coder
{
protected:
    std::uint8_t* data;
    std::size_t size;
    std::vector<std::uint8_t*> points;
public:
    Coder();
    Coder(std::uint8_t* data, std::size_t size);
    virtual std::vector<std::uint8_t> Encode(std::string msg);
    virtual std::string Decode();
    virtual int avaiableSpace() = 0;
};

#endif // CODER_H
