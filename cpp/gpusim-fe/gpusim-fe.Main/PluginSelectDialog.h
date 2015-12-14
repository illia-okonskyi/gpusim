#pragma once

#include "ui_PluginSelectDialog.h"
#include <QStringList>

class CPluginSelectDialog : public QDialog
{
    Q_OBJECT;
public:
    CPluginSelectDialog(const QStringList &pluginNames, QWidget *pParent = nullptr);

    QString getSelectedPluginName() const;

public slots:
    void onCurrentTextChanged(const QString &currentText);

private:
    Ui::CPluginSelectDialog ui;

private:
    QString m_selectedPluginName;
};