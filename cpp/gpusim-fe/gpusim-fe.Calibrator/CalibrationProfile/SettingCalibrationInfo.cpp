#include "SettingCalibrationInfo.h"

#include "../../QLogger/QLog"

#include <QStringList>

//////////////////////////////////////////////////////////////////////////

static void checkVariants(const QVariant &left, const QVariant &right);
static bool operator > (const QVariant &left, const QVariant &right);
static bool operator <= (const QVariant &left, const QVariant &right);

static QVariant increment(const QVariant &currentValue, const QVariant &valueIncremet, CIncrementType it);
static QVariant incrementAdd(const QVariant &currentValue, const QVariant &valueIncremet);
static QVariant incrementMultiply(const QVariant &currentValue, const QVariant &valueIncremet);

//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and equality comparision support
CSettingCalibrationInfo::CSettingCalibrationInfo()
    : m_type(QVariant::Invalid), m_incrementType(IT_Invalid)
{

}

bool CSettingCalibrationInfo::operator==(const CSettingCalibrationInfo &other) const
{
    return
        (m_name           == other.m_name          ) &&
        (m_type           == other.m_type          ) &&
        (m_startValue     == other.m_startValue    ) &&
        (m_endValue       == other.m_endValue      ) &&
        (m_valueIncrement == other.m_valueIncrement) &&
        (m_incrementType  == other.m_incrementType );
}

bool CSettingCalibrationInfo::operator!=(const CSettingCalibrationInfo &other) const
{
    return !(*this == other);
}
#pragma endregion

#pragma region Properties
#pragma region Name property
QString CSettingCalibrationInfo::getName() const
{
    return m_name;
}

void CSettingCalibrationInfo::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}
#pragma endregion

#pragma region Type property
QVariant::Type CSettingCalibrationInfo::getType() const
{
    return m_type;
}

void CSettingCalibrationInfo::setType(QVariant::Type type)
{
    if (m_type == type)
        return;

    m_type = type;
}
#pragma endregion

#pragma region StartValue property
QVariant CSettingCalibrationInfo::getStartValue() const
{
    return m_startValue;
}

void CSettingCalibrationInfo::setStartValue(const QVariant& startValue)
{
    if (m_startValue == startValue)
        return;

    m_startValue = startValue;
}
#pragma endregion

#pragma region EndValue property
QVariant CSettingCalibrationInfo::getEndValue() const
{
    return m_endValue;
}

void CSettingCalibrationInfo::setEndValue(const QVariant& endValue)
{
    if (m_endValue == endValue)
        return;

    m_endValue = endValue;
}
#pragma endregion

#pragma region ValueIncrement property
QVariant CSettingCalibrationInfo::getValueIncrement() const
{
    return m_valueIncrement;
}

void CSettingCalibrationInfo::setValueIncrement(const QVariant& valueIncrement)
{
    if (m_valueIncrement == valueIncrement)
        return;

    m_valueIncrement = valueIncrement;
}
#pragma endregion

#pragma region IncrementType property
CIncrementType CSettingCalibrationInfo::getIncrementType() const
{
    return m_incrementType;
}

void CSettingCalibrationInfo::setIncrementType(CIncrementType incrementType)
{
    if (m_incrementType == incrementType)
        return;

    m_incrementType = incrementType;
}
#pragma endregion
#pragma endregion

#pragma region Validation
bool CSettingCalibrationInfo::isValid() const
{
    if (m_name.isEmpty())
        return false;
    if (m_type == QVariant::Invalid)
        return false;
    if (m_startValue.isNull())
        return false;
    if (m_endValue.isNull())
        return false;
    if (m_valueIncrement.isNull())
        return false;

    if (m_incrementType == IT_Invalid)
        return false;

    if (m_incrementType == IT_Constant)
        return true;

    return m_startValue <= m_endValue;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Loading from string
namespace Parse
{
    enum
    {
        c_nameFieldIndex = 0,
        c_typeFieldIndex,
        c_startValueFieldIndex,
        c_endValueFieldIndex,
        c_valueIncrementFieldIndex,
        c_incrementTypeFieldIndex,
        c_fieldsCount,
    };

    CIncrementType incrementTypeFromString(const QString &s)
    {
        QChar c = s.at(0).toUpper();
        if (c == 'A')
            return IT_Additive;
        else if (c == 'M')
            return IT_Multiplicative;
        else if (c == 'C')
            return IT_Constant;
        else
            return IT_Invalid;
    }
}

bool CSettingCalibrationInfo::fromString(const QString &s)
{
    auto fields = s.split("; ", QString::SkipEmptyParts);
    if (fields.size() != Parse::c_fieldsCount)
    {
        qLog_WarningMsg() << "Wrong fields count (" << fields.size() << "). Should be " << Parse::c_fieldsCount;
        return false;
    }

    auto type = QVariant::nameToType(fields[Parse::c_typeFieldIndex].toAscii().constData());
    if (type == QVariant::Invalid)
    {
        qLog_WarningMsg() << "Invalid type";
        return false;
    }

    QVariant startValue = fields[Parse::c_startValueFieldIndex];
    if (!startValue.convert(type))
    {
        qLog_WarningMsg() << "Cannot convert start value: " << fields[Parse::c_startValueFieldIndex];
        return false;
    }

    QVariant endValue = fields[Parse::c_endValueFieldIndex];
    if (!endValue.convert(type))
    {
        qLog_WarningMsg() << "Cannot convert end value: " << fields[Parse::c_endValueFieldIndex];
        return false;
    }

    QVariant valueIncrement = fields[Parse::c_valueIncrementFieldIndex];
    if (!valueIncrement.convert(type))
    {
        qLog_WarningMsg() << "Cannot convert value increment: " << fields[Parse::c_valueIncrementFieldIndex];
        return false;
    }

    auto it = Parse::incrementTypeFromString(fields[Parse::c_incrementTypeFieldIndex]);
    if (it == IT_Invalid)
    {
        qLog_WarningMsg() << "Cannot convert value increment: " << fields[Parse::c_incrementTypeFieldIndex];
        return false;
    }

    // If we have constant increment mode - only start value has meaning.
    //
    if (it == IT_Constant)
    {
        endValue = QVariant(0);
        endValue.convert(type);

        valueIncrement = QVariant(0);
        valueIncrement.convert(type);
    }

    setName(fields[Parse::c_nameFieldIndex]);
    setType(type);
    setStartValue(startValue);
    setEndValue(endValue);
    setValueIncrement(valueIncrement);
    setIncrementType(it);
    return isValid();
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

QVariantList CSettingCalibrationInfo::createVariations() const
{
    if (!isValid())
        return QVariantList();

    QVariantList variations;
    if (m_incrementType == IT_Constant)
    {
        variations.push_back(m_startValue);
        return variations;
    }

    for (auto currentValue = m_startValue; currentValue <= m_endValue; )
    {
        variations.push_back(currentValue);
        currentValue = increment(currentValue, m_valueIncrement, m_incrementType);
    }
    return variations;
}

//////////////////////////////////////////////////////////////////////////

#pragma region QVariant comparision andincrement support;
static void checkVariants(const QVariant &left, const QVariant &right)
{
    Q_ASSERT(left.isValid());
    Q_ASSERT(right.isValid());

    Q_ASSERT(left.type() == right.type());
}

static bool operator > (const QVariant &left, const QVariant &right)
{
    checkVariants(left, right);

    switch(left.type())
    {
    case QVariant::Int:
        return left.toInt() > right.toInt();
    case QVariant::UInt:
        return left.toUInt() > right.toUInt();
    case QVariant::LongLong:
        return left.toLongLong() > right.toLongLong();
    case QVariant::ULongLong:
        return left.toULongLong() > right.toULongLong();
    case QVariant::Double:
        return left.toDouble() > right.toDouble();

    default:
        qLog_FatalMsg() << "Wrong type";
        return false;
    }
}

static bool operator <= (const QVariant &left, const QVariant &right)
{
    return !(left > right);
}

static QVariant increment(const QVariant &currentValue, const QVariant &valueIncremet, CIncrementType it)
{
    checkVariants(currentValue, valueIncremet);

    switch(it)
    {
    case IT_Additive:
        return incrementAdd(currentValue, valueIncremet);
    case IT_Multiplicative:
        return incrementMultiply(currentValue, valueIncremet);

    default:
        qLog_FatalMsg() << "Wrong increment type";
        return QVariant();
    }
}

static QVariant incrementAdd(const QVariant &currentValue, const QVariant &valueIncremet)
{
    switch(currentValue.type())
    {
    case QVariant::Int:
        return QVariant::fromValue(currentValue.toInt() + valueIncremet.toInt());
    case QVariant::UInt:
        return QVariant::fromValue(currentValue.toUInt() + valueIncremet.toUInt());
    case QVariant::LongLong:
        return QVariant::fromValue(currentValue.toLongLong() + valueIncremet.toLongLong());
    case QVariant::ULongLong:
        return QVariant::fromValue(currentValue.toULongLong() + valueIncremet.toULongLong());
    case QVariant::Double:
        return QVariant::fromValue(currentValue.toDouble() + valueIncremet.toDouble());

    default:
        qLog_FatalMsg() << "Wrong type";
        return QVariant();
    }
}

static QVariant incrementMultiply(const QVariant &currentValue, const QVariant &valueIncremet)
{
    switch(currentValue.type())
    {
    case QVariant::Int:
        return QVariant::fromValue(currentValue.toInt() * valueIncremet.toInt());
    case QVariant::UInt:
        return QVariant::fromValue(currentValue.toUInt() * valueIncremet.toUInt());
    case QVariant::LongLong:
        return QVariant::fromValue(currentValue.toLongLong() * valueIncremet.toLongLong());
    case QVariant::ULongLong:
        return QVariant::fromValue(currentValue.toULongLong() * valueIncremet.toULongLong());
    case QVariant::Double:
        return QVariant::fromValue(currentValue.toDouble() * valueIncremet.toDouble());

    default:
        qLog_FatalMsg() << "Wrong type";
        return QVariant();
    }
}
#pragma endregion