/* Automatically generated from Squeak on #(19 March 2005 10:08:58 am) */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
/*  The virtual machine proxy definition */
#include "sqVirtualMachine.h"
/* Configuration options */
#include "sqConfig.h"
/* Platform specific definitions */
#include "sqPlatformSpecific.h"

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
#undef EXPORT
// was #undef EXPORT(returnType) but screws NorCroft cc
#define EXPORT(returnType) static returnType
#endif

/* memory access macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)


/*** Constants ***/
#define A1v 46
#define A2f 34
#define A2v 47
#define A3f 35
#define A3v 48
#define A4f 36
#define A4v 49
#define A5f 37
#define A6f 38
#define Anv 45
#define Aspiration 9
#define Atv 50
#define B1 13
#define B2 17
#define B2f 40
#define B3 19
#define B3f 41
#define B4 21
#define B4f 42
#define B5 23
#define B5f 43
#define B6 25
#define B6f 44
#define Bnp 27
#define Bnz 29
#define Btp 31
#define Btz 33
#define Bypass 39
#define Diplophonia 4
#define Epsilon 0.0001
#define F0 0
#define F1 12
#define F2 16
#define F3 18
#define F4 20
#define F5 22
#define F6 24
#define Flutter 1
#define Fnp 26
#define Fnz 28
#define Friction 10
#define Ftp 30
#define Ftz 32
#define Gain 51
#define Jitter 2
#define PI 3.141592653589793
#define R1c 12
#define R1vp 3
#define R2c 13
#define R2fp 7
#define R2vp 4
#define R3c 14
#define R3fp 8
#define R3vp 5
#define R4c 15
#define R4fp 9
#define R4vp 6
#define R5c 16
#define R5fp 10
#define R6c 17
#define R6fp 11
#define R7c 18
#define R8c 19
#define Ra 7
#define Rk 8
#define Rnpc 20
#define Rnpp 1
#define Rnz 21
#define Ro 6
#define Rout 24
#define Rtpc 22
#define Rtpp 2
#define Rtz 23
#define Shimmer 3
#define Turbulence 11
#define Voicing 5

/*** Function Prototypes ***/
static void addAmplitudeDiplophonia(void);
static void addFlutter(void);
static void addFrequencyDiplophonia(void);
static void addJitter(void);
static void addShimmer(void);
static void antiResonatorfrequencybandwidth(int index, float freq, float bw);
static float antiResonatorvalue(int index, double aFloat);
static float cascadeBranch(float source);
static float * checkedFloatPtrOf(int oop);
static short * checkedShortPtrOf(int oop);
#pragma export on
EXPORT(const char*) getModuleName(void);
#pragma export off
static float glottalSource(void);
static int halt(void);
static float linearFromdB(double aNumber);
static int loadFrom(int klattOop);
static int msg(char *s);
static int nextRandom(void);
static void normalizeGlottalPulse(void);
static float parallelFrictionBranch(float source);
static float parallelVoicedBranch(float source);
static void pitchSynchronousReset(void);
#pragma export on
EXPORT(int) primitiveSynthesizeFrameIntoStartingAt(void);
#pragma export off
static float quphicosphisinphirphid(float u, float phi, float cosphi, float sinphi, float rphid);
static void resonatorfrequencybandwidth(int index, float freq, float bw);
static void resonatorfrequencybandwidthgain(int index, float freq, float bw, float gain);
static float resonatorvalue(int index, float aFloat);
static float resonatorA(int index);
static void resonatorAput(int index, float aFloat);
static float resonatorB(int index);
static void resonatorBput(int index, float aFloat);
static float resonatorC(int index);
static void resonatorCput(int index, float aFloat);
static float resonatorP1(int index);
static void resonatorP1put(int index, float aFloat);
static float resonatorP2(int index);
static void resonatorP2put(int index, float aFloat);
static void rorark(float roNumber, float raNumber, float rkNumber);
static int saveTo(int origKlattOop);
static void setCurrentFrame(float *aKlattFrame);
#pragma export on
EXPORT(int) setInterpreter(struct VirtualMachine* anInterpreter);
#pragma export off
static void synthesizeFrameintostartingAt(float *aKlattFrame, short *buffer, int startIndex);
static void voicedPitchSynchronousReset(void);
static float zeroQphicosphisinphirphid(float phi, float cosphi, float sinphi, float rphid);
/*** Variables ***/
static float a1;
static float a2;
static float b1;
static float c1;
static int cascade;
static float* frame;
static float glast;

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"Klatt 19 March 2005 (i)"
#else
	"Klatt 19 March 2005 (e)"
#endif
;
static float nlast;
static int nmod;
static int nopen;
static int nper;
static int periodCount;
static float pitch;
static float* resonators;
static int samplesCount;
static int samplesPerFrame;
static int samplingRate;
static int seed;
static int t0;
static float vlast;
static float x1;
static float x2;



/*	Add diplophonia (bicyclic voice). Change voicing amplitude. */

static void addAmplitudeDiplophonia(void) {
	if (!((periodCount % 2) == 0)) {

		/* x1 must be <= 0 */

		x1 = x1 * (1.0 - (frame[Diplophonia]));
		if (x1 > 0) {
			x1 = 0;
		}
	}
}


/*	Add F0 flutter, as specified in:
		'Analysis, synthesis and perception of voice quality variations among
		female and male talkers' D.H. Klatt and L.C. Klatt JASA 87(2) February 1990.
	Flutter is added by applying a quasi-random element constructed from three
	slowly varying sine waves. */

static void addFlutter(void) {
    double deltaF0;
    float asin;
    float bsin;
    float csin;
    float timeCount;

	timeCount = (((float) samplesCount)) / (((float) samplingRate));
	asin = sin(((2.0 * PI) * 12.7) * timeCount);
	bsin = sin(((2.0 * PI) * 7.1) * timeCount);
	csin = sin(((2.0 * PI) * 4.7) * timeCount);
	deltaF0 = ((((frame[Flutter]) * 2.0) * (frame[F0])) / 100.0) * ((asin + bsin) + csin);
	pitch += deltaF0;
}


/*	Add diplophonia (bicyclic voice). Change F0. */

static void addFrequencyDiplophonia(void) {
	if ((periodCount % 2) == 0) {
		pitch += ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
	} else {
		pitch -= ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
	}
}


/*	Add jitter (random F0 perturbation). */

static void addJitter(void) {
	pitch += ((((nextRandom()) - 32767) * (frame[Jitter])) / 32768.0) * (frame[F0]);
}


/*	Add shimmer (random voicing amplitude perturbation). */

static void addShimmer(void) {

	/* x1 must be <= 0 */

	x1 += ((((nextRandom()) - 32767) * (frame[Shimmer])) / 32768.0) * x1;
	if (x1 > 0) {
		x1 = 0;
	}
}


/*	Set up an anti-resonator */

static void antiResonatorfrequencybandwidth(int index, float freq, float bw) {
    float a;
    float b;
    float c;
    float r;
    double arg;

	arg = ((0.0 - PI) / samplingRate) * bw;
	r = exp(arg);
	c = 0.0 - (r * r);
	arg = ((PI * 2.0) / samplingRate) * freq;
	b = (r * (cos(arg))) * 2.0;
	a = (1.0 - b) - c;
	a = 1.0 / a;
	b = (0.0 - b) * a;
	c = (0.0 - c) * a;
	resonatorAput(index, a);
	resonatorBput(index, b);
	resonatorCput(index, c);
}

static float antiResonatorvalue(int index, double aFloat) {
    double answer;
    double p1;

	answer = (((resonators[(index * 5) - 5]) * aFloat) + ((resonators[(index * 5) - 4]) * (p1 = resonators[(index * 5) - 2]))) + ((resonators[(index * 5) - 3]) * (resonators[(index * 5) - 1]));
	resonatorP2put(index, p1);
	resonatorP1put(index, aFloat);
	return answer;
}


/*	Cascade vocal tract, excited by laryngeal sources.
	Nasal antiresonator, nasal resonator, tracheal antirresonator,
	tracheal resonator, then formants F8, F7, F6, F5, F4, F3, F2, F1. */

static float cascadeBranch(float source) {
    float out;

	if (!(cascade > 0)) {
		return 0.0;
	}
	out = antiResonatorvalue(Rnz, source);
	out = resonatorvalue(Rnpc, out);
	out = antiResonatorvalue(Rtz, out);

	/* Do not use unless sample rate >= 16000 */

	out = resonatorvalue(Rtpc, out);
	if (cascade >= 8) {
		out = resonatorvalue(R8c, out);
	}
	if (cascade >= 7) {
		out = resonatorvalue(R7c, out);
	}
	if (cascade >= 6) {
		out = resonatorvalue(R6c, out);
	}
	if (cascade >= 5) {
		out = resonatorvalue(R5c, out);
	}
	if (cascade >= 4) {
		out = resonatorvalue(R4c, out);
	}
	if (cascade >= 3) {
		out = resonatorvalue(R3c, out);
	}
	if (cascade >= 2) {
		out = resonatorvalue(R2c, out);
	}
	if (cascade >= 1) {
		out = resonatorvalue(R1c, out);
	}
	return out;
}


/*	Return the first indexable word of oop which is assumed to be variableWordSubclass */

static float * checkedFloatPtrOf(int oop) {
	interpreterProxy->success(interpreterProxy->isWords(oop));
	if (interpreterProxy->failed()) {
		return 0;
	}
	return ((float *) (interpreterProxy->firstIndexableField(oop)));
}


/*	Return the first indexable word of oop which is assumed to be variableWordSubclass */

static short * checkedShortPtrOf(int oop) {
	interpreterProxy->success(interpreterProxy->isWords(oop));
	if (interpreterProxy->failed()) {
		return 0;
	}
	return ((short *) (interpreterProxy->firstIndexableField(oop)));
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*) getModuleName(void) {
	return moduleName;
}

static float glottalSource(void) {
    float x0;

	if (t0 == 0) {
		return 0;
	}
	if (nper < nopen) {
		x0 = (a1 * x1) + (a2 * x2);
		x2 = x1;
		x1 = x0;
	} else {
		x0 = (b1 * x1) - c1;
		x1 = x0;
	}
	if (nper >= t0) {
		nper = 0;
		/* begin pitchSynchronousReset */
		if ((frame[F0]) > 0) {
			/* begin voicedPitchSynchronousReset */
			pitch = frame[F0];
			addFlutter();
			/* begin addJitter */
			pitch += ((((nextRandom()) - 32767) * (frame[Jitter])) / 32768.0) * (frame[F0]);
			/* begin addFrequencyDiplophonia */
			if ((periodCount % 2) == 0) {
				pitch += ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
			} else {
				pitch -= ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
			}
			if (pitch < 0) {
				pitch = 0;
			}
			t0 = ((int) (samplingRate / pitch) );
			nmod = t0;
			if ((frame[Voicing]) > 0) {
				nmod = ((int) nmod >> 1);
			}
			nopen = ((int) (t0 * (frame[Ro])) );
			rorark(frame[Ro], frame[Ra], frame[Rk]);
			/* begin addShimmer */
			x1 += ((((nextRandom()) - 32767) * (frame[Shimmer])) / 32768.0) * x1;
			if (x1 > 0) {
				x1 = 0;
			}
			/* begin addAmplitudeDiplophonia */
			if (!((periodCount % 2) == 0)) {
				x1 = x1 * (1.0 - (frame[Diplophonia]));
				if (x1 > 0) {
					x1 = 0;
				}
			}
			periodCount = (periodCount + 1) % 65535;
		} else {
			t0 = 1;
			nmod = t0;
		}
	}
	nper += 1;
	return x0;
}

static int halt(void) {
	;
}

static float linearFromdB(double aNumber) {
	return (pow(2.0,((aNumber - 87.0) / 6.0))) * 32.767;
}

static int loadFrom(int klattOop) {
    int oop;

	interpreterProxy->success((interpreterProxy->slotSizeOf(klattOop)) == 22);
	if (interpreterProxy->failed()) {
		return 0;
	}
	oop = interpreterProxy->fetchPointerofObject(0, klattOop);
	/* begin checkedFloatPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(oop));
	if (interpreterProxy->failed()) {
		resonators = 0;
		goto l1;
	}
	resonators = ((float *) (interpreterProxy->firstIndexableField(oop)));
l1:	/* end checkedFloatPtrOf: */;
	pitch = interpreterProxy->fetchFloatofObject(2, klattOop);
	t0 = interpreterProxy->fetchIntegerofObject(3, klattOop);
	nper = interpreterProxy->fetchIntegerofObject(4, klattOop);
	nopen = interpreterProxy->fetchIntegerofObject(5, klattOop);
	nmod = interpreterProxy->fetchIntegerofObject(6, klattOop);
	a1 = interpreterProxy->fetchFloatofObject(7, klattOop);
	a2 = interpreterProxy->fetchFloatofObject(8, klattOop);
	x1 = interpreterProxy->fetchFloatofObject(9, klattOop);
	x2 = interpreterProxy->fetchFloatofObject(10, klattOop);
	b1 = interpreterProxy->fetchFloatofObject(11, klattOop);
	c1 = interpreterProxy->fetchFloatofObject(12, klattOop);
	glast = interpreterProxy->fetchFloatofObject(13, klattOop);
	vlast = interpreterProxy->fetchFloatofObject(14, klattOop);
	nlast = interpreterProxy->fetchFloatofObject(15, klattOop);
	periodCount = interpreterProxy->fetchIntegerofObject(16, klattOop);
	samplesCount = interpreterProxy->fetchIntegerofObject(17, klattOop);
	seed = interpreterProxy->fetchIntegerofObject(18, klattOop);
	cascade = interpreterProxy->fetchIntegerofObject(19, klattOop);
	samplesPerFrame = interpreterProxy->fetchIntegerofObject(20, klattOop);
	samplingRate = interpreterProxy->fetchIntegerofObject(21, klattOop);
	return (interpreterProxy->failed()) == 0;
}

static int msg(char *s) {
	fprintf(stderr, "\n%s: %s", moduleName, s);
}


/*	Answer a random number between 0 and 65535. */

static int nextRandom(void) {
	seed = ((seed * 1309) + 13849) & 65535;
	return seed;
}

static void normalizeGlottalPulse(void) {
    float s1;
    int ingore;
    float s2;
    float s0;

	s0 = 0.0;
	s1 = x1;
	s2 = x2;
	for (ingore = 1; ingore <= nopen; ingore += 1) {
		s0 = (a1 * s1) + (a2 * s2);
		s2 = s1;
		s1 = s0;
	}
	if (!(s0 == 0.0)) {
		x1 = (x1 / s0) * 10000.0;
	}
}


/*	Friction-excited parallel vocal tract formants F6, F5, F4, F3, F2,
	outputs added with alternating sign. Sound source for other
	parallel resonators is friction plus first difference of
	voicing waveform. */

static float parallelFrictionBranch(float source) {
	return ((((resonatorvalue(R2fp, source)) - (resonatorvalue(R3fp, source))) + (resonatorvalue(R4fp, source))) - (resonatorvalue(R5fp, source))) + (resonatorvalue(R6fp, source));
}


/*	Voice-excited parallel vocal tract F1, F2, F3, F4, FNP and FTP. */

static float parallelVoicedBranch(float source) {
	return (((((resonatorvalue(R1vp, source)) + (resonatorvalue(R2vp, source))) + (resonatorvalue(R3vp, source))) + (resonatorvalue(R4vp, source))) + (resonatorvalue(Rnpp, source))) + (resonatorvalue(Rtpp, source));
}

static void pitchSynchronousReset(void) {
	if ((frame[F0]) > 0) {
		/* begin voicedPitchSynchronousReset */
		pitch = frame[F0];
		addFlutter();
		/* begin addJitter */
		pitch += ((((nextRandom()) - 32767) * (frame[Jitter])) / 32768.0) * (frame[F0]);
		/* begin addFrequencyDiplophonia */
		if ((periodCount % 2) == 0) {
			pitch += ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
		} else {
			pitch -= ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
		}
		if (pitch < 0) {
			pitch = 0;
		}
		t0 = ((int) (samplingRate / pitch) );
		nmod = t0;
		if ((frame[Voicing]) > 0) {
			nmod = ((int) nmod >> 1);
		}
		nopen = ((int) (t0 * (frame[Ro])) );
		rorark(frame[Ro], frame[Ra], frame[Rk]);
		/* begin addShimmer */
		x1 += ((((nextRandom()) - 32767) * (frame[Shimmer])) / 32768.0) * x1;
		if (x1 > 0) {
			x1 = 0;
		}
		/* begin addAmplitudeDiplophonia */
		if (!((periodCount % 2) == 0)) {
			x1 = x1 * (1.0 - (frame[Diplophonia]));
			if (x1 > 0) {
				x1 = 0;
			}
		}
		periodCount = (periodCount + 1) % 65535;
	} else {
		t0 = 1;
		nmod = t0;
	}
}

EXPORT(int) primitiveSynthesizeFrameIntoStartingAt(void) {
    short * buffer;
    float * aKlattFrame;
    int startIndex;
    int rcvr;
    int bufferOop;
    int oop;
    int oop1;

	/* begin checkedFloatPtrOf: */
	oop = interpreterProxy->stackValue(2);
	interpreterProxy->success(interpreterProxy->isWords(oop));
	if (interpreterProxy->failed()) {
		aKlattFrame = 0;
		goto l1;
	}
	aKlattFrame = ((float *) (interpreterProxy->firstIndexableField(oop)));
l1:	/* end checkedFloatPtrOf: */;
	/* begin checkedShortPtrOf: */
	oop1 = bufferOop = interpreterProxy->stackValue(1);
	interpreterProxy->success(interpreterProxy->isWords(oop1));
	if (interpreterProxy->failed()) {
		buffer = 0;
		goto l2;
	}
	buffer = ((short *) (interpreterProxy->firstIndexableField(oop1)));
l2:	/* end checkedShortPtrOf: */;
	startIndex = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	rcvr = interpreterProxy->stackObjectValue(3);
	if (!(loadFrom(rcvr))) {
		return null;
	}
	interpreterProxy->success(((interpreterProxy->stSizeOf(bufferOop)) * 2) >= samplesPerFrame);
	if (interpreterProxy->failed()) {
		return null;
	}
	synthesizeFrameintostartingAt(aKlattFrame, buffer, startIndex);
	if (!(saveTo(rcvr))) {
		return null;
	}
	interpreterProxy->pop(3);
}

static float quphicosphisinphirphid(float u, float phi, float cosphi, float sinphi, float rphid) {
    float expuphi;

	expuphi = exp(u * phi);
	return (expuphi * ((((rphid * ((u * u) + 1.0)) + u) * sinphi) - cosphi)) + 1.0;
}


/*	Convert formant frequencies and bandwidth into
	resonator difference equation coefficients. */

static void resonatorfrequencybandwidth(int index, float freq, float bw) {
    float a;
    float b;
    float c;
    float r;
    double arg;

	arg = ((0.0 - PI) / samplingRate) * bw;
	r = exp(arg);
	c = 0.0 - (r * r);
	arg = ((PI * 2.0) / samplingRate) * freq;
	b = (r * (cos(arg))) * 2.0;
	a = (1.0 - b) - c;
	resonatorAput(index, a);
	resonatorBput(index, b);
	resonatorCput(index, c);
}


/*	Convert formant frequencies and bandwidth into
	resonator difference equation coefficients. */

static void resonatorfrequencybandwidthgain(int index, float freq, float bw, float gain) {
	resonatorfrequencybandwidth(index, freq, bw);
	resonatorAput(index, (resonators[(index * 5) - 5]) * gain);
}

static float resonatorvalue(int index, float aFloat) {
    float answer;
    float p1;


	/* (p1 between: -100000 and: 100000) ifFalse: [self halt].
	(answer between: -100000 and: 100000) ifFalse: [self halt]. */

	answer = (((resonators[(index * 5) - 5]) * aFloat) + ((resonators[(index * 5) - 4]) * (p1 = resonators[(index * 5) - 2]))) + ((resonators[(index * 5) - 3]) * (resonators[(index * 5) - 1]));
	resonatorP2put(index, p1);
	resonatorP1put(index, answer);
	return answer;
}

static float resonatorA(int index) {
	return resonators[(index * 5) - 5];
}

static void resonatorAput(int index, float aFloat) {
	resonators[(index * 5) - 5] = aFloat;
}

static float resonatorB(int index) {
	return resonators[(index * 5) - 4];
}

static void resonatorBput(int index, float aFloat) {
	resonators[(index * 5) - 4] = aFloat;
}

static float resonatorC(int index) {
	return resonators[(index * 5) - 3];
}

static void resonatorCput(int index, float aFloat) {
	resonators[(index * 5) - 3] = aFloat;
}

static float resonatorP1(int index) {
	return resonators[(index * 5) - 2];
}

static void resonatorP1put(int index, float aFloat) {
	resonators[(index * 5) - 2] = aFloat;
}

static float resonatorP2(int index) {
	return resonators[(index * 5) - 1];
}

static void resonatorP2put(int index, float aFloat) {
	resonators[(index * 5) - 1] = aFloat;
}

static void rorark(float roNumber, float raNumber, float rkNumber) {
    float phi;
    float cosphi;
    float u;
    float sinphi;
    float d;
    float theta;
    int te;
    float rho;
    float ro;
    float ra;
    float rk;
    float rphid;
    float gamma;
    float gammapwr;
    float r;

	te = ((int) (t0 * roNumber) );
	ro = (((double) te )) / (((double) t0 ));
	rk = rkNumber;
	ra = raNumber;
	if (ra <= 0.0) {
		d = 1.0;
	} else {
		r = (1.0 - ro) / ra;
		d = 1.0 - (r / ((exp(r)) - 1.0));
	}
	phi = PI * (rk + 1.0);
	cosphi = cos(phi);
	sinphi = sin(phi);
	rphid = ((ra / ro) * phi) * d;
	u = zeroQphicosphisinphirphid(phi, cosphi, sinphi, rphid);
	theta = phi / te;
	rho = exp(u * theta);
	a1 = (2.0 * (cos(theta))) * rho;
	a2 = 0.0 - (rho * rho);
	x2 = 0.0;
	x1 = rho * (sin(theta));
	gamma = exp(-1.0 / (ra * t0));
	gammapwr = pow(gamma,(t0 - te));
	b1 = gamma;
	c1 = ((1.0 - gamma) * gammapwr) / (1.0 - gammapwr);
	normalizeGlottalPulse();
}

static int saveTo(int origKlattOop) {
    int vlastOop;
    int nlastOop;
    int b1Oop;
    int glastOop;
    int klattOop;
    int pitchOop;
    int a1Oop;
    int a2Oop;
    int x1Oop;
    int x2Oop;
    int c1Oop;

	interpreterProxy->pushRemappableOop(origKlattOop);
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(pitch));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(a1));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(a2));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(x1));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(x2));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(b1));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(c1));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(glast));
	interpreterProxy->pushRemappableOop(interpreterProxy->floatObjectOf(vlast));
	nlastOop = interpreterProxy->floatObjectOf(nlast);
	vlastOop = interpreterProxy->popRemappableOop();
	glastOop = interpreterProxy->popRemappableOop();
	c1Oop = interpreterProxy->popRemappableOop();
	b1Oop = interpreterProxy->popRemappableOop();
	x2Oop = interpreterProxy->popRemappableOop();
	x1Oop = interpreterProxy->popRemappableOop();
	a2Oop = interpreterProxy->popRemappableOop();
	a1Oop = interpreterProxy->popRemappableOop();
	pitchOop = interpreterProxy->popRemappableOop();
	klattOop = interpreterProxy->popRemappableOop();
	if (interpreterProxy->failed()) {
		return 0;
	}
	interpreterProxy->storePointerofObjectwithValue(2, klattOop, pitchOop);
	interpreterProxy->storeIntegerofObjectwithValue(3, klattOop, t0);
	interpreterProxy->storeIntegerofObjectwithValue(4, klattOop, nper);
	interpreterProxy->storeIntegerofObjectwithValue(5, klattOop, nopen);
	interpreterProxy->storeIntegerofObjectwithValue(6, klattOop, nmod);
	interpreterProxy->storePointerofObjectwithValue(7, klattOop, a1Oop);
	interpreterProxy->storePointerofObjectwithValue(8, klattOop, a2Oop);
	interpreterProxy->storePointerofObjectwithValue(9, klattOop, x1Oop);
	interpreterProxy->storePointerofObjectwithValue(10, klattOop, x2Oop);
	interpreterProxy->storePointerofObjectwithValue(11, klattOop, b1Oop);
	interpreterProxy->storePointerofObjectwithValue(12, klattOop, c1Oop);
	interpreterProxy->storePointerofObjectwithValue(13, klattOop, glastOop);
	interpreterProxy->storePointerofObjectwithValue(14, klattOop, vlastOop);
	interpreterProxy->storePointerofObjectwithValue(15, klattOop, nlastOop);
	interpreterProxy->storeIntegerofObjectwithValue(16, klattOop, periodCount);
	interpreterProxy->storeIntegerofObjectwithValue(17, klattOop, samplesCount);
	interpreterProxy->storeIntegerofObjectwithValue(18, klattOop, seed);
	return (interpreterProxy->failed()) == 0;
}

static void setCurrentFrame(float *aKlattFrame) {
    float ampF4F;
    float ampFNV;
    float ampF6F;
    float ampFTV;
    float ampF2V;
    float ampF1V;
    float ampF4V;
    float ampF3V;
    float ampF2F;
    float ampF3F;
    float ampF5F;


	/* Fudge factors... */

	frame = aKlattFrame;

	/* -4.44 dB */

	ampFNV = (linearFromdB(frame[Anv])) * 0.6;

	/* -4.44 dB */

	ampFTV = (linearFromdB(frame[Atv])) * 0.6;

	/* -7.96 dB */

	ampF1V = (linearFromdB(frame[A1v])) * 0.4;

	/* -16.5 dB */

	ampF2V = (linearFromdB(frame[A2v])) * 0.15;

	/* -24.4 dB */

	ampF3V = (linearFromdB(frame[A3v])) * 0.06;

	/* -28.0 dB */

	ampF4V = (linearFromdB(frame[A4v])) * 0.04;

	/* -16.5 dB */

	ampF2F = (linearFromdB(frame[A2f])) * 0.15;

	/* -24.4 dB */

	ampF3F = (linearFromdB(frame[A3f])) * 0.06;

	/* -28.0 dB */

	ampF4F = (linearFromdB(frame[A4f])) * 0.04;

	/* -33.2 dB */

	ampF5F = (linearFromdB(frame[A5f])) * 0.022;

	/* -30.5 dB */
	/* Set coefficients of variable cascade resonators */

	ampF6F = (linearFromdB(frame[A6f])) * 0.03;
	if (cascade >= 8) {
		if (samplingRate >= 16000) {
			resonatorfrequencybandwidth(R8c, 7500, 600);
		} else {
			cascade = 6;
		}
	}
	if (cascade >= 7) {
		if (samplingRate >= 16000) {
			resonatorfrequencybandwidth(R7c, 6500, 500);
		} else {
			cascade = 6;
		}
	}
	if (cascade >= 6) {
		resonatorfrequencybandwidth(R6c, frame[F6], frame[B6]);
	}
	if (cascade >= 5) {
		resonatorfrequencybandwidth(R5c, frame[F5], frame[B5]);
	}
	resonatorfrequencybandwidth(R4c, frame[F4], frame[B4]);
	resonatorfrequencybandwidth(R3c, frame[F3], frame[B3]);
	resonatorfrequencybandwidth(R2c, frame[F2], frame[B2]);
	resonatorfrequencybandwidth(R1c, frame[F1], frame[B1]);
	resonatorfrequencybandwidth(Rnpc, frame[Fnp], frame[Bnp]);
	resonatorfrequencybandwidth(Rtpc, frame[Ftp], frame[Btp]);
	antiResonatorfrequencybandwidth(Rnz, frame[Fnz], frame[Bnz]);
	antiResonatorfrequencybandwidth(Rtz, frame[Ftz], frame[Btz]);
	resonatorfrequencybandwidthgain(Rnpp, frame[Fnp], frame[Bnp], ampFNV);
	resonatorfrequencybandwidthgain(Rtpp, frame[Ftp], frame[Btp], ampFTV);
	resonatorfrequencybandwidthgain(R1vp, frame[F1], frame[B1], ampF1V);
	resonatorfrequencybandwidthgain(R2vp, frame[F2], frame[B2], ampF2V);
	resonatorfrequencybandwidthgain(R3vp, frame[F3], frame[B3], ampF3V);
	resonatorfrequencybandwidthgain(R4vp, frame[F4], frame[B4], ampF4V);
	resonatorfrequencybandwidthgain(R2fp, frame[F2], frame[B2f], ampF2F);
	resonatorfrequencybandwidthgain(R3fp, frame[F3], frame[B3f], ampF3F);
	resonatorfrequencybandwidthgain(R4fp, frame[F4], frame[B4f], ampF4F);
	resonatorfrequencybandwidthgain(R5fp, frame[F5], frame[B5f], ampF5F);
	resonatorfrequencybandwidthgain(R6fp, frame[F6], frame[B6f], ampF6F);
}


/*	Note: This is coded so that is can be run from Squeak. */

EXPORT(int) setInterpreter(struct VirtualMachine* anInterpreter) {
    int ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}

static void synthesizeFrameintostartingAt(float *aKlattFrame, short *buffer, int startIndex) {
    float parGlotout;
    float source;
    float bypass;
    int index;
    float noise;
    float aspiration;
    float voicing;
    float ampGain;
    float frictionNoise;
    float turbulence;
    float gain;
    float voice;
    float aspirationNoise;
    float glotout;
    float out;
    int top;
    float parVoicing;
    int temp;
    float friction;

	setCurrentFrame(aKlattFrame);
	if (pitch > 0) {
		voicing = linearFromdB((frame[Voicing]) - 7);
		parVoicing = linearFromdB(frame[Voicing]);
		turbulence = (linearFromdB(frame[Turbulence])) * 0.1;
	} else {
		voicing = parVoicing = turbulence = 0.0;
	}
	friction = (linearFromdB(frame[Friction])) * 0.25;
	aspiration = (linearFromdB(frame[Aspiration])) * 0.05;

	/* -26.0 dB */
	/* Flod overall gain into output resonator (low-pass filter) */

	bypass = (linearFromdB(frame[Bypass])) * 0.05;
	gain = (frame[Gain]) - 3;
	if (gain <= 0) {
		gain = 57;
	}
	ampGain = linearFromdB(gain);
	resonatorfrequencybandwidthgain(Rout, 0, samplingRate, ampGain);
	noise = nlast;
	index = startIndex;
	top = (samplesPerFrame + startIndex) - 1;
	while (index <= top) {

		/* radom number between -8196.0 and 8196.0 */
		/* Tilt down noise spectrum by soft low-pass filter having
		 a pole near the origin in the z-plane. */

		noise = (((double) ((nextRandom()) - 32768) )) / 4.0;
		noise += 0.75 * nlast;

		/* Amplitude modulate noise (reduce noise amplitude during second
		 half of glottal period) if voicing  simultaneously present. */

		nlast = noise;
		if (nper > nmod) {
			noise = noise * 0.5;
		}

		/* Compute voicing waveform. */

		frictionNoise = friction * noise;
		voice = glottalSource();

		/* Add turbulence during glottal open phase.
		 Use random rather than noise because noise is low-passed. */

		vlast = voice;
		if (nper < nopen) {
			voice += (turbulence * (((double) ((nextRandom()) - 32768) ))) / 4.0;
		}
		glotout = voicing * voice;

		/* Compute aspiration amplitude and add to voicing source. */

		parGlotout = parVoicing * voice;
		aspirationNoise = aspiration * noise;
		glotout += aspirationNoise;

		/* Cascade vocal tract, excited by laryngeal sources.
		 Nasal antiresonator, nasal resonator, trachearl antirresonator,
		 tracheal resonator, then formants F8, F7, F6, F5, F4, F3, F2, F1. */

		parGlotout += aspirationNoise;

		/* Voice-excited parallel vocal tract F1, F2, F3, F4, FNP and FTP. */

		out = cascadeBranch(glotout);

		/* Source is voicing plus aspiration. */

		source = parGlotout;

		/* Friction-excited parallel vocal tract formants F6, F5, F4, F3, F2,
		 outputs added with alternating sign. Sound source for other
		 parallel resonators is friction plus first difference of
		 voicing waveform. */

		out += parallelVoicedBranch(source);
		source = (frictionNoise + parGlotout) - glast;
		glast = parGlotout;

		/* Apply bypas and output low-pass filter */

		out = (parallelFrictionBranch(source)) - out;
		out = (bypass * source) - out;
		out = resonatorvalue(Rout, out);
		temp = ((int) (out * ampGain) );
		if (temp < -32768) {
			temp = -32768;
		}
		if (temp > 32767) {
			temp = 32767;
		}
		buffer[index - 1] = temp;
		index += 1;
		samplesCount += 1;
	}
}

static void voicedPitchSynchronousReset(void) {

	/* Add flutter and jitter (F0 perturbations). */

	pitch = frame[F0];
	addFlutter();
	/* begin addJitter */
	pitch += ((((nextRandom()) - 32767) * (frame[Jitter])) / 32768.0) * (frame[F0]);
	/* begin addFrequencyDiplophonia */
	if ((periodCount % 2) == 0) {
		pitch += ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
	} else {
		pitch -= ((frame[Diplophonia]) * (frame[F0])) * (1.0 - (frame[Ro]));
	}
	if (pitch < 0) {
		pitch = 0;
	}

	/* Duration of period before amplitude modulation. */

	t0 = ((int) (samplingRate / pitch) );
	nmod = t0;
	if ((frame[Voicing]) > 0) {
		nmod = ((int) nmod >> 1);
	}

	/* Set the LF glottal pulse model parameters. */

	nopen = ((int) (t0 * (frame[Ro])) );
	rorark(frame[Ro], frame[Ra], frame[Rk]);
	/* begin addShimmer */
	x1 += ((((nextRandom()) - 32767) * (frame[Shimmer])) / 32768.0) * x1;
	if (x1 > 0) {
		x1 = 0;
	}
	/* begin addAmplitudeDiplophonia */
	if (!((periodCount % 2) == 0)) {
		x1 = x1 * (1.0 - (frame[Diplophonia]));
		if (x1 > 0) {
			x1 = 0;
		}
	}
}

static float zeroQphicosphisinphirphid(float phi, float cosphi, float sinphi, float rphid) {
    float qzero;
    float uc;
    float qc;
    float qa;
    float ua;
    float ub;
    float qb;

	qzero = quphicosphisinphirphid(0, phi, cosphi, sinphi, rphid);
	if (qzero > 0) {
		ua = 0;
		ub = 1;
		qa = qzero;
		qb = quphicosphisinphirphid(ub, phi, cosphi, sinphi, rphid);
		while (qb > 0) {
			ua = ub;
			qa = qb;
			ub = ub * 2;
			qb = quphicosphisinphirphid(ub, phi, cosphi, sinphi, rphid);
		}
	} else {
		ua = -1;
		ub = 0;
		qa = quphicosphisinphirphid(ua, phi, cosphi, sinphi, rphid);
		qb = qzero;
		while (qa < 0) {
			ub = ua;
			qb = qa;
			ua = ua * 2;
			qa = quphicosphisinphirphid(ua, phi, cosphi, sinphi, rphid);
		}
	}
	while ((ub - ua) > Epsilon) {
		uc = (ub + ua) / 2.0;
		qc = quphicosphisinphirphid(uc, phi, cosphi, sinphi, rphid);
		if (qc > 0) {
			ua = uc;
			qa = qc;
		} else {
			ub = uc;
			qb = qc;
		}
	}
	return (ub + ua) / 2.0;
}


#ifdef SQUEAK_BUILTIN_PLUGIN


void* Klatt_exports[][3] = {
	{"Klatt", "getModuleName", (void*)getModuleName},
	{"Klatt", "primitiveSynthesizeFrameIntoStartingAt", (void*)primitiveSynthesizeFrameIntoStartingAt},
	{"Klatt", "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};


#endif /* ifdef SQ_BUILTIN_PLUGIN */

