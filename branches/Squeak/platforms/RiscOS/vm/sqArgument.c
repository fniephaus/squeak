#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "sq.h"
#include "sqArguments.h"

static opt = 0;
static int numOptions;
static char** optionArray;
int ImageVersionNumber;

static char* nextOption() {
	if(opt >= numOptions) return 0;
	return optionArray[opt++];
}


static int IsImage(char *name) {
	FILE *fp;
	int magic;
	int byteSwapped(int);

	fp = fopen(name,"rb");
	if(!fp) return 0; /* not an image */
	if(fread(&magic, 1, sizeof(magic), fp) != sizeof(magic)) {
		fclose(fp);
		return 0;
	}
	fclose(fp);
	if (magic > 0xFFFF) {
		magic = byteSwapped(magic);
	}
	ImageVersionNumber = magic;
	return readableFormat(magic);
}


/* parse an unsigned integer argument */
static char *parseUnsignedArg(char *src, unsigned *dst) {
	char buf[50];
	char *tmp = buf;
	int factor = 1;

	while(isdigit(*src)) *(tmp++) = *(src++);

	if(*src && tolower(*src) == 'k') {factor = 1024; src++;}
	else if(*src && tolower(*src) == 'm') {factor = 1024*1024; src++;}
	else if(*src && *src != ' ') /* strange chars at end */
		return NULL;
	if(tmp == buf) /* no numbers found */
		return NULL;
	*tmp = 0;
	*dst = atol(buf) * factor;
	if(*src) *(src++) = 0;
	return src;
}

/* parse a (possibly signed) integer argument */
static char *parseSignedArg(char *src, int *dst) {
	int negative;
	unsigned value;

	negative = *src == '-';
	if(negative) src++;
	src = parseUnsignedArg(src, &value);
	if(!src) return NULL;
	if(negative) *dst = 0-(int)value;
	else *dst = (int) value;
	return src;
}

/* parse all arguments meaningful to the VM */
static int parseVMArgs(vmArg args[]) {
	vmArg *arg;
	int arglen;
	char * string;

	while(1)
		{
			if((string = nextOption()) == NULL)
				return NULL; /* no more */

			if(*string != '-') {
				opt--;
				return 1; /* no '-' means this isn't a VM option - probably the image name */
			}

			// save the string for the image to look at
			vmOptions[numOptionsVM++] = string;

			/* search args list */
			arg = args;
			while(arg->type != ARG_NONE){
				arglen = strlen(arg->name);
				if(strncmp(arg->name, string, strlen(arg->name)) == 0)
					break;
				arg++;
			}
			if(arg->type == ARG_NONE)
				return NULL; /* done */

			// if the char at the end of the option name is ':', null it out and skip ahead one
			string += (arglen-1);
			if(*string== ':') *(string++) = 0;

			switch(arg->type) {
				case ARG_FLAG:
					*(int*)arg->value = 1;
					break;

				case ARG_STRING:
					vmOptions[numOptionsVM++] = string;
					*(char**) arg->value = string;
					break;

				case ARG_INT:
					vmOptions[numOptionsVM++] = string;
					string = parseSignedArg(string, (int*)arg->value);
					if(!string) return NULL;
					break;

				case ARG_UINT:
					vmOptions[numOptionsVM++] = string;
					string = parseUnsignedArg(string, (unsigned int*)arg->value);
					if(!string) return NULL;
					break;

				default:
					fprintf(stderr,"Unknown option encountered!\n");
					return NULL;
			 };
		}
}

/* parse all arguments starting with the image name */
static int parseGenericArgs(void) {
	char *string;
	extern char vmPath[];
	extern void decodePath(char*, char*);

	if (!(string = nextOption()) ) {
		// no options left, so can only use default image name
		strcpy(imageName, vmPath);
		strcat(imageName, "Squeak/image");
		imageOptions[numOptionsImage++] = imageName;
		return 1;
	}

	/* now get decode the putative image name */
	decodePath(string, imageName);

	if(*imageName && IsImage(imageName)) {
		// all is ok, its the image name
	} else { /* Not the image name -- use a default in the !Squeak appdir */
		strcpy(imageName, vmPath);
		strcat(imageName, "Squeak/image");
		// and skip back 1 on the option list
		opt--;
	}
	imageOptions[numOptionsImage++] = imageName;

	// now go through any more options
	while((string = nextOption()) && *string) {
			if(numOptionsImage > MAX_OPTIONS) return NULL; /* too many args */
			while(*string && *string == ' ') string++; /* skip blanks */
			imageOptions[numOptionsImage++] = string;
			if(!string) return NULL;
	}
	return 1;
}


int parseArguments(char *argv[], int argc, vmArg args[]) {
	extern char vmPath[];
	extern void decodeVMPath(char*);
	numOptionsVM = 0;
	numOptionsImage = 0;
	numOptions = argc;
	optionArray = &argv[0];

	/* argv[0] = executable name */
	decodeVMPath( nextOption());
	vmOptions[numOptionsVM++] = vmPath;

	/* parse VM options */
	parseVMArgs(args);

	/* parse image and generic args */
	return parseGenericArgs() != NULL;
}
