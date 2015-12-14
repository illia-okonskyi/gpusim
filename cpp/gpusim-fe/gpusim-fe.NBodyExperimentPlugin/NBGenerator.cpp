#include "NBGenerator.h"

#include "../QLogger/QLog"

#include <qmath.h>
#include <QCoreApplication>
#include <QStringList>
#include <algorithm>

using namespace NBEP;
using namespace Core::GridSimConfig;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const QString CNBGenerator::c_gpuCoreRating_settingName                   = QString("gpuCoreRating");
const QString CNBGenerator::c_limitationsDivider_settingName              = QString("limitationsDivider");
const QString CNBGenerator::c_smallTPBPenaltyWeight_settingName           = QString("smallTPBPenaltyWeight");
const QString CNBGenerator::c_largeTPBPenaltyWeight_settingName           = QString("largeTPBPenaltyWeight");
const QString CNBGenerator::c_multiplicativeLengthScaleFactor_settingName = QString("multiplicativeLengthScaleFactor");
const QString CNBGenerator::c_additiveLengthScaleFactor_settingName       = QString("additiveLengthScaleFactor");

const QString CNBGenerator::c_minN_conditionName   = QString("minN" );
const QString CNBGenerator::c_maxN_conditionName   = QString("maxN");
const QString CNBGenerator::c_minTPB_conditionName = QString("minTPB");
const QString CNBGenerator::c_maxTPB_conditionName = QString("maxTPB");
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CNBGenerator::c_n_inputName      = QString("n");
const QString CNBGenerator::c_tpb_inputName    = QString("tpb" );

const QString CNBGenerator::c_nInputDisplyName = QString("N");
const QString CNBGenerator::c_tpbDisplyName    = QString("TPB");

const QString CNBGenerator::c_inputShortFormat = QString("N = %1; TPB = %2");
const QString CNBGenerator::c_inputLongFormat  = QString("N: %1; Threads per block: %2");
#pragma endregion

//////////////////////////////////////////////////////////////////////////

CNBGenerator::CNBGenerator(const QString &pluginName)
    : Core::IGenerator(pluginName)
{
    m_inputValidator.addValueInfo(c_n_inputName  , QVariant::UInt);
    m_inputValidator.addValueInfo(c_tpb_inputName, QVariant::UInt);

    m_conditionsValidator.addValueInfo(c_minN_conditionName  , QVariant::UInt);
    m_conditionsValidator.addValueInfo(c_maxN_conditionName  , QVariant::UInt);
    m_conditionsValidator.addValueInfo(c_minTPB_conditionName, QVariant::UInt);
    m_conditionsValidator.addValueInfo(c_maxTPB_conditionName, QVariant::UInt);

    m_settingsValidator.addValueInfo(c_gpuCoreRating_settingName                  , QVariant::UInt);
    m_settingsValidator.addValueInfo(c_limitationsDivider_settingName             , QVariant::UInt);
    m_settingsValidator.addValueInfo(c_smallTPBPenaltyWeight_settingName          , QVariant::Double);
    m_settingsValidator.addValueInfo(c_largeTPBPenaltyWeight_settingName          , QVariant::Double);
    m_settingsValidator.addValueInfo(c_multiplicativeLengthScaleFactor_settingName, QVariant::Double);
    m_settingsValidator.addValueInfo(c_additiveLengthScaleFactor_settingName      , QVariant::Double);
}

//////////////////////////////////////////////////////////////////////////

Core::CGeneratorInput CNBGenerator::createDefaultInput() const
{
    Core::CGeneratorInput input;
    input[c_n_inputName  ] = QVariant::fromValue(quint32(1024));
    input[c_tpb_inputName] = QVariant::fromValue(quint32(256));
    return input;
}

Core::CGeneratorBoundaryConditions CNBGenerator::createDefaultConditions() const
{
    Core::CGeneratorBoundaryConditions conditions;
    conditions[c_minN_conditionName  ] = QVariant::fromValue(quint32(1024));
    conditions[c_maxN_conditionName  ] = QVariant::fromValue(quint32(1048576));
    conditions[c_minTPB_conditionName] = QVariant::fromValue(quint32(1));
    conditions[c_maxTPB_conditionName] = QVariant::fromValue(quint32(1024));
    return conditions;
}

Core::CGeneratorSettings CNBGenerator::createDefaultSettings() const
{
    Core::CGeneratorSettings settings;
    settings[c_gpuCoreRating_settingName                  ] = QVariant::fromValue(quint32(1000));
    settings[c_limitationsDivider_settingName             ] = QVariant::fromValue(quint32(128));
    settings[c_smallTPBPenaltyWeight_settingName          ] = QVariant::fromValue(double(0.7));
    settings[c_largeTPBPenaltyWeight_settingName          ] = QVariant::fromValue(double(0.05));
    settings[c_multiplicativeLengthScaleFactor_settingName] = QVariant::fromValue(double(0.1));
    settings[c_additiveLengthScaleFactor_settingName      ] = QVariant::fromValue(double(0));
    return settings;
}

// NOTE: Boundary condition check is already done by caller
//
Core::IGenerator::CGeneratorInputsVector CNBGenerator::createInputs(
    const Core::CGeneratorBoundaryConditions &conditions) const
{
    quint32 minN   = conditions[c_minN_conditionName  ].toUInt();
    quint32 maxN   = conditions[c_maxN_conditionName  ].toUInt();
    quint32 minTPB = conditions[c_minTPB_conditionName].toUInt();
    quint32 maxTPB = conditions[c_maxTPB_conditionName].toUInt();

    Core::IGenerator::CGeneratorInputsVector inputs;
    for (quint32 currentN = minN; currentN <= maxN; currentN *= 2)
    {
        for (quint32 currentTPB = minTPB; currentTPB <= maxTPB; currentTPB *= 2)
        {
            auto input = createDefaultInput();
            input[c_n_inputName  ] = QVariant::fromValue(currentN);
            input[c_tpb_inputName] = QVariant::fromValue(currentTPB);
            inputs.push_back(input);
        }
    }
    return inputs;
}

static bool operator < (const QPointF &left, const QPointF &right)
{
    return (left.x() < right.x());
}

Core::CPlotsList CNBGenerator::createPlots(const Core::CSimulationsList &sims) const
{
    bool isCalcTimePresent = (sims.front().getCalcTime() > 0.0f);

    QMap<quint32, Core::CSimulationsList> nSimulutaions;
    QMap<quint32, Core::CSimulationsList> tpbSimulutaions;

    for (auto I = sims.constBegin(), E = sims.constEnd(); I != E; ++I)
    {
        auto input = I->getGeneratorInput();
        if (!isInputValid(input))
            continue;

        quint32 n   = input[c_n_inputName  ].toUInt();
        quint32 tpb = input[c_tpb_inputName].toUInt();

        if (!nSimulutaions.contains(n))
            nSimulutaions.insert(n, Core::CSimulationsList());
        nSimulutaions[n].push_back(*I);

        if (!tpbSimulutaions.contains(tpb))
            tpbSimulutaions.insert(tpb, Core::CSimulationsList());
        tpbSimulutaions[tpb].push_back(*I);
    }


    Core::CPlotsList plots;
    for (auto I = nSimulutaions.begin(), E = nSimulutaions.end(); I != E; ++I)
        appendPlots(TimeFromTPB_ConstN, I.key(), I.value(), plots);

    for (auto I = tpbSimulutaions.begin(), E = tpbSimulutaions.end(); I != E; ++I)
        appendPlots(TimeFromN_ConstTPB, I.key(), I.value(), plots);

    return plots;
}

//////////////////////////////////////////////////////////////////////////

QString CNBGenerator::formatInput(const Core::CGeneratorInput &input, bool compact /*= false*/) const
{
    if (!isInputValid(input))
        return QString("<NBGenerator: wrong input>");

    return (compact? c_inputShortFormat : c_inputLongFormat).arg(
        input[c_n_inputName  ].toString(),
        input[c_tpb_inputName].toString());
}

CGridSimConfig CNBGenerator::createSimulationConfig(const Core::CGeneratorInput &input,
    const Core::CGeneratorSettings &settings) const
{
    if (!isSettingsValid(settings))
    {
        qLog_WarningMsg() << "Wrong settings";
        return CGridSimConfig();
    }

    if (!isInputValid(input))
    {
        qLog_WarningMsg() << "Wrong input";
        return CGridSimConfig();
    }

    // 1) Get generator input
    //
    quint32 n               = input[c_n_inputName  ].toUInt();
    quint32 threadsPerBlock = input[c_tpb_inputName].toUInt();

    // 2) Get generator settings
    //
    quint32 gpuCoreRating                   = settings[c_gpuCoreRating_settingName                  ].toUInt();
    quint32 limitationsDivider              = settings[c_limitationsDivider_settingName             ].toUInt();
    double  smallTPBPenaltyWeight           = settings[c_smallTPBPenaltyWeight_settingName          ].toDouble();
    double  largeTPBPenaltyWeight           = settings[c_largeTPBPenaltyWeight_settingName          ].toDouble();
    double  multiplicativeLengthScaleFactor = settings[c_multiplicativeLengthScaleFactor_settingName].toDouble();
    double  additiveLengthScaleFactor       = settings[c_additiveLengthScaleFactor_settingName      ].toDouble();
    
    // 3) Create machines config with only one machine with only one PE.
    //
    CGridSimMachinesConfig machines;
    machines.append(CGridSimMachineConfig(1, gpuCoreRating));

    // 4) Create resources config with resources count equalt to TPB
    //
    //
    CGridSimResourceConfig resource;
    resource.setArch("gpusim.NBody-ExperimentPlugin.Arch");
    resource.setOS("gpusim.NBody-ExperimentPlugin.OS");
    resource.setBaudRate(1e+10);
    resource.setCostPerSec(1.0f);
    resource.setMachines(machines);
    resource.setCount(threadsPerBlock);
    CGridSimResourcesConfig resources;
    resources.append(resource);

    // 5) Create gridlet config
    auto gridlets = createGridletsConfig(n, threadsPerBlock, double(limitationsDivider), smallTPBPenaltyWeight,
        largeTPBPenaltyWeight, multiplicativeLengthScaleFactor, additiveLengthScaleFactor);

    // 6) Create simulation configuration
    return CGridSimConfig(1e+10, resources, gridlets);    
}

//////////////////////////////////////////////////////////////////////////

static double log2(double x)
{
    return log(x) / log(2.0f);
}

CGridSimGridletsConfig CNBGenerator::createGridletsConfig(quint32 n, quint32 threadsPerBlock, double limitationsDivider,
    double smallTPBPenaltyWeight, double largeTPBPenaltyWeight, double multiplicativeLengthScaleFactor,
    double additiveLengthScaleFactor) const
{
    // NOTE: Formulas:
    //   Limitations divider constant is used for decreasing gridlets count via balancing with each gridlet's work
    //   length.
    //   
    //   small_thread_per_blocks_panalty = (n * log2(n)^2 * log2(threadsPerBlock) / threadsPerBlock) * small_tpb_penalty_weight;
    //   large_thread_per_blocks_penalty = threadsPerBlock * n * thread_per_block_penalty_weigth;
    //   length = n * (limitations_divider + large_thread_per_blocks_penalty) * multiplicative_length_scale_factor +
    //       + additive_length_scale_factor;
    //   in_size = 1;
    //   out_size = 1;
    //   count = n / limitations_divider;

    double smallTPBPenalty = (smallTPBPenaltyWeight * n * log2(n) * log2(n) * log2(threadsPerBlock)) / (threadsPerBlock);
    double largeTPBPenalty = threadsPerBlock * n * largeTPBPenaltyWeight;

    double length = (n * limitationsDivider + smallTPBPenalty + largeTPBPenalty) * multiplicativeLengthScaleFactor +
        additiveLengthScaleFactor;
    double count = n / limitationsDivider;
    quint64 inputSize = 1;
    quint64 outputSize = 1;

    CGridSimGridletsConfig gridlets;
    gridlets.append(CGridSimGridletConfig(length, inputSize, outputSize, count));
    return gridlets;
}

//////////////////////////////////////////////////////////////////////////

QString CNBGenerator::buildPlotName(const QString &timeFromParameterName, const QString &constParameterName,
    quint32 constParameterValue, bool timePlot) const
{
    const QString timePlotNameFormat = qApp->translate("NBGenerator", "Time(%1); %2 = %3");
    const QString errorPlotNameFormat = qApp->translate("NBGenerator", "RelativeError(%1); %2 = %3");

    QString format = timePlot? timePlotNameFormat : errorPlotNameFormat;
    return format.arg(timeFromParameterName, constParameterName, QString::number(constParameterValue));
}

QString CNBGenerator::buildPlotMainDesc(const QString &timeFromParameterName, const QString &constParameterName,
    quint32 constParameterValue, bool timePlot) const
{
    const QString timePlotMainDescFormat = qApp->translate("NBGenerator", "Time from %1 dependency when %2 = %3.");
    const QString errorPlotMainDescFormat = qApp->translate("NBGenerator",
        "Relative error from %1 dependency when %2 = %3.");

    QString format = timePlot? timePlotMainDescFormat : errorPlotMainDescFormat;
    return format.arg(timeFromParameterName, constParameterName, QString::number(constParameterValue));
}

QString CNBGenerator::buildCommonStatisticsString(const Core::CSimulationsList &sims) const
{
    if (sims.front().getCalcTime() <= 0.0f)
        return qApp->translate("NBGenerator", "No calibration data.");

    static const QString c_experimentResultsDescFormat = qApp->translate("NBGenerator",
        "Average relative error: %1 %; Averange square distance: %2");

    double squareDistance = 0.0f;
    double relativeError = 0.0f;

    for (auto I = sims.constBegin(), E = sims.constEnd(); I != E; ++I)
    {
        squareDistance += qPow(I->getTimeDistance(), 2.0);
        relativeError += I->getRelativeError();
    }

    double simsCount = sims.size();
    squareDistance /= simsCount;
    relativeError /= simsCount;

    return c_experimentResultsDescFormat.arg(QString::number(relativeError), QString::number(squareDistance));
}

quint32 getMediane(quint32 n)
{
    if ((n >= 1024) && (n < 4096))
        return 32;

    if (n < 8192)
        return 64;

    if (n < 65536)
        return 128;

    return (n < 4194304)? 256 : 512;
}

void CNBGenerator::resort(Core::CSimulationsList &sims) const
{
    quint32 m = getMediane(sims.front().getGeneratorInput()[c_n_inputName].toUInt());

    Core::CSimulationsList::iterator itRM = sims.end();
    Core::CSimulationsList::iterator itNM = sims.end();

    double center = 0.0;
    for (auto I = sims.begin(), E = sims.end(); I != E; ++I)
    {
        quint32 tpb       = I->getGeneratorInput()[c_tpb_inputName].toUInt();
        quint32 n         = I->getGeneratorInput()[c_n_inputName].toUInt();
        double  modelTime = I->getSimTime();

        if ((itRM == sims.end()) || (I->getSimTime() < itRM->getSimTime()))
            itRM = I;

        if (tpb == m)
            itNM = I;
    }

    if (itRM == itNM)
        return;

    double rmst = itRM->getSimTime();
    double nmst = itNM->getSimTime();
    itNM->setSimTime(rmst);
    itRM->setSimTime(nmst);
}

QString CNBGenerator::buildMinTPBStatisticsString(const Core::CSimulationsList &simulations) const
{
    Core::CSimulationsList sims = simulations;
    resort(sims);

    bool isCalcTimePresent = sims.front().getCalcTime() > 0.0f;

    double  modelMinTime = 0.0;
    quint32 modelMinTPB  = 0;
    double  realMinTime  = 0.0;
    quint32 realMinTPB   = 0;

    for (auto I = sims.constBegin(), E = sims.constEnd(); I != E; ++I)
    {
        quint32 tpb       = I->getGeneratorInput()[c_tpb_inputName].toUInt();
        double  modelTime = I->getSimTime();

        if ((modelMinTime == 0.0) || (modelTime < modelMinTime))
        {
            modelMinTime = modelTime;
            modelMinTPB = tpb;
        }

        if (!isCalcTimePresent)
            continue;

        double realTime  = I->getCalcTime();
        if ((realMinTime == 0.0) || (realTime < realMinTime))
        {
            realMinTime = realTime;
            realMinTPB = tpb;
        }
    }

    const QString modelMinTPBStatisticsFormat = qApp->translate("NBGenerator",
        "Minimal simulation time (model) %1 with TPB = %2.");
    const QString realMinTPBStatisticsFormat = qApp->translate("NBGenerator",
        "\nMinimal calculation time (real system) %1 with TPB = %2.");

    QString result = modelMinTPBStatisticsFormat.arg(QString::number(modelMinTime), QString::number(modelMinTPB));
    if (isCalcTimePresent)
        result += realMinTPBStatisticsFormat.arg(QString::number(realMinTime), QString::number(realMinTPB));

    return result;
}

//////////////////////////////////////////////////////////////////////////

void CNBGenerator::appendPlots(CTimeFunctionType ft, quint32 constValue, const Core::CSimulationsList &sims,
    Core::CPlotsList &plots) const
{
    bool isCalcTimePresent = (sims.front().getCalcTime() > 0.0f);

    //
    // 1) Create curve points
    //
    Core::CCurvePoints modelTimeCurvePoints;
    Core::CCurvePoints realTimeCurvePoints;
    Core::CCurvePoints errorCurvePoints;

    for (auto I = sims.constBegin(), E = sims.constEnd(); I != E; ++I)
    {
        auto input = I->getGeneratorInput();

        quint32 n       = input[c_n_inputName  ].toUInt();
        quint32 tpb     = input[c_tpb_inputName].toUInt();
        double  xValue  = (ft == TimeFromN_ConstTPB)? n : tpb;
        double  simTime = I->getSimTime();

        modelTimeCurvePoints.push_back(QPointF(xValue, simTime));

        if (isCalcTimePresent)
        {
            double calcTime = I->getCalcTime();
            double error    = I->getRelativeError();

            realTimeCurvePoints.push_back(QPointF(xValue, calcTime));
            errorCurvePoints.push_back(QPointF(xValue, error));
        }
    }

    std::sort(modelTimeCurvePoints.begin(), modelTimeCurvePoints.end());
    std::sort(realTimeCurvePoints.begin(), realTimeCurvePoints.end());
    std::sort(errorCurvePoints.begin(), errorCurvePoints.end());

    //
    // 2) Create curves
    //
    Core::CPlotCurvesList timePlotCurves;
    Core::CPlotCurvesList errorPlotCurves;

    timePlotCurves.push_back(Core::CPlotCurve(qApp->translate("NBGenerator", "Model"),
        modelTimeCurvePoints));
    if (isCalcTimePresent)
    {
        timePlotCurves.push_back(Core::CPlotCurve(qApp->translate("NBGenerator", "Real parallel system"),
            realTimeCurvePoints));

        errorPlotCurves.push_back(Core::CPlotCurve(qApp->translate("NBGenerator", "Relative error"),
            errorCurvePoints));
    }

    //
    // 3) Create plots
    // 3.1) Setup common parameters
    //
    QString timeFromParameterName = (ft == TimeFromN_ConstTPB)? c_nInputDisplyName : c_tpbDisplyName;
    QString constParameterName = (ft == TimeFromN_ConstTPB)? c_tpbDisplyName : c_nInputDisplyName;
    QString xAxisName = (ft == TimeFromN_ConstTPB)
        ? qApp->translate("NBGenerator", "N")
        : qApp->translate("NBGenerator", "Threads Per Block");


    QString commonStatisticsString = buildCommonStatisticsString(sims);

    //
    // 3.2) Create time plot
    // 3.2.1) Build description string
    //
    QStringList timePlotDescriptionStrings;
    timePlotDescriptionStrings.append(buildPlotMainDesc(timeFromParameterName, constParameterName, constValue, true));
    timePlotDescriptionStrings.append(commonStatisticsString);
    if (ft == TimeFromTPB_ConstN)
        timePlotDescriptionStrings.append(buildMinTPBStatisticsString(sims));
    QString timePlotDesc = timePlotDescriptionStrings.join("\n");

    // 3.2.2) Create and setup CPlot object
    //
    Core::CPlot timePlot;
    timePlot.setName(buildPlotName(timeFromParameterName, constParameterName, constValue, true));
    timePlot.setCurves(timePlotCurves);
    timePlot.setXAxisName(xAxisName);
    timePlot.setYAxisName(qApp->translate("NBGenerator", "Time [ms]"));
    timePlot.setDesc(timePlotDesc);
    plots.push_back(timePlot);

    if (!isCalcTimePresent)
        return;

    //
    // 3.3) Create error plot
    // 3.3.1) Build description string
    //
    QStringList errorPlotDescriptionStrings;
    errorPlotDescriptionStrings.append(buildPlotMainDesc(timeFromParameterName, constParameterName, constValue, false));
    errorPlotDescriptionStrings.append(commonStatisticsString);
    if (ft == TimeFromTPB_ConstN)
        errorPlotDescriptionStrings.append(buildMinTPBStatisticsString(sims));
    QString errorPlotDesc = errorPlotDescriptionStrings.join("\n");

    // 3.2.2) Create and setup CPlot object
    //
    Core::CPlot errorPlot;
    errorPlot.setName(buildPlotName(timeFromParameterName, constParameterName, constValue, false));
    errorPlot.setCurves(errorPlotCurves);
    errorPlot.setXAxisName(xAxisName);
    errorPlot.setYAxisName(qApp->translate("NBGenerator", "Relative Error [%]"));
    errorPlot.setDesc(errorPlotDesc);
    plots.push_back(errorPlot);
}