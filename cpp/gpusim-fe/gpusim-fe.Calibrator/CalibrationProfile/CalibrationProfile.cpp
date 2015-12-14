#include "CalibrationProfile.h"

#include "../../QLogger/QLog"

#include <QFile>
#include <QTextStream>

bool readCalibrationProfileFromFile(const QString &filePath, CCalibrationProfile &profile)
{
    profile.clear();

    // 1) Try to open file
    //
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qLog_WarningMsg() << "Failed to open calibration profile file: " << filePath;
        return false;
    }
    
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        auto line = stream.readLine().trimmed();
        
        // Skip empty lines and comments
        //
        if (line.isEmpty() || (line.at(0) == '#'))
            continue;

        CSettingCalibrationInfo info;
        if (info.fromString(line))
            profile.push_back(info);
    }

    qLog_DebugMsg() << profile.size() << " entries have been read.";
    return (!profile.isEmpty());
}

//////////////////////////////////////////////////////////////////////////

CGeneratorSettingsSet createGeneratorSettingsSet(const CCalibrationProfile &profile,
    const Core::CGeneratorSettings &defaultSettings)
{
    CGeneratorSettingsSet currentSettingsSet;
    currentSettingsSet.push_back(defaultSettings);

    for (int i = profile.size() - 1; i >= 0; --i)
    {
        CGeneratorSettingsSet newSettingsSet;
        foreach(auto settingValue, profile.at(i).createVariations())
        {
            foreach(Core::CGeneratorSettings generatorSettings, currentSettingsSet)
            {
                generatorSettings[profile.at(i).getName()] = settingValue;
                newSettingsSet.push_back(generatorSettings);
            }
        }

        currentSettingsSet = newSettingsSet;
    }

    return currentSettingsSet;
}