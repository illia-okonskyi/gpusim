#pragma once

#include "Tools_global.h"

#include <QVariantMap>
#include <QHash>

namespace Tools
{
    class GPUSIM_FE_TOOLS_EXPORT QVariantMapValidator
    {
    public:
        void addValueInfo(const QString &name, QVariant::Type type);
        bool validate(const QVariantMap &map) const;

    private:
        QHash<QString, QVariant::Type> m_infos;
    };
}