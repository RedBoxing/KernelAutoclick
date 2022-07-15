#pragma once

#define IO_ACTION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x666, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _INPUT_REPORT {
	unsigned char reportId;

	union {
		struct {
			UCHAR buttons;
			USHORT x;
			USHORT y;
		} mouseReport;
	} report;
} INPUT_REPORT, *PINPUT_REPORT;

#define MOUSE_REPORT_ID			2

#define MOUSE_BUTTON_1			0x01
#define MOUSE_BUTTON_2			0x02
#define MOUSE_BUTTON_3			0x04

#define DEBUG(msg, ...) DbgPrintEx(0, 0, msg, __VA_ARGS__)