package gpusim.config;

import java.io.Serializable;
import java.util.LinkedList;

public class GridSimConfig implements Serializable {
    //<editor-fold defaultstate="collapsed" desc="Constants">
    /**
     * Current target version of the GridSimConfig entity.
     */
    public static final int CURRENT_CONFIG_VERSION = 1;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Internal fields">
    private int _version;
    private double _linkBaudRate;
    private LinkedList<GridSimResourceConfig> _resources;
    private LinkedList<GridSimGridletConfig> _gridlets;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Constructors">
    public GridSimConfig() {
        _version = CURRENT_CONFIG_VERSION;
        _linkBaudRate = 1.0f;
        _resources = new LinkedList<GridSimResourceConfig>();
        _gridlets = new LinkedList<GridSimGridletConfig>();
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

    //<editor-fold defaultstate="collapsed" desc="LinkBaudRate property">
    public double getLinkBaudRate() {
        return _linkBaudRate;
    }

    public void setLinkBaudRate(double linkBaudRate) {
        if (linkBaudRate < 1.0f) {
            throw new AssertionError(linkBaudRate >= 1.0f);
        }

        _linkBaudRate = linkBaudRate;
    }
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="Resources property">
    public LinkedList<GridSimResourceConfig> getResources() {
        return _resources;
    }

    public void setResources(LinkedList<GridSimResourceConfig> resources) {
        if (resources == null) {
            throw new AssertionError(resources != null);
        }

        _resources = resources;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Gridlets property">
    public LinkedList<GridSimGridletConfig> getGridlets() {
        return _gridlets;
    }

    public void setGridlets(LinkedList<GridSimGridletConfig> gridlets) {
        if (gridlets == null) {
            throw new AssertionError(gridlets != null);
        }

        _gridlets = gridlets;
    }
    //</editor-fold>
    //</editor-fold>
}