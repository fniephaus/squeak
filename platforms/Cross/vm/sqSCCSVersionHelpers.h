#if GIT

#include <stdio.h>

static char *
cmdAsString(char* cmd)
{
	FILE *fp;
	char res[256];

	fp = popen(cmd, "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}

	if(fgets(res, sizeof(res)-1, fp) == NULL) 
	{
		pclose(fp);
		return "unknown";
	}

	pclose(fp);
	return &res[0];
}

#endif