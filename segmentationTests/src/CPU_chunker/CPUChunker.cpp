/**
 * CPUChunker.cpp
 *
 *  Created on: Jul 8, 2013
 *      Author: Zahari Dichev <zaharidichev@gmail.com>
 */

#include "CPUChunker.h"

CPUChunker::CPUChunker(size_t min, size_t max) {
	this->min = min;
	this->max = max;
}

CPUChunker::~CPUChunker() {
	// TODO Auto-generated destructor stub
}

std::vector<shared_ptr<Chunk> > CPUChunker::chunkData(shared_ptr<BYTE> data, size_t len) {
	std::vector<shared_ptr<Chunk> > finalChunks;

	rabinData rd;
	initWindow(&rd, 0xbfe6b8a5bf378d83);

	byteBuffer b;
	initBuffer(&b);

	POLY_64 fingerprint = 0; // the fingerprint that will be used
	size_t pos = 0;
	size_t lastBP = 0;
	for (; pos < len; ++pos) {
		fingerprint = update(&rd, data.get()[pos], fingerprint, &b);

		if (pos - lastBP < this->min) {
			continue;
		}

		if (this->min <= pos - lastBP <= this->max) {

			if (bitMod(fingerprint, 8192) == 8192 - 1) {

				shared_ptr<Chunk> tmpPtr(new Chunk(lastBP, pos));
				shared_ptr<BYTE> digest((BYTE*) malloc(20));
				SHA1(data.get() + lastBP, tmpPtr.get()->getSize(), digest.get());
				tmpPtr.get()->setHash(digest);
				finalChunks.push_back(tmpPtr);
				lastBP = pos;
				pos = pos + this->min;
				continue;
			}
			continue;
		}

		if (pos - lastBP == max) {
			shared_ptr<Chunk> tmpPtr(new Chunk(lastBP, pos));
			shared_ptr<BYTE> digest((BYTE*) malloc(20));
			SHA1(data.get() + lastBP, tmpPtr.get()->getSize(), digest.get());
			tmpPtr.get()->setHash(digest);
			finalChunks.push_back(tmpPtr);
			lastBP = pos;
			pos = pos + this->min;

			continue;

		}

	}

	shared_ptr<Chunk> tmpPtr(new Chunk(lastBP, pos));
	shared_ptr<BYTE> digest((BYTE*) malloc(20));
	SHA1(data.get() + lastBP, tmpPtr.get()->getSize(), digest.get());
	tmpPtr.get()->setHash(digest);
	finalChunks.push_back(tmpPtr);
	return finalChunks;
}
