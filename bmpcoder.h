#ifndef BMPCODER_H
#define BMPCODER_H

#include "coder.h"

#include <vector>
#include <cstdint>
#include <string>

class BMPCoder : public Coder
{
private:
    //std::vector<std::uint8_t*> points;
public:
    BMPCoder();
    BMPCoder(std::uint8_t* data, std::size_t size);
    virtual int avaiableSpace() override;
};

#endif // BMPCODER_H
