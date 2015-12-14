package gpusim.runtime;

import gpusim.config.IncopatibleVersionException;
import gridsim.GridSim;
import java.io.FileNotFoundException;

// JVM parameters -Xms1024m -Xmx1024m -Xss20m

public class EntryPoint {
    private static String _configFilePath;
    private static String _outputFilePath;

    public static String getConfigFilePath() {
        return _configFilePath;
    }

    public static String getOutputFilePath() {
        return _outputFilePath;
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Run format: app <GridSimConfigFileName> <GridSimOutputFileName>");
            System.exit(-1);
        }

        if (args[0].equals("--test")) {
            createTestConfig(args[1]);
            System.exit(0);
        }

        _configFilePath = args[0];
        _outputFilePath = args[1];

        try {
            GridSimRunTime runtime = GridSimRunTime.getInstance();
            runtime.loadConfig(_configFilePath);
            runtime.initGridSim();
            runtime.createResources();
            runtime.createGridletsContainer();
            runtime.createUser();
            GridSim.startGridSimulation();
            runtime.saveOutput(_outputFilePath);
            System.exit(0);
        } catch (FileNotFoundException ex) {
            System.out.println("Configuration or ouput file not found.");
            System.out.println(ex);
            System.exit(-2);
        } catch (IncopatibleVersionException ex) {
            System.out.println("Configuration file has wrong version.");
            System.out.println(ex);
            System.exit(-3);
        } catch (GridSimRunTimeException ex) {
            System.out.println("GridSimRuntime exception:");
            System.out.println(ex);
            System.exit(-4);
        } catch (Exception ex) {
            System.out.println("Exception:");
            System.out.println(ex);
            System.exit(-5);
        }
    }

    private static void createTestConfig(String testConfigFilePath) {
        try {
            GridSimRunTime.createTestConfig(testConfigFilePath);
        } catch (FileNotFoundException ex) {
            System.out.println("Output file for test configuration not found.");
            System.out.println(ex);
            System.exit(-2);
        }
    }
}