#pragma once

#include "SettingCalibrationInfo.h"
#include <QVector>

// NOTE: Calibration profile.
// Calibration profile - is the file with serialized to string CSettingCalibrationInfo sets. Each
// CSettingCalibrationInfo must be represented by separate line. Empty and commented with "#" lines are skipped during
// file parsing.
// Specifying more than one CSettingCalibrationInfo in calibration profile means recursively settings variation process
// (brute-force, .
// For example if You specify next profile:
//   SettingName1; int; 1; 2; 1; A
//   SettingName2; int; 7; 0; 0; C
//   SettingName3; int; 4; 8; 2; M
// next settings variations will be created:
//   1) SettingName1 = 1;
//      SettingName2 = 7;
//      SettingName3 = 4;
//
//   2) SettingName1 = 1;
//      SettingName2 = 7;
//      SettingName3 = 4;
//
//   3) SettingName1 = 2;
//      SettingName2 = 7;
//      SettingName3 = 4;
//
//   4) SettingName1 = 2;
//      SettingName2 = 7;
//      SettingName3 = 4;
//   
//
typedef QVector<CSettingCalibrationInfo> CCalibrationProfile;
bool readCalibrationProfileFromFile(const QString &filePath, CCalibrationProfile &profile);

typedef QVector<Core::CGeneratorSettings> CGeneratorSettingsSet;
CGeneratorSettingsSet createGeneratorSettingsSet(const CCalibrationProfile &profile,
    const Core::CGeneratorSettings &defaultSettings);