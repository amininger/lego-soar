/*
 * LcmUtil.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 */

#ifndef LCMUTIL_H_
#define LCMUTIL_H_

#include <vector>
#include <iostream>

typedef std::vector<unsigned int> IntBuffer;
typedef IntBuffer::iterator IntBufferIt;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

using namespace std;

inline void printBinary32(uint i){
	for(int d = 31; d >= 0; d--){
		cout << (i & (1 << d) ? "1" : "0");
	}
	cout << endl;
}

inline void printBinary16(ushort i){
	for(int d = 15; d >= 0; d--){
		cout << (i & (1 << d) ? "1" : "0");
	}
	cout << endl;
}

inline void printBinary8(uchar i){
	for(int d = 7; d >= 0; d--){
		cout << (i & (1 << d) ? "1" : "0");
	}
	cout << endl;
}


// size is size in bytes
// Fills params with integers extracted from the buffer
void unpackBuffer(const uchar* buffer, const uint size, IntBuffer& params);

// returns a chunk of memory (buffer) and it's size in bytes
void packBuffer(IntBuffer& params, uchar*& buffer, uint& size);

inline uint packShorts(ushort a, ushort b){
	return (a << 16) | b;
}

inline void unpackShorts(uint i, ushort& a, ushort& b){
	a = (i >> 16) & 0xffff;
	b = i & 0xffff;
}
inline uint packBytes(uchar a, uchar b, uchar c, uchar d){
	return (a << 24) | (b << 16) | (c << 8) | d;
}

inline uint packBytes(uchar* a){
	return (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

inline void unpackBytes(uint i, uchar& a, uchar& b, uchar& c, uchar& d){
	a = (i >> 24) & 0xff;
	b = (i >> 16) & 0xff;
	c = (i >> 8) & 0xff;
	d = i & 0xff;
}

inline void unpackBytes(uint i, uchar* a){
	a[0] = (i >> 24) & 0xff;
	a[1] = (i >> 16) & 0xff;
	a[2] = (i >> 8) & 0xff;
	a[3] = i & 0xff;
}

#endif /* LCMUTIL_H_ */
