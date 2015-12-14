#include "Experiment.h"
#include "../../QLogger/QLog"

#include <QSettings>
#include <qmath.h>

using namespace Core;

//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Equality comparision support
CExperiment::CExperiment()
    : m_name("<No Experiment Name>"), m_desc("<No Experiment Description>")
{
}

bool CExperiment::operator==(const CExperiment& other) const
{
    return 
        (m_name                == other.m_name               ) &&
        (m_desc                == other.m_desc               ) &&
        (m_pluginName          == other.m_pluginName         ) &&
        (m_calibrationFilePath == other.m_calibrationFilePath) &&
        (m_conditions          == other.m_conditions         ) &&
        (m_settings            == other.m_settings           ) &&
        (m_simulations         == other.m_simulations        ) &&
        (m_plots               == other.m_plots              );
}

bool CExperiment::operator!=(const CExperiment& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Name property
QString CExperiment::getName() const
{
    return m_name;
}

void CExperiment::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}
#pragma endregion

#pragma region Desc property
QString CExperiment::getDesc() const
{
    return m_desc;
}

void CExperiment::setDesc(const QString &desc)
{
    if (m_desc == desc)
        return;

    m_desc = desc;
}
#pragma endregion

#pragma region PluginName property
QString CExperiment::getPluginName() const
{
    return m_pluginName;
}

void CExperiment::setPluginName(const QString &pluginName)
{
    if (m_pluginName == pluginName)
        return;

    m_pluginName = pluginName;
}
#pragma endregion

#pragma region CalibrationFilePath property
QString CExperiment::getCalibrationFilePath() const
{
    return m_calibrationFilePath;
}

void CExperiment::setCalibrationFilePath(const QString &calibrationFilePath)
{
    if (m_calibrationFilePath == calibrationFilePath)
        return;

    m_calibrationFilePath = calibrationFilePath;
}
#pragma endregion

#pragma region Conditions property
CGeneratorBoundaryConditions CExperiment::getConditions() const
{
    return m_conditions;
}

void CExperiment::setConditions(const CGeneratorBoundaryConditions &conditions)
{
    if (m_conditions == conditions)
        return;

    m_conditions = conditions;
}
#pragma endregion

#pragma region Settings property
CGeneratorSettings CExperiment::getSettings() const
{
    return m_settings;
}

void CExperiment::setSettings(const CGeneratorSettings &settings)
{
    if (m_settings == settings)
        return;

    m_settings = settings;
}
#pragma endregion

#pragma region Simulations property
CSimulationsList CExperiment::getSimulations() const
{
    return getSimulationsRef();
}

const CSimulationsList & CExperiment::getSimulationsRef() const
{
    return m_simulations;
}

void CExperiment::setSimulations(const CSimulationsList &simulations)
{
    if (m_simulations == simulations)
        return;

    m_simulations = simulations;
}
#pragma endregion

#pragma region Plots property
CPlotsList CExperiment::getPlots() const
{
    return getPlotsRef();
}

const CPlotsList &CExperiment::getPlotsRef() const
{
    return m_plots;
}

void CExperiment::setPlots(const CPlotsList &plots)
{
    if (m_plots == plots)
        return;

    m_plots = plots;
}

void CExperiment::clearPlots()
{
    m_plots.clear();
}
#pragma endregion
#pragma endregion

#pragma region Serialization support
bool CExperiment::saveToFile(const QString &filePath) const
{
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue("Experiment-project", QVariant::fromValue(*this));
    settings.sync();
    if (settings.status() != QSettings::NoError)
    {
        qLog_WarningMsg() << "Failed to save experiment to file " << filePath << ". Status: " << settings.status();
        return false;
    }

    return true;
}

bool CExperiment::loadFromFile(const QString &filePath)
{
    QSettings settings(filePath, QSettings::IniFormat);
    auto experiment = settings.value("Experiment-project").value<CExperiment>();
    if (settings.status() != QSettings::NoError)
    {
        qLog_WarningMsg() << "Failed to load experiment from file " << filePath << ". Status: " << settings.status();
        return false;
    }  

    *this = experiment;
    return true;
}

QDataStream &operator << (QDataStream &out, const Core::CExperiment &exp)
{
    out << exp.getName() << exp.getDesc() << exp.getPluginName() << exp.getCalibrationFilePath() <<
        exp.getConditions() << exp.getSettings() << exp.getSimulations() << exp.getPlots();
    return out;
}

QDataStream &operator >> (QDataStream &in, Core::CExperiment &exp)
{
    QString name;
    QString desc;
    QString pluginName;
    QString calibrationFilePath;
    CGeneratorBoundaryConditions conditions;
    CGeneratorSettings settings;
    CSimulationsList simulations;
    CPlotsList plots;

    in >> name >> desc >> pluginName >> calibrationFilePath>> conditions >> settings >> simulations >> plots;

    exp.setName(name);
    exp.setDesc(desc);
    exp.setPluginName(pluginName);
    exp.setCalibrationFilePath(calibrationFilePath);
    exp.setConditions(conditions);
    exp.setSettings(settings);
    exp.setSimulations(simulations);
    exp.setPlots(plots);

    return in;
}
#pragma endregion

bool CExperiment::calculateStatistics(const CExperiment &experiment, double &relativeError,
    double &squareDistance)
{
    squareDistance = 0.0f;
    relativeError = 0.0f;

    if (experiment.getSimulationsRef().isEmpty())
        return false;

    auto I = experiment.getSimulationsRef().constBegin();
    auto E = experiment.getSimulationsRef().constEnd();
    for (; I != E; ++I)
    {
        squareDistance += qPow(I->getTimeDistance(), 2.0);
        auto err = I->getRelativeError();
        if (err == -1.0f)
            return false;

        relativeError += err;
    }

    squareDistance /= double(experiment.getSimulationsRef().size());
    relativeError /= double(experiment.getSimulationsRef().size());
    return true;
}