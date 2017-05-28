#ifndef JPGCODER_H
#define JPGCODER_H

#include "coder.h"

class JPGCoder : public Coder
{
public:
    JPGCoder();
    JPGCoder(std::uint8_t* data, std::size_t size);
    virtual int avaiableSpace() override;
private:
    int findDQT(std::uint8_t* data, int pos);
};

#endif // JPGCODER_H
