#include "MetaTypes.h"

#include "Enums.h"
#include "Experiment/Experiment.h"

#include <QProcess>

void Core::registerMetaTypes()
{
    qRegisterMetaType<CExitCode>("Core::CExitCode");

    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    
    qRegisterMetaType<Core::CSimulation>("Core::CSimulation");
    qRegisterMetaTypeStreamOperators<Core::CSimulation>("Core::CSimulation");

    qRegisterMetaType<Core::CPlotCurve>("Core::CPlotCurve");
    qRegisterMetaTypeStreamOperators<Core::CPlotCurve>("Core::CPlotCurve");

    qRegisterMetaType<Core::CPlot>("Core::CPlot");
    qRegisterMetaTypeStreamOperators<Core::CPlot>("Core::CPlot");

    qRegisterMetaType<Core::CExperiment>("Core::CExperiment");
    qRegisterMetaTypeStreamOperators<Core::CExperiment>("Core::CExperiment");
}