#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <QByteArray>

namespace Checksum
{
quint16 crc16ForModbus(const QByteArray &data);

quint16 crc16ForX25(const QByteArray &data);

quint32 crc32(const QByteArray &data);
}


#endif // CHECKSUM_H
