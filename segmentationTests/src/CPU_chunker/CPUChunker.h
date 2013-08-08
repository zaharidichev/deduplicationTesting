/**
 * CPUChunker.h
 *
 * This class implements a CPU based chunker which purpose is to
 * test deduplication efficiency and data skipping optimization
 *
 *  Created on: Jul 8, 2013
 *      Author: Zahari Dichev <zaharidichev@gmail.com>
 */

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Chunk.h"
#include "../rabin_chunking_code/rabin_fingerprint/RabinData.h"
#include "../rabin_chunking_code/rabin_fingerprint/RabinFingerprint.h"
#include "../rabin_chunking_code/math/BitOps.h"
#include <openssl/sha.h>

#ifndef CPUCHUNKER_H_
#define CPUCHUNKER_H_

using namespace std;
using namespace boost;
class CPUChunker {

private:
	size_t min; // the min threshold
	size_t max; // the max threshold

public:
	 /**
	  * The main constructor that takes the min and max thresholds
	  *
	  * @param min
	  * @param max
	  */
	CPUChunker(size_t min, size_t max) {
		this->min = min;
		this->max = max;
	}
	virtual ~CPUChunker() {
		//// nothing here ////
	}

	/**
	 *
	 * Discovers chunks based on rabin fingerprints and hashes their contents.
	 *
	 * @param data
	 * @param len
	 * @return
	 */

	std::vector<shared_ptr<Chunk> > chunkData(shared_ptr<BYTE> data, size_t len) {
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

};

#endif /* CPUCHUNKER_H_ */
