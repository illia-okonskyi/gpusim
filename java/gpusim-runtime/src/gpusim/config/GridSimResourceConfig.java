package gpusim.config;

import java.io.Serializable;
import java.util.LinkedList;

public class GridSimResourceConfig implements Serializable {
    //<editor-fold defaultstate="collapsed" desc="Internal fields">
    private String _arch;
    private String _os;
    private double _costPerSec;
    private double _timeZone;
    private int _allocPolicy;
    private double _baudRate;
    private LinkedList<GridSimMachineConfig> _machines;
    private int _count;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Constructors">
    public GridSimResourceConfig() {
        _arch = "Unnamed Architecture";
        _os = "Unnamed OS";
        _costPerSec = 0.0f;
        _timeZone = 0.0f;
        _allocPolicy = 0;
        _baudRate = 1.0f;
        _machines = new LinkedList<GridSimMachineConfig>();
        _count = 1;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Properties">
    //<editor-fold defaultstate="collapsed" desc="Arch property">
    public String getArch() {
        return _arch;
    }
    
    public void setArch(String arch) {
        if (arch == null) {
            throw new AssertionError(arch != null);
        }
        
        _arch = arch;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="OS property">
    public String getOS() {
        return _os;
    }
    
    public void setOS(String os) {
        if (os == null) {
            throw new AssertionError(os != null);
        }
        
        _os = os;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="CostPerSec property">
    public double getCostPerSec() {
        return _costPerSec;
    }
    
    public void setCostPerSec(double costPerSec) {
        if (costPerSec < 0) {
            throw new AssertionError(costPerSec >= 0);
        }
        
        _costPerSec = costPerSec;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="TimeZone property">
    public double getTimeZone() {
        return _timeZone;
    }
    
    public void setTimeZone(double timeZone) {
        if ((timeZone < -12) || (timeZone > 13)) {
            throw new AssertionError((timeZone >= -12) && (timeZone <= 13));
        }
        
        _timeZone = timeZone;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="AllocPolicy property">
    // NOTE: Allocation policy constants (in gridsim.ResourceCharacteristics):
    //       public static final int TIME_SHARED = 0;
    //       public static final int SPACE_SHARED = 1;
    public int getAllocPolicy() {
        return _allocPolicy;
    }
    
    public void setAllocPolicy(int allocPolicy) {
        if ((allocPolicy < 0) || (allocPolicy > 1)) {
            throw new AssertionError((allocPolicy >= 0) && (allocPolicy <= 1));
        }
        
        _allocPolicy = allocPolicy;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="BaudRate property">
    public double getBaudRate() {
        return _baudRate;
    }
    
    public void setBaudRate(double baudRate) {
        if (baudRate <= 0) {
            throw new AssertionError(baudRate > 0);
        }
        
        _baudRate = baudRate;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Machines property">
    public LinkedList<GridSimMachineConfig> getMachines() {
        return _machines;
    }
    
    public void setMachines(LinkedList<GridSimMachineConfig> machines) {
        if (machines == null) {
            throw new AssertionError(machines != null);
        }
        
        _machines = machines;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Count property">
    public int getCount() {
        return _count;
    }

    public void setCount(int count) {
        if (count < 1) {
            throw new AssertionError(count >= 1);
        }

        _count = count;
    }
    //</editor-fold>
    //</editor-fold>
}