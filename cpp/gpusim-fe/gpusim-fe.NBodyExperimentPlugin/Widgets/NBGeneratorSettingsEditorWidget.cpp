#include "NBGeneratorSettingsEditorWidget.h"
#include "../NBGenerator.h"

using namespace NBEP;

CNBGeneratorSettingsEditorWidget::CNBGeneratorSettingsEditorWidget(QWidget *pParent /*= nullptr*/)
    : Core::Widgets::IGeneratorSettingsEditorWidget(pParent)
{
    ui.setupUi(this);
}

CNBGeneratorSettingsEditorWidget::~CNBGeneratorSettingsEditorWidget()
{

}

Core::CGeneratorSettings CNBGeneratorSettingsEditorWidget::getSettings() const
{
    quint32 gpuCoreRating                   = ui.sbGPUCoreRating->value();
    quint32 limitationsDivider              = ui.sbLimitationsDivider->value();
    double  smallTPBPenaltyWeight           = ui.sbSmallTPBPenaltyWeight->value();
    double  largeTPBPenaltyWeight           = ui.sbLargeTPBPenaltyWeight->value();
    double  multiplicativeLengthScaleFactor = ui.sbMultiplicativeLengthScaleFactor->value();
    double  additiveLengthScaleFactor       = ui.sbAdditiveLengthScaleFactor->value();

    Core::CGeneratorSettings settings;
    settings[CNBGenerator::c_gpuCoreRating_settingName                  ] = QVariant::fromValue(gpuCoreRating                  );
    settings[CNBGenerator::c_limitationsDivider_settingName             ] = QVariant::fromValue(limitationsDivider             );
    settings[CNBGenerator::c_smallTPBPenaltyWeight_settingName          ] = QVariant::fromValue(smallTPBPenaltyWeight          );
    settings[CNBGenerator::c_largeTPBPenaltyWeight_settingName          ] = QVariant::fromValue(largeTPBPenaltyWeight          );
    settings[CNBGenerator::c_multiplicativeLengthScaleFactor_settingName] = QVariant::fromValue(multiplicativeLengthScaleFactor);
    settings[CNBGenerator::c_additiveLengthScaleFactor_settingName      ] = QVariant::fromValue(additiveLengthScaleFactor      );
    return settings;
}

void CNBGeneratorSettingsEditorWidget::setSettings(const Core::CGeneratorSettings &settings)
{
    quint32 gpuCoreRating                   = settings[CNBGenerator::c_gpuCoreRating_settingName                  ].toUInt();
    quint32 limitationsDivider              = settings[CNBGenerator::c_limitationsDivider_settingName             ].toUInt();
    double  smallTPBPenaltyWeight           = settings[CNBGenerator::c_smallTPBPenaltyWeight_settingName          ].toDouble();
    double  largeTPBPenaltyWeight           = settings[CNBGenerator::c_largeTPBPenaltyWeight_settingName          ].toDouble();
    double  multiplicativeLengthScaleFactor = settings[CNBGenerator::c_multiplicativeLengthScaleFactor_settingName].toDouble();
    double  additiveLengthScaleFactor       = settings[CNBGenerator::c_additiveLengthScaleFactor_settingName      ].toDouble();

    ui.sbGPUCoreRating->setValue(gpuCoreRating);
    ui.sbLimitationsDivider->setValue(limitationsDivider);
    ui.sbSmallTPBPenaltyWeight->setValue(smallTPBPenaltyWeight);
    ui.sbLargeTPBPenaltyWeight->setValue(largeTPBPenaltyWeight);
    ui.sbMultiplicativeLengthScaleFactor->setValue(multiplicativeLengthScaleFactor);
    ui.sbAdditiveLengthScaleFactor->setValue(additiveLengthScaleFactor);
}