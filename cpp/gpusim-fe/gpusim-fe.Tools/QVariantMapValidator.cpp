#include "QVariantMapValidator.h"

#include "../QLogger/QLog"

using namespace Tools;

//////////////////////////////////////////////////////////////////////////

void QVariantMapValidator::addValueInfo(const QString &name, QVariant::Type type)
{
    if (m_infos.contains(name))
    {
        qLog_DebugMsg() << "Value info with name " << name << " is already exist."; 
        return;
    }

    m_infos[name] = type;
}


bool QVariantMapValidator::validate(const QVariantMap &map) const
{
    if (map.size() != m_infos.size())
        return false;

    for (auto I = map.constBegin(), E = map.constEnd(); I != E; ++I)
    {
        if (!m_infos.contains(I.key()))
            return false;
        
        if (m_infos.value(I.key()) != I.value().type())
            return false;
    }

    return true;
}
