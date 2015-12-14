#pragma once

#include "PlotCurve.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CPlot
    {
    public:
        QString getName() const;
        void setName(const QString &name);

        QString getDesc() const;
        void setDesc(const QString &desc);

        QString getXAxisName() const;
        void setXAxisName(const QString &xAxisName);

        QString getYAxisName() const;
        void setYAxisName(const QString &yAxisName);

        CPlotCurvesList getCurves() const;
        const CPlotCurvesList &getCurvesRef() const;
        void setCurves(const CPlotCurvesList &curves);

        bool operator == (const CPlot &other) const;
        bool operator != (const CPlot &other) const;
    
    private:
        QString m_name;
        QString m_desc;
        QString m_xAxisName;
        QString m_yAxisName;
        CPlotCurvesList m_curves;
    };

    typedef QVector<CPlot> CPlotsList;
}

Q_DECLARE_METATYPE(Core::CPlot);
Q_DECLARE_METATYPE(Core::CPlotsList);

GPUSIM_FE_CORE_EXPORT QDataStream &operator << (QDataStream &out, const Core::CPlot &plot);
GPUSIM_FE_CORE_EXPORT QDataStream &operator >> (QDataStream &in, Core::CPlot &plot);