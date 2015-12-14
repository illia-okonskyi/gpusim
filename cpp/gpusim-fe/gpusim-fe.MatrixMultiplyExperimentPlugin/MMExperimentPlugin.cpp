#include "MMExperimentPlugin.h"
#include "MMGenerator.h"
#include "Widgets/MMGeneratorBoundaryConditionsEditorWidget.h"
#include "Widgets/MMGeneratorSettingsEditorWidget.h"

using namespace MMEP;

QString CMMExperimentPlugin::getName() const
{
    return QString("gpusim-fe.MatrixMultiplyExperimentPlugin");
}

void CMMExperimentPlugin::registerMetaTypes() const
{

}

Core::IGeneratorPtr CMMExperimentPlugin::createGenerator() const
{
    return Core::IGeneratorPtr(new CMMGenerator(getName()));
}

Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
    CMMExperimentPlugin::createGeneratorBoundaryConditionsEditorWidget() const
{
    return new CMMGeneratorBoundaryConditionsEditorWidget();
}

Core::Widgets::IGeneratorSettingsEditorWidget *CMMExperimentPlugin::createGeneratorSettingsEditorWidget() const
{
    return new CMMGeneratorSettingsEditorWidget();
}