import ctypes
import numpy as np
import math
# Load the shared library
lib = ctypes.CDLL('.\calci.dll')


# Define the C++ function's signature
lib.calcPossibleConfig.restype = ctypes.POINTER(ctypes.POINTER(ctypes.POINTER(ctypes.c_int)))

def call_generate_array(l, vElectron, limit):
    # Call the C++ function
    m = (2*l+1)
    nbConfiguration = math.factorial(m*2)/(math.factorial(vElectron)*math.factorial(m*2-vElectron))
    c_array = lib.calcPossibleConfig(l, vElectron, limit)

    # Convert the C array to a NumPy array
    np_array = np.empty((nbConfiguration, m, 2), dtype=np.int32)
    for i in range(nbConfiguration):
        for j in range(m):
            for k in range(2):
                np_array[i, j, k] = c_array[i][j][k]


    lib.deleteArray(c_array, l, vElectron)
    return np_array

# Test the function
l = 1
vElectron = 2
limit = 1000
result = call_generate_array(l, vElectron,limit)
print(result)
