#pragma once

#include "../GridSimConfig/GridSimConfig.h"
#include "../Experiment/Experiment.h"
#include "../../gpusim-fe.Tools/QVariantMapValidator.h"

#include <QSharedPointer>

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT IGenerator
    {
    public:
        virtual ~IGenerator();

        bool isInputValid(const CGeneratorInput &input) const;
        bool isBoundaryConditionsValid(const CGeneratorBoundaryConditions &conditions) const;
        bool isSettingsValid(const CGeneratorSettings &settings) const;
        bool isExperimentValid(const CExperiment &experiment, bool checkSimulations = true) const;

        CExperimentPtr createExperiment(const CGeneratorBoundaryConditions &conditions) const;
        void updateSimulations(CExperiment &experiment) const;
        void updatePlots(CExperiment &experiment) const;


        virtual CGeneratorInput createDefaultInput() const = 0;
        virtual CGeneratorBoundaryConditions createDefaultConditions() const = 0;
        virtual CGeneratorSettings createDefaultSettings() const = 0;

        // Returns string representation (short if compact = true and long otherwise) of passed input.
        virtual QString formatInput(const CGeneratorInput &input, bool compact = false) const = 0;
        
        // Creates configuration for gpusim.simulator module by parameters set.
        virtual GridSimConfig::CGridSimConfig createSimulationConfig(const CGeneratorInput &input,
            const CGeneratorSettings &settings) const = 0;

    protected:
        typedef QVector<CGeneratorInput> CGeneratorInputsVector;

    protected:
        IGenerator(const QString &pluginName);

        virtual CGeneratorInputsVector createInputs(const CGeneratorBoundaryConditions &conditions) const = 0;
        virtual CPlotsList createPlots(const CSimulationsList &sims) const = 0;

    private:
        CSimulation createSimulation(const CGeneratorInput &input) const;

    protected:
        Tools::QVariantMapValidator m_inputValidator;
        Tools::QVariantMapValidator m_conditionsValidator;
        Tools::QVariantMapValidator m_settingsValidator;

    private:
        QString m_pluginName;
    };

    typedef QSharedPointer<IGenerator> IGeneratorPtr;
}