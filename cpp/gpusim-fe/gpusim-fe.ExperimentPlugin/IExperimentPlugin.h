#pragma once

#include "ExperimentPlugin_global.h"
#include "../gpusim-fe.Core/IGenerator/IGenerator.h"
#include "../gpusim-fe.Core/Widgets/IGeneratorBoundaryConditionsEditorWidget.h"
#include "../gpusim-fe.Core/Widgets/IGeneratorSettingsEditorWidget.h"

#include <QtPlugin>

namespace ExperimentPlugin
{
    class GPUSIM_FE_EXPERIMENTPLUGIN_EXPORT IExperimentPlugin
    {
    public:
        virtual ~IExperimentPlugin();

        virtual QString getName() const = 0;
        virtual void registerMetaTypes() const = 0;

        virtual Core::IGeneratorPtr createGenerator() const = 0;

        virtual Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
            createGeneratorBoundaryConditionsEditorWidget() const = 0;
        virtual Core::Widgets::IGeneratorSettingsEditorWidget *createGeneratorSettingsEditorWidget() const = 0;
    };

    typedef QSharedPointer<IExperimentPlugin> IExperimentPluginPtr;
}

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ExperimentPlugin::IExperimentPlugin, "com.logrus.gpusim-fe.IExperimentPlugin/1.0")
QT_END_NAMESPACE