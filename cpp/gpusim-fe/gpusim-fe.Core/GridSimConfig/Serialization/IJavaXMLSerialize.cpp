#include "IJavaXMLSerialize.h"
#include "JavaXMLHelper.h"

using namespace Core::GridSimConfig::Serialization;

IJavaXMLSerialize::IJavaXMLSerialize(const QString &className /*= QString()*/): m_className(className)
{

}

QString IJavaXMLSerialize::getClassName()
{
    return m_className;
}


bool IJavaXMLSerialize::isValid(const IJavaXMLSerialize& o)
{
    return o.isValid();
}

//////////////////////////////////////////////////////////////////////////

QDomElement IJavaXMLSerialize::toXMLNode(QDomDocument &doc, const IJavaXMLSerialize &o, bool listDecorated /*= false*/)
{
    return o.toXMLNode(doc, listDecorated);
}

QDomElement IJavaXMLSerialize::toXMLNode(QDomDocument &doc, bool listDecorated /*= false*/) const
{
    QDomElement elem = CJavaXMLHelper::createObjectElement(doc, m_className, listDecorated);
    saveDataToXMLNode(doc, listDecorated? elem.firstChildElement() : elem);
    return elem;
}

//////////////////////////////////////////////////////////////////////////

bool IJavaXMLSerialize::fromXMLNode(const QDomElement&elem, IJavaXMLSerialize &o, bool listDecorated /*= false*/)
{
    return o.fromXMLNode(elem, listDecorated);
}

bool IJavaXMLSerialize::fromXMLNode(const QDomElement&elem, bool listDecorated /*= false*/)
{
    if (CJavaXMLHelper::getObjectClassName(elem, listDecorated) != m_className)
        return false;

    IJavaXMLSerializePtr deserialized = create();
    deserialized->loadDataFromXMLNode(listDecorated? elem.firstChildElement() : elem);
    if (!deserialized->isValid())
        return false;

    copy(deserialized);
    return true;
}

//////////////////////////////////////////////////////////////////////////

bool IJavaXMLSerialize::saveToFile(const QString &filePath) const
{
    QDomDocument doc = CJavaXMLHelper::createDocument();
    QDomElement root = doc.firstChildElement();
    root.appendChild(toXMLNode(doc));
    return CJavaXMLHelper::saveDocumentToFile(doc, filePath);
}

bool IJavaXMLSerialize::loadFromFile(const QString &filePath)
{
    QDomDocument doc;
    if (!CJavaXMLHelper::loadDocumentFromFile(doc, filePath))
        return false;

    if (!CJavaXMLHelper::verifyDocument(doc))
        return false;

    return fromXMLNode(doc.firstChildElement().firstChildElement());
}