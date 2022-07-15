#include <iostream>
#include <windows.h>
#include "common.h"
#include "HidDevice.h"
#include "SendInput.h"

UINT SendLeftClick();
UINT SendRightClick();

int main()
{
	printf("Initializing HID device...\n");

	if (OpenHidInjectorDevice()) {
		printf("Found HID device !\n");
		printf("Injecting Left click...\n");

		UINT click = SendLeftClick();
	}
	else {
		printf("Error : Failed to find HID device !\n");
		exit(0);
	}
}

UINT SendLeftClick() {
	INPUT inputs[1] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	return InjectSendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

UINT SendRightClick() {
	INPUT inputs[1] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	return InjectSendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}