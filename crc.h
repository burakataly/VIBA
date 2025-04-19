
#ifndef CRC_H
#define CRC_H
#include <QByteArray>
#include <QtGlobal>
class CRC{public: static quint16 calc(const QByteArray&);};
#endif
