#pragma once
#ifndef _DRIVER_H
#define _DRIVER_h

#include <ntddk.h>
#include <wdf.h>
#include <vhf.h>

#include "common.h"

typedef struct _DEVICE_CONTEXT
{
	VHFHANDLE vhfHandle;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD  UnloadDriver;

EVT_WDF_DRIVER_DEVICE_ADD DriverEvtDeviceAdd;

NTSTATUS SubmitReport(PDEVICE_CONTEXT DeviceContext, PINPUT_REPORT Report);

WDFDEVICE GetWDFDevice();

#endif