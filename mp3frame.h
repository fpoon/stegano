#ifndef MP3FRAME_H
#define MP3FRAME_H

#include <cstdint>
#include <vector>

#define MP3FRAME_SYNC 0x

class MP3Frame
{
    std::uint8_t * frame;
    int bitrate;
    int sampling;
    int mpeg;
    int layer;
    int padding;
    bool crc;

    void setBitrateAndSampling(int bi, int si);
public:
    MP3Frame(std::uint8_t *frame);
    void setPrivateBit(int bit);
    int getPrivateBit();

    int getCRC();

    int getTotalFrameLength();
    int getFrameLength();

    std::vector<std::uint8_t> getFrame();
    void info();
};

#endif // MP3FRAME_H
