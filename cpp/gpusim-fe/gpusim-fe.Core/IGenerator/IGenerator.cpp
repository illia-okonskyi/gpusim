#include "IGenerator.h"

#include "../../QLogger/QLog"

using namespace Core;

IGenerator::IGenerator(const QString &pluginName)
    : m_pluginName(pluginName)
{
    
}

IGenerator::~IGenerator()
{

}

//////////////////////////////////////////////////////////////////////////

bool IGenerator::isInputValid(const CGeneratorInput &input) const
{
    return m_inputValidator.validate(input);
}

bool IGenerator::isBoundaryConditionsValid(const CGeneratorBoundaryConditions &conditions) const
{
    return m_conditionsValidator.validate(conditions);
}

bool IGenerator::isSettingsValid(const CGeneratorSettings &settings) const
{
    return m_settingsValidator.validate(settings);
}

bool IGenerator::isExperimentValid(const CExperiment &experiment, bool checkSimulations /*= true*/) const
{
    if (experiment.getPluginName() != m_pluginName)
        return false;
    
    if (!isBoundaryConditionsValid(experiment.getConditions()))
        return false;

    if (!isSettingsValid(experiment.getSettings()))
        return false;

    if (!checkSimulations)
        return true;

    auto I = experiment.getSimulationsRef().constBegin();
    auto E = experiment.getSimulationsRef().constEnd();
    for (; I != E; ++I)
    {
        if (!isInputValid(I->getGeneratorInput()))
            return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

CExperimentPtr IGenerator::createExperiment(const CGeneratorBoundaryConditions &conditions) const
{
    if (!isBoundaryConditionsValid(conditions))
    {
        qLog_WarningMsg() << "Wrong boundary conditions";
        return CExperimentPtr();
    }

    auto pExperiment = CExperimentPtr(new CExperiment());
    pExperiment->setPluginName(m_pluginName);
    pExperiment->setConditions(conditions);

    updateSimulations(*pExperiment);
    return pExperiment;
}

void IGenerator::updateSimulations(CExperiment &experiment) const
{
    CSimulationsList sims;
    foreach(const CGeneratorInput &input, createInputs(experiment.getConditions()))
        sims.push_back(createSimulation(input));

    experiment.setSimulations(sims);
}

void IGenerator::updatePlots(CExperiment &experiment) const
{
    experiment.setPlots(createPlots(experiment.getSimulationsRef()));
}


//////////////////////////////////////////////////////////////////////////

CSimulation IGenerator::createSimulation(const CGeneratorInput &input) const
{
    CSimulation sim;
    sim.setName(formatInput(input, true));
    sim.setDesc(formatInput(input));
    sim.setGeneratorInput(input);
    sim.setCalcTime(0.0f);
    sim.setSimTime(0.0f);
    return sim;
}