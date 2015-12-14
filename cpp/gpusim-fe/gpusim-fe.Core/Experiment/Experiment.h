#pragma once

#include "../Simulation/Simulation.h"
#include "../IGenerator/GeneratorBorderConditions.h"
#include "../IGenerator/GeneratorSettings.h"
#include "../IGenerator/GeneratorInput.h"
#include "../Plot/Plot.h"

#include <QSharedPointer>

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CExperiment
    {
    public:
        CExperiment();

        bool operator == (const CExperiment& other) const;
        bool operator != (const CExperiment& other) const;

        QString getName() const;
        void setName(const QString &name);

        QString getDesc() const;
        void setDesc(const QString &desc);

        QString getPluginName() const;
        void setPluginName(const QString &pluginName);

        QString getCalibrationFilePath() const;
        void setCalibrationFilePath(const QString &calibrationFilePath);

        CGeneratorBoundaryConditions getConditions() const;
        void setConditions(const CGeneratorBoundaryConditions &conditions);
        
        CGeneratorSettings getSettings() const;
        void setSettings(const CGeneratorSettings &settings);

        CSimulationsList getSimulations() const;
        const CSimulationsList &getSimulationsRef() const;
        void setSimulations(const CSimulationsList &simulations);

        CPlotsList getPlots() const;
        const CPlotsList &getPlotsRef() const;
        void setPlots(const CPlotsList &plots);
        void clearPlots();

        bool saveToFile(const QString &filePath) const;
        bool loadFromFile(const QString &filePath);

        static bool calculateStatistics(const CExperiment &experiment, double &relativeError,
            double &squareDistance);
    private:
        QString m_name;
        QString m_desc;
        QString m_pluginName;
        QString m_calibrationFilePath;
        CGeneratorBoundaryConditions m_conditions;
        CGeneratorSettings m_settings;
        CSimulationsList m_simulations;
        CPlotsList m_plots;
    };

    typedef QSharedPointer<CExperiment> CExperimentPtr;
}

Q_DECLARE_METATYPE(Core::CExperiment);

GPUSIM_FE_CORE_EXPORT QDataStream &operator << (QDataStream &out, const Core::CExperiment &exp);
GPUSIM_FE_CORE_EXPORT QDataStream &operator >> (QDataStream &in, Core::CExperiment &exp);