#ifndef WAVCODER_H
#define WAVCODER_H

#include "coder.h"

class WAVCoder : public Coder
{
public:
    WAVCoder();
    WAVCoder(std::uint8_t* data, std::size_t size);
    virtual int avaiableSpace() override;
};

#endif // WAVCODER_H
