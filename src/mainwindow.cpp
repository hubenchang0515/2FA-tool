#include "mainwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QFile>

#include "totp.h"

namespace TowFATool
{

MainWindow::MainWindow(QWidget* parent) noexcept:
    QMainWindow{parent},
    m_mainLayout{new QHBoxLayout{nullptr}},
    m_leftLayout{new QVBoxLayout{nullptr}},
    m_rightLayout{new QVBoxLayout{nullptr}},
    m_buttonLayout{new QHBoxLayout{nullptr}},
    m_centralWidget{new QWidget{this}},
    m_accountList{new QListWidget{this}},
    m_addAccountButton{new QPushButton{tr("Add"), this}},
    m_editAccountButton{new QPushButton{tr("Edit"), this}},
    m_delAccountButton{new QPushButton{tr("Delete"), this}},
    m_site{new QLabel{"", this}},
    m_user{new QLabel{"", this}},
    m_copy{new QPushButton{tr("Copy Password"), this}},
    m_progress{new QProgressBar{this}},
    m_password{new QLCDNumber{6, this}},
    m_config{new Config{QCoreApplication::applicationDirPath() + "/2fa.ini", this}},
    m_configDialog{new ConfigDialog{m_config, this}},
    m_timer{new QTimer{this}}
{
    m_buttonLayout->addWidget(m_addAccountButton);
    m_buttonLayout->addWidget(m_editAccountButton);
    m_buttonLayout->addWidget(m_delAccountButton);
    m_buttonLayout->setStretch(0, 1);
    m_buttonLayout->setStretch(1, 1);
    m_buttonLayout->setStretch(2, 1);

    m_leftLayout->addWidget(m_accountList);
    m_leftLayout->addLayout(m_buttonLayout);

    m_rightLayout->addWidget(m_site);
    m_rightLayout->addWidget(m_user);
    m_rightLayout->addWidget(m_copy);
    m_rightLayout->addWidget(m_progress);
    m_rightLayout->addWidget(m_password);

    m_mainLayout->addLayout(m_leftLayout);
    m_mainLayout->addLayout(m_rightLayout);

    m_centralWidget->setLayout(m_mainLayout);
    setCentralWidget(m_centralWidget);

    m_accountList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_addAccountButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_progress->setFormat("%v");
    m_password->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_password->setSegmentStyle(QLCDNumber::Filled);
    m_password->display(QString("------"));
    m_password->setMinimumSize(420, 420);
    
    m_timer->setInterval(1000);
    m_timer->start();

    refreshList();
    refreshProgress();
    refreshPassword();
    copyPassword();

    connect(m_addAccountButton, &QPushButton::clicked, this, &MainWindow::addConfig);
    connect(m_editAccountButton, &QPushButton::clicked, this, &MainWindow::editConfig);
    connect(m_delAccountButton, &QPushButton::clicked, this, &MainWindow::delConfig);
    connect(m_config, &Config::changed, this, &MainWindow::refreshList);
    connect(m_accountList, &QListWidget::currentRowChanged, this, &MainWindow::refreshProgress);
    connect(m_accountList, &QListWidget::currentRowChanged, this, &MainWindow::refreshPassword);
    connect(m_accountList, &QListWidget::currentRowChanged, this, &MainWindow::copyPassword);
    connect(m_copy, &QPushButton::clicked, this, &MainWindow::copyPassword);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::refreshProgress);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::refreshPassword);

    QFile qss(":/QtTheme/QtTheme_Flat_Dark_LightBlue_Indigo.qss");
    qss.open(QFile::ReadOnly);
    setStyleSheet(qss.readAll());

    m_delAccountButton->setProperty("Color", "Danger");
    m_copy->setProperty("Color", "Primary");
    m_accountList->setProperty("Color", "Primary");
}


MainWindow::~MainWindow() noexcept
{

}


QString MainWindow::password() const noexcept
{   
    quint16 digits = m_config->digits(m_site->text(), m_user->text());
    quint16 period = m_config->period(m_site->text(), m_user->text());
    QCryptographicHash::Algorithm algorithm = Config::ALGORITHM_MAP[m_config->algorithm(m_site->text(), m_user->text())];

    Totp totp{algorithm, 0, period, digits};
    return totp.password(m_config->secret(m_site->text(), m_user->text()));
}


void MainWindow::addConfig() const noexcept
{
    m_configDialog->show();
}

void MainWindow::editConfig() const noexcept
{
    m_configDialog->show(m_site->text(), m_user->text());
}

void MainWindow::delConfig() const noexcept
{
    m_config->remove(m_site->text(), m_user->text());
    refreshList();
}


void MainWindow::refreshList() const noexcept
{
    QStringList keys = m_config->allKeys();
    QStringListIterator iter{keys};

    m_accountList->clear();
    while (iter.hasNext())
    {
        m_accountList->addItem(iter.next());
    }
    
    if (m_accountList->count() > 0 && m_accountList->currentItem() == nullptr)
    {
        m_accountList->setCurrentRow(0);
    }
}

void MainWindow::refreshAccount() const noexcept
{
    QListWidgetItem* item = m_accountList->currentItem();
    if (item == nullptr)
    {
        m_site->setText("");
        m_user->setText("");
        return;
    }
    
    QStringList path = item->text().remove("/").split(":");
    if (path.size() < 2)
    {
        m_site->setText("");
        m_user->setText("");
        return;
    }
        
    m_site->setText(path[0]);
    m_user->setText(path[1]);
}


void MainWindow::refreshProgress() const noexcept
{
    refreshAccount();
    quint64 period = m_config->period(m_site->text(), m_user->text());
    if (period > 0)
    {
        int time = period - (QDateTime::currentSecsSinceEpoch() % period);
        m_progress->setRange(0, period);
        m_progress->setValue(time);

        if (time > period / 2)
        {
            m_progress->setProperty("Color", "Success");
        }
        else if(time > 5)
        {
            m_progress->setProperty("Color", "Warning");
        }
        else
        {
            m_progress->setProperty("Color", "Danger");
        }

        // update style manually
        m_progress->style()->unpolish(m_progress);
        m_progress->style()->polish(m_progress);
    }
}


void MainWindow::refreshPassword() const noexcept
{
    refreshAccount();
    QString password = this->password();

    if (password.isEmpty())
    {
        m_password->setDigitCount(6);
        m_password->display("------");
    }
    else
    {
        m_password->setDigitCount(password.size());
        m_password->display(password);
    }
}


void MainWindow::copyPassword() const noexcept
{
    refreshAccount();
    QString password = this->password();
    if (!password.isEmpty())
    {
        QClipboard* clipboard = QGuiApplication::clipboard();
        clipboard->setText(password);
    }
}

}; // namespace TowFATool