#pragma once

#include "../gpusim-fe.Core/Experiment/Experimenter.h"
#include "../gpusim-fe.Core/IGenerator/IGenerator.h"
#include "ui_SimulationDialog.h"

class CSimulationDialog: public QDialog
{
    Q_OBJECT;
public:
    CSimulationDialog(const Core::CExperiment &experiment, const Core::IGeneratorPtr &pGenerator,
        QWidget *pParent = nullptr);
    ~CSimulationDialog();

    Core::CExitCode getExitCode() const;
    Core::CExperiment getExperiment() const;

public slots:
    void start();
    void cancel();

private slots:
    void onExperimentExecuted(Core::CExitCode ec);

private:
    Ui::CSimulationDialog ui;
    Core::CExperiment m_experiment;
    Core::CExperimenter m_experimenter;
    Core::CExitCode m_ec;
};