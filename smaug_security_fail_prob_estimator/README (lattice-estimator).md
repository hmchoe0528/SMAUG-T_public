# Lizard_Error
Calculate errors of Lizard

# Run
Run main.py
runLizardError calculates Lizard error of input category & n.
runRLizardError calculates RLizard error of input category & n.

# Inputs
All default inputs are saved in inputs.xlsx. You can change parameters by changing this file.
Sheet "parameters" :  input parameters.
Sheet "distribution" : input distribution. on the row "i", input frequency of i as integer.
                        (The probability of -i is same as i)
Sheets "R_parameters" and "R_distribution" are those for Ring-Lizard.

# Outputs
===== CATERORY(category)_N(n) =====
(parameters)
Error(log2) : (log2 value of error)
# LizardError
