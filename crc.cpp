
#include "crc.h"
quint16 CRC::calc(const QByteArray &bytes){
    quint16 crc=0xFFFF, poly=0x1021;
    for(unsigned char b:bytes){
        crc ^= quint16(b)<<8;
        for(int i=0;i<8;++i)
            crc = (crc & 0x8000)? (crc<<1)^poly : (crc<<1);
    }
    return crc & 0xFFFF;
}
