#include "Scaler.h"


int16_t* Scaler::scale_track(int16_t* audio, int num_channels, float scale, int sample_size, int samples_count) {
	if (num_channels == 1) {
		int16_t* new_data = scale_fun(audio, samples_count, scale);
		return new_data;
	}
	else if (num_channels == 2) {
		int16_t* new_data = scale_fun_channels(audio, samples_count, scale);
		return new_data;
	}
}

int16_t* Scaler::scale_fun_channels(int16_t* audio, int samples_count, float scale) {
	int insertPosition = 0;
	int  out_samples = samples_count * scale;
	int16_t* new_data = new int16_t[out_samples];
	for (float i = 0; i < samples_count; i += 1 / scale) {
		int index_prev = int(i);
		int index_next = int(i) + 1;

		int8_t prevL = getLeft(audio[index_prev]);
		int8_t prevR = getRight(audio[index_prev]);
		int8_t nextL = getLeft(audio[index_next]);
		int8_t nextR = getRight(audio[index_next]);

		int8_t newL = interpolate(index_prev, prevL, index_next, nextL, i);
		int8_t newR = interpolate(index_prev, prevR, index_next, nextL, i);

		int16_t newValue = joinCh(newL, newR);

		new_data[insertPosition] = newValue;
		insertPosition++;
	}
	return new_data;
}

int16_t* Scaler::scale_fun(int16_t* audio, int length, float scale) {
	int out_samples = length * scale;
	int16_t* new_data = new int16_t[out_samples];

	double step = 1 / scale;
	for (long int i = 0; i < out_samples; i++) {
		double index_input = i * step;
		int index_prev = index_input;
		int index_next = index_input + 1;
		new_data[i] = interpolate(index_prev, audio[index_prev], index_next, audio[index_next], index_input);
	}

	return new_data;
}

template <typename T>
T Scaler::interpolate(int32_t x0, T y0, int32_t x1, T y1, float x) {
	return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}


int8_t Scaler::getLeft(int16_t val) {
	return static_cast<int8_t>(val >> 8);
}


int8_t Scaler::getRight(int16_t val) {
	return static_cast<int8_t>(val & 0xff);
}


int16_t Scaler::joinCh(int8_t left, int8_t right) {
	return static_cast<int16_t>((left << 8) + (right & 0x00ff));
}