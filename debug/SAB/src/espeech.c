#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <espeak/speak_lib.h>

#include "espeech.h"
#include "../../debug.h"

int SynthCallback(short *wav, int numsamples, espeak_EVENT *events) {
  if(wav == NULL || numsamples < 0 || events == NULL)
    return -1;
  
  return 0;
}

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

void speakCleanup(void) {
  espeak_Terminate();
}

/* takes char array and length, will speak the words.
 */
int speak(char *text, int len) {
  espeak_ERROR speakErr;

  ERROR_FIXED((speakErr = espeak_Synth(text, len, 0, 0, 0, espeakCHARS_AUTO, NULL, NULL)) != EE_OK,
	      "Something went wrong with speaking text.");
  return 0;

error:
  return -1;
}
