#include "LcmUtil.h"

// size is size in bytes
// Fills params with integers extracted from the buffer
void unpackBuffer(const uchar* buffer, const uint size, IntBuffer& params){
	uint* intBuffer = (uint*)buffer;
	uint len = size/sizeof(uint);
	for(int i = 0; i < len; i++){
		params.push_back(intBuffer[i]);
	}
}

// returns a chunk of memory (buffer) and it's size in bytes
void packBuffer(IntBuffer& params, uchar*& buffer, uint& size){
	uint* intBuffer = new uint[params.size()];
	for(uint i = 0; i < params.size(); i++){
		intBuffer[i] = params[i];
	}

	size = sizeof(uint) * params.size();
	buffer = (uchar*)intBuffer;
}
