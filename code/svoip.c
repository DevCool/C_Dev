/********************************************************************
 * svoip.c - Going to be a simple VOIP (Voice Over IP) application. *
 ********************************************************************
 * By Philip R. Simonson                                            *
 ********************************************************************
 */

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define ALIAS "random_str"

int record_play(void);

int main(int argc, char *argv[])
{
	printf("*****************************\n"
		"* Tiny VOIP Application     *\n"
		"* By 5n4k3                  *\n"
		"*****************************\n\n");

	if(record_play() != 0)
		return 1;
	if(!PlaySound("tmp.wav", NULL, SND_FILENAME))
		return 1;
	if(remove("tmp.wav"))
		return 1;

	return 0;
}

int record_play(void)
{
	char mci_command[100];
	char retstr[300];
	int mci_error;

	/* open wav type alias */
	sprintf(mci_command, "open new type waveaudio alias %s", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	/* set the time format */
	sprintf(mci_command, "set %s time format ms", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	sprintf(mci_command, "record %s notify", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	printf("Now on air, press a key to stop.\n");
	getch();

	sprintf(mci_command, "stop %s", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	sprintf(mci_command, "save %s %s", ALIAS, "tmp.wav");
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	sprintf(mci_command, "close %s", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);
	printf("Recording stopped.\n");
	getch();
	return mci_error;
}
