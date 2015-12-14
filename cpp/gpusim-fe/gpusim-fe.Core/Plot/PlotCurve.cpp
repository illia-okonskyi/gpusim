#include "PlotCurve.h"

using namespace Core;

//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and comparision support
CPlotCurve::CPlotCurve(const QString &name /*= QString()*/, const CCurvePoints &points /*= CPlotPoints()*/)
    : m_name(name), m_points(points)
{

}

bool CPlotCurve::operator == (const CPlotCurve &other) const
{
    return
        (m_name   == other.m_name  ) &&
        (m_points == other.m_points);
}

bool CPlotCurve::operator != (const CPlotCurve &other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Name property
QString CPlotCurve::getName() const
{
    return m_name;
}

void CPlotCurve::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}
#pragma endregion

#pragma region Points property
CCurvePoints CPlotCurve::getPoints() const
{
    return m_points;
}

void CPlotCurve::setPoints(const CCurvePoints &points)
{
    if (m_points == points)
        return;

    m_points = points;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
QDataStream & operator << (QDataStream &out, const Core::CPlotCurve &curve)
{
    out << curve.getName() << curve.getPoints();
    return out;
}

QDataStream & operator >> (QDataStream &in, Core::CPlotCurve &curve)
{
    QString name;
    Core::CCurvePoints points;

    in >> name >> points;

    curve.setName(name);
    curve.setPoints(points);

    return in;
}
#pragma endregion