#ifndef MP3FRAMECODER_H
#define MP3FRAMECODER_H

#include <string>

#include "coder.h"
#include "mp3frame.h"

class MP3FrameCoder : public Coder
{
private:
    std::vector<MP3Frame*> frames;
    std::uint32_t id3size;
    int getID3TagSize();
    int findNextFrame(int start);
public:
    MP3FrameCoder();
    MP3FrameCoder(std::uint8_t* data, std::size_t size);
    virtual std::vector<std::uint8_t> Encode(std::string msg) override;
    virtual std::string Decode() override;
    ~MP3FrameCoder();
};

#endif // MP3FRAMECODER_H
