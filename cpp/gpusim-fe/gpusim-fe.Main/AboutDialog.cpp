#include "AboutDialog.h"

CAboutDialog::CAboutDialog(const QStringList &pluginNames, QWidget *pParent /*= nullptr*/)
    : QDialog(pParent)
{
    ui.setupUi(this);
    ui.lwAvailablePlugins->addItems(pluginNames);
}