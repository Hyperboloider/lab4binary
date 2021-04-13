#include "TrackWave.h"

int main() {
	string file_in, file_out;
	cout << "Input name of file: "; cin >> file_in;
	cout << "Input name of new file: "; cin >> file_out;
	TrackWave track(file_in, file_out);
	track.scaleFile();
	cout << "\nDone\n";
	return 0;
}