/**
 * Copyright (C) 2018 ADVACAM
 * @author    Daniel Turecek <daniel.turecek@advacam.com>
 */
#include "pxcapi.h"
#include <cstring>
#include <algorithm>

#define SINGLE_CHIP_PIXSIZE      65536
#define ERRMSG_BUFF_SIZE         512
#define PIXEL_BUFF_LEN           1000000



int printError(const char* errorPrefix)
{
    char errorMsg[ERRMSG_BUFF_SIZE];
    pxcGetLastError(errorMsg, ERRMSG_BUFF_SIZE);
    printf("%s: %s\n", errorPrefix, errorMsg);
    return -1;
}

// ############################################## Timepix Examples ############################################33

int singleMeasurementTest(unsigned deviceIndex)
{
    // Measure one frame and get its data
    unsigned short frameData[SINGLE_CHIP_PIXSIZE];
    unsigned frameSize = SINGLE_CHIP_PIXSIZE;
    int rc = pxcMeasureSingleFrame(deviceIndex, 0.1, frameData, &frameSize);
    if (rc)
        return printError("Could not measure frame");
    rc = pxcSaveMeasuredFrame(0, 0, "test.pmf");
    printf("Save measure frame: %d\n", rc);
    return 0;
}

void measurementCallback(intptr_t acqIndex, intptr_t userData)
{
    unsigned devIndex = *((unsigned*)userData);
    printf("Measured frame %lu\n", acqIndex);
    printf("Measured frame count: %d\n", pxcGetMeasuredFrameCount(devIndex));

    unsigned short frameData[SINGLE_CHIP_PIXSIZE];
    unsigned frameSize = SINGLE_CHIP_PIXSIZE;
    pxcGetMeasuredFrame(devIndex, (unsigned)acqIndex, frameData, &frameSize);
}

int multipleMeasurementTestWithCallback(unsigned deviceIndex)
{
    double frameTime = 0.001;
    unsigned frameCount = 100;

    // Measure multiple frames and receive callback after each measured frame
    int devIdx = deviceIndex;
    int rc = pxcMeasureMultipleFramesWithCallback(deviceIndex, frameCount, frameTime, PXC_TRG_NO, measurementCallback, (intptr_t)&devIdx);
    if (rc)
        return printError("Could not measure frames");
    return 0;
}


// ############################################## Timepix3 Examples ############################################33



int main (int argc, char const* argv[])
{
    // Initialize Pixet
    int rc = pxcInitialize();
    if (rc)
        return printError("Could not initialize Pixet");

    int connectedDevicesCount = pxcGetDevicesCount();
    printf("Connected devices: %d\n", connectedDevicesCount);

    if (connectedDevicesCount == 0)
        return pxcExit();

    // list all connected devices (name + chipID)
    for (unsigned devIdx = 0; devIdx < connectedDevicesCount; devIdx++){
        char deviceName[256];
        memset(deviceName, 0, 256);
        pxcGetDeviceName(devIdx, deviceName, 256);

        char chipID[256];
        memset(chipID, 0, 256);
        pxcGetDeviceChipID(0, 0, chipID, 256);

        printf("Device %d Name %s, (ChipID: %s)\n", devIdx, deviceName, chipID);
    }

    // Different Example Measurements:

    singleMeasurementTest(0);
    //multipleMeasurementTestWithCallback(0);


    // Exit Pixet
    return pxcExit();
}
