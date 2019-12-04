#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include "FFTW_LIBx32/fftw3.h"
#include "WavReader.h"
#include "StandardBeatDetection.h"
#include "AdvancedBeatDetector.h"

int main(int args, char** argv) {

	FILE* file = ReadWAV("test6.wav");
	StandardBeatDetect(file);
	//AdvancedBeatDetector(file);
	fclose(file);

	return 0;
}