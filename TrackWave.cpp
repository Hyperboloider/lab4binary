#include "TrackWave.h"

void TrackWave::endError(string msg) {
	cout << "Error: " << msg << endl;
	exit(1);
}

void TrackWave::Reader() {
	in = fopen((filesystem::current_path().string() + "\\" + from).c_str(), "rb");
	cout << filesystem::current_path().string() + "\\" + from << endl;

	if (!in) {
		endError("Can't open the file");
	}
	else {
		fread(&header, sizeof(header), 1, in);

		fread(&info, sizeof(info), 1, in);

		fread(&dataInfo, sizeof(dataInfo), 1, in);

		sample_size = info.bitsPerSample / 8;
		samples_count = dataInfo.subchunk2Size * 8 / info.bitsPerSample;

		if (info.bitsPerSample == 8) {
			audio8 = new int8_t[samples_count];
			for (int i = 0; i < samples_count; i++)
				fread(&audio8[i], sample_size, 1, in);
		}
		else if (info.bitsPerSample == 16) {
			audio16 = new int16_t[samples_count];
			for (int i = 0; i < samples_count; i++)
				fread(&audio16[i], sample_size, 1, in);
		}

		fclose(in);
	}
}

void TrackWave::scale_track(float scale) {
	int input_samples = dataInfo.subchunk2Size * 8 / info.bitsPerSample;
	int output_samples = input_samples * scale;
	float step = round(1 / scale * 1000) / 1000;
	int16_t* new_data = new int16_t[output_samples];
	for (float i = 0; i < input_samples; i += step) {
		int index_prev = int(i);
		int index_next = int(i) + 1;
		int16_t value = interpolate(index_prev, audio16[index_prev], index_next, audio16[index_next], i);
		new_data[int(i * scale)] = value;
	}
}

template <typename T>
T interpolate(int32_t x0,  T y0, int32_t x1, T y1, float x) {
	return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}