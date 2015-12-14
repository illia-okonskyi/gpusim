#pragma once

#include "../../gpusim-fe.Core/IGenerator/GeneratorSettings.h"

typedef enum _tagCIncrementType
{
    IT_Invalid,
    IT_Additive,
    IT_Multiplicative,
    IT_Constant
} CIncrementType;

class CSettingCalibrationInfo
{
public:
    CSettingCalibrationInfo();

    QString getName() const;
    void setName(const QString &name);

    QVariant::Type getType() const;
    void setType(QVariant::Type type);

    QVariant getStartValue() const;
    void setStartValue(const QVariant& startValue);

    QVariant getEndValue() const;
    void setEndValue(const QVariant& endValue);

    QVariant getValueIncrement() const;
    void setValueIncrement(const QVariant& valueIncrement);

    CIncrementType getIncrementType() const;
    void setIncrementType(CIncrementType incrementType);

    bool isValid() const;

    bool operator == (const CSettingCalibrationInfo &other) const;
    bool operator != (const CSettingCalibrationInfo &other) const;

    // Parses string and loads fields from it. Returns true if string was parsed successfully and false otherwise (
    // details are written to log).
    // 
    // String should consist of fields divided by "; " (without quotes).
    // Fields:
    //   - Name: setting name;
    //   - TypeName: setting type name, QVarint typenames should be used;
    //   - StartValue: should have type specified by TypeName field;
    //   - EndValue: should have type specified by TypeName field;
    //   - ValueIncrement: should have type specified by TypeName field;
    //   - IncrementType: "A" (without quotes) stands for IT_Additive and "M" (without quotes) stands for
    //                    IT_Multiplicative and "C" (without quotes) stands for IT_Constant;
    // 
    // NOTE: Constant increment type means that setting value should be setted to the value specified in StartValue
    //       field.
    //
    bool fromString(const QString &s);

    QVariantList createVariations() const;

private:
    QString m_name;
    QVariant::Type m_type;
    QVariant m_startValue;
    QVariant m_endValue;
    QVariant m_valueIncrement;
    CIncrementType m_incrementType;
};