#include "crc.h"

quint16 CRC::computeCRC(const QByteArray &data)
{
    quint16 crc = 0xFFFF;
    quint16 poly = 0x1021;
    // data, '0' ve '1' karakterlerinden oluşan bit dizisi olarak kabul ediliyor.
    for (char bitChar : data) {
        bool inputBit = (bitChar == '1');
        bool msb = (crc & 0x8000) != 0;
        crc <<= 1;
        if (msb ^ inputBit) {
            crc ^= poly;
        }
    }
    return crc;
}
