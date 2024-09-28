#include "configdialog.h"

#include <QSizePolicy>
#include <QUrl>
#include <QUrlQuery>
#include <QFileDialog>
#include <QMessageBox>
#include <QValidator>

#include "qrdecoder.h"

namespace TowFATool
{

ConfigDialog::ConfigDialog(QPointer<Config> config, QWidget* parent) noexcept:
    QDialog(parent),
    m_formLayout{new QGridLayout{this}},
    m_siteLabel{new QLabel{tr("Site"), this}},
    m_site{new QLineEdit{this}},
    m_userLabel{new QLabel{tr("User"), this}},
    m_user{new QLineEdit{this}},
    m_secretLabel{new QLabel{tr("Secret"), this}},
    m_secret{new QLineEdit{this}},
    m_digitsLabel{new QLabel{tr("Digits"), this}},
    m_digits{new QLineEdit{this}},
    m_periodLabel{new QLabel{tr("Period"), this}},
    m_period{new QLineEdit{this}},
    m_algorithmLabel{new QLabel{tr("Algorithm"), this}},
    m_algorithm{new QComboBox{this}},
    m_importButton{new QPushButton{tr("Import By QR-Code"), this}},
    m_okButton{new QPushButton{tr("OK"), this}},
    m_config{config}
{
    m_formLayout->addWidget(m_siteLabel, 0, 0);
    m_formLayout->addWidget(m_site, 0, 1);
    m_formLayout->addWidget(m_userLabel, 1, 0);
    m_formLayout->addWidget(m_user, 1, 1);
    m_formLayout->addWidget(m_secretLabel, 2, 0);
    m_formLayout->addWidget(m_secret, 2, 1);
    m_formLayout->addWidget(m_digitsLabel, 3, 0);
    m_formLayout->addWidget(m_digits, 3, 1);
    m_formLayout->addWidget(m_periodLabel, 4, 0);
    m_formLayout->addWidget(m_period, 4, 1);
    m_formLayout->addWidget(m_algorithmLabel, 5, 0);
    m_formLayout->addWidget(m_algorithm, 5, 1);
    m_formLayout->addWidget(m_importButton, 6, 0, 1, 2);
    m_formLayout->addWidget(m_okButton, 7, 0, 1, 2);
    setLayout(m_formLayout);

    m_algorithm->addItems(Config::ALGORITHM_LIST);

    m_secret->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    m_digits->setValidator(new QIntValidator{6, 8, this});
    m_period->setValidator(new QIntValidator{10, 60, this});

    setFixedWidth(300);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    refreshOkButton();

    connect(m_importButton, &QPushButton::clicked, this, &ConfigDialog::importQrCode);
    connect(m_okButton, &QPushButton::clicked, this, &ConfigDialog::saveConfig);
    connect(m_site, &QLineEdit::textChanged, this, &ConfigDialog::refreshOkButton);
    connect(m_user, &QLineEdit::textChanged, this, &ConfigDialog::refreshOkButton);
    connect(m_secret, &QLineEdit::textChanged, this, &ConfigDialog::refreshOkButton);
}

ConfigDialog::~ConfigDialog() noexcept
{
    
}

void ConfigDialog::show() noexcept
{
    m_site->setText("");
    m_user->setText("");
    m_secret->setText("");
    m_digits->setText("6");
    m_period->setText("30");
    m_algorithm->setCurrentText("SHA1");

    adjustSize();
    QDialog::show();
}

void ConfigDialog::show(const QString& site, const QString& user) noexcept
{
    m_site->setText(site);
    m_user->setText(user);
    m_secret->setText(m_config->secret(site, user));
    m_digits->setText(QString::number(m_config->digits(site, user)));
    m_period->setText(QString::number(m_config->period(site, user)));
    m_algorithm->setCurrentText(m_config->algorithm(site, user));
    QDialog::show();
}

void ConfigDialog::enableConfig(bool enabled) noexcept
{
    m_site->setEnabled(enabled);
    m_user->setEnabled(enabled);
    m_secret->setEnabled(enabled);
    m_digits->setEnabled(enabled);
    m_period->setEnabled(enabled);
    m_algorithm->setEnabled(enabled);
    m_importButton->setEnabled(enabled);

    if (enabled) 
    {
        refreshOkButton();
    }
    else
    {
        m_okButton->setEnabled(false);
    }
}

void ConfigDialog::refreshOkButton() noexcept
{
    m_okButton->setEnabled(!m_site->text().trimmed().isEmpty() &&
                            !m_user->text().trimmed().isEmpty() &&
                            !m_secret->text().trimmed().isEmpty());
}

void ConfigDialog::parseQrCode(const QByteArray& data) noexcept
{
    QRDecoder decoder;
    QImage image;
    image.loadFromData(data);
    QByteArrayList infos = decoder.decode(image);
    QByteArrayListIterator iter{infos};
    while (iter.hasNext())
    {
        QUrl url{iter.next()};

        if (url.scheme() != Config::SCHEMA || url.host() != Config::HOST)
            continue;

        QStringList path = url.path().remove("/").split(":");
        QUrlQuery query{url};

        if (path.size() >= 2 && query.hasQueryItem(Config::SECRET))
        {
            m_site->setText(path[0]);
            m_user->setText(path[1]);
            m_secret->setText(query.queryItemValue(Config::SECRET));

            if (query.hasQueryItem(Config::DIGITS))
            {
                m_digits->setText(query.queryItemValue(Config::DIGITS));
            }
            else
            {
                m_digits->setText("6");
            }

            if (query.hasQueryItem(Config::PERIOD))
            {
                m_period->setText(query.queryItemValue(Config::PERIOD));
            }
            else
            {
                m_period->setText("30");
            }

            if (query.hasQueryItem(Config::ALGORITHM))
            {
                m_algorithm->setCurrentText(query.queryItemValue(Config::ALGORITHM).toUpper());
            }
            else
            {
                m_algorithm->setCurrentText("SHA1");
            }
        }
        else
        {
            continue;
        }

        return;
    }

    QMessageBox::warning(this, tr("Import by QR-Code Failed"), tr("Cannot find QR-Code of TOTP message"));
}

void ConfigDialog::importQrCode() noexcept
{
    auto callback = [this](const QString& filename, const QByteArray& data) {
        enableConfig(false);
        m_importButton->setText(tr("Be Parsing..."));
        if (!filename.isEmpty())
        {
            parseQrCode(data);
        }
        enableConfig(true);
        m_importButton->setText(tr("Import By QR-Code"));
    };

    QFileDialog::getOpenFileContent("", callback);
}

void ConfigDialog::saveConfig() noexcept
{
    m_config->setSecret(m_site->text(), m_user->text(), m_secret->text());
    m_config->setDigits(m_site->text(), m_user->text(), m_digits->text().toUInt());
    m_config->setPeriod(m_site->text(), m_user->text(), m_period->text().toUInt());
    m_config->setAlgorithm(m_site->text(), m_user->text(), m_algorithm->currentText());
    m_config->sync();
    close();
}

}; // namespace TowFATool