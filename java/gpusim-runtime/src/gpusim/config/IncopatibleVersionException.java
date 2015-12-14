package gpusim.config;

public class IncopatibleVersionException extends RuntimeException {
    private int _currentVersion;
    private int _targetVersion;

    public IncopatibleVersionException(int currentVersion, int targetVersion) {
        super(String.format("Incopatible version (current supported version: %1$d; target version: %2$d)",
                currentVersion, targetVersion));

        _currentVersion = currentVersion;
        _targetVersion = targetVersion;
    }

    public int getCurrentVersion() {
        return _currentVersion;
    }

    public int getTargetVersion() {
        return _targetVersion;
    }
}