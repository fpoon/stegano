#include <string>

#include "mp3frame.h"
#include "loghelper.h"

using namespace std;

int mpeg1l1[] = {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0};
int mpeg1l2[] = {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 0};
int mpeg1l3[] = {0, 32, 40, 48, 56, 64, 80,  96, 112, 128, 160, 192, 224, 256, 320, 0};
int mpeg2l1[] = {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0};
int mpeg2l2[] = {0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160, 0};
int mpeg2l3[] = {0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160, 0};

int mpeg1samp[] = {44100, 48000, 32000, 0};
int mpeg2samp[] = {22050, 24000, 16000, 0};
int mpeg25samp[] = {11025, 12000, 8000, 0};

void MP3Frame::setBitrateAndSampling(int bi, int si)
{
    switch (mpeg)
    {
    case 0: //mpeg-2.5
        switch (layer)
        {
            case 1: //Layer III
                bitrate = mpeg2l3[bi];
                break;
            case 2: //Layer II
                bitrate = mpeg2l2[bi];
                break;
            case 3: //Layer I
                bitrate = mpeg2l1[bi];
                break;
        }
        sampling = mpeg25samp[si];
        break;
    case 2: //mpeg-2
        switch (layer)
        {
            case 1: //Layer III
                bitrate = mpeg2l3[bi];
                break;
            case 2: //Layer II
                bitrate = mpeg2l2[bi];
                break;
            case 3: //Layer I
                bitrate = mpeg2l1[bi];
                break;
        }
        sampling = mpeg2samp[si];
        break;
    case 3: //mpeg-1
        switch (layer)
        {
            case 1: //Layer III
                bitrate = mpeg1l3[bi];
                break;
            case 2: //Layer II
                bitrate = mpeg1l2[bi];
                break;
            case 3: //Layer I
                bitrate = mpeg1l1[bi];
                break;
        }
        sampling = mpeg1samp[si];
        break;
    }
    bitrate *= 1000;
}

MP3Frame::MP3Frame(uint8_t *frame) : frame(frame)
{
    if (!(frame[0] == 0xFF &&
            ((frame[1] >> 4) == 0xE || (frame[1] >> 4) == 0xF)))
    {
        throw -1;
    }
    mpeg = (frame[1] >> 3) & 0x3;
    if (mpeg == 1)
        throw -2;
    layer = (frame[1] >> 1) & 0x3;
    if (layer == 0)
        throw -3;
    int bi = frame[2] >> 4;
    int si = (frame[2] >> 2) & 0x3;
    setBitrateAndSampling(bi, si);
    if (sampling == 0)
        throw -5;
    if (bitrate == 0)
        throw -6;

    if ((frame[2] >> 1) & 0x1)
        padding = 1;
    else
        padding = 0;

    if (frame[1] & 1)
        crc = false;
    else
        crc = true;
   // info();
}

void MP3Frame::setPrivateBit(int bit)
{
    uint8_t b = bit;
    b = b?1:0;
    frame[2] &= 0xFE;
    frame[2] |= b;
}

int MP3Frame::getPrivateBit()
{
    return frame[2] & 0x01;
}

int MP3Frame::getTotalFrameLength()
{
    return getFrameLength()+(crc?2:0)+4;
}

int MP3Frame::getFrameLength()
{
    if (layer == 3)
        return (12 * bitrate / sampling + padding) * 4;
    else
        return 144 * bitrate / sampling + padding;
}

void MP3Frame::info()
{
    string s = "MPEG-";
    if (mpeg == 0)
        s += "2.5";
    if (mpeg == 2)
        s += "2";
    if (mpeg == 3)
        s += "1";
    s += " Layer ";
    if (layer == 3)
        s += "1";
    if (layer == 2)
        s += "2";
    if (layer == 1)
        s += "3";
    s += " | Bitrate: ";
    s += to_string(bitrate);
    s += " Samples: ";
    s += to_string(sampling);
    s += " | Size: ";
    s += to_string(getFrameLength());
    s += " Total size: ";
    s += to_string(getTotalFrameLength());
    log(s);
}
