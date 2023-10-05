#include "qrdecoder.h"
#include <QPainter>
#include <QPixmap>

#define BORDER_PIXEL_WIDTH 1

namespace TowFATool
{

QRDecoder::QRDecoder() noexcept:
    m_decoder{quirc_new()}
{
    
}

QRDecoder::~QRDecoder() noexcept
{
    if (m_decoder != nullptr)
        quirc_destroy(m_decoder);

    m_decoder = nullptr;
}

QByteArrayList QRDecoder::decode(const QImage& image) const noexcept
{
    QImage grayImage{image.width()+2*BORDER_PIXEL_WIDTH, image.height()+2*BORDER_PIXEL_WIDTH, QImage::Format_Grayscale8};
    grayImage.fill(Qt::white);
    QPainter painter{&grayImage};
    painter.drawImage(image.rect().adjusted(BORDER_PIXEL_WIDTH,BORDER_PIXEL_WIDTH,BORDER_PIXEL_WIDTH,BORDER_PIXEL_WIDTH), image);

    if (quirc_resize(m_decoder, grayImage.width(), grayImage.height()) < 0)
    {
        return {};
    }

    int width;
    int height; 
    uint8_t* buffer = quirc_begin(m_decoder, &width, &height);
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            buffer[row*width + col] = grayImage.pixelColor(col, row).lightness();
        }
    }

    quirc_end(m_decoder);
    int num = quirc_count(m_decoder);

    QByteArrayList infos;

    for (int i = 0; i < num; i++)
    {
        struct quirc_code code;
        struct quirc_data data;

        quirc_extract(m_decoder, i, &code);
        quirc_decode_error_t err = quirc_decode(&code, &data);
        if (err) continue;
        
        QByteArray info{reinterpret_cast<const char*>(data.payload), data.payload_len};
        infos.append(info);
    }

    return infos;
}

}; // namespace TowFATool