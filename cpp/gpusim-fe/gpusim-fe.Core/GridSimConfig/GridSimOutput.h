#pragma once

#include "Serialization/IJavaXMLSerialize.h"

namespace Core
{
    namespace GridSimConfig
    {
        class GPUSIM_FE_CORE_EXPORT CGridSimOutput: public Serialization::IJavaXMLSerialize
        {
        public:
            static const quint32 c_minVersion;
            static const double  c_minTotalSimulationTime;

            static const quint32 c_currentOutputVersion;
            static const double  c_defaultTotalSimulationTime;

        public:
            CGridSimOutput(double totalSimulationTime = c_defaultTotalSimulationTime);

            bool operator == (const CGridSimOutput& other) const;
            bool operator != (const CGridSimOutput& other) const;

            quint32 getVersion() const;
            void setVersion(quint32 version);

            double getTotalSimulationTime() const;
            void setTotalSimulationTime(double totalSimulationTime);

            virtual bool isValid() const;

        protected:
            // You must overload this stubs for data (de)serialization.
            //
            virtual void saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const;
            virtual void loadDataFromXMLNode(const QDomElement &elem);

            // Factory method for creating instances of derived objects
            virtual Serialization::IJavaXMLSerializePtr create() const;

            // Method from copying data from other instance of derived object
            virtual void copy(const Serialization::IJavaXMLSerializePtr &from);

        private:
            quint32 m_version;
            double m_totalSimulationTime;

        private:
            static const QString c_className;
            static const QString c_versionPropName;
            static const QString c_totalSimulationTimePropName;
        };
    }
}