#pragma once

extern "C" {
	BOOL OpenHidInjectorDevice();
	void CloseHidInjectorDevice();

	extern HANDLE g_hFile;

}