#pragma once

#include "../../Core_global.h"

#include <QDomElement>
#include <QSharedPointer>

namespace Core
{
    namespace GridSimConfig
    {
        namespace Serialization
        {
            class IJavaXMLSerialize;
            typedef QSharedPointer<IJavaXMLSerialize> IJavaXMLSerializePtr;

            class GPUSIM_FE_CORE_EXPORT IJavaXMLSerialize
            {
            public:
                QString getClassName();

                static QDomElement toXMLNode(QDomDocument &doc, const IJavaXMLSerialize &o, bool listDecorated = false);
                QDomElement toXMLNode(QDomDocument &doc, bool listDecorated = false) const;

                static bool fromXMLNode(const QDomElement&elem, IJavaXMLSerialize &o, bool listDecoration = false);
                bool fromXMLNode(const QDomElement&elem, bool listDecorated = false);

                static bool isValid(const IJavaXMLSerialize& o);
                // You must rewrite this stub for correct deserialization support.
                virtual bool isValid() const { return true; };

                virtual ~IJavaXMLSerialize() { }

                bool saveToFile(const QString &filePath) const;
                bool loadFromFile(const QString &filePath);

            protected:
                IJavaXMLSerialize(const QString &className = QString());

                // You must overload this stubs for data (de)serialization.
                //
                virtual void saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const { }
                virtual void loadDataFromXMLNode(const QDomElement &elem) { }

                // Factory method for creating instances of derived objects
                virtual IJavaXMLSerializePtr create() const = 0;

                // Method from copying data from other instance of derived object
                virtual void copy(const IJavaXMLSerializePtr &from) = 0;

            private:
                QString m_className;
            };
        }
    }
}