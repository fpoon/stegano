#include <string>

#include "mp3framecoder.h"
#include "loghelper.h"

using namespace std;

int MP3FrameCoder::getID3TagSize()
{
    if (size < 3) return 0;
    if (!(data[0] == 'I' && data[1] == 'D' && data[2] == '3'))
        return 0;
    int size = 0;
    for (int i = 0; i < 3; i++)
    {
        size |= data[6+i];
        size = size << 7;
    }
    size |= data[9];
    if (data[5] & 0x10)
        return size+10;
    else
        return size+20;
}

int MP3FrameCoder::findNextFrame(int start)
{
    for (int i = start+1; i < size; i++)
    {
        if (data[i-1] == 0xFF &&
                ((data[i] >> 4) == 0xE || (data[i] >> 4) == 0xF))
        {
            return i-1;
        }
    }
    return -1;
}

MP3FrameCoder::MP3FrameCoder() : Coder()
{
}

MP3FrameCoder::MP3FrameCoder(uint8_t *data, size_t size) : Coder(data, size)
{
    id3size = getID3TagSize();
    int frameOffset = findNextFrame(id3size);
    log(string("ID3 size: ")+to_string(id3size));
    log(string("Found first frame at: ")+to_string(frameOffset));


    for(int ind = frameOffset; ind < size-6;)
    {
        MP3Frame *frame;
        try {
            frame = new MP3Frame(&data[ind]);
            //throw -1;
        }
        catch (int e)
        {
            //delete frame;
            int foo = findNextFrame(ind+1);
            if (foo == -1)
                break;
            ind = foo;
            continue;
        }
        frames.push_back(frame);
        ind += frame->getTotalFrameLength();
    }
    frames.back()->info();
    string s = "Frames: " + to_string(frames.size());
    log (s);

}

std::vector<uint8_t> MP3FrameCoder::Encode(string msg)
{
    uint16_t f = msg.size();
    for (int i = 0; i < 2*8; i++)
    {
        frames[i]->setPrivateBit((f >> i) & 1);
        //log(to_string(frames[i]->getPrivateBit()));
    }
    if (msg.size() > 0)
    {
        for (int i = 2*8, c = 0, l = 0; i < frames.size(); i++,c++)
        {
            if (c >= 8)
                l++;
            c %= 8;
            if (l >= msg.size())
                break;
            char s = msg[l];
            frames[i]->setPrivateBit((s >> c) & 1);
        }
    }
    return vector<uint8_t>(data, data+size);
}

string MP3FrameCoder::Decode()
{
    string ret;
    uint16_t s = 0;
    if (frames.size() < 2*8)
        return "";

    for (int i = 15; i >= 0; i--)
    {
        s <<= 1;
        s |= frames[i]->getPrivateBit();
    }
    s += 2;

    char l = 0;
    for (int i = 2*8, c = 0; i < frames.size() && i < s*8; i++,c++)
    {
        c %= 8;
        l |= frames[i]->getPrivateBit() << c;
        if (c == 7) {
            ret += l;
            l = 0;
        }

    }
    return ret;

}

MP3FrameCoder::~MP3FrameCoder()
{
    for (auto f : frames)
        delete f;
}
