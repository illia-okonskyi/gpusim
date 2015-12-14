#include "../QLogger/QLogger.h"
#include "../QLogger/QLog"

#include "../gpusim-fe.Core/MetaTypes.h"

#include "Calibrator.h"

#include <QtCore/QCoreApplication>
#include <QTimer>
#include <iostream>

//////////////////////////////////////////////////////////////////////////

void installLogger();
void deinstallLogger();
int process(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Parameters not set. usage:" << std::endl;
        std::cout << "gpusim-fe.Calibrator <experiment-file-path> <calibration-profile-path> [--trace]" << std::endl;
        system("pause");
        return 0;
    }

    installLogger();
    int res = process(argc, argv);
    deinstallLogger();
    return res;
}

int process(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Core::registerMetaTypes();

    if (app.arguments().contains("--trace"))
        QLogger::instance()->setLogLevel(QLogger::LogLevel_Trace);

    ExperimentPlugin::CPluginsHost pluginsHost;
    QDir pluginsDir = QDir(app.applicationDirPath());
    pluginsDir.cd("../plugins");
    pluginsHost.loadPlugins(pluginsDir.absolutePath());

    CCalibrator calibrator(pluginsHost, argv[1], argv[2]);
    app.connect(&calibrator, SIGNAL(calibrated()), SLOT(quit()));
    QTimer::singleShot(0, &calibrator, SLOT(calibrate()));

    int res = app.exec();

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

    QLogger::instance()->setLogFileName("gpusim-fe.Calibrator.log");
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