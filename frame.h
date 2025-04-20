
#ifndef FRAME_H
#define FRAME_H
#include <QByteArray>
#include <QtGlobal>
struct Frame
{
    static constexpr quint8 FLAG = 0x7E;
    quint8 ctrl = 0;
    QByteArray data; // 13 bytes
    quint16 crc = 0;
    QByteArray toByteArray() const;
    static Frame fromByteArray(const QByteArray &raw);
};
#endif
