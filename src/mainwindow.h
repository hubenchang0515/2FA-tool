#ifndef TWOFATOOL_MAINWINDOW_H
#define TWOFATOOL_MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QLCDNumber>
#include <QTimer>

#include "config.h"
#include "configdialog.h"

namespace TowFATool
{

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget* parent=nullptr) noexcept;
    ~MainWindow() noexcept;

    QString password() const noexcept;

public slots:
    void showConfigDialog() const noexcept;
    void refreshList() const noexcept;
    void refreshPassword() const noexcept;
    void copyPassword() const noexcept;

private:
    QHBoxLayout*    m_mainLayout;
    QVBoxLayout*    m_leftLayout;
    QVBoxLayout*    m_rightLayout;
    QWidget*        m_centralWidget;
    QListWidget*    m_accountList;
    QPushButton*    m_addAccountButton;
    QLabel*         m_site;
    QLabel*         m_user;
    QPushButton*    m_copy;
    QLCDNumber*     m_password;

    Config*         m_config;
    ConfigDialog*   m_configDialog;
    QTimer*         m_timer;
};

}; // namespace TowFATool

#endif // TWOFATOOL_MAINWINDOW_H