#include "Scaler.h"

/*
template <typename T>
T* Scaler::scale_track(T* audio) {
	T* new_data = new T[int(samples_count * scale)];
	cout << "HEY";
	if (num_channels == 1) {
		new_data = scale_fun(audio, samples_count);
	}
	else if (num_channels == 2) {
		int channel_len = samples_count / 2;
		T* left_channel = new T[channel_len];
		T* right_channel = new T[channel_len];
		for (int i = 0; i < channel_len; i++) {
			left_channel[i] = audio[2 * i];
			right_channel[i] = audio[2 * i + 1];
		}
		left_channel = scale_fun(left_channel, channel_len);
		right_channel = scale_fun(right_channel, channel_len);
		for (int i = 0; i < int(channel_len * scale); i++) {
			new_data[2 * i] = left_channel[i];
			new_data[2 * i + 1] = right_channel[i];
		}
	}
	return new_data;
}
template <typename T>
T* Scaler::scale_fun(T* audio, int length) {
	T* new_data = new T[int(length * scale)];
	float step = round(1 / scale * 1000) / 1000;
	for (float i = 0; i < length; i += step) {
		int index_prev = int(i);
		int index_next = int(i) + 1;
		new_data[int(i/step)] = interpolate(index_prev, audio[index_prev], index_next, audio[index_next], i);
		cout << new_data[int(i / step)] << endl;
	}
	return new_data;
}

template <typename T>
T Scaler::interpolate(int32_t x0, T y0, int32_t x1, T y1, float x) {
	return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}
*/

int16_t* Scaler::scale_track(int16_t* audio, int num_channels, float scale) {
	if (num_channels == 1) {
		int16_t* new_data = scale_fun(audio, samples_count, scale);
		return new_data;
	}
	else if (num_channels == 2) {
		int channel_len = samples_count / 2;
		int16_t* left_channel = new int16_t[channel_len];
		int16_t* right_channel = new int16_t[channel_len];
		for (int i = 0; i < channel_len; i++) {
			left_channel[i] = audio[2 * i];
			right_channel[i] = audio[2 * i + 1];
		}
		int  out_samples = samples_count * scale;
		int16_t* new_data = new int16_t[out_samples];
		int16_t* new_left_channel = scale_fun(left_channel, channel_len, scale);
		int16_t* new_right_channel = scale_fun(right_channel, channel_len, scale);
		for (int i = 0; i < int(channel_len * scale); i++) {
			new_data[2 * i] = new_left_channel[i];
			new_data[2 * i + 1] = new_right_channel[i];
		}
		return new_data;
	}
}

int16_t* Scaler::scale_fun(int16_t* audio, int length, float scale) {
	int16_t* new_data = new int16_t[int(length * scale)];
	double step = round(1 / scale * 1000) / 1000;
	int out_samples = length * scale;
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