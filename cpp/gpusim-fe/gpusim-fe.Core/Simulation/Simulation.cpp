#include "Simulation.h"

#include <qmath.h>

using namespace Core;

//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and comparision support
CSimulation::CSimulation()
    : m_name("<No Simulation Name>"), m_desc("<No Simulation Description>"), m_calcTime(0.0f), m_simTime(0.0f)
{
}

bool CSimulation::operator==(const CSimulation& other) const
{
    return
        (m_name     == other.m_name    ) &&
        (m_desc     == other.m_desc    ) &&
        (m_calcTime == other.m_calcTime) &&
        (m_simTime  == other.m_simTime );
}

bool CSimulation::operator!=(const CSimulation& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Name property
QString CSimulation::getName() const
{
    return m_name;
}

void CSimulation::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}
#pragma endregion

#pragma region Desc property
QString CSimulation::getDesc() const
{
    return m_desc;
}

void CSimulation::setDesc(const QString &desc)
{
    if (m_desc == desc)
        return;

    m_desc = desc;
}
#pragma endregion

#pragma region GeneratorInput property
CGeneratorInput CSimulation::getGeneratorInput() const
{
    return m_generatorInput;
}

void CSimulation::setGeneratorInput(const CGeneratorInput &generatorInput)
{
    if (m_generatorInput == generatorInput)
        return;

    m_generatorInput = generatorInput;
}
#pragma endregion

#pragma region CalcTime property
double CSimulation::getCalcTime() const
{
    return m_calcTime;
}

void CSimulation::setCalcTime(double calcTime)
{
    if (m_calcTime == calcTime)
        return;

    m_calcTime = calcTime;
}
#pragma endregion

#pragma region SimTime property
double CSimulation::getSimTime() const
{
    return m_simTime;
}

void CSimulation::setSimTime(double simTime)
{
    if (m_simTime == simTime)
        return;

    m_simTime = simTime;
}
#pragma endregion
#pragma endregion

#pragma region Time distance and relative error calculation
double CSimulation::getTimeDistance() const
{
    return qAbs(m_calcTime - m_simTime);
}

double CSimulation::getRelativeError() const
{
    if (m_calcTime == 0.0f)
        return -1.0f;

    return (getTimeDistance() * 100.0) / m_calcTime;
}
#pragma endregion

#pragma region Serialization support
QDataStream &operator << (QDataStream &out, const Core::CSimulation &sim)
{
    out << sim.getName() << sim.getDesc() << sim.getGeneratorInput() << sim.getCalcTime() << sim.getSimTime();
    return out;
}

QDataStream &operator >> (QDataStream &in, Core::CSimulation &sim)
{
    QString name;
    QString desc;
    Core::CGeneratorInput genInput;
    double calcTime = 0.0f;
    double simTime = 0.0f;

    in >> name >> desc >> genInput >> calcTime >> simTime;

    sim.setName(name);
    sim.setDesc(desc);
    sim.setGeneratorInput(genInput);
    sim.setCalcTime(calcTime);
    sim.setSimTime(simTime);

    return in;
}
#pragma endregion