"""
effectiveMatPropRVE.py

A repository of user defined functions to be compiled as .pyc.

"""

# Import the functions.
import dataHandling
import dataHandlingInPlane

# Copy the execfile() order on the Abaqus Command Line Interface (CLI)
# or use the "Run script" option from the File menu.

# execfile('effectiveMatPropRVE.py')
# import dataHandling; reload(dataHandling); execfile('effectiveMatPropRVE.py')


# CHANGE THE MODEL NAME HERE.
# DO NOT ADD THE '.ODB' EXTENSION.
# This is added automatically by the script when needed.

modelName = 'VoxelContinuumTest'
# Set the thermo-mechanical switch to 1 (CTE evaluation) or 0 (no CTE evaluation)
TM=1
# Set SingleLayerRVE switch to True for single layer voxel export and staggered boundary conditions
# False for all others (single layer will only calculate in-plane properties)
SingleLayerRVE = False
# Perform the operations.
if ( SingleLayerRVE):
    dataHandlingInPlane.mainScript(modelName, TM)
else:
    dataHandling.mainScript(modelName, TM)