#include "MMGeneratorSettingsEditorWidget.h"
#include "../MMGenerator.h"

using namespace MMEP;

CMMGeneratorSettingsEditorWidget::CMMGeneratorSettingsEditorWidget(QWidget *pParent /*= nullptr*/)
    : Core::Widgets::IGeneratorSettingsEditorWidget(pParent)
{
    ui.setupUi(this);
}

CMMGeneratorSettingsEditorWidget::~CMMGeneratorSettingsEditorWidget()
{

}

Core::CGeneratorSettings CMMGeneratorSettingsEditorWidget::getSettings() const
{
    quint32 cpuMachinePECount  = ui.sbCPUMachinePECount->value();
    quint32 cpuMachinePERating = ui.sbCPUMachinePERating->value();
    quint32 gpuMachinePECount  = ui.sbGPUMachinePECount->value();
    quint32 gpuMachinePERating = ui.sbGPUMachinePERating->value();
    double  resourceBaudRate   = ui.sbResourceBaudRate->value() * double(1000000);
    double  linkBaudRate       = ui.sbLinkBaudRate->value() * double(1000000);
    double  loadOperationCost  = ui.sbLoadOperationCost->value();
    double  saveOperationCost  = ui.sbSaveOperationCost->value();    
    
    Core::CGeneratorSettings settings;
    settings[CMMGenerator::c_cpuMachinePECount_settingName ] = QVariant::fromValue(cpuMachinePECount );
    settings[CMMGenerator::c_cpuMachinePERating_settingName] = QVariant::fromValue(cpuMachinePERating);
    settings[CMMGenerator::c_gpuMachinePECount_settingName ] = QVariant::fromValue(gpuMachinePECount );
    settings[CMMGenerator::c_gpuMachinePERating_settingName] = QVariant::fromValue(gpuMachinePERating);
    settings[CMMGenerator::c_resourceBaudRate_settingName  ] = QVariant::fromValue(resourceBaudRate  );
    settings[CMMGenerator::c_linkBaudRate_settingName      ] = QVariant::fromValue(linkBaudRate      );
    settings[CMMGenerator::c_loadOperationCost_settingName ] = QVariant::fromValue(loadOperationCost );
    settings[CMMGenerator::c_saveOperationCost_settingName ] = QVariant::fromValue(saveOperationCost );
    return settings;
}

void CMMGeneratorSettingsEditorWidget::setSettings(const Core::CGeneratorSettings &settings)
{
    quint32 cpuMachinePECount  = settings[CMMGenerator::c_cpuMachinePECount_settingName ].toUInt();
    quint32 cpuMachinePERating = settings[CMMGenerator::c_cpuMachinePERating_settingName].toUInt();
    quint32 gpuMachinePECount  = settings[CMMGenerator::c_gpuMachinePECount_settingName ].toUInt();
    quint32 gpuMachinePERating = settings[CMMGenerator::c_gpuMachinePERating_settingName].toUInt();
    double  resourceBaudRate   = settings[CMMGenerator::c_resourceBaudRate_settingName  ].toDouble() / double(1000000);
    double  linkBaudRate       = settings[CMMGenerator::c_linkBaudRate_settingName      ].toDouble() / double(1000000);
    double  loadOperationCost  = settings[CMMGenerator::c_loadOperationCost_settingName ].toDouble();
    double  saveOperationCost  = settings[CMMGenerator::c_saveOperationCost_settingName ].toDouble();

    ui.sbCPUMachinePECount->setValue(cpuMachinePECount);
    ui.sbCPUMachinePERating->setValue(cpuMachinePERating);
    ui.sbGPUMachinePECount->setValue(gpuMachinePECount);
    ui.sbGPUMachinePERating->setValue(gpuMachinePERating);
    ui.sbResourceBaudRate->setValue(resourceBaudRate);
    ui.sbLinkBaudRate->setValue(linkBaudRate);
    ui.sbLoadOperationCost->setValue(loadOperationCost);
    ui.sbSaveOperationCost->setValue(saveOperationCost); 
}