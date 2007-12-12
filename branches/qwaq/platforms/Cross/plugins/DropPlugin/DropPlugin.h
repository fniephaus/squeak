/* drop support primitives */

#ifndef DragEnter
#define DragEnter	1 /* drag operation from OS entered Squeak window	 */
#define DragMove	2 /* drag operation from OS moved within Squeak window */
#define DragLeave	3 /* drag operation from OS left Squeak window	 */
#define DragDrop	4 /* drag operation dropped contents onto Squeak.      */
#endif

/* module initialization/shutdown */
int dropInit(void);
int dropShutdown(void);

char* dropRequestFileName(int dropIndex); /* return name of file or NULL if error */
/* note: dropRequestFileHandle needs to bypass plugin security checks when implemented */
int dropRequestFileHandle(int dropIndex); /* return READ-ONLY file handle OOP or nilObject if error */
int sqSecFileAccessCallback(void *);
void sqSetNumberOfDropFiles(int numberOfFiles);
void sqSetFileInformation(int dropIndex, void *dropFile);