/* SoundPlugin.h - header for Sound Plugins :-) tim@sumeru.stanford.edu */

/* module initialization/shutdown */
int soundInit(void);
int soundShutdown(void);

/* sound output */
int snd_AvailableSpace _iarg();
int snd_InsertSamplesFromLeadTime _iargs(int frameCount, int srcBufPtr, int samplesOfLeadTime);
int snd_PlaySamplesFromAtLength _iargs(int frameCount, int arrayIndex, int startIndex);
int snd_PlaySilence _iarg();
int snd_Start _iargs(int frameCount, int samplesPerSec, int stereo, int semaIndex);
int snd_Stop _iarg();

/* sound input */
int snd_SetRecordLevel _iargs(int level);
int snd_StartRecording _iargs(int desiredSamplesPerSec, int stereo, int semaIndex);
int snd_StopRecording _iarg();
double snd_GetRecordingSampleRate _iarg();
int snd_RecordSamplesIntoAtLength _iargs(int buf, int startSliceIndex, int bufferSizeInBytes);
void snd_Volume _iargs(double *left, double *right); //johnmci@smalltalkconsulting.com Nov 6th 2000
void snd_SetVolume _iargs(double left, double right);//johnmci@smalltalkconsulting.com Nov 6th 2000
int snd_SetRecordBufferFrameCount _iargs(int frameCount);