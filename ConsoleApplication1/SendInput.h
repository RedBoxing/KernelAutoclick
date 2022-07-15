#pragma once

extern "C" {
	void InjectKeyDown(
		UCHAR vk
	);

	void InjectKeyUp(
		UCHAR vk
	);

	void InjectScanKeyDown(
		WORD scanCode
	);
	void InjectUnicode(
		WORD wch
	);
	void InjectScanKeyUp(
		WORD scanCode
	);

	void InjectMouseMove(
		WORD X,
		WORD Y,
		UINT Buttons
	);


	UINT InjectSendInput(
		_In_ UINT    nInputs,
		_In_ LPINPUT pInputs,
		_In_ int     cbSize
	);
}