#pragma once

#include "../Simulation/Simulator.h"
#include "Experiment.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CExperimenter: public QObject
    {
        Q_OBJECT;

    public:
        CExperimenter(QObject *pParent = nullptr);
        virtual ~CExperimenter();

        void setGenerator(const IGeneratorPtr &pGenerator);
        
        CExperiment getExperiment() const;
        void setExperiment(const CExperiment &experiment);

        bool isReady() const;
        bool isExecuting() const;
        bool isCanceling() const;

        CExitCode getExitCode() const;

        CExitCode executeSynchronously(const CExperiment &experiment, const IGeneratorPtr &pGenerator);

    public slots:
        void execute();
        void execute(const Core::CExperiment &experiment);
        void cancel();

    signals:
        // Progress can be from 0 to 100.
        void progress(int val);
        void executed(Core::CExitCode ec = EC_None);

    private slots:
        void onSimulatorProgress(int val);
        void onSimulatorExecuted(Core::CExitCode ec);

    private:
        void processCurrentSim();

        void emitProgress(quint8 val);
        void processExecuted(CExitCode ec);

        void createSimulator(const CSimulation &sim);
    private:
        typedef enum _tagCState
        {
            State_Ready,
            State_Executing,
            State_Canceling
        } CState;

    private:
        CState m_state;
        CSimulator *m_pSimulator;
        CExperiment m_experiment;
        CSimulationsList m_simulations;
        CSimulationsList::iterator m_itCurrentSim;
        CExitCode m_ec;
    };
}