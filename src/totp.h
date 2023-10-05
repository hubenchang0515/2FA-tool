#ifndef TWOFATOOL_TOTP_H
#define TWOFATOOL_TOTP_H

#include <QString>
#include <QCryptographicHash>

namespace TowFATool
{

class Totp
{
public:
    Totp(QCryptographicHash::Algorithm algorithm=QCryptographicHash::Sha1, 
        int64_t originTime=0, 
        int64_t intervalTime=30, 
        size_t length=6) noexcept;
    ~Totp() noexcept = default;

    void setAlgorithm(QCryptographicHash::Algorithm algorithm) noexcept;
    void setOriginTime(int64_t originTime) noexcept;
    void setIntervalTime(int64_t intervalTime) noexcept;
    void setLength(size_t length) noexcept;

    QString generate(const QByteArray& key, int64_t currentTime) const noexcept;
    QString password(const QString& base32Key) const noexcept;

private:
    QCryptographicHash::Algorithm m_algorithm;
    int64_t m_originTime;
    int64_t m_intervalTime;
    size_t m_length;

    int blockSize() const noexcept;
    QByteArray decodeBase32(const QString& key) const noexcept;
    QByteArray hash(const QByteArray& data) const noexcept;
    QString truncate(const QByteArray& data) const noexcept;
    QByteArray hmac(const QByteArray& key, const QByteArray& message) const noexcept;
};

}; // namespace TowFATool

#endif // TWOFATOOL_TOTP_H