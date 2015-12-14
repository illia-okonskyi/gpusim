package gpusim.runtime;

import gpusim.config.*;
import gridsim.GridResource;
import gridsim.GridSim;
import gridsim.Machine;
import gridsim.MachineList;
import gridsim.ResourceCalendar;
import gridsim.ResourceCharacteristics;
import java.beans.XMLDecoder;
import java.beans.XMLEncoder;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.Calendar;
import java.util.LinkedList;

public class GridSimRunTime {
    private static final double RESOURCES_PEAK_LOAD = 0.0;
    private static final double RESOURCES_OFFPEAK_LOAD = 0.0;
    private static final double RESOURCES_HOLIDAY_LOAD = 0.0;
    private static final long RESOURCES_CALENDAR_SEED = 11L*13*17*19*23+1;

    private static final LinkedList<Integer> RESOURCES_CALENDAR_HOLIDAYS;
    private static final LinkedList<Integer> RESOURCES_CALENDAR_WEEKENDS;
    static {
        // All resources will work all days in month work:
        //
        RESOURCES_CALENDAR_HOLIDAYS = new LinkedList<Integer>();
        RESOURCES_CALENDAR_WEEKENDS = new LinkedList<Integer>();
        RESOURCES_CALENDAR_WEEKENDS.add(new Integer(Calendar.SATURDAY));
        RESOURCES_CALENDAR_WEEKENDS.add(new Integer(Calendar.SUNDAY));
    }

    private GridSimConfig _config;
    private GridSimOutput _output;
    
    private GridletsContainer _gridletsContainer;
    private GpuSimUser _gpuSimUser;

    //<editor-fold defaultstate="collapsed" desc="Singletone">
    private static GridSimRunTime _instance;
    public static GridSimRunTime getInstance() {
        if (_instance == null) {
            _instance = new GridSimRunTime();
        }
        
        return _instance;
    }
    //</editor-fold>
    
    private GridSimRunTime() {
        _config = new GridSimConfig();
        _output = new GridSimOutput();
    }
        
    public GridSimOutput getOutput() {
        return _output;
    }

    public GridletsContainer getGridletsContainer() {
        return _gridletsContainer;
    }

    //<editor-fold defaultstate="collapsed" desc="Configuration loading and output saving">
    public void loadConfig(String filePath) throws FileNotFoundException, IncopatibleVersionException {
        printRuntimeMessage("Loading configuration from file: " + filePath);

        GridSimConfig config;
        
        FileInputStream in = new FileInputStream(filePath);
        XMLDecoder xmlDecoder = new XMLDecoder(in);
        config = (GridSimConfig) xmlDecoder.readObject();
        xmlDecoder.close();
        
        if (config.getVersion() != GridSimConfig.CURRENT_CONFIG_VERSION) {
            throw new IncopatibleVersionException(GridSimConfig.CURRENT_CONFIG_VERSION, config.getVersion());
        }
        
        _config = config;

        printRuntimeMessage("Configuration loaded");
    }
    
    public void saveOutput(String filePath) throws FileNotFoundException {
        printRuntimeMessage("Saving output to file: " + filePath);

        FileOutputStream out = new FileOutputStream(filePath);
        XMLEncoder xmlEncoder = new XMLEncoder(out);
        xmlEncoder.writeObject(_output);
        xmlEncoder.flush();
        xmlEncoder.close();

        printRuntimeMessage("Output saved");
    }
    
    public static void createTestConfig(String filePath) throws FileNotFoundException {
        printRuntimeMessage("Creating and saving test configuration to file: " + filePath);
        
        //
        // Machines list
        //
        GridSimMachineConfig machine = new GridSimMachineConfig();
        machine.setPECount(6);
        machine.setPERating(3);
        machine.setCount(5);

        LinkedList<GridSimMachineConfig> machines = new LinkedList<GridSimMachineConfig>();
        machines.add(machine);
        
        //
        // Resources list
        //
        GridSimResourceConfig resource = new GridSimResourceConfig();
        resource.setArch("Test Architecture");
        resource.setOS("Test OS");
        resource.setBaudRate(100.0f);
        resource.setCostPerSec(55.0f);
        resource.setMachines(machines);
        resource.setCount(2);

        LinkedList<GridSimResourceConfig> resources = new LinkedList<GridSimResourceConfig>();
        resources.add(resource);
        
        //
        // Gridlets list
        //
        GridSimGridletConfig gridlet = new GridSimGridletConfig();
        gridlet.setLength(100.0f);
        gridlet.setInputSize(50);
        gridlet.setOutputSize(30);
        gridlet.setCount(10);

        LinkedList<GridSimGridletConfig> gridlets = new LinkedList<GridSimGridletConfig>();
        gridlets.add(gridlet);

        
        // Total configuration
        //
        GridSimConfig config = new GridSimConfig();
        config.setLinkBaudRate(560.0f);
        config.setResources(resources);
        config.setGridlets(gridlets);
        
        // Save configuration to passed file path
        //
        FileOutputStream out = new FileOutputStream(filePath);
        XMLEncoder xmlEncoder = new XMLEncoder(out);
        xmlEncoder.writeObject(config);
        xmlEncoder.flush();
        xmlEncoder.close();
        
        printRuntimeMessage("Test configuration created");
    }
    //</editor-fold>
    
    public void initGridSim() {
        // We have only one GridSim-derived entity, so only one user.
        int num_user = 1;

        Calendar calendar = Calendar.getInstance();
        
        // Mean trace GridSim events/activities
        boolean trace_flag = false;

        // List of files or processing names to be excluded from any statistical measures
        //
        String[] exclude_from_file = {""};
        String[] exclude_from_processing = {""};

        // The name of a report file to be written.
        String report_name = null;

        // Initialize the GridSim package
        //
        printRuntimeMessage("Initializing GridSim package");
        GridSim.init(num_user, calendar, trace_flag, exclude_from_file,
                exclude_from_processing, report_name);
        printRuntimeMessage("Initializing GridSim package initialized");
    }

    public void createResources() throws GridSimRunTimeException {
        printRuntimeMessage("Creating resouces");
        
        int currentResourceID = 0;
        for (GridSimResourceConfig resConfig: _config.getResources()) {
            MachineList machines = new MachineList();
            int currentMachineID = 0;
            for (GridSimMachineConfig mc: resConfig.getMachines()) {
                machines.add(new Machine(currentMachineID, mc.getPECount(), mc.getPERating()));
                ++currentMachineID;
            }
            
            ResourceCharacteristics rc = new ResourceCharacteristics(resConfig.getArch(), resConfig.getOS(),
                    machines, resConfig.getAllocPolicy(), resConfig.getTimeZone(), resConfig.getCostPerSec());
            
            ResourceCalendar calendar = new ResourceCalendar(resConfig.getTimeZone(), RESOURCES_PEAK_LOAD,
                    RESOURCES_OFFPEAK_LOAD, RESOURCES_HOLIDAY_LOAD, RESOURCES_CALENDAR_WEEKENDS,
                    RESOURCES_CALENDAR_HOLIDAYS, RESOURCES_CALENDAR_SEED);

            for (long i = 0; i < resConfig.getCount(); ++i) {
                GridResource resource;
                String resName = String.format("Resource_%1$d", currentResourceID);

                try {
                    printRuntimeMessage("Creating resource " + resName);
                    resource = new GridResource(resName, resConfig.getBaudRate(), rc, calendar);
                }
                catch (Exception e) {
                    String desc = "Failed to create resource " + resName + ": " + e.getMessage();
                    printRuntimeMessage(desc);
                    throw new GridSimRunTimeException(desc);
                }
                
                ++currentResourceID;
            }
        }
        
        printRuntimeMessage("Resouces created");
    }

    public void createGridletsContainer()
    {
        printRuntimeMessage("Creating Gridlets Container");

        _gridletsContainer = new GridletsContainer(_config.getGridlets());

        printRuntimeMessage("Gridlets Container created");
    }

    public void createUser() throws Exception {
        printRuntimeMessage("Creating user");

        _gpuSimUser = new GpuSimUser(_config.getLinkBaudRate(),
                _gridletsContainer);

        printRuntimeMessage("User created");
    }
    
    private static void printRuntimeMessage(String message) {
        System.out.println("[GridSimRuntime] " + message);
    }
}