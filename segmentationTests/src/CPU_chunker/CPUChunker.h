/**
 * CPUChunker.h
 *
 * This class implements a CPU based chunker which purpose is to
 * test deduplication efficiency and data skipping optimisation
 *
 *  Created on: Jul 8, 2013
 *      Author: Zahari Dichev <zaharidichev@gmail.com>
 */

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Chunk.h"
#include "../rabin_fingerprint/RabinData.h"
#include "../rabin_fingerprint/RabinFingerprint.h"
#include "../math/BitOps.h"
#include <openssl/sha.h>

#ifndef CPUCHUNKER_H_
#define CPUCHUNKER_H_

using namespace std;
using namespace boost;
class CPUChunker {
public:
	CPUChunker(size_t min, size_t max);
	virtual ~CPUChunker();
	std::vector<shared_ptr<Chunk> > chunkData(shared_ptr<BYTE> data, size_t len);

private:
	size_t min;
	size_t max;
};

#endif /* CPUCHUNKER_H_ */
