#include "driver.h"
#include "communication.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, DriverEvtDeviceAdd)
#endif

UNICODE_STRING dev;
UNICODE_STRING dos;
PDEVICE_OBJECT DeviceObject;
WDFDEVICE device;

UCHAR       G_DefaultReportDescriptor[] =
{
    // Mouse
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //  Report Id(2)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x16, 0x00, 0x00,              //     Logical Minimum(0)
    0x26, 0xFF, 0x7F,              //     Logical Maximum(32767)
    0x36, 0x00, 0x00,              //     Physical Minimum(0)
    0x46, 0xFF, 0x7F,              //     Physical Maximum(32767)
    0x66, 0x00, 0x00,              //     Unit(None)
    0x75, 0x10,                    //     Report Size(16)
    0x95, 0x02,                    //     Report Count(2)
    0x81, 0x62,                    //     Input(Data, Variable, Absolute, No Preferred, Null State)
    0x95, 0x05,                    //     REPORT_COUNT (2)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0xc0,                          // END_COLLECTION
};

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    NTSTATUS status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG config;

    PAGED_CODE();

    DEBUG("Loading Driver...\n");

    WDF_DRIVER_CONFIG_INIT(&config, DriverEvtDeviceAdd);

    DEBUG("WDF_NO_OBJECT_ATTRIBUTES: %x\n", WDF_NO_OBJECT_ATTRIBUTES);

    __try {
        status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        status = GetExceptionCode();
    }

    if (!NT_SUCCESS(status))
    {
        DEBUG("WdfDriverCreate failed with status 0x%x\n", status);
        return status;
    }

    /*  RtlInitUnicodeString(&dev, L"\\Device\\redclicker");
    RtlInitUnicodeString(&dos, L"\\DosDevices\\redclicker");

    IoCreateDevice(DriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
    IoCreateSymbolicLink(&dos, &dev);

    DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;*/
    DriverObject->DriverUnload = UnloadDriver;

	/*
    DeviceObject->Flags |= DO_DIRECT_IO;
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;	*/
    
    DEBUG("Loaded Driver !\n");
    return status;
}


VOID UnloadDriver(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    DEBUG("Unloading Driver...");

    IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverEvtDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit)
{
    DEBUG("DriverEvtDeviceAdd");

    UNREFERENCED_PARAMETER(Driver);
	
    NTSTATUS status = STATUS_SUCCESS;
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    VHF_CONFIG vhfConfig;
    PDEVICE_CONTEXT deviceContext;

    PAGED_CODE();

    WdfFdoInitSetFilter(DeviceInit);
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if(!NT_SUCCESS(status))
	{
		DEBUG("WdfDeviceCreate failed with status 0x%x", status);
		return status;
	}

    deviceContext = GetDeviceContext(device);
    
    VHF_CONFIG_INIT(&vhfConfig, WdfDeviceWdmGetDeviceObject(device), sizeof(G_DefaultReportDescriptor), G_DefaultReportDescriptor);

    status = VhfCreate(&vhfConfig, &deviceContext->vhfHandle);
    if (!NT_SUCCESS(status))
    {
        DEBUG("VhfCreate failed with status 0x%x", status);
        return status;
    }

    status = VhfStart(deviceContext->vhfHandle);
	if (!NT_SUCCESS(status))
	{
		DEBUG("VhfStart failed with status 0x%x", status);
		return status;
	}

    return status;
}

NTSTATUS SubmitReport(PDEVICE_CONTEXT DeviceContext, PINPUT_REPORT Report)
{
    NTSTATUS status = STATUS_SUCCESS;
    HID_XFER_PACKET packet;

    packet.reportId = Report->reportId;
    packet.reportBufferLen = sizeof(INPUT_REPORT);
    packet.reportBuffer = (PUCHAR) Report;

    status = VhfReadReportSubmit(DeviceContext->vhfHandle, &packet);
    if (!NT_SUCCESS(status)) {
        DEBUG("VhfReadReportSubmit failed: %d", status);
    }

    return status;
}

WDFDEVICE GetWDFDevice()
{
    return device;
}
