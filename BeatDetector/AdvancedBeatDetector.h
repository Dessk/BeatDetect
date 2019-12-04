
double SumaPrzedzial(double* arr, int index) {
	double suma = 0.0;
	for (int i = index*32; i < (index + 1) * 32; i++) {
		suma += arr[i];
	}
	return suma;
}

void AdvancedBeatDetector(FILE* file) {
	long aktualny = ftell(file);
	fseek(file, 0, SEEK_END);
	long koniec = ftell(file);
	fseek(file, aktualny, SEEK_SET);

	short int right[1024];
	short int left[1024];

	

	double separate_to_sub_chunks[32];

	double** history_subband = malloc(32 * sizeof(double*));
	for (int i = 0; i < 32; i++) {
		history_subband[i] = calloc(43,sizeof(double));
	}


	double* module_square = malloc(1024*sizeof(double));


	printf("======ADVANCED=====\n");
	while (ftell(file) != koniec) {
		fftw_complex* c_buffer_complex = fftw_malloc(1024 * sizeof(fftw_complex));
		fftw_complex* out = fftw_malloc(1024 * sizeof(fftw_complex));
		fftw_plan plan;
		int index = 0;
		for (int i = 0; i < 1024; i++) {
			fread(left + i, 2, 1, file);
			fread(right + i, 2, 1, file);
		}
		for (int i = 0; i < 1024; i++) {
			(*(c_buffer_complex + i))[0] = (double)left[i] / 32786.0;
			(*(c_buffer_complex + i))[1] = (double)right[i] / 32786.0;
		}
		plan = fftw_plan_dft_1d(1024, c_buffer_complex, out, FFTW_FORWARD, FFTW_ESTIMATE);
		fftw_execute(plan);

		for (int i = 0; i < 1024; i++) {
			module_square[i] = pow((*(out + i))[0], 2.0) + pow((*(out + i))[1], 2.0);
		}

		for (int i = 0; i < 32; i++) {
			separate_to_sub_chunks[i] = (32.0 / 1024.0) * SumaPrzedzial(module_square, i);
		}

		double E_AVG[32];
		for (int i = 0; i < 32; i++) {
			E_AVG[i] = (1.0/43.0)*Sum_Standard(history_subband[i], 43);
		}

		for (int i = 0; i < 32; i++) {
			for (int k = 42; k > 0; k--) {
				history_subband[i][k] = history_subband[i][k - 1];
			}
		}
		for (int i = 0; i < 32; i++) {
			history_subband[i][0] = separate_to_sub_chunks[i];
			if (history_subband[i][0] > 240.0 * E_AVG[i]) {
				printf("peak\n %f %f\n", history_subband[i][0] , 250.0 * E_AVG[i]);
			}

		}
		fftw_free(c_buffer_complex);
		fftw_free(out);
	}
	free(module_square);
	free(history_subband);
}
