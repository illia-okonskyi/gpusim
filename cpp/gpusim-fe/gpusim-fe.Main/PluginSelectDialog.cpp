#include "PluginSelectDialog.h"

CPluginSelectDialog::CPluginSelectDialog(const QStringList &pluginNames, QWidget *pParent /*= nullptr*/)
    : QDialog(pParent)
{
    ui.setupUi(this);

    ui.lwPluginsNames->addItems(pluginNames);
    if (!pluginNames.isEmpty())
        ui.lwPluginsNames->setCurrentRow(0);
}

QString CPluginSelectDialog::getSelectedPluginName() const
{
    return m_selectedPluginName;
}

void CPluginSelectDialog::onCurrentTextChanged(const QString &currentText)
{
    m_selectedPluginName = currentText;
    ui.btnOK->setEnabled(!m_selectedPluginName.isEmpty());
}
