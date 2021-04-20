#include "TrackWave.h"

TrackWave::TrackWave(string from, string to) {
	this->path = filesystem::current_path().string();
	this->from = from;
	this->to = to;
	Reader();
}

void TrackWave::Reader() {
	in = fopen((filesystem::current_path().string() + "\\" + from).c_str(), "rb");
	cout << filesystem::current_path().string() + "\\" + from << endl;

	if (!in) {
		endError("Can't open the file");
	}
	else {
		fread(&header, sizeof(header), 1, in);

		if (header.chunkId != 1179011410) endError("no RIFF signature found");
		cout << "supposed: " << static_cast<int64_t>(header.chunkSize) + 8 << " real: " << filesystem::file_size(path + "\\" + from) << endl;
		if (static_cast<int64_t>(header.chunkSize) + 8 != filesystem::file_size(path + "\\" + from)) endError("File size is incorrect");
		if (header.format != 1163280727) endError("unsupportable filetype");


		fread(&info, sizeof(info), 1, in);
		if (info.subchunk1Id != 544501094) endError("subchunk may be corrupted");

		fread(&dataInfo, sizeof(dataInfo), 1, in);


		sample_size = info.bitsPerSample / 8;
		samples_count = dataInfo.subchunk2Size * 8 / info.bitsPerSample;

		if (info.bitsPerSample == 16) {
			audio16 = new int16_t[samples_count];
			for (int i = 0; i < samples_count; i++)
				fread(&audio16[i], sample_size, 1, in);
		}
		else if(info.bitsPerSample == 8 && info.numChannels != 2){
			audio8 = new int8_t[samples_count];
			for (int i = 0; i < samples_count; i++)
				fread(&audio8[i], sample_size, 1, in);
			
		}
		else {
			endError("Incorrect size of samples or num channels!");
		}

		fclose(in);
	}
}

void TrackWave::scaleFile(float s) {
	float scale = s;

	header.chunkSize += int(dataInfo.subchunk2Size * (scale - 1));
	dataInfo.subchunk2Size *= scale;

	Scaler scaler;
	if (sample_size == 1) {
		audio8 = scaler.scale_track(audio8, info.numChannels, scale, sample_size, samples_count);
	}
	else if (sample_size == 2) {
		audio16 = scaler.scale_track(audio16, info.numChannels, scale, sample_size, samples_count);
	}	
	samples_count *= scale;
	
	Writer();
}

void TrackWave::Writer() {
	out = fopen((filesystem::current_path().string() + "\\" + to).c_str(), "wb");

	fwrite(&header, sizeof(header), 1, out);
	fwrite(&info, sizeof(info), 1, out);
	fwrite(&dataInfo, sizeof(dataInfo), 1, out);
	if (sample_size == 1) {
		for (int i = 0; i < samples_count; i++) {
			int8_t buffer = audio8[i];
			fwrite(&buffer, sizeof(audio8[i]), 1, out);
		}
	}
	else if (sample_size == 2) {
		for (int i = 0; i < samples_count; i++) {
			int16_t buffer = audio16[i];
			fwrite(&buffer, sizeof(audio16[i]), 1, out);
		}
	}
	fclose(out);
}

void TrackWave::showInfo() {

	cout << "chunkId: 0x" << hex << swap_int32_t(header.chunkId) << endl << dec;
	cout << "chunkSize: " << static_cast<int64_t>(header.chunkSize) + 8 << endl;
	cout << "format: 0x" << hex << swap_int32_t(header.format) << endl << dec;

	cout << "subchunk1Id: 0x" << hex << swap_int32_t(info.subchunk1Id) << dec << endl;
	cout << "subchunkSize: " << info.subchunk1Size << endl;
	cout << "audioFormat: " << info.audioFormat << endl;
	cout << "numChannels: " << info.numChannels << endl;
	cout << "sampleRate: " << info.sampleRate << endl;
	cout << "byteRate: " << info.byteRate << endl;
	cout << "blockAlign: " << info.blockAlign << endl;
	cout << "bitsPerSample: " << info.bitsPerSample << endl;

	cout << "dataChunkId: 0x" << hex << swap_int32_t(dataInfo.subchunk2Id) << dec << endl;
	cout << "dataChunkSize" << dataInfo.subchunk2Size << endl;
}

void TrackWave::endError(string msg) {
	cout << "Error: " << msg << endl;
	exit(1);
}

int32_t TrackWave::swap_int32_t(int32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | ((val >> 16) & 0xFFFF);
}