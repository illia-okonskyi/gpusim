package gpusim.runtime;

import gridsim.GridSim;
import gridsim.Gridlet;
import java.util.LinkedList;

public class GpuSimUser extends GridSim {
    public static final String GPUSIM_USER_NAME = "GpuSimUser";

    private int _gpuSimUserID;
    private LinkedList<Integer> _resourcesIDs;
    private GridletsContainer _gridlets;

    public GpuSimUser(double linkBaudRate, GridletsContainer gridlets) throws Exception {
        super(GPUSIM_USER_NAME, linkBaudRate);
        
        _gpuSimUserID = GridSim.getEntityId(GPUSIM_USER_NAME);
        _gridlets = gridlets;
    }

    public int getGpuSimUserID() {
        return _gpuSimUserID;
    }

    public LinkedList<Integer> getResourcesIDs() throws Exception {
        if (_resourcesIDs == null) {
            throw new Exception("Resources aren't avaliable");
        }

        return _resourcesIDs;
    }

    private void loadResourcesIDs() {
        LinkedList resList;
        _resourcesIDs = new LinkedList<Integer>();

        // waiting to get list of resources. Since GridSim package uses multi-threaded environment, your request might
        // arrive earlier before one or more grid resource entities manage to register themselves to
        // GridInformationService (GIS) entity. Therefore, it's better to wait in the first place
        //
        while (true) {
            // need to pause for a while to wait GridResources finish registering to GIS. Hold by 1 second.
            //
            super.gridSimHold(1.0);

            resList = GridSim.getGridResourceList();
            if (resList.size() == 0) {
                System.out.println("GpuSimUser: Waiting to get list of resources ...");
                continue;
            }

            for (Object val: resList) {
                _resourcesIDs.add((Integer)val);
            }
            break;
        }
    }


    @Override
    public void body() {
        loadResourcesIDs();

        while (true)
        {
            printRuntimeMessage("Submitting next gridlets chunk...");
            int submittedCount = submitGridletsChunk();
            printRuntimeMessage(String.format("...%1$d gridlets submitted",
                    submittedCount));

            if (submittedCount == 0)
                break;

            printRuntimeMessage(String.format("Receiving %1$d gridlets...",
                    submittedCount));
            for (int i = 0; i < submittedCount; ++i){
                super.gridletReceive();

                // NOTE: Here we can write statistics for received gridlet
            }
            printRuntimeMessage("Received...");
        }

        super.shutdownGridStatisticsEntity();
        super.shutdownUserEntity();
        super.terminateIOEntities();

        GridSimRunTime.getInstance().getOutput().setTotalSimulationTime(GridSim.clock());
    }

    // Returns count of submitted gridlets. Zero count means no gridlets were
    // submitted.
    //
    private int submitGridletsChunk()
    {
        int submittedCount = 0;

        try {
            for (Integer resourceID: _resourcesIDs) {
                int freePECount = super.getNumFreePE(resourceID);
                for (int i = 0; i < freePECount; ++i) {
                    Gridlet g = _gridlets.getNextGridlet(_gpuSimUserID);
                    super.gridletSubmit(g, resourceID);
                    ++submittedCount;
                }
            }
        } catch (NoMoreGridletsException ex) {
            System.out.println(ex.getMessage());
        }

        return submittedCount;
    }

    private static void printRuntimeMessage(String message) {
        System.out.println("[GpuSimUser] " + message);
    }
}