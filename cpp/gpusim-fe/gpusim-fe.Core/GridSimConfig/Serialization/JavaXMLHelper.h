#pragma once

#include "../../Core_global.h"

#include <QDomDocument>

namespace Core
{
    namespace GridSimConfig
    {
        namespace Serialization
        {
            class GPUSIM_FE_CORE_EXPORT CJavaXMLHelper
            {
            public:
                static bool loadDocumentFromFile(QDomDocument &doc, const QString &filePath);
                static bool saveDocumentToFile(const QDomDocument &doc, const QString &filePath);

                static QDomDocument createDocument();
                static bool verifyDocument(const QDomDocument &doc);

                // Creates following structure to elem (doc is used as factory for QDom* objects):
                //   <void property="propName"> 
                //     <propType>propValue</propType>
                //   </void>
                // The overloading without property value creates an empty property structure:
                //   <void property="propName"> 
                //   </void>
                //
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName);
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName,
                    const QString &propType, const QString &propValue);
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName,
                    const QString &propValue);
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName,
                    qint32 propValue);
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName,
                    quint32 propValue);
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName,
                    qint64 propValue);
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName,
                    quint64 propValue);
                static QDomElement createPropertyElement(QDomDocument &doc, const QString &propName,
                    double propValue);

                // Creates following structure to elem (doc is used as factory for QDom* objects):
                //   <object class="className"> 
                //   </object>
                // In case of listDecorated is true created next structure with list decoration:
                //   <void method="add">
                //     <object class="className"> 
                //     </object>
                //   </void>
                // Returns root element (decoration or object according to the listDecorated parameter value).
                //
                static QDomElement createObjectElement(QDomDocument &doc, const QString& className,
                    bool listDecorated = false);

                static bool getPropertyElement(const QDomElement &elem, const QString &propName, QDomElement &propElem);

                static bool getPropertyValue(const QDomElement &elem, const QString &propName, const QString &propType,
                    QString &propValue, const QString &defaultPropValue = QString());
                static bool getPropertyValue(const QDomElement &elem, const QString &propName, QString &propValue,
                    const QString &defaultPropValue = QString());
                static bool getPropertyValue(const QDomElement &elem, const QString &propName, qint32 &propValue,
                    qint32 defaultPropValue = 0);
                static bool getPropertyValue(const QDomElement &elem, const QString &propName, quint32 &propValue,
                    quint32 defaultPropValue = 0);
                static bool getPropertyValue(const QDomElement &elem, const QString &propName, qint64 &propValue,
                    qint64 defaultPropValue = 0LL);
                static bool getPropertyValue(const QDomElement &elem, const QString &propName, quint64 &propValue,
                    quint64 defaultPropValue = 0LL);
                static bool getPropertyValue(const QDomElement &elem, const QString &propName, double &propValue,
                    double defaultPropValue = 0.0f);

                // Returns an empty string in case of error
                //
                static QString getObjectClassName(const QDomElement &elem, bool listDecorated = false);

            private:
                CJavaXMLHelper();

            private:
                static const QString c_piTarget;
                static const QString c_piData;

                static const QString c_rootElemTagName;
                static const QString c_rootElemVersionPropName;
                static const QString c_rootElemVersionPropValue;
                static const QString c_rootElemClassPropName;
                static const QString c_rootElemClassPropValue;

                static const QString c_propElemTagName;
                static const QString c_propElemProperyAttributeName;

                static const QString c_objElemTagName;
                static const QString c_objElemClassAttributeName;

                static const QString c_listDecoratedElemTagName;
                static const QString c_listDecoratedElemMethodAttributeName;
                static const QString c_listDecoratedElemMethodAttributeValue;

                static const QString c_stringTypeName;
                static const QString c_int32TypeName;
                static const QString c_int64TypeName;
                static const QString c_doubleTypeName;

                static const qint32 c_doubleValuePrecision;
            };
        }
    }
}