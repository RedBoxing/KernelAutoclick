#pragma once

#include <Windows.h>
#include "common.h"
#include <ntstatus.h>

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

class KernelInterface
{
public:
	HANDLE hDriver;

	KernelInterface(LPCSTR RegistryPath)
	{
		hDriver = CreateFileA(RegistryPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	void SendKeyDown(UCHAR key)
	{
		
	}
	
	void SendKeyUp(UCHAR key)
	{
		
	}
	
	NTSTATUS SendInput(
		_In_ UINT    nInputs,
		_In_ LPINPUT pInputs,
		_In_ int     cbSize)
	{
		if (hDriver == INVALID_HANDLE_VALUE) {
			return -1;
		}

		NTSTATUS status;
		for (UINT i = 0; i < nInputs; i++) 
		{
			if (!NT_SUCCESS(status = SendInputInternal(&pInputs[i])))
			{
				break;
			}
		}

		return status;
	}
private:
	NTSTATUS SendInputMouseInternal(LPINPUT input)
	{
		INPUT_REPORT data;
		data.reportId = MOUSE_REPORT_ID;
		
		if (input->mi.dwFlags & MOUSEEVENTF_ABSOLUTE == 0)
		{
			printf("Error: Absolute mouse coordinates not supported\n");
			return false;
		}

		if (input->mi.dwFlags & MOUSEEVENTF_LEFTDOWN)
		{
			data.report.mouseReport.buttons |= MOUSE_BUTTON_1;
		}

		if (input->mi.dwFlags & MOUSEEVENTF_LEFTUP)
		{
			data.report.mouseReport.buttons &= ~MOUSE_BUTTON_1;
		}
		
		if (input->mi.dwFlags & MOUSEEVENTF_RIGHTDOWN)
		{
			data.report.mouseReport.buttons |= MOUSE_BUTTON_2;
		}

		if (input->mi.dwFlags & MOUSEEVENTF_RIGHTUP)
		{
			data.report.mouseReport.buttons &= ~MOUSE_BUTTON_2;
		}

		data.report.mouseReport.x = input->mi.dx >> 1;
		data.report.mouseReport.y = input->mi.dy >> 1;

		NTSTATUS status;
		DWORD Bytes;
		if (!DeviceIoControl(hDriver, IO_ACTION, &data, sizeof(INPUT_REPORT), &status, sizeof(NTSTATUS), &Bytes, NULL))
		{
			return STATUS_UNSUCCESSFUL;
		}
		
		return status;
	}

	NTSTATUS SendInputInternal(LPINPUT Input)
	{
		if (Input->type == INPUT_MOUSE)
		{
			return SendInputMouseInternal(Input);
		}
		else
		{
			return STATUS_NOT_SUPPORTED;
		}
	}
};