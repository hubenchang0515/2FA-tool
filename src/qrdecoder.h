#ifndef TWOFATOOL_QRDECODER_H
#define TWOFATOOL_QRDECODER_H

#include <QString>
#include <QImage>
#include <quirc.h>

namespace TowFATool
{

class QRDecoder
{
public:
    QRDecoder() noexcept;
    ~QRDecoder() noexcept;
    QRDecoder(const QRDecoder&) = delete;
    QRDecoder(QRDecoder&&) = delete;

    QByteArrayList decode(const QImage& image) const noexcept;

private:
    quirc* m_decoder;
};

}; // namespace TowFATool

#endif // TWOFATOOL_QRDECODER_H