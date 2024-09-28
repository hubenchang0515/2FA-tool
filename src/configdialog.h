#ifndef TWOFATOOL_CONFIGDIALOG_H
#define TWOFATOOL_CONFIGDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QPointer>

#include "config.h"

namespace TowFATool
{

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigDialog(QPointer<Config> config, QWidget* parent=nullptr) noexcept;
    ~ConfigDialog() noexcept;

    void show() noexcept;
    void show(const QString& site, const QString& user) noexcept;
    void enableConfig(bool enabled) noexcept;

public slots:
    void refreshOkButton() noexcept;
    void importQrCode() noexcept;
    void parseQrCode(const QByteArray& data) noexcept;
    void saveConfig() noexcept;

private:
    QGridLayout*    m_formLayout;
    QLabel*         m_siteLabel;
    QLineEdit*      m_site;
    QLabel*         m_userLabel;
    QLineEdit*      m_user;
    QLabel*         m_secretLabel;
    QLineEdit*      m_secret;
    QLabel*         m_digitsLabel;
    QLineEdit*      m_digits;
    QLabel*         m_periodLabel;
    QLineEdit*      m_period;
    QLabel*         m_algorithmLabel;
    QComboBox*      m_algorithm;
    QPushButton*    m_importButton;
    QPushButton*    m_okButton;

    QPointer<Config>  m_config;
};

}; // namespace TowFATool

#endif // TWOFATOOL_CONFIGDIALOG_H