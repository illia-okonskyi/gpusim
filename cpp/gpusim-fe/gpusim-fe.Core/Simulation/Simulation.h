#pragma once

#include "../IGenerator/GeneratorInput.h"

#include <QVector>
#include <QDataStream>

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CSimulation
    {
    public:
        CSimulation();

        bool operator == (const CSimulation& other) const;
        bool operator != (const CSimulation& other) const;

        QString getName() const;
        void setName(const QString &name);

        QString getDesc() const;
        void setDesc(const QString &desc);

        CGeneratorInput getGeneratorInput() const;
        void setGeneratorInput(const CGeneratorInput &generatorInput);

        double getCalcTime() const;
        void setCalcTime(double calcTime);

        double getSimTime() const;
        void setSimTime(double calcTime);

        double getTimeDistance() const;
        double getRelativeError() const;

    private:
        QString m_name;
        QString m_desc;
        CGeneratorInput m_generatorInput;
        double m_calcTime;
        double m_simTime;
    };

    typedef QVector<CSimulation> CSimulationsList;
}

Q_DECLARE_METATYPE(Core::CSimulation);
Q_DECLARE_METATYPE(Core::CSimulationsList);

GPUSIM_FE_CORE_EXPORT QDataStream &operator << (QDataStream &out, const Core::CSimulation &sim);
GPUSIM_FE_CORE_EXPORT QDataStream &operator >> (QDataStream &in, Core::CSimulation &sim);