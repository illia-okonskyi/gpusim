#pragma once

#include "../gpusim-fe.ExperimentPlugin/PluginsHost.h"
#include "ui_MainWindow.h"

class CExperimentEditorWidget;

class CMainWindow : public QWidget
{
    Q_OBJECT;

public:
    CMainWindow(const ExperimentPlugin::CPluginsHost &pluginsHost);
    ~CMainWindow();

public slots:
    void newExperiment();
    void openExperiment();
    void saveExperiment();
    void saveExperimentAs();
    void apply();
    void runExperiment();
    void viewExperimentPlots();
    void about();

private:
    void updateUI();
    void reset();

    void createExperimentEditor();
    void deleteExperimentEditor();

    void saveCurrentExperimentQueryAndReset();

private:
    const ExperimentPlugin::CPluginsHost &m_pluginsHost;

    Ui::CMainWindow ui;
    CExperimentEditorWidget *m_pExperimentEditor;

    Core::CExperimentPtr m_pCurrentExperiment;
    QString m_currentExperimentFilePath;
};