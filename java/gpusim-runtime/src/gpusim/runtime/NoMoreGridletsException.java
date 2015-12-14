package gpusim.runtime;

public class NoMoreGridletsException extends GridSimRunTimeException {
    public NoMoreGridletsException() {
        super("No more gridlets for current configs");
    }
}