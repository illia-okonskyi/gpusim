#include "SimulationDialog.h"
#include <QMessageBox>

CSimulationDialog::CSimulationDialog(const Core::CExperiment &experiment, const Core::IGeneratorPtr &pGenerator,
    QWidget *pParent /*= nullptr*/)
    : QDialog(pParent, Qt::CustomizeWindowHint | Qt::WindowTitleHint), m_experiment(experiment), m_experimenter(this),
    m_ec(Core::EC_None)
{
    ui.setupUi(this);
    
    m_experimenter.setGenerator(pGenerator);
    m_experimenter.setExperiment(experiment);

    ui.pbProgress->connect(&m_experimenter, SIGNAL(progress(int)), SLOT(setValue(int)), Qt::QueuedConnection);
    connect(&m_experimenter, SIGNAL(executed(Core::CExitCode)), SLOT(onExperimentExecuted(Core::CExitCode)),
        Qt::QueuedConnection);
}

CSimulationDialog::~CSimulationDialog()
{
    // NOTE: parent should be cleared to avoid second deletion as QObject child.
    //
    m_experimenter.disconnect(this);
    m_experimenter.disconnect(ui.pbProgress);
    m_experimenter.setParent(nullptr);
}

//////////////////////////////////////////////////////////////////////////

Core::CExitCode CSimulationDialog::getExitCode() const
{
    return m_ec;
}

Core::CExperiment CSimulationDialog::getExperiment() const
{
    return m_experiment;
}

//////////////////////////////////////////////////////////////////////////

void CSimulationDialog::start()
{
    ui.btnStart->setEnabled(false);
    ui.btnCancel->setEnabled(true);
    ui.lblStatus->setText(tr("Simulation in progress. Please wait."));
    m_experimenter.execute();
}

void CSimulationDialog::cancel()
{
    if (m_experimenter.isReady())
    {
        m_ec = Core::EC_Canceled;
        accept();
        return;
    }

    ui.btnCancel->setEnabled(false);
    ui.lblStatus->setText(tr("Canceling simulation. Please wait..."));
    m_experimenter.cancel();
}

//////////////////////////////////////////////////////////////////////////

void CSimulationDialog::onExperimentExecuted(Core::CExitCode ec)
{
    m_ec = ec;
    m_experiment = m_experimenter.getExperiment();

    if (m_ec == Core::EC_Error)
    {
        QMessageBox::critical(this, tr("Error during simulation"),
            tr("Error during simulation.\nSee details in log."));
    }

    accept();
}