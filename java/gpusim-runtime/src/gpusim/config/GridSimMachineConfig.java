package gpusim.config;

import java.io.Serializable;

public class GridSimMachineConfig implements Serializable {
    //<editor-fold defaultstate="collapsed" desc="Internal fields">
    private int _peCount;
    private int _peRating;
    private int _count;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Constructors">
    public GridSimMachineConfig() {
        _peCount = 1;
        _peRating = 1;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Properties">
    //<editor-fold defaultstate="collapsed" desc="PECount property">
    public int getPECount() {
        return _peCount;
    }
    
    public void setPECount(int peCount) {
        if (peCount < 1) {
            throw new AssertionError(peCount >= 1);
        }
        
        _peCount = peCount;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="PERating property">
    public int getPERating() {
        return _peRating;
    }
    
    public void setPERating(int peRating) {
        if (peRating < 1) {
            throw new AssertionError(peRating >= 1);
        }
        
        _peRating = peRating;
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