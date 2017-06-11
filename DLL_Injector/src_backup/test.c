#include <windows.h>

int MainFunc_Default(int *argc, char ***args, BOOL bReg);
int (*Function)(int *argc, char ***args, BOOL bReg);

int main(int argc, char *argv[])
{
	// Callback Setup
	Function = &MainFunc_Default;
	return (*Function)(&argc, &argv, TRUE);
}

int MainFunc_Default(int *argc, char ***args, BOOL bReg)
{
	int retVal;

	if (bReg) {
		MessageBox(NULL, "You need to add some code!", "INFO", MB_OK | MB_ICONINFORMATION);
		retVal = 0;
	}

	return retVal;
}