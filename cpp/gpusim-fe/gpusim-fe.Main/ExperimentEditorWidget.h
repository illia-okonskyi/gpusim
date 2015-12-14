#pragma once

#include "ui_ExperimentEditorWidget.h"

#include "../gpusim-fe.Core/Experiment/Experiment.h"
#include "../gpusim-fe.Core/Widgets/IGeneratorBoundaryConditionsEditorWidget.h"
#include "../gpusim-fe.Core/Widgets/IGeneratorSettingsEditorWidget.h"

class CExperimentEditorWidget: public QFrame
{
    Q_OBJECT;

public:
    CExperimentEditorWidget(const Core::CExperimentPtr &pExperiment,
        Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *pConditionsEditor,
        Core::Widgets::IGeneratorSettingsEditorWidget *pSettingsEditor,
        QWidget *pParent = nullptr);
    virtual ~CExperimentEditorWidget();

    Core::CExperiment getCurrentExperiment() const;
    void refresh();

public slots:
    void selectCalibrateFile();

private:
    Ui::CExperimentEditorWidget ui;
    Core::CExperimentPtr m_pExperiment;
    Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *m_pConditionsEditor;
    Core::Widgets::IGeneratorSettingsEditorWidget *m_pSettingsEditor;
};

