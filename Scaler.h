#pragma once
#include <iostream>
using namespace std;
template <class T>
class Scaler
{
	T* scale_fun(T* audio, int length,float scale);
	T interpolate(int32_t x0, T y0, int32_t x1, T y1, float x);
	int8_t getRight(int16_t);
	int8_t getLeft(int16_t);
	int16_t joinCh(int8_t, int8_t);
public:
	T* scale_track(T* audio, int num_channels, float scale, int sample_size, int samples_count);
	//template <typename T>
	//T* scale_track(T* audio);
	//template <typename T>
	//T* scale_fun(T* audio, int length);
	//template <typename T>
	//T interpolate(int32_t x0, T y0, int32_t x1, T y1, float x);
};
template <typename T>
T* Scaler<T>::scale_track(T* audio, int num_channels, float scale, int sample_size, int samples_count) {
	if (num_channels == 1) {
		T* new_data = scale_fun(audio, samples_count, scale);
		return new_data;
	}
	else if (num_channels == 2) {
		int insertPosition = 0;
		int  out_samples = samples_count * scale;
		T* new_data = new T[out_samples];
		
		for (float i = 0; i < samples_count; i += 1/scale ) {
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

		/*
		int channel_len = samples_count / 2;
		T* left_channel = new T[channel_len];
		T* right_channel = new T[channel_len];
		for (int i = 0; i < channel_len; i++) {
			left_channel[i] = audio[2 * i];
			right_channel[i] = audio[2 * i + 1];
		}
		int  out_samples = samples_count * scale;
		T* new_data = new T[out_samples];
		T* new_left_channel = scale_fun(left_channel, channel_len, scale);
		T* new_right_channel = scale_fun(right_channel, channel_len, scale);
		for (int i = 0; i < int(channel_len * scale); i++) {
			new_data[2 * i] = new_left_channel[i];
			new_data[2 * i + 1] = new_right_channel[i];
		}
		*/
		return new_data;
	}
}
template <typename T>
T* Scaler<T>::scale_fun(T* audio, int length, float scale) {
	T* new_data = new T[int(length * scale)];
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
T Scaler<T>::interpolate(int32_t x0, T y0, int32_t x1, T y1, float x) {
	return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

template <typename T>
int8_t Scaler<T>::getLeft(int16_t val) {
	return static_cast<int8_t>(val >> 8);
}

template <typename T>
int8_t Scaler<T>::getRight(int16_t val) {
	return static_cast<int8_t>(val & 0xff);
}

template <typename T>
int16_t Scaler<T>::joinCh(int8_t left, int8_t right) {
	return static_cast<int16_t>((left << 8) + (right & 0x00ff));
}