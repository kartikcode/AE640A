# AE640A: GPS Module


## Files
`prnSubroutine.mlx` : Contains functions and implementations for first four questions. 
This includes:
- Function for creating PRN chip code for any specific satellite number
-  Function for calculating the circular autocorrelation of a specific satellite with its delayed PRN code or cross correlation.
- Function for calculating thecircular autocorrelation of a specific satellite with its noisy delayed PRN code.

`parallelPhaseCode.mlx`: Contains implementation of parallel code phase search acquisition algorithm to identify the satellites (PRN IDs), carrier frequency and code phase.


**NOTE:** Download the data file from [here](https://sdr.ion.org/api-sample-data.html). 
File name: SiGe_Bands-L1.dat
Make sure to retain the filename and add the file to the working directory of MATLAB. 
