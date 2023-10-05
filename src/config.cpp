#include "config.h"

namespace TowFATool
{

const QString Config::SCHEMA = "otpauth";
const QString Config::HOST = "totp";
const QString Config::SECRET = "secret";
const QString Config::DIGITS = "digits";
const QString Config::PERIOD = "period";
const QString Config::ALGORITHM = "algorithm";

const QMap<QString, QCryptographicHash::Algorithm> Config::ALGORITHM_MAP = 
{
    {"SHA1", QCryptographicHash::Sha1},
    {"SHA256", QCryptographicHash::Sha256},
    {"SHA512", QCryptographicHash::Sha512},
};


Config::Config(const QString& filename, QObject* parent) noexcept:
    QObject{parent},
    m_settings{new QSettings{filename, QSettings::IniFormat}}
{

}

Config::~Config() noexcept
{

}

QStringList	Config::allKeys() const noexcept
{
    return m_settings->childGroups();
}

QString Config::secret(const QString& site, const QString& user) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    QString value = m_settings->value(Config::SECRET).toString();
    m_settings->endGroup();
    return value;
}

void Config::setSecret(const QString& site, const QString& user, const QString& secret) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    m_settings->setValue(Config::SECRET, QVariant{secret});
    m_settings->endGroup();
}

quint16 Config::digits(const QString& site, const QString& user) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    quint16 value = m_settings->value(Config::DIGITS).toUInt();
    m_settings->endGroup();
    return value;
}

void Config::setDigits(const QString& site, const QString& user, quint16 digits) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    m_settings->setValue(Config::DIGITS, QVariant{digits});
    m_settings->endGroup();
}

quint16 Config::period(const QString& site, const QString& user) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    quint16 value = m_settings->value(Config::PERIOD).toUInt();
    m_settings->endGroup();
    return value;
}

void Config::setPeriod(const QString& site, const QString& user, quint16 period) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    m_settings->setValue(Config::PERIOD, QVariant{period});
    m_settings->endGroup();
}

QString Config::algorithm(const QString& site, const QString& user) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    QString value = m_settings->value(Config::ALGORITHM).toString().toUpper();
    m_settings->endGroup();
    
    if (ALGORITHM_MAP.contains(value))
        return value;
    else
        return "SHA1";
}

void Config::setAlgorithm(const QString& site, const QString& user, const QString& algorithm) const noexcept
{
    m_settings->beginGroup(QString("/%1:%2").arg(site).arg(user));
    m_settings->setValue(Config::ALGORITHM, QVariant{algorithm});
    m_settings->endGroup();
}

void Config::sync() noexcept
{
    m_settings->sync();
    emit changed();
}


}; // namespace TowFATool