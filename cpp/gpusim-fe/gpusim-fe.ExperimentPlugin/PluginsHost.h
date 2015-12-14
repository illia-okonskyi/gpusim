#pragma once

#include "IExperimentPlugin.h"

#include <QPluginLoader>
#include <QHash>

namespace ExperimentPlugin
{
    class GPUSIM_FE_EXPERIMENTPLUGIN_EXPORT CExperimentPluginHandle
    {
    public:
        CExperimentPluginHandle(const QString &pluginFilePath);
        virtual ~CExperimentPluginHandle();

        bool isLoaded() const;
        bool load();
        void unload();

        IExperimentPluginPtr getExperimentPlugin() const;
        Core::IGeneratorPtr getGenerator() const;

    private:
        typedef QSharedPointer<QPluginLoader> QPluginLoaderPtr;

    private:
        QString m_pluginFilePath;
        QPluginLoaderPtr m_pLoader;
        IExperimentPluginPtr m_pExperimentPlugin;
        Core::IGeneratorPtr m_pGenerator;
    };

    //////////////////////////////////////////////////////////////////////////

    class GPUSIM_FE_EXPERIMENTPLUGIN_EXPORT CPluginsHost
    {
    public:
        virtual ~CPluginsHost();

        void loadPlugins(const QString &pluginsDirPath);
        void unloadPlugins();

        bool hasLoadedPlugins() const;
        QStringList getLoadedPluginsNames() const;

        bool isPluginLoaded(const QString &pluginName) const;

        Core::IGeneratorPtr getGenerator(const QString &pluginName) const;
        Core::IGeneratorPtr getGenerator(const Core::CExperiment &experiment) const;

        Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
            createGeneratorBoundaryConditionsEditorWidget(const QString &pluginName) const;
        Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
            createGeneratorBoundaryConditionsEditorWidget(const Core::CExperiment &experiment) const;

        Core::Widgets::IGeneratorSettingsEditorWidget *
            createGeneratorSettingsEditorWidget(const QString &pluginName) const;
        Core::Widgets::IGeneratorSettingsEditorWidget *
            createGeneratorSettingsEditorWidget(const Core::CExperiment &experiment) const;

    private:
        typedef QSharedPointer<CExperimentPluginHandle> CExperimentPluginHandlePtr;

    private:
        QHash<QString, CExperimentPluginHandlePtr> m_loadedPlugins;
    };
}