#pragma once

#include "Serialization/IJavaXMLSerialize.h"

#include <QVector>

namespace Core
{
    namespace GridSimConfig
    {
        class GPUSIM_FE_CORE_EXPORT CGridSimGridletConfig: public Serialization::IJavaXMLSerialize
        {
        public:
            static const double  c_minLength;
            static const quint64 c_minInputSize;
            static const quint64 c_minOutputSize;
            static const quint32 c_minCount;

            static const double  c_defaultLength;
            static const quint64 c_defaultInputSize;
            static const quint64 c_defaultOutputSize;
            static const quint32 c_defaultCount;

        public:
            CGridSimGridletConfig(double length = c_defaultLength, quint64 inputSize = c_defaultInputSize,
                quint64 outputSize = c_defaultOutputSize, quint32 count = c_defaultCount);

            bool operator == (const CGridSimGridletConfig& other) const;
            bool operator != (const CGridSimGridletConfig& other) const;

            double getLength() const;
            void setLength(double length);

            quint64 getInputSize() const;
            void setInputSize(quint64 inputSize);

            quint64 getOutputSize() const;
            void setOutputSize(quint64 outputSize);

            quint32 getCount() const;
            void setCount(quint32 count);

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
            double m_length;
            quint64 m_inputSize;
            quint64 m_outputSize;
            quint32 m_count;

        private:
            static const QString c_className;
            static const QString c_lengthPropName;
            static const QString c_inputSizePropName;
            static const QString c_outputSizePropName;
            static const QString c_countPropName;
        };

        typedef QVector<CGridSimGridletConfig> CGridSimGridletsConfig;
    }
}