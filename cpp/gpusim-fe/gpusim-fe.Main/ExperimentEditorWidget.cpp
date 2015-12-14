#include "ExperimentEditorWidget.h"

#include "../../QLogger/QLog"

#include <QFileDialog>

CExperimentEditorWidget::CExperimentEditorWidget(
    const Core::CExperimentPtr &pExperiment,
    Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *pConditionsEditor,
    Core::Widgets::IGeneratorSettingsEditorWidget *pSettingsEditor,
    QWidget *pParent /*= nullptr*/)
    : QFrame(pParent), m_pExperiment(pExperiment), m_pConditionsEditor(pConditionsEditor),
    m_pSettingsEditor(pSettingsEditor)
{
    ui.setupUi(this);

    m_pConditionsEditor->setObjectName("conditionsEditor");
    ui.gbConditionsLayout->addWidget(m_pConditionsEditor);

    m_pSettingsEditor->setObjectName("settingsEditor");
    ui.gbGeneratorSettingsLayout->addWidget(m_pSettingsEditor);
    ui.gbGeneratorSettings->setVisible(false);

    refresh();
}

CExperimentEditorWidget::~CExperimentEditorWidget()
{

}

Core::CExperiment CExperimentEditorWidget::getCurrentExperiment() const
{
    Core::CExperiment experiment = *m_pExperiment;
    experiment.setName(ui.leName->text());
    experiment.setDesc(ui.pteDesc->toPlainText());

    QString calibrationFilePath = ui.leCalibrationFilePath->text();
    if (!ui.cbCalibrationFile->isChecked())
        calibrationFilePath.clear();
    experiment.setCalibrationFilePath(calibrationFilePath);

    experiment.setConditions(m_pConditionsEditor->getConditions());
    experiment.setSettings(m_pSettingsEditor->getSettings());
    return experiment;
}

void CExperimentEditorWidget::refresh()
{
    ui.leName->setText(m_pExperiment->getName());
    ui.pteDesc->setPlainText(m_pExperiment->getDesc());

    QString calibrationFilePath = m_pExperiment->getCalibrationFilePath();
    ui.leCalibrationFilePath->setText(calibrationFilePath);
    ui.cbCalibrationFile->setChecked(!calibrationFilePath.isEmpty());
    ui.frmCalibrationFile->setVisible(!calibrationFilePath.isEmpty());

    m_pConditionsEditor->setConditions(m_pExperiment->getConditions());
    m_pSettingsEditor->setSettings(m_pExperiment->getSettings());
}

void CExperimentEditorWidget::selectCalibrateFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open gpusim-fe Calibration file"), QString(),
        tr("gpusim-fe Calibration files(*.gsc)"), 0, QFileDialog::ReadOnly);

    if (filePath.isEmpty())
        return;

    ui.leCalibrationFilePath->setText(filePath);
}