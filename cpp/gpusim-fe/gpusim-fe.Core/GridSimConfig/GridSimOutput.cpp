#include "GridSimOutput.h"
#include "Serialization/JavaXMLHelper.h"
#include "Serialization/JavaXMLListSerialize.hpp"

using namespace Core::GridSimConfig;
using namespace Core::GridSimConfig::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CGridSimOutput::c_minVersion                 = 1;
const double  CGridSimOutput::c_minTotalSimulationTime     = 0.0f;
const quint32 CGridSimOutput::c_currentOutputVersion       = 1;
const double  CGridSimOutput::c_defaultTotalSimulationTime = c_minTotalSimulationTime;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CGridSimOutput::c_className                   = QString("gpusim.config.GridSimOutput");
const QString CGridSimOutput::c_versionPropName             = QString("version");
const QString CGridSimOutput::c_totalSimulationTimePropName = QString("totalSimulationTime");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and equality comparision support
CGridSimOutput::CGridSimOutput(double totalSimulationTime /*= c_defaultTotalSimulationTime*/)
    :IJavaXMLSerialize(c_className), m_version(c_currentOutputVersion), 
    m_totalSimulationTime(c_defaultTotalSimulationTime)
{
    setTotalSimulationTime(totalSimulationTime);
}

bool CGridSimOutput::operator==(const CGridSimOutput& other) const
{
    return (m_version == other.m_version ) && (m_totalSimulationTime == other.m_totalSimulationTime);
}

bool CGridSimOutput::operator!=(const CGridSimOutput& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Version property
quint32 CGridSimOutput::getVersion() const
{
    return m_version;
}

void CGridSimOutput::setVersion(quint32 version)
{
    if (m_version == version)
        return;

    Q_ASSERT(version >= c_minVersion);
    m_version = version;
}
#pragma endregion

#pragma region TotalSimulationTime property
double CGridSimOutput::getTotalSimulationTime() const
{
    return m_totalSimulationTime;
}

void CGridSimOutput::setTotalSimulationTime(double totalSimulationTime)
{
    if (m_totalSimulationTime == totalSimulationTime)
        return;

    Q_ASSERT(totalSimulationTime >= c_minTotalSimulationTime);
    m_totalSimulationTime = totalSimulationTime;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CGridSimOutput::isValid() const
{
    return (m_version >= c_minVersion) && (m_totalSimulationTime >= c_minTotalSimulationTime);
}

void CGridSimOutput::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_versionPropName,             getVersion()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_totalSimulationTimePropName, getTotalSimulationTime()));
}

void CGridSimOutput::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 version;
    double totalSimulationTime;

    CJavaXMLHelper::getPropertyValue(elem, c_versionPropName, version, c_currentOutputVersion);
    CJavaXMLHelper::getPropertyValue(elem, c_totalSimulationTimePropName, totalSimulationTime,
        c_defaultTotalSimulationTime);

    setVersion(version);
    setTotalSimulationTime(totalSimulationTime);
}

IJavaXMLSerializePtr CGridSimOutput::create() const
{
    return IJavaXMLSerializePtr(new CGridSimOutput());
}

void CGridSimOutput::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CGridSimOutput>();
    *this = *ptr;
}
#pragma endregion