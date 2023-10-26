from Lizard import runLizardError
from RLizard import runRLizardError
from RLizardv2 import runRLizardv2Error
from smaug import runSmaugError

# runLizardError(category=1, n=536)
# runLizardError(category=1, n=663)
# runLizardError(category=3, n=816)
# runLizardError(category=3, n=952)
# runLizardError(category=5, n=1088)
# runLizardError(category=5, n=1300)


# runRLizardError(category=1, n=1024)
# runRLizardError(category=3, n=1024)
# runRLizardError(category=3, n=2048)
# runRLizardError(category=5, n=2048)

# runRLizardv2Error(1024)
# runRLizardv2Error(2048)


# for l in range(-20,21,2):
#     hr128 = 128+2*l
#     hs128 = 128+2*l
#     runSmaugError('128-smaug', 256, 2, 1024, 256, hr128, hs128, {0: 384, 1: 247, 2: 66, 3: 7})

# for l in range(-13,5,2):
#     hr192  = 192+3*l
#     hs192  = 192+3*l
#     # runSmaugError('192-smaug sig=1.0625', 256, 3, 1024, 256, 192+3*l, 192+3*l, {0: 384, 1: 247, 2: 66, 3: 7})
#     runSmaugError('192-smaug sig=1.45', 256, 3, 1024, 256, hr192, hs192, {0: 562, 1: 444, 2: 218, 3: 67, 4: 13})

# runSmaugError('192-smaug',            256, 3, 1024, 256, 192, 192, {0: 562, 1: 444, 2: 218, 3: 67, 4: 13})
# l=13 # l=13 is optimal or maybe 14?
# for l in range(0,14,1):
#     print("l="+str(l))
#     runSmaugError('192-smaug sig=1.0625', 256, 3, 1024, 256, 192-3*l, 192-3*l, {0: 384, 1: 247, 2: 66, 3: 7})

# l=16
# runSmaugError('256-smaug',          256, 4, 1024, 256, 176, 176, {0: 622, 1: 579, 2: 466, 3: 324, 4: 195, 5: 102, 6: 46, 7: 18, 8: 6, 9: 1})
# runSmaugError('256-smaug sig=2.63', 256, 4, 1024, 256, 128, 128, {0: 622, 1: 579, 2: 466, 3: 324, 4: 195, 5: 102, 6: 46, 7: 18, 8: 6, 9: 1})
# runSmaugError('256-smaug sig=1.45', 256, 4, 1024, 256, 176, 176, {0: 562, 1: 444, 2: 218, 3: 67, 4: 13})

# for l in range(0,16,3):
#     hr256 = 176+4*l
#     hs256 = 176+4*l
#     runSmaugError('256-smaug sig=2.63', 256, 4, 1024, 256, hr256, hr256, {0: 622, 1: 579, 2: 466, 3: 324, 4: 195, 5: 102, 6: 46, 7: 18, 8: 6, 9: 1})

# print('=== 256-smaug sig=1.06 ===')
# for l in range(-1, 5, 1):
#     hr256 = 176+4*l
#     hs256 = int( (176+8.4*l+80)/4 )*4
#     runSmaugError('256-smaug sig=1.06', 256, 4, 1024, 128, hs256, hr256, {0: 384, 1: 247, 2: 66, 3: 7})

# print('\n=== 256-smaug sig=1.02 ===')
# for l in range(-7, 16, 2):
#     hr256 = 176+4*l
#     hs256 = 176+4*l
#     runSmaugError('256-smaug sig=1.45', 256, 4, 1024, 128, hs256, hr256, {0: 801, 1: 495, 2: 117, 3: 11})

for l in range(5, 12, 1):
    hr256 = 184
    hs256 = 268+4*l
    answer = runSmaugError('256-smaug sig=1.45', 256, 4, 1024, 128, hs256, hr256, {0: 1021, 1: 448, 2: 45, 3: 1})
    print(answer)
for l in range(5, 12, 1):
    hr256 = 256
    hs256 = 268+4*l
    runSmaugError('256-smaug sig=1.45', 256, 4, 1024, 256, hs256, hr256, {0: 1021, 1: 448, 2: 45, 3: 1})
for l in range(3):
    hr256 = 184
    hs256 = 268+4*l
    runSmaugError('256-smaug sig=1.45', 256*4, 1, 1024, 128, hs256, hr256, {0: 1021, 1: 448, 2: 45, 3: 1})


# print('\n=== 256-smaug sig=2.63 ===')
# for l in range(-29, -20, 2):
#     hr256 = 176+4*l
#     hs256 = 176+4*l
#     runSmaugError('256-smaug sig=2.63', 256, 4, 1024, 128, hs256, hr256, {0: 622, 1: 579, 2: 466, 3: 324, 4: 195, 5: 102, 6: 46, 7: 18, 8: 6, 9: 1})

# runSmaugError('256-smaug sig=1.06', 256, 4, 1024, 128, 176, 176, {0: 384, 1: 247, 2: 66, 3: 7})
