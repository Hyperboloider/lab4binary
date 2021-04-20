#include "Scaler.h"


int8_t Scaler::getLeft(int16_t val) {
	return static_cast<int8_t>(val >> 8);
}


int8_t Scaler::getRight(int16_t val) {
	return static_cast<int8_t>(val & 0xff);
}


int16_t Scaler::joinCh(int8_t left, int8_t right) {
	return static_cast<int16_t>((left << 8) + (right & 0x00ff));
}