#ifndef CRC_H
#define CRC_H

#include <QByteArray>
#include <QtGlobal>

class CRC
{
public:
    // CRC-16-CCITT polinomu: x^16 + x^12 + x^5 + 1 (hex: 0x1021)
    static quint16 computeCRC(const QByteArray &data);
};

#endif // CRC_H
