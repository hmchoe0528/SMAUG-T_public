## Security and Failure probability estimator 

Security and Failure probability estimator for Smaug parameter sets 128, 192 and 256. 

The code is modified from lattice estimator (https://github.com/malb/lattice-estimator) and Lizard error estimator (https://github.com/swanhong/LizardError).  

## Run

Run Smaug_estimator_final.py. 

It will call the lattice estimator (lwe.estimate) with cost models ADPS16_classical/quantum for the Smaug128, 192 and 256.  

It will also call the Smaug decryption failure probability estimator (smaug.runSmaugError). 

