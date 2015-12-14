#pragma once

#include "../Core_global.h"

#include <QVector>
#include <QPointF>
#include <QString>
#include <QMetaType>

namespace Core
{
    typedef QVector<QPointF> CCurvePoints;

    class GPUSIM_FE_CORE_EXPORT CPlotCurve
    {
    public:
        CPlotCurve(const QString &name = QString(), const CCurvePoints &points = CCurvePoints());

        QString getName() const;
        void setName(const QString &name);

        CCurvePoints getPoints() const;
        void setPoints(const CCurvePoints &points);

        bool operator == (const CPlotCurve &other) const;
        bool operator != (const CPlotCurve &other) const;

    private:
        QString m_name;
        CCurvePoints m_points;
    };

    typedef QVector<CPlotCurve> CPlotCurvesList;
}

Q_DECLARE_METATYPE(Core::CPlotCurve);
Q_DECLARE_METATYPE(Core::CPlotCurvesList);

GPUSIM_FE_CORE_EXPORT QDataStream &operator << (QDataStream &out, const Core::CPlotCurve &curve);
GPUSIM_FE_CORE_EXPORT QDataStream &operator >> (QDataStream &in, Core::CPlotCurve &curve);