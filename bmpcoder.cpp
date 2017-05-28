#include "bmpcoder.h"
#include "loghelper.h"

using namespace std;

BMPCoder::BMPCoder()
{

}

BMPCoder::BMPCoder(std::uint8_t *data, std::size_t size) : Coder(data, size)
{
    uint32_t offset = *((uint32_t*)&data[10]);
    for(int i = offset; i < size; i++)
        points.push_back(&data[i]);
    log(to_string(points.size()));
}

int BMPCoder::avaiableSpace()
{
    return points.size()/8;
}
