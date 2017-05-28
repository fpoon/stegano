#include "wavcoder.h"
#include "loghelper.h"

#include <cstdint>

using namespace std;

WAVCoder::WAVCoder()
{

}

WAVCoder::WAVCoder(std::uint8_t *data, std::size_t size) : Coder(data, size)
{
    int offset = 44;
    uint16_t format = *((uint16_t*)&data[20]);
    if (format != 1)
    {
        log("Not PCM audio");
        return;
    }
    log("Found PCM audio");
    uint16_t res = *((uint16_t*)&data[34]);
    log(string("Resolution: ") + to_string(res));

    int step = format/8;
    if (!step)
        step = 1;
    for (int i = offset; i < size; i += step)
    {
        points.push_back(&data[i]);
    }
}

int WAVCoder::avaiableSpace()
{
    return points.size()/8;
}
