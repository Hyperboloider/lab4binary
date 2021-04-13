#include "Scaler.h"


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