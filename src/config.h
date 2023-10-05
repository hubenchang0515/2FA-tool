#ifndef TWOFATOOL_CONFIG_H
#define TWOFATOOL_CONFIG_H

#include <QSettings>
#include <QCryptographicHash>

namespace TowFATool
{

class Config : public QObject
{
    Q_OBJECT
public:
    static const QString SCHEMA;
    static const QString HOST;
    static const QString SECRET;
    static const QString DIGITS;
    static const QString PERIOD;
    static const QString ALGORITHM;
    static const QMap<QString, QCryptographicHash::Algorithm> ALGORITHM_MAP;
    
    Config(const QString& filename, QObject* parent=nullptr) noexcept;
    ~Config() noexcept;

    QStringList	allKeys() const noexcept;

    QString secret(const QString& site, const QString& user) const noexcept;
    void setSecret(const QString& site, const QString& user, const QString& secret) const noexcept;
    
    quint16 digits(const QString& site, const QString& user) const noexcept;
    void setDigits(const QString& site, const QString& user, quint16 digits) const noexcept;

    quint16 period(const QString& site, const QString& user) const noexcept;
    void setPeriod(const QString& site, const QString& user, quint16 period) const noexcept;

    QString algorithm(const QString& site, const QString& user) const noexcept;
    void setAlgorithm(const QString& site, const QString& user, const QString& algorithm) const noexcept;

    void sync() noexcept;

signals:
    void changed() const;

private:
    QSettings* m_settings;
};

}; // namespace TowFATool

#endif // TWOFATOOL_CONFIG_H