#include "coder.h"
#include "loghelper.h"

using namespace std;

Coder::Coder() : data(0), size(0)
{

}

Coder::Coder(uint8_t *data, std::size_t size) : data(data), size(size)
{

}

std::vector<uint8_t> Coder::Encode(std::string msg)
{
    log(to_string(points.size()));
    uint16_t f = msg.size();
    if (points.size() < 2*8)
        return vector<uint8_t>(data, data+size);
    for (int i = 0; i < 2*8; i++)
    {
        *points[i] &= 0xFE;
        *points[i] |= (f >> i) & 1;
    }
    if (msg.size() > 0)
    {
        for (int i = 2*8, c = 0, l = 0; i < points.size(); i++,c++)
        {
            if (c >= 8)
                l++;
            c %= 8;
            if (l >= msg.size())
                break;
            char s = msg[l];
            *points[i] &= 0xFE;
            *points[i] |= (s >> c) & 1;
        }
    }
    return vector<uint8_t>(data, data+size);
}

std::string Coder::Decode()
{
    string ret;
    uint16_t s = 0;

    for (int i = 15; i >= 0; i--)
    {
        s <<= 1;
        s |= *points[i] & 0x1;
    }
    s += 2;
;
    char l = 0;
    for (int i = 2*8, c = 0; i < points.size() && i < s*8; i++,c++)
    {
        c %= 8;
        l |=  (*points[i] & 0x1) << c;
        if (c == 7) {
            ret += l;
            l = 0;
        }

    }
    return ret;
}
