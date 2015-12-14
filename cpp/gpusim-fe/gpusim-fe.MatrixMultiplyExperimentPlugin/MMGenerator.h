#pragma once

#include "../gpusim-fe.Core/IGenerator/IGenerator.h"

namespace MMEP
{
    class CMMGenerator: public Core::IGenerator
    {
    public:
        static const QString c_cpuMachinePECount_settingName;
        static const QString c_cpuMachinePERating_settingName;
        static const QString c_gpuMachinePECount_settingName;
        static const QString c_gpuMachinePERating_settingName;
        static const QString c_resourceBaudRate_settingName;
        static const QString c_linkBaudRate_settingName;
        static const QString c_loadOperationCost_settingName;
        static const QString c_saveOperationCost_settingName;

        static const QString c_minMatrixSize_conditionName;
        static const QString c_maxMatrixSize_conditionName;
        static const QString c_matrixSizeIncrement_conditionName;
        static const QString c_blockSize_conditionName;

    public:
        CMMGenerator(const QString &pluginName);

        virtual Core::CGeneratorInput createDefaultInput() const;
        virtual Core::CGeneratorBoundaryConditions createDefaultConditions() const;
        virtual Core::CGeneratorSettings createDefaultSettings() const;

        // Returns string representation (short if compact = true and long otherwise) of passed input.
        virtual QString formatInput(const Core::CGeneratorInput &input, bool compact = false) const;

        // Creates configuration for gpusim.simulator module by parameters set.
        virtual Core::GridSimConfig::CGridSimConfig createSimulationConfig(const Core::CGeneratorInput &input,
            const Core::CGeneratorSettings &settings) const;

    protected:
        virtual Core::IGenerator::CGeneratorInputsVector createInputs(
            const Core::CGeneratorBoundaryConditions &conditions) const;
        virtual Core::CPlotsList createPlots(const Core::CSimulationsList &sims) const;

    private:
        Core::GridSimConfig::CGridSimGridletsConfig createGridletsConfig(quint32 matrixSize, quint32 blockSize,
            double loadOperationCost, double saveOperationCost) const;

    private:
        static const QString c_matrixSize_inputName;
        static const QString c_blockSize_inputName;

        static const QString c_inputShortFormat;
        static const QString c_inputLongFormat;
    };
}