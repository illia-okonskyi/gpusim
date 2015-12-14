#include "Simulator.h"
#include "../GridSimConfig/GridSimConfig.h"
#include "../GridSimConfig/GridSimOutput.h"

#include "../../QLogger/QLog"

#include <QTemporaryFile>

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
// The Java process exec format is next:
//   java -jar "<jar-file-path>" "<config-file-path>" "<output-file-path>"
//
const QString CSimulator::c_javaProcessExecFormat = QString("java -Xss64k -jar \"%1\" \"%2\" \"%3\"");
const QString CSimulator::c_simulatorJarPath = QString("../simulator/gpusim-runtime.jar");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

CSimulator::CSimulator(QObject *pParent /*= nullptr*/)
    : QObject(pParent), m_state(State_Ready), m_pJavaProcess(nullptr)
{
}

CSimulator::~CSimulator()
{
    killJavaProcess();
}

void CSimulator::setGenerator(const IGeneratorPtr &pGenerator)
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    if (m_pGenerator == pGenerator)
        return;

    m_pGenerator = pGenerator;
}

CSimulation CSimulator::getSimulation() const
{
    return m_simulation;
}

void CSimulator::setSimulation(const CSimulation &simulation, const CGeneratorSettings &settings)
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    m_simulation = simulation;
    m_settings = settings;

    //
    // Apply new simulation
    //
    m_pJavaProcess = new QProcess(this);
    connect(m_pJavaProcess,
        SIGNAL(error(QProcess::ProcessError)),
        SLOT(onJavaProcessError(QProcess::ProcessError)),
        Qt::QueuedConnection);
    connect(m_pJavaProcess,
        SIGNAL(started()),
        SLOT(onJavaProcessStarted()),
        Qt::QueuedConnection);
    connect(m_pJavaProcess,
        SIGNAL(finished(int, QProcess::ExitStatus)),
        SLOT(onJavaProcessFinished(int, QProcess::ExitStatus)),
        Qt::QueuedConnection);

    connect(m_pJavaProcess,
        SIGNAL(readyReadStandardOutput()),
        SLOT(onJavaProcessReadyReadStdOut()),
        Qt::QueuedConnection);
    connect(m_pJavaProcess,
        SIGNAL(readyReadStandardError()),
        SLOT(onJavaProcessReadyReadStdErr()),
        Qt::QueuedConnection);
}

//////////////////////////////////////////////////////////////////////////

bool CSimulator::isReady() const
{
    return (m_state == State_Ready);
}

bool CSimulator::isExecuting() const
{
    return !(isReady() || isCanceling());
}

bool CSimulator::isCanceling() const
{
    return (m_state == State_Canceling);
}

//////////////////////////////////////////////////////////////////////////

void CSimulator::execute()
{
    Q_ASSERT(m_state == State_Ready);
    Q_ASSERT(!m_pGenerator.isNull());
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        processExecuted(EC_Error);
        return;
    }

    if (!m_pGenerator)
    {
        qLog_WarningMsg() << "Generator is not set.";
        processExecuted(EC_Error);
        return;
    }

    emit progress(0);

    // 1) Create config and output temp files.
    // 
    m_configFilePath = createTempFile();
    m_outputFilePath = createTempFile();

    // 2) Generate and save config
    //
    auto config = m_pGenerator->createSimulationConfig(m_simulation.getGeneratorInput(), m_settings);
    if (!config.saveToFile(m_configFilePath))
    {
        qLog_WarningMsg() << "Failed to save configuration to file.";
        processExecuted(EC_Error);
        return;
    }

    QString execString = c_javaProcessExecFormat.arg(c_simulatorJarPath, m_configFilePath, m_outputFilePath);
    m_pJavaProcess->start(execString);

    m_state = State_StartingJavaProcess;
    emit progress(10);
}

void CSimulator::execute(const CSimulation &simulation, const CGeneratorSettings &settings)
{
    setSimulation(simulation, settings);
    execute();
}

void CSimulator::cancel()
{
    if ((m_state == State_Ready) || (m_state == State_Canceling))
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    CState prevState = m_state;
    m_state = State_Canceling;

    // Cancellation can be done after process will be started
    //
    if (prevState == State_StartingJavaProcess)
        return;

    processCancel();
}

//////////////////////////////////////////////////////////////////////////

void CSimulator::onJavaProcessError(QProcess::ProcessError error)
{
    // Ignore errors when cancel. Process process termination in onJavaProcessFinished method
    //
    if (m_state == State_Canceling)
        return;

    qLog_WarningMsg() << "Process error: " << error;
    processExecuted(EC_Error);
}

void CSimulator::onJavaProcessStarted()
{
    if (m_state == State_Canceling)
    {
        processCancel();
        return;
    }

    m_state = State_WaitJavaProcessExecResult;
    emit progress(20);
}

void CSimulator::onJavaProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    emit progress(90);

    if (m_state == State_Canceling)
    {
        processExecuted(EC_Canceled);
        return;
    }

    if (exitCode)
    {
        qLog_DebugMsg() << "Java process exit error code: " << exitCode;
        processExecuted(EC_Error);
        return;
    }

    GridSimConfig::CGridSimOutput output;
    if (!output.loadFromFile(m_outputFilePath))
    {
        qLog_WarningMsg() << "Failed to load output from file.";
        processExecuted(EC_Error);
    }
    m_simulation.setSimTime(output.getTotalSimulationTime());

    processExecuted(EC_Ok);
}

void CSimulator::onJavaProcessReadyReadStdOut()
{
    qLog_TraceMsg() << m_pJavaProcess->readAllStandardOutput();
}

void CSimulator::onJavaProcessReadyReadStdErr()
{
    qLog_WarningMsg() << m_pJavaProcess->readAllStandardError();
}

//////////////////////////////////////////////////////////////////////////
#pragma region Tools
QString CSimulator::createTempFile()
{
    QTemporaryFile f;
    if (!f.open())
        return QString();

    f.setAutoRemove(false);
    return f.fileName();
}

void CSimulator::killJavaProcess()
{
    if (m_pJavaProcess)
        m_pJavaProcess->kill();
}

void CSimulator::processExecuted(CExitCode ec)
{
    QFile::remove(m_configFilePath);
    QFile::remove(m_outputFilePath);
    m_configFilePath.clear();
    m_outputFilePath.clear();

    disconnect(m_pJavaProcess);
    delete m_pJavaProcess;
    m_pJavaProcess = nullptr;

    m_state = State_Ready;

    emit progress(100);
    emit executed(ec);
}

void CSimulator::processCancel()
{
    killJavaProcess();
}
#pragma endregion