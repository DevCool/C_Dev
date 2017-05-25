/****************************************************
 * SAB 2 Project                                    *
 ****************************************************
 * Copyright (C) 2017 Philip Simonson.              *
 ****************************************************
 * Do NOT use this for malicious activities, no     *
 * "hacking" or "cracking" with this software.      *
 ****************************************************
 */

#include <stdio.h>
#include "helper.h"

/* main() - entry point for all programs.
 */
int main(int argc, char *argv[])
{
#if defined(_WIN32) || (_WIN64)
	return run_code(&argc, &argv);
#endif
	return 0;
}

/* run_code() - branch over to WinMain().
 */
int run_code(int *argc, char **argv[])
{
	if(*argc > 1)
		goto usage;
	FreeConsole();
	WinMain(GetModuleHandle(NULL), NULL, **argv, SW_SHOWNORMAL);
	return 0;

usage:
	fprintf(stdout, "Usage: %s", *argv[0]);
	return 1;
}
