# scintillators
Programs used for the study of the efficiency of two LaBr_3(Cs) and CeBr_3 detectors through coincidence events (reconstructed by a program).
The pdf is in the 'advanced_lab' file.

Programs to calculate the coincidences between two scintillator detectors given a time threshold and the peak of interest in the energy spectrum NAMEFILE: 'coinc2channel.C'.
![Screenshot 2025-05-12 144845](https://github.com/user-attachments/assets/22049ef4-4f24-42dc-b668-e04da639795a)


Various methods to subtract the background: Fermi function, trapezoid, step function NAMEFILES: 'bkg_fermi2.C',  'bkg_step_media.C', 'bkg_trap2.C'.

Program to subtract the overall background from an energy spectrum NAMEFILE: 'subtract_bkg_2multich.C'. 

Program to subtract the background of a peak with the Fermi function method by varying at random the extrema of the peak NAMEFILE: 'bkg_fermiRAN.C'.

![Screenshot 2025-05-12 144822](https://github.com/user-attachments/assets/f50af035-219b-4212-adbe-b102dd8d96df)

Program to make a fit of the yield profile used to calibrate the AN2000 accelerator with an arctangent function NAMEFILE: 'grafici_fit.C'.

![Screenshot 2025-05-12 145216](https://github.com/user-attachments/assets/66868e77-ebd7-4b70-994d-6b1297223d09)
