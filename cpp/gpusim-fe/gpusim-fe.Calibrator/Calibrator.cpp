#include "Calibrator.h"
#include "../gpusim-fe.Core/Experiment/Experimenter.h"
#include "../QLogger/QLog"

CCalibrator::CCalibrator(const ExperimentPlugin::CPluginsHost &pluginsHost, const QString &experimenFilePath,
    const QString &profileFilePath)
    : QObject(nullptr), m_pluginsHost(pluginsHost), m_experimenFilePath(experimenFilePath),
    m_profileFilePath(profileFilePath)
{

}

void CCalibrator::calibrate()
{
    processCalibrate();
    emit calibrated();
}

//////////////////////////////////////////////////////////////////////////

void CCalibrator::processCalibrate()
{
    if (!loadExperiment())
    {
        qLog_WarningMsg() << "Failed to load experiment";
        return;
    }

    if (!readCalibrationProfileFromFile(m_profileFilePath, m_profile))
    {
        qLog_WarningMsg() << "Failed to load calibration profile";
        return;
    }

    if (!generateSettings())
    {
        qLog_WarningMsg() << "Failed to load experiment";
        return;
    }

    if (!findBestSettings())
    {
        qLog_WarningMsg() << "Error during best settings search.";
        return;
    }

    qLog_DebugMsg() << "--------------------";
    qLog_DebugMsg() << "Calibration results:";
    printSettings(m_experiment.getSettings(), m_bestRelativeError, m_bestSquareDistance);

    if (!m_experiment.saveToFile(m_experimenFilePath))
        qLog_WarningMsg() << "Failed to save experiment.";
}

//////////////////////////////////////////////////////////////////////////

bool CCalibrator::loadExperiment()
{
    if (!m_experiment.loadFromFile(m_experimenFilePath))
        return false;

    bool pluginLoaded = m_pluginsHost.isPluginLoaded(m_experiment.getPluginName());
    if (!pluginLoaded)
        qLog_WarningMsg() << "Plugin " << m_experiment.getPluginName() << " is not loaded.";

    return pluginLoaded;
}

bool CCalibrator::generateSettings()
{
    auto pGenerator = m_pluginsHost.getGenerator(m_experiment);
    m_settingsSet = createGeneratorSettingsSet(m_profile, pGenerator->createDefaultSettings());
    if (m_settingsSet.isEmpty())
    {
        qLog_WarningMsg() << "No generator settings created for calibration profile";;
        return false;
    }

    qLog_DebugMsg() << m_settingsSet.size() << " generator settings created.";
    bool settingsValid = pGenerator->isSettingsValid(m_settingsSet.front());
    if (!settingsValid)
        qLog_WarningMsg() << "Generated settings not valid for " << m_experiment.getPluginName() << " plugin.";

    return settingsValid;
}

bool CCalibrator::findBestSettings()
{
    auto pGenerator = m_pluginsHost.getGenerator(m_experiment);
    Core::CExperiment currentExperiment = m_experiment;

    Core::CExperimenter experimenter;

    for (size_t i = 0; i < m_settingsSet.size(); ++i)
    {
        auto currentSettings = m_settingsSet.at(i);
        qLog_DebugMsg() << "Processing generator settings #" << i << " of " << m_settingsSet.size();
        printSettings(currentSettings);

        currentExperiment.setSettings(currentSettings);
        auto ec = experimenter.executeSynchronously(m_experiment, pGenerator);
        if (ec != Core::EC_Ok)
            return false;
        
        currentExperiment = experimenter.getExperiment();

        double currentSquareDistance = 0.0f;
        double currentRelativeError = 0.0f;

        if (!Core::CExperiment::calculateStatistics(currentExperiment, currentRelativeError, currentSquareDistance))
            return false;

        bool foundBest = (i == 0) ? true : currentRelativeError < m_bestRelativeError;
        if (!foundBest)
            continue;


        qLog_DebugMsg() << "Found new best settings";
        printSettings(currentSettings, currentRelativeError, currentSquareDistance);

        m_experiment = currentExperiment;
        m_bestRelativeError = currentRelativeError;
        m_bestSquareDistance = currentSquareDistance;

        pGenerator->updatePlots(m_experiment);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

void CCalibrator::printSettings(const Core::CGeneratorSettings &settings,  double relativeError /*= -1.0f*/,
    double squareDistance /*= -1.0f*/)
{
    qLog_DebugMsg() << "Generator settings:";
    if (relativeError != -1.0f)
    {
        qLog_DebugMsg() << "  -> Averange relative error:  " << relativeError << "%";
        qLog_DebugMsg() << "  -> Averange square distance: " << squareDistance;
    }

    for (auto I = settings.constBegin(), E = settings.constEnd(); I != E; ++I)
        qLog_DebugMsg() << "  " << I.key() << " = " << I.value().toString();
}