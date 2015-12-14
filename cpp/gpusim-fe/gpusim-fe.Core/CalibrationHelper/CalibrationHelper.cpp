#include "CalibrationHelper.h"
#include "../QLogger/QLog"

#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace Core;

//////////////////////////////////////////////////////////////////////////

typedef QHash<CGeneratorInput, double> CCalibrationDataSet;
static const QString c_calibrationFileSeparator = QString("; ");
static CCalibrationDataSet readCalibrationFile(const QString &filePath);

//////////////////////////////////////////////////////////////////////////

void CCalibrationHelper::applyCalibrationFileFilter(CExperiment &experiment)
{
    if (experiment.getCalibrationFilePath().isEmpty())
        return;

    auto calibrationDataSet = readCalibrationFile(experiment.getCalibrationFilePath());
    if (calibrationDataSet.isEmpty())
        return;

    CSimulationsList filtererSimulations;
    auto I = experiment.getSimulationsRef().constBegin();
    auto E = experiment.getSimulationsRef().constEnd();
    for (; I != E; ++I)
    {
        if (!calibrationDataSet.contains(I->getGeneratorInput()))
            continue;
        auto sim = *I;
        sim.setCalcTime(calibrationDataSet.value(I->getGeneratorInput()));
        filtererSimulations.push_back(sim);
    }

    experiment.setSimulations(filtererSimulations);
}

//////////////////////////////////////////////////////////////////////////

static CCalibrationDataSet readCalibrationFile(const QString &filePath)
{
    // 1) Try to open file
    //
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qLog_WarningMsg() << "Failed to open calibration file: " << filePath;
        return CCalibrationDataSet();
    }
    QTextStream stream(&file);

    // 2) Read first line with names
    //
    QStringList names = stream.readLine().split(c_calibrationFileSeparator, QString::SkipEmptyParts);
    if (names.isEmpty())
    {
        qLog_WarningMsg() << "Failed to obtain names for generator input. Calibration file:" << filePath;
        return CCalibrationDataSet();
    }

    // 3) Read second line with type names. Try to get type from type name and if success - append to types list
    //
    QVector<QVariant::Type> types;
    foreach(QString typeName, stream.readLine().split(c_calibrationFileSeparator, QString::SkipEmptyParts))
    {
        QVariant::Type t = QVariant::nameToType(typeName.toAscii().constData());
        if (t == QVariant::Invalid)
        {
            qLog_WarningMsg() << "Wrong type name: " << typeName;
            return CCalibrationDataSet();
        }
        types.push_back(t);
    }

    // 4) If all correct - we types and names sizes should be equal.
    //
    if (types.size() != names.size())
    {
        qLog_WarningMsg() << "Names and types cound are different. Names: " << names.size() <<
            "; Types: " << types.size();
        return CCalibrationDataSet();
    }

    //
    // 5) Read all next lines with generator input values and calculation times.
    // 
    CCalibrationDataSet dataSet;
    while(true)
    {
        // Should be one more value for calculation time
        // 
        QStringList values = stream.readLine().split(c_calibrationFileSeparator, QString::SkipEmptyParts);
        if (values.isEmpty())
            break;

        if (values.size() != names.size() + 1)
            continue;

        CGeneratorInput input;
        for (int i = 0; i < names.size(); ++i)
        {
            QVariant v(values[i]);
            if (!v.convert(types[i]))
            {
                qLog_WarningMsg() << "Cannot create generator input (skipped):";
                qLog_WarningMsg() << "  Name:  " << names[i] << ";";
                qLog_WarningMsg() << "  Type:  " << QVariant::typeToName(types[i]) << ";";
                qLog_WarningMsg() << "  Value: " << values[i] << ".";
                
                input.clear();
                break;
            }

            input[names[i]] = v;
        }

        // The last value is the calculation time
        //
        if (!input.isEmpty())
            dataSet[input] = values[names.size()].toDouble();

    }

    return dataSet;
}