#pragma once

#define WM_ACCEPTSUCCESS WM_USER + 1
#define WM_CMDDATA WM_USER + 2
typedef struct HEAD_HEAD {
	int commend;
	int len;
}HEAD_INFO;

enum allprotol {
	OPEN_CALC,
	OPEN_NOT,
	OPEN_CMD,
	ON_CMDDATA,
	ON_CMDCOMMAND
};