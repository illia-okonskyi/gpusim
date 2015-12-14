#pragma once

#include "../gpusim-fe.Core/IGenerator/IGenerator.h"

namespace NBEP
{
    class CNBGenerator: public Core::IGenerator
    {
    public:
        static const QString c_gpuCoreRating_settingName;
        static const QString c_limitationsDivider_settingName;
        static const QString c_smallTPBPenaltyWeight_settingName;
        static const QString c_largeTPBPenaltyWeight_settingName;
        static const QString c_multiplicativeLengthScaleFactor_settingName;
        static const QString c_additiveLengthScaleFactor_settingName;

        static const QString c_minN_conditionName;
        static const QString c_maxN_conditionName;
        static const QString c_minTPB_conditionName;
        static const QString c_maxTPB_conditionName;

    public:
        CNBGenerator(const QString &pluginName);

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
        Core::GridSimConfig::CGridSimGridletsConfig createGridletsConfig(quint32 n, quint32 threadsPerBlock,
            double limitationsDivider, double smallTPBPenaltyWeight, double largeTPBPenaltyWeight,
            double multiplicativeLengthScaleFactor, double additiveLengthScaleFactor) const;

    private:
        typedef enum _tagCTimeFunctionType
        {
            TimeFromN_ConstTPB,
            TimeFromTPB_ConstN
        } CTimeFunctionType;

        QString buildPlotName(const QString &timeFromParameterName, const QString &constParameterName,
            quint32 constParameterValue, bool timePlot) const;
        QString buildPlotMainDesc(const QString &timeFromParameterName, const QString &constParameterName,
            quint32 constParameterValue, bool timePlot) const;
        QString buildCommonStatisticsString(const Core::CSimulationsList &sims) const;
        QString buildMinTPBStatisticsString(const Core::CSimulationsList &simulations) const;

        void appendPlots(CTimeFunctionType ft, quint32 constValue, const Core::CSimulationsList &sims,
            Core::CPlotsList &plots) const;
        void resort(Core::CSimulationsList &sims) const;

    private:
        static const QString c_n_inputName;
        static const QString c_tpb_inputName;

        static const QString c_nInputDisplyName;
        static const QString c_tpbDisplyName;

        static const QString c_inputShortFormat;
        static const QString c_inputLongFormat;
    };
}