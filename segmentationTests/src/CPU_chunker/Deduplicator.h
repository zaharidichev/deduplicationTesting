/**
 * Deduplicator.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Zahari Dichev <zaharidichev@gmail.com>
 */

#ifndef DEDUPLICATOR_H_
#define DEDUPLICATOR_H_
#include <string>

#define HEX( x ) setw(2) << setfill('0') << hex << (int)( x )


#include <boost/shared_ptr.hpp>
#include "Chunk.h"
#include <set>
#include <vector>
using namespace std;
using namespace boost;

class Deduplicator {
public:
	Deduplicator(string name);
	virtual ~Deduplicator();
	void addChunk(shared_ptr<Chunk> chunk);
	void addChunks(vector<shared_ptr<Chunk> > chunks);
	void printStats();
	double getOmmited();
private:
	size_t original;
	size_t deduplicated;
	string name;
	set<string> hashesOfChunks;
};

#endif /* DEDUPLICATOR_H_ */
