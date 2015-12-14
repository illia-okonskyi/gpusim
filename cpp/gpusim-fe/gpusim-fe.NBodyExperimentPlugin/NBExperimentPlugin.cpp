#include "NBExperimentPlugin.h"
#include "NBGenerator.h"
#include "Widgets/NBGeneratorBoundaryConditionsEditorWidget.h"
#include "Widgets/NBGeneratorSettingsEditorWidget.h"

using namespace NBEP;

QString CNBExperimentPlugin::getName() const
{
    return QString("gpusim-fe.NBodyExperimentPlugin");
}

void CNBExperimentPlugin::registerMetaTypes() const
{

}

Core::IGeneratorPtr CNBExperimentPlugin::createGenerator() const
{
    return Core::IGeneratorPtr(new CNBGenerator(getName()));
}

Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
    CNBExperimentPlugin::createGeneratorBoundaryConditionsEditorWidget() const
{
    return new CNBGeneratorBoundaryConditionsEditorWidget();
}

Core::Widgets::IGeneratorSettingsEditorWidget *CNBExperimentPlugin::createGeneratorSettingsEditorWidget() const
{
    return new CNBGeneratorSettingsEditorWidget();
}
