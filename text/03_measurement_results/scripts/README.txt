Matevz Cerv
Date: 2015-07-30

Scripts for reading and analysing TCT pulses acquired along a temperature renge between 4 K and 295 K at various bias voltages

grabpulses
------------------------
The script looks through the raw data root files and fetches the pulse profiles, saving them into one single file: plots/pulses.root


decay (partially obsolete)
-----------------------
Fetches the pulse profiles and saves them into one single file. Makes graphs out of profiles. Fits a decay and produces plots wrt temperature and voltage.


decaycompare
----------------------
Decay fitting
Decay calculation based on area
Comparison of the two decay Tau derivations.

plotgraphs
---------------------
saves the plots! needs folders plots/pulses, plots/pulsesVolt.
Plots:
for every sample and every temp and every polarity plot all voltages
for every irradiated sample and every temp and every polarity and every voltage plot before-after.
for every sample and voltage=500 and every polarity plot all temperatures

colours
--------------------
the same as plotgraphs, but with different colour palette




OTHER

colours
influenza, Deep_Skyblues, The First Raindrop, Nobody ♥ Sugar
