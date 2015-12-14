package gpusim.runtime;

import gpusim.config.GridSimGridletConfig;
import java.util.LinkedList;
import gridsim.Gridlet;

public final class GridletsContainer {
    private LinkedList<GridSimGridletConfig> _configs;
    private java.util.ListIterator<GridSimGridletConfig> _itCurrentGC;
    private GridSimGridletConfig _currentConfig;
    private int _currentConfigGridletsCount;
    private int _currentGridletID;

    public GridletsContainer()
    {
        reset();
    }

    public GridletsContainer(LinkedList<GridSimGridletConfig> configs)
    {
        setConfigs(configs);
    }

    public void setConfigs(LinkedList<GridSimGridletConfig> configs)
    {
        reset();

        _configs = configs;
        if (_configs == null)
            return;

        _itCurrentGC = _configs.listIterator();
        _currentConfig = _itCurrentGC.next();
    }

    public Gridlet getNextGridlet(int userID) throws NoMoreGridletsException
    {
        if ((_configs == null) || (_currentConfig == null))
            throw new NoMoreGridletsException();


        if (_currentConfigGridletsCount >= _currentConfig.getCount())
        {
            if (!_itCurrentGC.hasNext())
                throw new NoMoreGridletsException();

            _currentConfig = _itCurrentGC.next();
            _currentConfigGridletsCount = 0;
        }

        Gridlet g = new Gridlet(_currentGridletID, _currentConfig.getLength(),
                _currentConfig.getInputSize(), _currentConfig.getOutputSize());
        g.setUserID(userID);

        ++_currentGridletID;
        ++_currentConfigGridletsCount;
        return g;
    }

    private void reset()
    {
        _itCurrentGC = null;
        _currentConfigGridletsCount = 0;
        _currentGridletID = 0;
        _currentConfig = null;
    }
}