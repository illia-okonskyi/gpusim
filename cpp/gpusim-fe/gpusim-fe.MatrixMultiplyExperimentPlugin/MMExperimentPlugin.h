#pragma once

#include "../gpusim-fe.ExperimentPlugin/IExperimentPlugin.h"

// MMEP is for MatrixMultiplyExperimentPlugin
namespace MMEP
{
    class CMMExperimentPlugin : public QObject, public ExperimentPlugin::IExperimentPlugin
    {
        Q_OBJECT
        Q_INTERFACES(ExperimentPlugin::IExperimentPlugin)

    public:
        virtual QString getName() const;
        virtual void registerMetaTypes() const;

        virtual Core::IGeneratorPtr createGenerator() const;
        virtual Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
            createGeneratorBoundaryConditionsEditorWidget() const;
        virtual Core::Widgets::IGeneratorSettingsEditorWidget *createGeneratorSettingsEditorWidget() const;
    };
}