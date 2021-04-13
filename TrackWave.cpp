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