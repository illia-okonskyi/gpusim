#include "PluginsHost.h"

#include "../QLogger/QLog"

#include <QDir>

using namespace ExperimentPlugin;

CExperimentPluginHandle::CExperimentPluginHandle(const QString &pluginFilePath)
    : m_pluginFilePath(pluginFilePath)
{

}

CExperimentPluginHandle::~CExperimentPluginHandle()
{
    unload();
}

bool CExperimentPluginHandle::isLoaded() const
{
    return (m_pLoader && m_pExperimentPlugin && m_pGenerator);
}

bool CExperimentPluginHandle::load()
{
    if (isLoaded())
        return true;

    qLog_DebugMsg() << "Loading " << m_pluginFilePath;

    QPluginLoaderPtr pLoader(new QPluginLoader(m_pluginFilePath));
    IExperimentPluginPtr pPlugin(qobject_cast<ExperimentPlugin::IExperimentPlugin *>(pLoader->instance()));
    if (!pPlugin)
    {
        qLog_WarningMsg() << "Failed to load " << m_pluginFilePath << " plugin.";
        qLog_WarningMsg() << "Error string: " << pLoader->errorString();
        return false;
    }

    m_pLoader = pLoader;
    m_pExperimentPlugin = pPlugin;
    m_pExperimentPlugin->registerMetaTypes();
    m_pGenerator = m_pExperimentPlugin->createGenerator();

    qLog_DebugMsg() << "Loaded plugin: " << m_pExperimentPlugin->getName();
    return true;
}

void CExperimentPluginHandle::unload()
{
    if (!isLoaded())
        return;
    
    qLog_DebugMsg() << "Unloading " << m_pExperimentPlugin->getName() << " plugin: " << m_pluginFilePath;

    m_pGenerator.clear();
    m_pExperimentPlugin.clear();
    if (!m_pLoader->unload())
    {
        qLog_WarningMsg() << "Failed to unload " << m_pluginFilePath << " plugin.";
        qLog_WarningMsg() << "Error string: " << m_pLoader->errorString();
    }
    m_pLoader.clear();
}

IExperimentPluginPtr CExperimentPluginHandle::getExperimentPlugin() const
{
    return m_pExperimentPlugin;
}

Core::IGeneratorPtr CExperimentPluginHandle::getGenerator() const
{
    return m_pGenerator;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CPluginsHost::~CPluginsHost()
{
    unloadPlugins();
}

//////////////////////////////////////////////////////////////////////////

void CPluginsHost::loadPlugins(const QString &pluginsDirPath)
{
    if (hasLoadedPlugins())
    {
        qLog_WarningMsg() << "Already loaded.";
        return;
    }

    qLog_DebugMsg() << "Loading plugins from folder: " << pluginsDirPath;
    QDir pluginsDir = QDir(pluginsDirPath);
    foreach (const QString &fileName, pluginsDir.entryList(QStringList() << "*.dll", QDir::Files))
    {
        CExperimentPluginHandlePtr pHandle(new CExperimentPluginHandle(pluginsDir.absoluteFilePath(fileName)));
        if (!pHandle->load())
            continue;

        m_loadedPlugins[pHandle->getExperimentPlugin()->getName()] = pHandle;
    }
}

void CPluginsHost::unloadPlugins()
{
    if (!hasLoadedPlugins())
        return;

    m_loadedPlugins.clear();
}

//////////////////////////////////////////////////////////////////////////

bool CPluginsHost::hasLoadedPlugins() const
{
    return !m_loadedPlugins.isEmpty();
}

QStringList CPluginsHost::getLoadedPluginsNames() const
{
    return m_loadedPlugins.keys();
}

bool CPluginsHost::isPluginLoaded(const QString &pluginName) const
{
    return m_loadedPlugins.contains(pluginName);
}

//////////////////////////////////////////////////////////////////////////

Core::IGeneratorPtr CPluginsHost::getGenerator(const QString &pluginName) const
{
    if (!isPluginLoaded(pluginName))
        return Core::IGeneratorPtr();

    return m_loadedPlugins[pluginName]->getGenerator();
}

Core::IGeneratorPtr CPluginsHost::getGenerator(const Core::CExperiment &experiment) const
{
    return getGenerator(experiment.getPluginName());
}

//////////////////////////////////////////////////////////////////////////

Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
    CPluginsHost::createGeneratorBoundaryConditionsEditorWidget(const QString &pluginName) const
{
    if (!isPluginLoaded(pluginName))
        return nullptr;

    return m_loadedPlugins[pluginName]->getExperimentPlugin()->createGeneratorBoundaryConditionsEditorWidget();
}

Core::Widgets::IGeneratorBoundaryConditionsEditorWidget *
    CPluginsHost::createGeneratorBoundaryConditionsEditorWidget(const Core::CExperiment &experiment) const
{
    return createGeneratorBoundaryConditionsEditorWidget(experiment.getPluginName());
}

//////////////////////////////////////////////////////////////////////////

Core::Widgets::IGeneratorSettingsEditorWidget *
    CPluginsHost::createGeneratorSettingsEditorWidget(const QString &pluginName) const
{
    if (!isPluginLoaded(pluginName))
        return nullptr;

    return m_loadedPlugins[pluginName]->getExperimentPlugin()->createGeneratorSettingsEditorWidget();
}

Core::Widgets::IGeneratorSettingsEditorWidget *
    CPluginsHost::createGeneratorSettingsEditorWidget(const Core::CExperiment &experiment) const
{
    return createGeneratorSettingsEditorWidget(experiment.getPluginName());
}