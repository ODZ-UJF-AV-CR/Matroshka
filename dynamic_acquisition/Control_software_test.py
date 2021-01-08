import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams['font.size'] = 17

ls_frames, ls_occ = [], []
max_frame = 150
min_frame = 6

alldevices = pixet.devices() # get all devices (including motors)
detectors = pixet.devicesByType(1) # get all connected Medipix/Timepix devices
detector = detectors[0] # get first connected detector
print detector.fullName() # prints the name of the device
measurement_time = 30.0 # seconds
acqCount, acqTime = 3, 10.0 # number of frames = 1, acq. time = 1 s
fileType = 1 # auto detect, 0 = no output file
bias = detector.setBias(50)
outputFile = "/home/user/Documents/Data/I08_data.pmf"

for i in range(30):
    outputFile = "/home/user/Documents/Data/I08_data_" + str(i) + ".pmf"
    detector.doSimpleAcquisition(acqCount, acqTime, fileType, outputFile)
    df = pd.read_csv(outputFile, sep=' ', header=None)
    numpy_matrix = df.as_matrix()
    non_zero = np.count_nonzero(numpy_matrix)
    total = numpy_matrix.shape
    total = total[0] * total[1]
    occupancy = non_zero / float(total)
    current_frames = 60 / acqTime
    if (occupancy > 0.03 or occupancy < 0.01):
        out1 = (occupancy / 0.02) * current_frames
        if (out1 > max_frame):
            out1 = max_frame
        if (out1 < min_frame):
            out1 = min_frame
    else:
        out1 = current_frames
    ls_frames.append(out1)
    ls_occ.append(occupancy * 100)
    print(out1, occupancy)
    acqTime = 60.0 / out1
    acqCount = int(measurement_time / acqTime)
    ls_count.append(acqCount)
    ls_time.append(acqTime)
    print(acqCount, acqTime, acqTime * acqCount)

color1 = 'tab:red'
color2 = 'tab:blue'
fig1 = plt.figure()
ax1 = fig1.add_subplot(1, 1, 1)
ax1.plot(ls_frames, color=color1)
ax1.set_xlabel('Time [-]')
ax1.set_ylabel('Num. of frames [/min]', color=color1)

ax2 = ax1.twinx()
ax2.plot(ls_occ, color=color2)
ax2.set_ylabel('Occupancy [%]', color=color2)

plt.show()
