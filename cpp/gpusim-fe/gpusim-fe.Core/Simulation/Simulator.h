#pragma once

#include "../IGenerator/IGenerator.h"
#include "Simulation.h"
#include "../Enums.h"

#include <QProcess>

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CSimulator: public QObject
    {
        Q_OBJECT;

    public:
        CSimulator(QObject *pParent = nullptr);
        virtual ~CSimulator();

        void setGenerator(const IGeneratorPtr &pGenerator);

        CSimulation getSimulation() const;
        void setSimulation(const CSimulation &simulation, const CGeneratorSettings &settings);

        bool isReady() const;
        bool isExecuting() const;
        bool isCanceling() const;

    public slots:
        void execute();
        void execute(const Core::CSimulation &simulation, const Core::CGeneratorSettings &settings);
        void cancel();

    signals:
        // Progress can be from 0 to 100.
        void progress(int val);
        void executed(Core::CExitCode ec = EC_None);

    private slots:
        void onJavaProcessError(QProcess::ProcessError error);
        void onJavaProcessStarted();
        void onJavaProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
        void onJavaProcessReadyReadStdOut();
        void onJavaProcessReadyReadStdErr();

    private:
        // Creates a temporary file and returns it's path.
        static QString createTempFile();
        
        void killJavaProcess();

        void processExecuted(CExitCode ec);
        void processCancel();

    private:
        typedef enum _tagCState
        {
            State_Ready,
            State_StartingJavaProcess,
            State_WaitJavaProcessExecResult,
            State_Canceling
        } CState;

    private:
        CState m_state;
        IGeneratorPtr m_pGenerator;
        CSimulation m_simulation;
        CGeneratorSettings m_settings;
        QString m_configFilePath;
        QString m_outputFilePath;
        QProcess *m_pJavaProcess;

    private:
        static const QString c_javaProcessExecFormat;
        static const QString c_simulatorJarPath;
    };
}