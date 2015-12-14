#include "MMGenerator.h"

#include "../QLogger/QLog"

#include <qmath.h>
#include <QCoreApplication>

using namespace MMEP;
using namespace Core::GridSimConfig;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const QString CMMGenerator::c_cpuMachinePECount_settingName  = QString("cpuMachinePECount");
const QString CMMGenerator::c_cpuMachinePERating_settingName = QString("cpuMachinePERating");
const QString CMMGenerator::c_gpuMachinePECount_settingName  = QString("gpuMachinePECount");
const QString CMMGenerator::c_gpuMachinePERating_settingName = QString("gpuMachinePERating");
const QString CMMGenerator::c_resourceBaudRate_settingName   = QString("resourceBaudRate");
const QString CMMGenerator::c_linkBaudRate_settingName       = QString("linkBaudRate");
const QString CMMGenerator::c_loadOperationCost_settingName  = QString("loadOperationCost");
const QString CMMGenerator::c_saveOperationCost_settingName  = QString("saveOperationCost");

const QString CMMGenerator::c_minMatrixSize_conditionName       = QString("minMatrixSize");
const QString CMMGenerator::c_maxMatrixSize_conditionName       = QString("maxMatrixSize");
const QString CMMGenerator::c_matrixSizeIncrement_conditionName = QString("matrixSizeIncrement");
const QString CMMGenerator::c_blockSize_conditionName           = QString("blockSize");
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CMMGenerator::c_matrixSize_inputName = QString("matrixSize");
const QString CMMGenerator::c_blockSize_inputName  = QString("blockSize");

const QString CMMGenerator::c_inputShortFormat = QString("M = %1; B = %2");
const QString CMMGenerator::c_inputLongFormat  = QString("Matrix size: %1; Block size %2");
#pragma endregion


CMMGenerator::CMMGenerator(const QString &pluginName)
    : Core::IGenerator(pluginName)
{
    m_inputValidator.addValueInfo(c_matrixSize_inputName, QVariant::UInt);
    m_inputValidator.addValueInfo(c_blockSize_inputName , QVariant::UInt);

    m_conditionsValidator.addValueInfo(c_minMatrixSize_conditionName      , QVariant::UInt);
    m_conditionsValidator.addValueInfo(c_maxMatrixSize_conditionName      , QVariant::UInt);
    m_conditionsValidator.addValueInfo(c_matrixSizeIncrement_conditionName, QVariant::UInt);
    m_conditionsValidator.addValueInfo(c_blockSize_conditionName          , QVariant::UInt);

    m_settingsValidator.addValueInfo(c_cpuMachinePECount_settingName , QVariant::UInt);
    m_settingsValidator.addValueInfo(c_cpuMachinePERating_settingName, QVariant::UInt);
    m_settingsValidator.addValueInfo(c_gpuMachinePECount_settingName , QVariant::UInt);
    m_settingsValidator.addValueInfo(c_gpuMachinePERating_settingName, QVariant::UInt);
    m_settingsValidator.addValueInfo(c_resourceBaudRate_settingName  , QVariant::Double);
    m_settingsValidator.addValueInfo(c_linkBaudRate_settingName      , QVariant::Double);
    m_settingsValidator.addValueInfo(c_loadOperationCost_settingName , QVariant::Double);
    m_settingsValidator.addValueInfo(c_saveOperationCost_settingName , QVariant::Double);
}

//////////////////////////////////////////////////////////////////////////

Core::CGeneratorInput CMMGenerator::createDefaultInput() const
{
    Core::CGeneratorInput input;
    input[c_matrixSize_inputName] = QVariant::fromValue(quint32(16));
    input[c_blockSize_inputName ] = QVariant::fromValue(quint32(16));
    return input;
}

Core::CGeneratorBoundaryConditions CMMGenerator::createDefaultConditions() const
{
    Core::CGeneratorBoundaryConditions conditions;
    conditions[c_minMatrixSize_conditionName      ] = QVariant::fromValue(quint32(16));
    conditions[c_maxMatrixSize_conditionName      ] = QVariant::fromValue(quint32(4096));
    conditions[c_matrixSizeIncrement_conditionName] = QVariant::fromValue(quint32(16));
    conditions[c_blockSize_conditionName          ] = QVariant::fromValue(quint32(16));
    return conditions;
}

Core::CGeneratorSettings CMMGenerator::createDefaultSettings() const
{
    Core::CGeneratorSettings settings;
    settings[c_cpuMachinePECount_settingName ] = QVariant::fromValue(quint32(8));
    settings[c_cpuMachinePERating_settingName] = QVariant::fromValue(quint32(1000));
    settings[c_gpuMachinePECount_settingName ] = QVariant::fromValue(quint32(384));
    settings[c_gpuMachinePERating_settingName] = QVariant::fromValue(quint32(10000));
    settings[c_resourceBaudRate_settingName  ] = QVariant::fromValue(double(1e+10));
    settings[c_linkBaudRate_settingName      ] = QVariant::fromValue(double(1e+10));
    settings[c_loadOperationCost_settingName ] = QVariant::fromValue(double(1.8e-4));
    settings[c_saveOperationCost_settingName ] = QVariant::fromValue(double(0.936e-3));
    return settings;
}

// NOTE: Boundary condition check is already done by caller
//
Core::IGenerator::CGeneratorInputsVector CMMGenerator::createInputs(
    const Core::CGeneratorBoundaryConditions &conditions) const
{
    quint32 minMatrixSize       = conditions[c_minMatrixSize_conditionName      ].toUInt();
    quint32 maxMatrixSize       = conditions[c_maxMatrixSize_conditionName      ].toUInt();
    quint32 matrixSizeIncrement = conditions[c_matrixSizeIncrement_conditionName].toUInt();
    quint32 blockSize           = conditions[c_blockSize_conditionName          ].toUInt();

    Core::IGenerator::CGeneratorInputsVector inputs;
    for (quint32 i = minMatrixSize; i <= maxMatrixSize; i += matrixSizeIncrement)
    {
        auto input = createDefaultInput();
        input[c_matrixSize_inputName] = QVariant::fromValue(i);
        input[c_blockSize_inputName ] = QVariant::fromValue(blockSize);
        inputs.push_back(input);
    }

    return inputs;
}

Core::CPlotsList CMMGenerator::createPlots(const Core::CSimulationsList &sims) const
{
    bool isCalcTimePresent = (sims.front().getCalcTime() > 0.0f);

    //
    // 1) Prepare points
    //
    Core::CCurvePoints modelTimeCurvePoints;
    Core::CCurvePoints realSystemTimeCurvePoints;
    Core::CCurvePoints errorCurvePoints;

    double squareDistance = 0.0f;
    double relativeError = 0.0f;

    for (auto I = sims.constBegin(), E = sims.constEnd(); I != E; ++I)
    {
        auto input = I->getGeneratorInput();
        if (!isInputValid(input))
            continue;

        quint32 matrixSize  = input[c_matrixSize_inputName].toUInt();
        double  simTime     = I->getSimTime();
        double  calcTime    = I->getCalcTime();
        double  error       = I->getRelativeError();

        modelTimeCurvePoints.push_back(QPointF(double(matrixSize), simTime));
        
        if (isCalcTimePresent)
        {
            realSystemTimeCurvePoints.push_back(QPointF(double(matrixSize), calcTime));
            errorCurvePoints.push_back(QPointF(double(matrixSize), error));

            squareDistance += qPow(I->getTimeDistance(), 2.0);
            relativeError += error;
        }

    }

    QString experimentResultsDesc;
    if (isCalcTimePresent)    
    {
        squareDistance /= double(sims.size());
        relativeError /= double(sims.size());

        QString experimentResultsDesc_format = qApp->translate("MMGenerator",
            "\nAverange relative error: %1%\nAverange square distance: %2");
        experimentResultsDesc = experimentResultsDesc_format.arg(QString::number(relativeError),
            QString::number(squareDistance));
    }

    //
    // 2) Prepare curves list
    //
    Core::CPlotCurvesList timePlotCurves;
    Core::CPlotCurvesList errorPlotCurves;

    timePlotCurves.push_back(Core::CPlotCurve(
        qApp->translate("MMGenerator", "Model"),
        modelTimeCurvePoints));
    if (isCalcTimePresent)
    {
        timePlotCurves.push_back(Core::CPlotCurve(
            qApp->translate("MMGenerator", "Real parallel system"),
            realSystemTimeCurvePoints));
        errorPlotCurves.push_back(Core::CPlotCurve(
            qApp->translate("MMGenerator", "Relative error"),
            errorCurvePoints));
    }

    //
    // 3) Create plots
    //
    Core::CPlot timePlot;
    timePlot.setName(qApp->translate("MMGenerator", "Time(MatrixSize)"));
    timePlot.setDesc(
        qApp->translate("MMGenerator", "Time from matrixSize dependency.") +
        experimentResultsDesc);
    timePlot.setXAxisName(qApp->translate("MMGenerator", "Matrix size"));
    timePlot.setYAxisName(qApp->translate("MMGenerator", "Time [ms]"));
    timePlot.setCurves(timePlotCurves);

    Core::CPlot relativeErrorPlot;
    relativeErrorPlot.setName(qApp->translate("MMGenerator", "RelativeError(MatrixSize)"));
    relativeErrorPlot.setDesc(
        qApp->translate("MMGenerator", "Relative error of time from matrixSize dependency.") +
        experimentResultsDesc);
    relativeErrorPlot.setXAxisName(qApp->translate("MMGenerator", "Matrix size"));
    relativeErrorPlot.setYAxisName(qApp->translate("MMGenerator", "Relative error [%]"));
    relativeErrorPlot.setCurves(errorPlotCurves);

    // 4) Return plots
    //
    Core::CPlotsList plots;
    plots.push_back(timePlot);
    if (isCalcTimePresent)
        plots.push_back(relativeErrorPlot);
    return plots;
}

//////////////////////////////////////////////////////////////////////////

QString CMMGenerator::formatInput(const Core::CGeneratorInput &input, bool compact /*= false*/) const
{
    if (!isInputValid(input))
        return QString("<MMGenerator: wrong input>");

    return (compact? c_inputShortFormat : c_inputLongFormat).arg(
        input[c_matrixSize_inputName].toString(),
        input[c_blockSize_inputName ].toString());
}

CGridSimConfig CMMGenerator::createSimulationConfig(const Core::CGeneratorInput &input,
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
    quint32 matrixSize  = input[c_matrixSize_inputName].toUInt();
    quint32 blockSize   = input[c_blockSize_inputName ].toUInt();

    // 2) Get generator settings
    //
    quint32 cpuMachinePECount  = settings[c_cpuMachinePECount_settingName ].toUInt();
    quint32 cpuMachinePERating = settings[c_cpuMachinePERating_settingName].toUInt();
    quint32 gpuMachinePECount  = settings[c_gpuMachinePECount_settingName ].toUInt();
    quint32 gpuMachinePERating = settings[c_gpuMachinePERating_settingName].toUInt();
    double  resourceBaudRate   = settings[c_resourceBaudRate_settingName  ].toDouble();
    double  linkBaudRate       = settings[c_linkBaudRate_settingName      ].toDouble();
    double  loadOperationCost  = settings[c_loadOperationCost_settingName ].toDouble();
    double  saveOperationCost  = settings[c_saveOperationCost_settingName ].toDouble();
    
    // 3) Create machines config with 2 machines: one for CPU and one for GPU
    //
    CGridSimMachinesConfig machines;
    machines.append(CGridSimMachineConfig(gpuMachinePECount, gpuMachinePERating));
    machines.append(CGridSimMachineConfig(cpuMachinePECount, cpuMachinePERating));

    // 4) Create resources config with only one resource
    //
    CGridSimResourceConfig resource;
    resource.setArch("gpusim.MatrixMultiply-ExperimentPlugin.Arch");
    resource.setOS("gpusim.MatrixMultiply-ExperimentPlugin.OS");
    resource.setBaudRate(resourceBaudRate);
    resource.setCostPerSec(1.0f);
    resource.setMachines(machines);
    CGridSimResourcesConfig resources;
    resources.append(resource);

    // 5) Create gridlet config
    auto gridlets = createGridletsConfig(matrixSize, blockSize, loadOperationCost, saveOperationCost);

    // 6) Create simulation configuration
    return CGridSimConfig(linkBaudRate, resources, gridlets);    
}

//////////////////////////////////////////////////////////////////////////

CGridSimGridletsConfig CMMGenerator::createGridletsConfig(quint32 matrixSize, quint32 blockSize,
    double loadOperationCost, double saveOperationCost) const
{
    // NOTE: Formulas:
    //   length = block_size * matrix_size^2 * saveOperationCosr + 2 * matrix_size^3 * loadOperationCost;
    //   in_size = 3 * block_size;
    //   out_size = block_size;
    //   count = (matrixSize / blockSize)
    //
    double length = double(blockSize) * pow(double(matrixSize), 2) * saveOperationCost +
        2 * pow(double(matrixSize), 3) * loadOperationCost;
    quint64 inputSize = 3 * blockSize;
    quint64 outputSize = blockSize;
    quint32 count = matrixSize / blockSize;

    CGridSimGridletsConfig gridlets;
    gridlets.append(CGridSimGridletConfig(length, inputSize, outputSize, count));
    return gridlets;
}

