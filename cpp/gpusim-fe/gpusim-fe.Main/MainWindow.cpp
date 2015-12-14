#include "MainWindow.h"
#include "ExperimentEditorWidget.h"
#include "PluginSelectDialog.h"
#include "SimulationDialog.h"
#include "ViewPlotsDialog.h"
#include "AboutDialog.h"

#include "../gpusim-fe.Core/CalibrationHelper/CalibrationHelper.h"
#include "../QLogger/QLog"

#include <QMessageBox>
#include <QFileDialog>

CMainWindow::CMainWindow(const ExperimentPlugin::CPluginsHost &pluginsHost)
    : QWidget(nullptr), m_pluginsHost(pluginsHost), m_pExperimentEditor(nullptr)
{
    ui.setupUi(this);
    reset();
}

CMainWindow::~CMainWindow()
{
    reset();
}

void CMainWindow::newExperiment()
{
    CPluginSelectDialog pluginSelectDialog(m_pluginsHost.getLoadedPluginsNames(), this);
    if (pluginSelectDialog.exec() == QDialog::Rejected)
        return;

    saveCurrentExperimentQueryAndReset();

    auto pGenerator = m_pluginsHost.getGenerator(pluginSelectDialog.getSelectedPluginName());
    auto conditions = pGenerator->createDefaultConditions();
    auto settings = pGenerator->createDefaultSettings();
    m_pCurrentExperiment = pGenerator->createExperiment(conditions);
    m_pCurrentExperiment->setSettings(settings);

    createExperimentEditor();
    updateUI();
}

void CMainWindow::openExperiment()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open gpusim-fe Experiment"), QString(),
        tr("gpusim-fe Experiments files(*.gse)"), 0, QFileDialog::ReadOnly);

    if (filePath.isEmpty())
        return;

    saveCurrentExperimentQueryAndReset();

    m_currentExperimentFilePath = filePath;
    Core::CExperiment experiment;
    if (!experiment.loadFromFile(m_currentExperimentFilePath))
        return;

    if (!m_pluginsHost.isPluginLoaded(experiment.getPluginName()))
    {
        QMessageBox::critical(this, tr("Plugin not found"),
            tr("Plugin %1 not loaded.").arg(experiment.getPluginName()));
        return;
    }

    auto pGenerator = m_pluginsHost.getGenerator(experiment);
    if (!pGenerator->isExperimentValid(experiment))
    {
        QMessageBox::critical(this, tr("Experiment not valid"),
            tr("Experiment is not valid for %1 plugin.").arg(experiment.getPluginName()));
        return;
    }

    m_pCurrentExperiment = Core::CExperimentPtr(new Core::CExperiment(experiment));
    
    createExperimentEditor();
    updateUI();
}

void CMainWindow::saveExperiment()
{
    if (m_currentExperimentFilePath.isEmpty())
        return saveExperimentAs();

    m_pCurrentExperiment->saveToFile(m_currentExperimentFilePath);
}

void CMainWindow::saveExperimentAs()
{
    QString filePath =  QFileDialog::getSaveFileName(this,
        tr("Save gpusim-fe Experiment As..."), QString(),
        tr("gpusim-fe Experiments files(*.gse)"));
    
    if (filePath.isEmpty())
        return;

    m_currentExperimentFilePath = filePath;
    saveExperiment();
}

void CMainWindow::apply()
{
    auto experiment = m_pExperimentEditor->getCurrentExperiment();
    auto pGenerator = m_pluginsHost.getGenerator(experiment);
    if (!pGenerator->isExperimentValid(experiment, false))
    {
        QMessageBox::critical(this, tr("Experiment not valid"),
            tr("Current user input is not valid for generator.\nSee log for details."));
        return;
    }

    experiment.clearPlots();
    pGenerator->updateSimulations(experiment);
    Core::CCalibrationHelper::applyCalibrationFileFilter(experiment);

    *m_pCurrentExperiment = experiment;
    updateUI();
}

void CMainWindow::runExperiment()
{
    apply();

    auto pGenerator = m_pluginsHost.getGenerator(*m_pCurrentExperiment);
    CSimulationDialog d(*m_pCurrentExperiment, pGenerator, this);
    d.exec();
    if (d.getExitCode() != Core::EC_Ok)
        return;

    auto exp = d.getExperiment();
    pGenerator->updatePlots(exp);
    *m_pCurrentExperiment = exp;

    updateUI();
    viewExperimentPlots();
}

void CMainWindow::viewExperimentPlots()
{
    CViewPlotsDialog(m_pCurrentExperiment->getPlotsRef(), this).exec();
}

void CMainWindow::about()
{
    CAboutDialog(m_pluginsHost.getLoadedPluginsNames(), this).exec();
}

//////////////////////////////////////////////////////////////////////////

void CMainWindow::updateUI()
{
    ui.btnSave->setEnabled(m_pCurrentExperiment);
    ui.btnSaveAs->setEnabled(m_pCurrentExperiment);
    ui.btnApply->setEnabled(m_pCurrentExperiment);
    ui.btnRun->setEnabled(m_pCurrentExperiment);
    ui.btnViewPlots->setEnabled(m_pCurrentExperiment && (!m_pCurrentExperiment->getPlotsRef().isEmpty()));

    if (m_pExperimentEditor)
        m_pExperimentEditor->refresh();

    QString currentExperimentFileName = tr("Experiment not opened");
    if (m_pCurrentExperiment)
    {
        currentExperimentFileName = m_currentExperimentFilePath.isEmpty()
            ? tr("Unnamed experiment")
            : QFileInfo(m_currentExperimentFilePath).fileName();
    }
    
    setWindowTitle(tr("gpusim-fe - %1").arg(currentExperimentFileName));
}


void CMainWindow::reset()
{
    deleteExperimentEditor();
    m_pCurrentExperiment.clear();
    m_currentExperimentFilePath.clear();
    
    updateUI();
}

void CMainWindow::createExperimentEditor()
{
    if (m_pExperimentEditor)
    {
        qLog_WarningMsg() << "Experiment editor is already created";
        return;
    }

    m_pExperimentEditor = new CExperimentEditorWidget(m_pCurrentExperiment,
        m_pluginsHost.createGeneratorBoundaryConditionsEditorWidget(*m_pCurrentExperiment),
        m_pluginsHost.createGeneratorSettingsEditorWidget(*m_pCurrentExperiment));
    m_pExperimentEditor->setObjectName("settingsEditor");
    ui.frmExperimentEditorLayout->addWidget(m_pExperimentEditor);
}


void CMainWindow::deleteExperimentEditor()
{
    if (!m_pExperimentEditor)
        return;

    ui.frmExperimentEditorLayout->removeWidget(m_pExperimentEditor);
    m_pExperimentEditor->setParent(nullptr);
    delete m_pExperimentEditor;
    m_pExperimentEditor = nullptr;
}

void CMainWindow::saveCurrentExperimentQueryAndReset()
{
    if (!m_pCurrentExperiment)
        return;

    auto queryResult = QMessageBox::question(this,
        tr("Save current experiment?"),
        tr("Do you want to save current experiment before?"),
        QMessageBox::Yes | QMessageBox::No);

    if (queryResult == QMessageBox::Yes)
        saveExperiment();

    reset();
}

