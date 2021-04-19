#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Scaler.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
#include <fstream>

//using namespace std;

struct riffHeader {
	int32_t chunkId;  
	int32_t chunkSize; 				   
	int32_t format;   
};

struct subChunk1 {
	int32_t subchunk1Id;   
	int32_t subchunk1Size;
	int16_t audioFormat;   
	int16_t numChannels;  
	int32_t sampleRate;    
	int32_t byteRate;      
	int16_t blockAlign;   
	int16_t bitsPerSample; 
};

struct subChunk2 {
	int32_t subchunk2Id;   
	int32_t subchunk2Size; 
};

class TrackWave
{
private:
	
	riffHeader header;
	subChunk1 info;
	subChunk2 dataInfo;
	int8_t* audio8;
	int16_t* audio16;
	string from;
	string to;
	string path;
	FILE* out = NULL;
	FILE* in = NULL;
	int samples_count;
	int sample_size;
	int32_t swap_int32_t(int32_t);
	void endError(string);
	void Reader();
public:
	TrackWave(string, string);
	void scaleFile();
};
/*
template <typename T>
T interpolate(int32_t x0, T y0, int32_t x1, T y1, float x);*/
