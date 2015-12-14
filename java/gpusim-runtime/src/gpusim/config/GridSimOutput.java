package gpusim.config;

import java.io.Serializable;

public class GridSimOutput implements Serializable {
    //<editor-fold defaultstate="collapsed" desc="Constants">
    /**
     * Current target version of the GridSimOutput entity.
     */
    public static final int CURRENT_OUTPUT_VERSION = 1;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Internal fields">
    private int _version;
    private double _totalSimulationTime;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Constructors">
    public GridSimOutput() {
        _version = CURRENT_OUTPUT_VERSION;
        _totalSimulationTime = 0.0f;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Properties">
    //<editor-fold defaultstate="collapsed" desc="Version property">
    public int getVersion() {
        return _version;
    }

    public void setVersion(int version) {
        if (version <= 0) {
            throw new AssertionError(version > 0);
        }

        _version = version;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="TotalSimulationTime property">
    public double getTotalSimulationTime() {
        return _totalSimulationTime;
    }

    public void setTotalSimulationTime(double totalSimulationTime) {
        if (totalSimulationTime < 0.0f) {
            throw new AssertionError(totalSimulationTime >= 0.0f);
        }

        _totalSimulationTime = totalSimulationTime;
    }
    //</editor-fold>
    //</editor-fold>
}