#ifndef espeech_h
#define espeech_h

int SynthCallback(short *wav, int numsamples, espeak_EVENT *events);
int speakInit(void);
void speakCleanup(void);
int speak(char *text, int len);

#endif
