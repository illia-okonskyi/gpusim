#pragma once

#include "ui_AboutDialog.h"
#include <QStringList>

class CAboutDialog: public QDialog
{
    Q_OBJECT

public:
    CAboutDialog(const QStringList &pluginNames, QWidget *pParent = nullptr);

private:
    Ui::CAboutDialog ui;
};