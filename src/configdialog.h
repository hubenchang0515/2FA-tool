#ifndef TWOFATOOL_CONFIGDIALOG_H
#define TWOFATOOL_CONFIGDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#include "config.h"

namespace TowFATool
{

class ConfigDialog : public QDialog
{
public:
    ConfigDialog(Config* config, QWidget* parent=nullptr) noexcept;
    ~ConfigDialog() noexcept;

    void show() noexcept;

public slots:
    void refreshOkButton() noexcept;
    void importQrCode() noexcept;
    void parseQrCode(const QByteArray& data) noexcept;
    void saveConfig() noexcept;

private:
    QFormLayout*    m_formLayout;
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

    Config*         m_config;
};

}; // namespace TowFATool

#endif // TWOFATOOL_CONFIGDIALOG_H