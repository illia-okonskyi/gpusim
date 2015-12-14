#include "Plot.h"

using namespace Core;

//////////////////////////////////////////////////////////////////////////

#pragma region Comparision support
bool CPlot::operator == (const CPlot &other) const
{
    return
        (m_name      == other.m_name       ) &&
        (m_desc      == other.m_desc       ) &&
        (m_xAxisName == other.m_xAxisName  ) &&
        (m_yAxisName == other.m_yAxisName  ) &&
        (m_curves    == other.m_curves     );
}

bool CPlot::operator != (const CPlot &other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Name property
QString CPlot::getName() const
{
    return m_name;
}

void CPlot::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}
#pragma endregion

#pragma region Desc property
QString CPlot::getDesc() const
{
    return m_desc;
}

void CPlot::setDesc(const QString &desc)
{
    if (m_desc == desc)
        return;

    m_desc = desc;
}
#pragma endregion

#pragma region XAxis property
QString CPlot::getXAxisName() const
{
    return m_xAxisName;
}

void CPlot::setXAxisName(const QString &xAxisName)
{
    if (m_xAxisName == xAxisName)
        return;

    m_xAxisName = xAxisName;
}
#pragma endregion

#pragma region YAxis property
QString CPlot::getYAxisName() const
{
    return m_yAxisName;
}

void CPlot::setYAxisName(const QString &yAxisName)
{
    if (m_yAxisName == yAxisName)
        return;

    m_yAxisName = yAxisName;
}
#pragma endregion

#pragma region Curves property
CPlotCurvesList CPlot::getCurves() const
{
    return getCurvesRef();
}

const CPlotCurvesList &CPlot::getCurvesRef() const
{
    return m_curves;
}

void CPlot::setCurves(const CPlotCurvesList &curves)
{
    if (m_curves == curves)
        return;

    m_curves = curves;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
QDataStream & operator << (QDataStream &out, const Core::CPlot &plot)
{
    out << plot.getName() << plot.getDesc() << plot.getXAxisName() << plot.getYAxisName() << plot.getCurvesRef();
    return out;
}

QDataStream & operator >> (QDataStream &in, Core::CPlot &plot)
{
    QString name;
    QString desc;
    QString xAxisName;
    QString yAxisName;
    Core::CPlotCurvesList curves;

    in >> name >> desc >> xAxisName >> yAxisName >> curves;

    plot.setName(name);
    plot.setDesc(desc);
    plot.setXAxisName(xAxisName);
    plot.setYAxisName(yAxisName);
    plot.setCurves(curves);

    return in;
}
#pragma endregion