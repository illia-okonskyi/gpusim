#pragma once

#include "IJavaXMLSerialize.h"
#include "JavaXMLHelper.h"

#include <type_traits>

namespace Core
{
    namespace GridSimConfig
    {
        namespace Serialization
        {
            // TOutIter must be output iterator for type derived from IJavaXMLSerialize
            //
            template <class TOutIter>
            bool getListPropertyValue(const QDomElement &elem, const QString &propName, TOutIter dest)
            {
                static_assert(std::tr1::is_base_of<std::_Outit, TOutIter>::value, "TOutIter must be output iterator");
                static_assert(std::tr1::is_base_of<IJavaXMLSerialize, TOutIter::_Valty>::value,
                    "Value-type must be derived from IJavaXMLSerialize");

                QDomElement listPropRoot;
                if (!CJavaXMLHelper::getPropertyElement(elem, propName, listPropRoot))
                    return false;

                for (QDomNode n = listPropRoot.firstChild(); !n.isNull(); n = n.nextSibling())
                {
                    QDomElement e = n.toElement();
                    if (e.isNull())
                        continue;

                    TOutIter::_Valty o;
                    if (!o.fromXMLNode(e, true))
                        continue;

                    *dest = o;
                    ++dest;
                }

                return true;
            }

            // TForwardIter must be forward (bidirectional, random access) iterator for type derived from
            // IJavaXMLSerialize
            //
            template <class TForwardIter>
            QDomElement createListPropertyElement(QDomDocument &doc, const QString &propName,
                TForwardIter first, TForwardIter last)
            {
                QDomElement propRoot = CJavaXMLHelper::createPropertyElement(doc, propName);
                for (; first != last; ++first) 
                    propRoot.appendChild(first->toXMLNode(doc, true));

                return propRoot;
            }
        }
    }
}