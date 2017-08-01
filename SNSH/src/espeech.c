/****************************************************************
 * espeech.c - this handles all of the initialization of espeak *
 *             library. Do NOT modify this file, or espeak may  *
 *             may not work properly for this program.          *
 ****************************************************************
 * Created by Philip "5n4k3" Simonson            (2017)         *
 ****************************************************************
 */

#if defined(__linux__)
/* standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* espeak library */
#include <espeak/speak_lib.h>

/* my headers */
#include "espeech.h"
#include "debug.h"

/* SynthCallback() - This has to be here leave alone.
 */
int SynthCallback(short *wav, int numsamples, espeak_EVENT *events) {
  if(wav == NULL || numsamples < 0 || events == NULL) {}
  
  return 0;
}

/* speakInit() - This is my initialization function for espeak
 * library, leave alone.
 */
int speakInit(void) {
  /* must be called before any other functions */
  ERROR_FIXED(espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, 0, NULL,
				espeakINITIALIZE_PHONEME_EVENTS) < 0,
	      "Could not initialize espeak properly.");
  espeak_SetSynthCallback(&SynthCallback);
  return 0;

error:
  return -1;
}

/* speakCleanup() - This is my cleanup function doesn't
 * need to be messed with either.
 */
void speakCleanup(void) {
  espeak_Terminate();
}

/* speak() - takes char array and length, will speak the words.
 */
int speak(char *text, int len) {
  espeak_ERROR speakErr;

  ERROR_FIXED((speakErr = espeak_Synth(text, len, 0, 0, 0, espeakCHARS_AUTO, NULL, NULL)) != EE_OK,
	      "Something went wrong with speaking text.");
  return 0;

error:
  return -1;
}
#endif
