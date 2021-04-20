#include "TrackWave.h"

int main() {
	string file_in, file_out;
	cout << "Input name of file: "; cin >> file_in;
	cout << "Input name of new file: "; cin >> file_out;

	float scale;
	cout << "scaleFactor: ";
	cin >> scale;

	TrackWave track(file_in, file_out);
	track.showInfo();
	track.scaleFile(scale);

	cout << "\nDone\n";
	return 0;
}