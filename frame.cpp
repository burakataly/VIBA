
#include "frame.h"
static inline void stuffByte(quint8 b, QByteArray &out)
{
    if (b == 0x7E)
    {
        out.append(char(0x7D)).append(char(0x5E));
        return;
    }
    if (b == 0x7D)
    {
        out.append(char(0x7D)).append(char(0x5D));
        return;
    }
    out.append(char(b));
}

QByteArray Frame::toByteArray() const
{
    QByteArray raw;
    raw.append(char(FLAG)); // FLAG kaçışlanmaz

    /* CTRL + DATA + CRC baytları kaçışlanarak eklenir */
    stuffByte(ctrl, raw);
    for (auto ch : data)
        stuffByte(quint8(ch), raw);
    stuffByte(quint8((crc >> 8) & 0xFF), raw);
    stuffByte(quint8(crc & 0xFF), raw);

    raw.append(char(FLAG)); // isteğe bağlı kapanış flag'i
    return raw;
}
Frame Frame::fromByteArray(const QByteArray &raw)
{
    Frame f;
    QByteArray de;
    de.reserve(raw.size());

    /* FLAG'ler arasını destuff et */
    bool skipFirst = true;
    for (int i = 0; i < raw.size(); ++i)
    {
        quint8 b = quint8(raw[i]);
        if (b == Frame::FLAG)
        { // başlangıç / son
            if (skipFirst)
            {
                skipFirst = false;
                continue;
            }
            else
                break;
        }
        if (b == 0x7D && i + 1 < raw.size())
        {
            quint8 next = quint8(raw[++i]);
            de.append(char(next ^ 0x20));
        }
        else
        {
            de.append(char(b));
        }
    }

    if (de.size() != 1 + 13 + 2)
        return f; // CTRL+DATA+CRC bekleniyor
    f.ctrl = quint8(de[0]);
    f.data = de.mid(1, 13);
    f.crc = (quint16(quint8(de[14])) << 8) | quint8(de[15]);
    return f;
}
