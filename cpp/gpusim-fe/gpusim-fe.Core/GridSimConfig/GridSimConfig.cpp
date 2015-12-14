#include "GridSimConfig.h"
#include "Serialization/JavaXMLHelper.h"
#include "Serialization/JavaXMLListSerialize.hpp"

using namespace Core::GridSimConfig;
using namespace Core::GridSimConfig::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CGridSimConfig::c_minVersion           = 1;
const double  CGridSimConfig::c_minLinkBaudRate      = 1.0f;
const quint32 CGridSimConfig::c_currentConfigVersion = 1;
const double  CGridSimConfig::c_defaultLinkBaudRate  = 1.0f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CGridSimConfig::c_className            = QString("gpusim.config.GridSimConfig");
const QString CGridSimConfig::c_versionPropName      = QString("version");
const QString CGridSimConfig::c_linkBaudRatePropName = QString("linkBaudRate");
const QString CGridSimConfig::c_resourcesPropName    = QString("resources");
const QString CGridSimConfig::c_gridletsPropName     = QString("gridlets");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and equality comparision support
CGridSimConfig::CGridSimConfig(double linkBaudRate /*= c_defaultLinkBaudRate*/,
    const CGridSimResourcesConfig &resources /*= CGridSimResourcesConfig()*/,
    const CGridSimGridletsConfig &gridlets /*= CGridSimGridletsConfig()*/)
    : IJavaXMLSerialize(c_className), m_version(c_currentConfigVersion), m_linkBaudRate(c_defaultLinkBaudRate)
{
    setLinkBaudRate(linkBaudRate);
    setResources(resources);
    setGridlets(gridlets);
}

bool CGridSimConfig::operator==(const CGridSimConfig& other) const
{
    return
        (m_version   == other.m_version  ) &&
        (m_resources == other.m_resources) &&
        (m_gridlets  == other.m_gridlets );
}

bool CGridSimConfig::operator!=(const CGridSimConfig& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Version property
quint32 CGridSimConfig::getVersion() const
{
    return m_version;
}

void CGridSimConfig::setVersion(quint32 version)
{
    if (m_version == version)
        return;

    Q_ASSERT(version >= c_minVersion);
    m_version = version;
}
#pragma endregion

#pragma region LinkBaudRate property
double CGridSimConfig::getLinkBaudRate() const
{
    return m_linkBaudRate;
}

void CGridSimConfig::setLinkBaudRate(double linkBaudRate)
{
    if (m_linkBaudRate == linkBaudRate)
        return;

    Q_ASSERT(linkBaudRate >= c_minLinkBaudRate);
    m_linkBaudRate = linkBaudRate;
}
#pragma endregion

#pragma region Resources property
CGridSimResourcesConfig CGridSimConfig::getResources() const
{
    return getResourcesRef();
}

const CGridSimResourcesConfig &CGridSimConfig::getResourcesRef() const
{
    return m_resources;
}

void CGridSimConfig::setResources(const CGridSimResourcesConfig &resources)
{
    if (m_resources == resources)
        return;

    Q_ASSERT(!resources.isEmpty());
    m_resources = resources;
}
#pragma endregion

#pragma region Gridlets property
CGridSimGridletsConfig CGridSimConfig::getGridlets() const
{
    return getGridletsRef();
}

const CGridSimGridletsConfig &CGridSimConfig::getGridletsRef() const
{
    return m_gridlets;
}

void CGridSimConfig::setGridlets(const CGridSimGridletsConfig &gridlets)
{
    if (m_gridlets == gridlets)
        return;

    Q_ASSERT(!gridlets.isEmpty());
    m_gridlets = gridlets;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CGridSimConfig::isValid() const
{
    return (m_version >= c_minVersion) && (m_linkBaudRate >= c_minLinkBaudRate) &&
        (!(m_resources.isEmpty() || m_gridlets.isEmpty()));
}

void CGridSimConfig::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_versionPropName,      getVersion()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_linkBaudRatePropName, getLinkBaudRate()));

    elem.appendChild(createListPropertyElement(doc, c_resourcesPropName, getResourcesRef().constBegin(),
        getResourcesRef().constEnd()));
    elem.appendChild(createListPropertyElement(doc, c_gridletsPropName,  getGridletsRef().constBegin(),
        getGridletsRef().constEnd()));
}

void CGridSimConfig::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 version;
    double linkBaudRate;
    CGridSimResourcesConfig resources;
    CGridSimGridletsConfig gridlets;

    CJavaXMLHelper::getPropertyValue(elem, c_versionPropName,      version,      c_currentConfigVersion);
    CJavaXMLHelper::getPropertyValue(elem, c_linkBaudRatePropName, linkBaudRate, c_defaultLinkBaudRate);

    Serialization::getListPropertyValue(elem, c_resourcesPropName, std::back_inserter(resources));
    Serialization::getListPropertyValue(elem, c_gridletsPropName,  std::back_inserter(gridlets));

    setVersion(version);
    setLinkBaudRate(linkBaudRate);
    setResources(resources);
    setGridlets(gridlets);
}

IJavaXMLSerializePtr CGridSimConfig::create() const
{
    return IJavaXMLSerializePtr(new CGridSimConfig());
}

void CGridSimConfig::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CGridSimConfig>();
    *this = *ptr;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Tools
CGridSimConfig CGridSimConfig::createTestConfig()
{
    // Machines list
    //
    CGridSimMachinesConfig machines;
    machines.append(CGridSimMachineConfig(6, 3, 5));

    //
    // Resources list
    //
    CGridSimResourceConfig resource("Test Architecture", "Test OS");
    resource.setBaudRate(100.0f);
    resource.setCostPerSec(55.0f);
    resource.setMachines(machines);
    resource.setCount(2);

    CGridSimResourcesConfig resources;
    resources.append(resource);

    // Gridlets list
    //
    CGridSimGridletsConfig gridltes;
    gridltes.append(CGridSimGridletConfig(100.0f, 50, 30, 10));

    // And finally config
    //
    return CGridSimConfig(560.0f, resources, gridltes);
}
#pragma endregion