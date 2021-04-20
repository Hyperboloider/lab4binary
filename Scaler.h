#pragma once
#include <iostream>
using namespace std;

class Scaler
{
	int16_t* scale_fun(int16_t* audio, int length, float scale);
	int16_t* scale_fun_channels(int16_t* audio, int length, float scale);
	int8_t getRight(int16_t);
	int8_t getLeft(int16_t);
	int16_t joinCh(int8_t, int8_t);
	template <typename T>
	T interpolate(int32_t x0, T y0, int32_t x1, T y1, float x);
public:
	int16_t* scale_track(int16_t* audio, int num_channels, float scale, int sample_size, int samples_count);
};
