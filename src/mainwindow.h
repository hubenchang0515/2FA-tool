#ifndef TWOFATOOL_MAINWINDOW_H
#define TWOFATOOL_MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QProgressBar>
#include <QLCDNumber>
#include <QTimer>
#include <QPointer>

#include "config.h"
#include "configdialog.h"

namespace TowFATool
{

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent=nullptr) noexcept;
    ~MainWindow() noexcept;

    QString password() const noexcept;

public slots:
    void addConfig() const noexcept;
    void editConfig() const noexcept;
    void delConfig() const noexcept;
    void refreshList() const noexcept;
    void refreshAccount() const noexcept;
    void refreshProgress() const noexcept;
    void refreshPassword() const noexcept;
    void copyPassword() const noexcept;

private:
    QHBoxLayout*    m_mainLayout;
    QVBoxLayout*    m_leftLayout;
    QVBoxLayout*    m_rightLayout;
    QHBoxLayout*    m_buttonLayout;
    QWidget*        m_centralWidget;
    QListWidget*    m_accountList;
    QPushButton*    m_addAccountButton;
    QPushButton*    m_editAccountButton;
    QPushButton*    m_delAccountButton;
    QLabel*         m_site;
    QLabel*         m_user;
    QPushButton*    m_copy;
    QProgressBar*   m_progress;
    QLCDNumber*     m_password;

    QPointer<Config>    m_config;
    ConfigDialog*       m_configDialog;
    QTimer*             m_timer;
};

}; // namespace TowFATool

#endif // TWOFATOOL_MAINWINDOW_H