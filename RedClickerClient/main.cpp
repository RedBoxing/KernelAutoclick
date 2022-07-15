#include <iostream>
#include "kernel_interface.hpp"

int main()
{
	KernelInterface Driver = KernelInterface("\\\\.\\redclicker");

    std::cout << "Hello World!\n";

	INPUT pInputs[1];
	pInputs[0].type = INPUT_KEYBOARD;
	pInputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	NTSTATUS status = Driver.SendInput(ARRAYSIZE(pInputs), pInputs, sizeof(INPUT));
	std::cout << status << std::endl;
}