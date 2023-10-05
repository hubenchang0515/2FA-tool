#include "totp.h"

#include <QtEndian>
#include <QDateTime>
#include <QByteArray>

namespace TowFATool
{

Totp::Totp(QCryptographicHash::Algorithm algorithm, 
            int64_t originTime, 
            int64_t intervalTime, 
            size_t length) noexcept:
    m_algorithm{algorithm},
    m_originTime{originTime},
    m_intervalTime{intervalTime},
    m_length{length}
{

}

void Totp::setAlgorithm(QCryptographicHash::Algorithm algorithm) noexcept
{
    m_algorithm = algorithm;
}

void Totp::setOriginTime(int64_t originTime) noexcept
{
    m_originTime = originTime;
}

void Totp::setIntervalTime(int64_t intervalTime) noexcept
{
    m_intervalTime = intervalTime;
}

void Totp::setLength(size_t length) noexcept
{
    m_length = length;
}

QString Totp::generate(const QByteArray& key, int64_t currentTime) const noexcept
{
    qint64 counter = qToBigEndian<qint64>((currentTime - m_originTime) / m_intervalTime);
    QByteArray value = hmac(key, QByteArray{reinterpret_cast<char*>(&counter), sizeof(counter)});
    return truncate(value);
}


QString Totp::password(const QString& base32Key)  const noexcept
{
    return generate(decodeBase32(base32Key), QDateTime::currentSecsSinceEpoch());
}


int Totp::blockSize() const noexcept
{
    switch (m_algorithm)
    {
    case QCryptographicHash::Md5:
    case QCryptographicHash::Sha1:
    case QCryptographicHash::Sha224:
    case QCryptographicHash::Sha256:
        return 64;

    case QCryptographicHash::Sha384:
    case QCryptographicHash::Sha512:
        return 128;

    case QCryptographicHash::Sha3_224:
        return 144;

    case QCryptographicHash::Sha3_256:
        return 136;

    case QCryptographicHash::Sha3_384:
        return 104;

    case QCryptographicHash::Sha3_512:
        return 72;

    default:
        return 0;
    }
}


QByteArray Totp::decodeBase32(const QString& key) const noexcept
{
    // static const char* _base32Map = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QByteArray data;

    const char* code = key.toLatin1();
    quint64 bits40 = 0;
    int count = 0;
    for (int i = 0; i < key.size(); i++)
    {
        char ch = code[i];
        count += 1;
        if (ch >= 'A' && ch <= 'Z')
        {
            bits40 |= (quint64)(ch - 'A') << (40 - count*5);
        }
        else if (ch >= '2' && ch <= '7')
        {
            bits40 |= (quint64)(ch - '2' + 26) << (40 - count*5);
        }

        if (count == 8 || i == key.size() - 1)
        {
            bits40 = qToBigEndian<quint64>(bits40);
            data.append(reinterpret_cast<char*>(&bits40) + 3, count*5/8);
            bits40 = 0;
            count = 0;
        }
    }

    return data;
}


QByteArray Totp::hash(const QByteArray& data) const noexcept
{
    return QCryptographicHash::hash(data, m_algorithm);
}


QString Totp::truncate(const QByteArray& data) const noexcept
{
    uint offset   =  data[data.size() - 1] & 0xf ;
    uint32_t code = (data[offset]  & 0x7f) << 24  | 
                        (data[offset+1] & 0xff) << 16 |
                        (data[offset+2] & 0xff) <<  8 |
                        (data[offset+3] & 0xff) ;

    return QString("%1").arg(code).right(m_length);
}

QByteArray Totp::hmac(const QByteArray& key, const QByteArray& message) const noexcept
{
    QByteArray key0 = key.size() > blockSize() ? hash(key) : key;
    key0.append(blockSize() - key.size(), 0);

    uint8_t opad = 0x5c; 
    uint8_t ipad = 0x36;

    QByteArray okey;
    QByteArray ikey;

    for (int i = 0; i < key0.size(); i++)
    {
        okey.append(key0[i] ^ opad);
        ikey.append(key0[i] ^ ipad);
    }

    return hash(okey.append(hash(ikey.append(message))));
}
    
} // namespace TowFATool
