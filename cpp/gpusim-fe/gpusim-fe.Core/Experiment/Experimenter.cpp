#include "Experimenter.h"

#include "../../QLogger/QLog"

#include <QEventLoop>
#include <QTimer>

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

CExperimenter::CExperimenter(QObject *pParent /*= nullptr*/)
    : QObject(pParent), m_state(State_Ready), m_pSimulator(nullptr), m_ec(EC_None)
{
    m_pSimulator = new CSimulator(this);
    connect(m_pSimulator, SIGNAL(progress(int)), SLOT(onSimulatorProgress(int)), Qt::QueuedConnection);
    connect(m_pSimulator, SIGNAL(executed(Core::CExitCode)), SLOT(onSimulatorExecuted(Core::CExitCode)),
        Qt::QueuedConnection);
}

CExperimenter::~CExperimenter()
{
    // NOTE: m_pSimulator will be deleted automatically as child of current QObject;
    //
    disconnect(m_pSimulator);
    m_pSimulator = nullptr;
}

//////////////////////////////////////////////////////////////////////////

void CExperimenter::setGenerator(const IGeneratorPtr &pGenerator)
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    m_pSimulator->setGenerator(pGenerator);
}

CExperiment CExperimenter::getExperiment() const
{
    return m_experiment;
}

void CExperimenter::setExperiment(const CExperiment &experiment)
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    if (m_experiment == experiment)
        return;

    m_experiment = experiment;

    // Apply new experiment
    m_simulations = m_experiment.getSimulationsRef();
}

//////////////////////////////////////////////////////////////////////////

bool CExperimenter::isReady() const
{
    return (m_state == State_Ready);
}

bool CExperimenter::isExecuting() const
{
    return (m_state == State_Executing);
}

bool CExperimenter::isCanceling() const
{
    return (m_state == State_Canceling);
}

CExitCode CExperimenter::getExitCode() const
{
    return m_ec;
}

//////////////////////////////////////////////////////////////////////////

CExitCode CExperimenter::executeSynchronously(const CExperiment &experiment, const IGeneratorPtr &pGenerator)
{
    setExperiment(experiment);
    setGenerator(pGenerator);

    QEventLoop loop;
    QObject::connect(this, SIGNAL(executed()), &loop, SLOT(quit()));
    QTimer::singleShot(0, this, SLOT(execute()));

    loop.exec();

    disconnect(&loop);
    return getExitCode();
}

//////////////////////////////////////////////////////////////////////////

void CExperimenter::execute()
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        processExecuted(EC_Error);
        return;
    }

    emit progress(0);
    m_state = State_Executing;

    m_ec = EC_None;
    m_itCurrentSim = m_simulations.begin();
    processCurrentSim();
}

void CExperimenter::execute(const CExperiment &experiment)
{
    setExperiment(experiment);
    execute();
}

void CExperimenter::cancel()
{
    if (m_state != State_Executing)
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    if (m_pSimulator->isReady())
    {
        processExecuted(EC_Canceled);
        return;
    }

    m_state = State_Canceling;
    m_pSimulator->cancel();
}

//////////////////////////////////////////////////////////////////////////

void CExperimenter::onSimulatorProgress(int val)
{
    // total progress items = experiments count * 100
    // current items = passed_experiments count*100 + val
    
    size_t totalSimulationsCount = m_simulations.size();
    size_t passedSimulationsCount = std::distance(m_simulations.begin(), m_itCurrentSim);
    size_t p = ((passedSimulationsCount * 100 + val) * 100) / (totalSimulationsCount * 100);
    emit progress(p);
}

void CExperimenter::onSimulatorExecuted(Core::CExitCode ec)
{
    // 1) Update results
    *m_itCurrentSim = m_pSimulator->getSimulation();

    // 2) Process next simulation if previous processed successfully.
    //
    if (ec == EC_Ok)
    {
        m_itCurrentSim++;
        processCurrentSim();
        return;
    }

    // Here can be only error or cancel error codes
    //
    processExecuted(ec);
}

//////////////////////////////////////////////////////////////////////////

void CExperimenter::processCurrentSim()
{
    if (m_itCurrentSim == m_simulations.end())
    {
        // Simulation end with success.
        processExecuted(EC_Ok);
        return;
    }

    m_pSimulator->setSimulation(*m_itCurrentSim, m_experiment.getSettings());
    qLog_DebugMsg() << "Executing simulation " << m_itCurrentSim->getName();
    m_pSimulator->execute();
}

void CExperimenter::processExecuted(CExitCode ec)
{
    if (ec == EC_Ok)
        m_experiment.setSimulations(m_simulations);
    
    m_ec = ec;
    m_state = State_Ready;

    emit progress(100);
    emit executed(m_ec);
}