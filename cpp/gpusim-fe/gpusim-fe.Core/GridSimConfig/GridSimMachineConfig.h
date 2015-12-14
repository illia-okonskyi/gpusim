#pragma once

#include "Serialization/IJavaXMLSerialize.h"

#include <QVector>

namespace Core
{
    namespace GridSimConfig
    {
        class GPUSIM_FE_CORE_EXPORT CGridSimMachineConfig: public Serialization::IJavaXMLSerialize
        {
        public:
            static const quint32 c_minPECount;
            static const quint32 c_minPERating;
            static const quint32 c_minCount;

            static const quint32 c_defaultPECount;
            static const quint32 c_defaultPERating;
            static const quint32 c_defaultCount;
        public:
            CGridSimMachineConfig(quint32 peCount = c_defaultPECount,
                quint32 peRating = c_defaultPERating, quint32 count = c_defaultCount);

            bool operator == (const CGridSimMachineConfig& other) const;
            bool operator != (const CGridSimMachineConfig& other) const;

            quint32 getID() const;
            void setID(quint32 id);

            quint32 getPECount() const;
            void setPECount(quint32 peCount);

            quint32 getPERating() const;
            void setPERating(quint32 peRating);

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
            quint32 m_id;
            quint32 m_peCount;
            quint32 m_peRating;
            quint32 m_count;

        private:
            static const QString c_className;
            static const QString c_idPropName;
            static const QString c_peCountPropName;
            static const QString c_peRatingPropName;
            static const QString c_countPropName;
        };

        typedef QVector<CGridSimMachineConfig> CGridSimMachinesConfig;
    }
}