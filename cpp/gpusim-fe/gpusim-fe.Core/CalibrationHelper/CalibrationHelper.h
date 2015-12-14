#pragma once

#include "../Experiment/Experiment.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CCalibrationHelper
    {
    public:
        // If calibration file path is not null opens it, reads input series and sets for simulations real calculation
        // time (CalcTime property). If simulation input does not exist in calibration file - such simulation is removed
        // from simulations list.
        // NOTE: Calibration file format:
        //         - In-line separator is "; " (without quotes);
        //         - 1-st line: <names separated by in-line separator>
        //         - Available type names (type = typeName):
        //           - qint32  = int
        //           - quint32 = uint
        //           - qint64  = qlonglong
        //           - quint64 = qulonglong
        //           - double  = double
        //         - 2-nd line: <type names separated by in-line separator>
        //         - every next line: <parameters separated by in-line separator><in-line separator><calculation time>
        //
        static void applyCalibrationFileFilter(CExperiment &experiment);

    private:
        CCalibrationHelper() { }
        ~CCalibrationHelper() { }
    };
}