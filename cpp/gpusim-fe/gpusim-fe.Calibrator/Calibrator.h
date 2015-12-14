#pragma once

#include "../gpusim-fe.Core/Experiment/Experimenter.h"
#include "../gpusim-fe.ExperimentPlugin/PluginsHost.h"
#include "CalibrationProfile/CalibrationProfile.h"

#include <QObject>

class CCalibrator: public QObject
{
    Q_OBJECT;
public:
    CCalibrator(const ExperimentPlugin::CPluginsHost &pluginsHost, const QString &experimenFilePath,
        const QString &profileFilePath);

public slots:
    void calibrate();

signals:
    void calibrated();

private:
    void processCalibrate();
    bool loadExperiment();
    bool generateSettings();
    bool findBestSettings();
    void saveResults();

    static void printSettings(const Core::CGeneratorSettings &settings, double relativeError = -1.0f,
        double squareDistance = -1.0f);

private:
    const ExperimentPlugin::CPluginsHost &m_pluginsHost;
    QString m_experimenFilePath;
    QString m_profileFilePath;

    CCalibrationProfile m_profile;
    CGeneratorSettingsSet m_settingsSet;

    Core::CExperiment m_experiment;
    double m_bestRelativeError;
    double m_bestSquareDistance;
};