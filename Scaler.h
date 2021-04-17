#pragma once
#include <iostream>
using namespace std;

class Scaler
{
protected:
	float scale;
	int num_channels;
	int sample_size;
	int samples_count;
	int16_t* scale_track(int16_t* audio);
	int16_t* scale_fun(int16_t* audio, int length);
	template <typename T>
	T interpolate(int32_t x0, T y0, int32_t x1, T y1, float x);
	//template <typename T>
	//T* scale_track(T* audio);
	//template <typename T>
	//T* scale_fun(T* audio, int length);
	//template <typename T>
	//T interpolate(int32_t x0, T y0, int32_t x1, T y1, float x);
};

