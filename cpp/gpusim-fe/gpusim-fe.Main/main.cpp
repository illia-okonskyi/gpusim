#include "../QLogger/QLogger.h"
#include "../QLogger/QLog"

#include "../gpusim-fe.Core/MetaTypes.h"

#include "MainWindow.h"

#include <QtGui/QApplication>
#include <QTranslator>

//////////////////////////////////////////////////////////////////////////

void installLogger();
void deinstallLogger();
int process(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    installLogger();
    int res = process(argc, argv);
    deinstallLogger();
    return res;
}

int process(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDir translationsDir = QDir(app.applicationDirPath());
    translationsDir.cd("../translation");
    foreach (const QString &fileName, translationsDir.entryList(QStringList() << "*.qm", QDir::Files))
    {
        QTranslator *pTranslator = new QTranslator(&app);
        pTranslator->load(translationsDir.absoluteFilePath(fileName));
        app.installTranslator(pTranslator);
    }

    Core::registerMetaTypes();

    if (app.arguments().contains("--trace"))
        QLogger::instance()->setLogLevel(QLogger::LogLevel_Trace);

    ExperimentPlugin::CPluginsHost pluginsHost;
    QDir pluginsDir = QDir(app.applicationDirPath());
    pluginsDir.cd("../plugins");
    pluginsHost.loadPlugins(pluginsDir.absolutePath());

    CMainWindow *pMainWindow = new CMainWindow(pluginsHost);
    pMainWindow->show();

    int res = app.exec();

    delete pMainWindow;
    pluginsHost.unloadPlugins();
    return res;
}

//////////////////////////////////////////////////////////////////////////

void installLogger()
{
    QLogger::instance()->setLogDestination(QLogger::CLogDestination(
        QLogger::LogDest_Default | QLogger::LogDest_File));
    QLogger::instance()->setDateTimeFormat("hh:mm:ss.zzz");
    QLogger::instance()->setLogFormat("[%COL(SEV):5%]:[%COL(TIME):12%]:"
        "[%COL(MOD):20%]:[%COL(FUNC):40%]> %COL(MSG):0%");

    QLogger::instance()->setLogFileName("gpusim-fe.Main.log");
    QLogger::instance()->setLogLevel(QLogger::LogLevel_Debug);
    QLogger::instance()->setLogsDir(QDir::current());
    QLogger::instance()->writeStartMessage();
}

void deinstallLogger()
{
    QLogger::instance()->writeStopMessage();
    QLogger::instance()->writeEndl(2);
    QLogger::instance()->deinstall();
}