#include "JavaXMLHelper.h"

#include "../QLogger/QLog"

#include <QFile>
#include <QTextStream>

using namespace Core::GridSimConfig::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CJavaXMLHelper::c_piTarget = QString("xml");
const QString CJavaXMLHelper::c_piData   = QString("version=\"1.0\" encoding=\"UTF-8\"");

const QString CJavaXMLHelper::c_rootElemTagName          = QString("java");
const QString CJavaXMLHelper::c_rootElemVersionPropName  = QString("version");
const QString CJavaXMLHelper::c_rootElemVersionPropValue = QString("1.6.0_34");
const QString CJavaXMLHelper::c_rootElemClassPropName    = QString("class");
const QString CJavaXMLHelper::c_rootElemClassPropValue   = QString("java.beans.XMLDecoder");


const QString CJavaXMLHelper::c_propElemTagName              = QString("void");
const QString CJavaXMLHelper::c_propElemProperyAttributeName = QString("property");

const QString CJavaXMLHelper::c_objElemTagName            = QString("object");
const QString CJavaXMLHelper::c_objElemClassAttributeName = QString("class");

const QString CJavaXMLHelper::c_listDecoratedElemTagName              = QString("void");
const QString CJavaXMLHelper::c_listDecoratedElemMethodAttributeName  = QString("method");
const QString CJavaXMLHelper::c_listDecoratedElemMethodAttributeValue = QString("add");

const QString CJavaXMLHelper::c_stringTypeName = QString("string");
const QString CJavaXMLHelper::c_int32TypeName  = QString("int");
const QString CJavaXMLHelper::c_int64TypeName  = QString("long");
const QString CJavaXMLHelper::c_doubleTypeName = QString("double");

const qint32 CJavaXMLHelper::c_doubleValuePrecision = 17;
#pragma endregion Private

//////////////////////////////////////////////////////////////////////////
// Code
//////////////////////////////////////////////////////////////////////////

#pragma region Work with files
bool CJavaXMLHelper::loadDocumentFromFile(QDomDocument &doc, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&file))
        return false;

    return true;
}

bool CJavaXMLHelper::saveDocumentToFile(const QDomDocument &doc, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    doc.save(stream, 1);
    return true;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Document creation and verification
QDomDocument CJavaXMLHelper::createDocument()
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction(c_piTarget, c_piData));

    QDomElement root = doc.createElement(c_rootElemTagName);
    root.setAttribute(c_rootElemVersionPropName, c_rootElemVersionPropValue);
    root.setAttribute(c_rootElemClassPropName, c_rootElemClassPropValue);

    doc.appendChild(root);
    return doc;
}

bool CJavaXMLHelper::verifyDocument(const QDomDocument &doc)
{
    QDomElement root = doc.documentElement();
    if(root.tagName() !=  c_rootElemTagName)
        return false;

    QString javaXMLXMLDecoderVersion = root.attribute(c_rootElemVersionPropName);
    if (javaXMLXMLDecoderVersion.isEmpty())
        return false;

    if (javaXMLXMLDecoderVersion != c_rootElemVersionPropValue)
    {
        qLog_TraceMsg() << "JavaXMLDecoder version is unexpected. Expected: " <<
            c_rootElemVersionPropValue << "; Actual: " << javaXMLXMLDecoderVersion;
    }

    return (root.attribute(c_rootElemClassPropName, QString()) == c_rootElemClassPropValue);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region createPropertyElement functions
QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName)
{
    QDomElement propElem = doc.createElement(c_propElemTagName);
    propElem.setAttribute(c_propElemProperyAttributeName, propName);
    return propElem;
}

QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName,
    const QString &propType, const QString &propValue)
{
    QDomElement propElem = createPropertyElement(doc, propName);

    QDomElement propValueElem = doc.createElement(propType);
    propValueElem.appendChild(doc.createTextNode(propValue));
    propElem.appendChild(propValueElem);
    return propElem;
}

QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName,
    const QString &propValue)
{
    return createPropertyElement(doc, propName, c_stringTypeName, propValue);
}

QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName, qint32 propValue)
{
    return createPropertyElement(doc, propName, c_int32TypeName, QString::number(propValue));
}

QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName, quint32 propValue)
{
    return createPropertyElement(doc, propName, c_int32TypeName, QString::number(propValue));
}

QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName, qint64 propValue)
{
    return createPropertyElement(doc, propName, c_int64TypeName, QString::number(propValue));
}

QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName, quint64 propValue)
{
    return createPropertyElement(doc, propName, c_int64TypeName, QString::number(propValue));
}

QDomElement CJavaXMLHelper::createPropertyElement(QDomDocument &doc, const QString &propName, double propValue)
{
    return createPropertyElement(doc, propName, c_doubleTypeName, QString::number(propValue, 'g',
        c_doubleValuePrecision));
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

QDomElement CJavaXMLHelper::createObjectElement(QDomDocument &doc, const QString& className,
    bool listDecorated /*= false*/)
{
    QDomElement objectElement = doc.createElement(c_objElemTagName);
    objectElement.setAttribute(c_objElemClassAttributeName, className);

    if (!listDecorated)
        return objectElement;

    QDomElement listDecoratedObjectElement = doc.createElement(c_listDecoratedElemTagName);
    listDecoratedObjectElement.setAttribute(c_listDecoratedElemMethodAttributeName, c_listDecoratedElemMethodAttributeValue);
    listDecoratedObjectElement.appendChild(objectElement);
    return listDecoratedObjectElement;
}

//////////////////////////////////////////////////////////////////////////

#pragma region getPropertyElement and getPropertyValue functions
bool CJavaXMLHelper::getPropertyElement(const QDomElement &elem, const QString &propName, QDomElement &propElem)
{
    propElem = QDomElement();

    for (QDomNode n = elem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();
        if (e.isNull())
            continue;

        if (e.tagName() != c_propElemTagName)
            continue;
        if (e.attribute(c_propElemProperyAttributeName) != propName)
            continue;

        propElem = e;
        return true;
    }        

    qLog_TraceMsg() << "Property element with name " << propName << " not found.";
    return false;
}

bool CJavaXMLHelper::getPropertyValue(const QDomElement &elem, const QString &propName, const QString &propType,
    QString &propValue, const QString &defaultPropValue /*= QString()*/)
{
    propValue = defaultPropValue;

    QDomElement propElem;
    if (!getPropertyElement(elem, propName, propElem))
        return false;
    
    QDomElement valueElem = propElem.firstChildElement(propType);
    if (valueElem.isNull())
    {
        qLog_WarningMsg() << "No value with type " << propType << " for property" << propName << " found";
        return false;
    }

    propValue = valueElem.text();
    return true;
}

bool CJavaXMLHelper::getPropertyValue(const QDomElement &elem, const QString &propName, QString &propValue,
    const QString &defaultPropValue /*= QString()*/)
{
    return getPropertyValue(elem, propName, c_stringTypeName, propValue, defaultPropValue);
}

bool CJavaXMLHelper::getPropertyValue(const QDomElement &elem, const QString &propName, qint32 &propValue,
    qint32 defaultPropValue /*= 0*/)
{
    propValue = defaultPropValue;

    QString valueString;
    if (!getPropertyValue(elem, propName, c_int32TypeName, valueString))
        return false;
    
    bool ok;
    qint32 value = valueString.toInt(&ok);
    if (ok)
    {
        propValue = value;
        return true;
    }

    qLog_WarningMsg() << "Convertion failed (Property: " << propName << "; Actual value string: " <<
        valueString << "; Target type - qint32).";
    return false;
}

bool CJavaXMLHelper::getPropertyValue(const QDomElement &elem, const QString &propName, quint32 &propValue,
    quint32 defaultPropValue /*= 0*/)
{
    propValue = defaultPropValue;

    QString valueString;
    if (!getPropertyValue(elem, propName, c_int32TypeName, valueString))
        return false;

    bool ok;
    quint32 value = valueString.toUInt(&ok);
    if (ok)
    {
        propValue = value;
        return true;
    }

    qLog_WarningMsg() << "Convertion failed (Property: " << propName << "; Actual value string: " << valueString <<
        "; Target type - quint32).";
    return false;
}

bool CJavaXMLHelper::getPropertyValue(const QDomElement &elem, const QString &propName, qint64 &propValue,
    qint64 defaultPropValue /*= 0LL*/)
{
    propValue = defaultPropValue;

    QString valueString;
    if (!getPropertyValue(elem, propName, c_int64TypeName, valueString))
        return false;

    bool ok;
    qint64 value = valueString.toLongLong(&ok);
    if (ok)
    {
        propValue = value;
        return true;
    }

    qLog_WarningMsg() << "Convertion failed (Property: " << propName << "; Actual value string: " << valueString <<
        "; Target type - qint64).";
    return false;
}

bool CJavaXMLHelper::getPropertyValue(const QDomElement &elem, const QString &propName, quint64 &propValue,
    quint64 defaultPropValue /*= 0LL*/)
{
    propValue = defaultPropValue;

    QString valueString;
    if (!getPropertyValue(elem, propName, c_int64TypeName, valueString))
        return false;

    bool ok;
    quint64 value = valueString.toULongLong(&ok);
    if (ok)
    {
        propValue = value;
        return true;
    }

    qLog_WarningMsg() << "Convertion failed (Property: " << propName << "; Actual value string: " << valueString <<
        "; Target type - quint64).";
    return false;
}

bool CJavaXMLHelper::getPropertyValue(const QDomElement &elem, const QString &propName, double &propValue,
    double defaultPropValue /*= 0.0f*/)
{
    propValue = defaultPropValue;

    QString valueString;
    if (!getPropertyValue(elem, propName, c_doubleTypeName, valueString))
        return false;

    bool ok;
    double value = valueString.toDouble(&ok);
    if (ok)
    {
        propValue = value;
        return true;
    }

    qLog_WarningMsg() << "Convertion failed (Property: " << propName << "; Actual value string: " << valueString <<
        "; Target type - double).";
    return false;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

QString CJavaXMLHelper::getObjectClassName(const QDomElement &elem, bool listDecorated /*= false*/)
{
    if (elem.isNull())
        return QString();

    if (listDecorated)
    {
        if (elem.tagName() != c_listDecoratedElemTagName)
            return QString();

        if (elem.attribute(c_listDecoratedElemMethodAttributeName) != c_listDecoratedElemMethodAttributeValue)
            return QString();

        // Recursive call with listDecorated = false;
        //
        return getObjectClassName(elem.firstChildElement(), false);
    }

    if (elem.tagName() != c_objElemTagName)
        return QString();

    QString className = elem.attribute(c_objElemClassAttributeName);
    return className;
}