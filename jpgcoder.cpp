#include "jpgcoder.h"
#include "loghelper.h"

#include <string>

using namespace std;

JPGCoder::JPGCoder()
{

}

JPGCoder::JPGCoder(uint8_t *data, std::size_t size) : Coder(data, size)
{
    int dqt = 0;
    while((dqt = findDQT(data, dqt+1)) > 0)
    {
        log(string("Found QT at: ")+to_string(dqt));
        uint16_t s = (uint16_t)data[dqt+3] + ((uint16_t)data[dqt+2] << 8);
        int r = data[dqt+4] >> 4;
        r = r?2:1;
        for (int i = 0; i < 64; i++)
        {
            points.push_back(&data[dqt+5+i*r+(r-1)]);
        }
    }
}

int JPGCoder::avaiableSpace()
{
    return points.size()/8;
}

int JPGCoder::findDQT(uint8_t *data, int i)
{
    for (; i < size-1; i++)
        if (data[i] == 0xff)
            if (data[i+1] == 0xdb)
                return i;
    return -1;
}
