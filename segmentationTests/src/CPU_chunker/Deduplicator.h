/**
 * Deduplicator.h
 *
 * This class simply tests the efficiency of the deduplication performed
 * Its purpose is to contain a collection of UNIQUE chunks
 *
 *
 *  Created on: Jul 8, 2013
 *      Author: Zahari Dichev <zaharidichev@gmail.com>
 */

#ifndef DEDUPLICATOR_H_
#define DEDUPLICATOR_H_
#include <string>

//#define HEX( x ) setw(2) << setfill('0') << hex << (int)( x )

#include <boost/shared_ptr.hpp>
#include "Chunk.h"
#include <set>
#include <vector>
using namespace std;
using namespace boost;

class Deduplicator {
public:
	Deduplicator(string name) {
		this->name = name;
		this->original = 0; // stores how much data has been passed to the system
		this->deduplicated = 0; // sores how much data has been deduplicated  (actually written to disk)

	}
	virtual ~Deduplicator() {
	}
	void addChunk(shared_ptr<Chunk> chunk) {
		this->original = this->original + chunk.get()->getSize();
		unsigned char* uc;
		std::string s(reinterpret_cast<char const*>(chunk.get()->getHash().get()), 20);
		if (this->hashesOfChunks.insert(s).second == true) {
			this->deduplicated = this->deduplicated + chunk.get()->getSize();
		}
	}

	void addChunks(vector<shared_ptr<Chunk> > chunks) {

		for (std::vector<shared_ptr<Chunk> >::iterator it = chunks.begin(); it != chunks.end(); ++it) {
			this->addChunk(*it);
		}
	}

	/**
	 * Prints nice statistics about how much of the data has been discarded due to similarity
	 */
	void printStats() {
		std::cout << std::endl;
		std::cout << "---------------------------" << std::endl;
		std::cout << "Settings: " << this->name << std::endl << "Original: " << this->original << std::endl << "Wrote to disk: " << this->deduplicated
				<< std::endl << "Saved %: " << (this->deduplicated / static_cast<double>(this->original)) * 100 << std::endl << "Omitted %: "
				<< 100 - ((this->deduplicated / static_cast<double>(this->original)) * 100) << std::endl;
		std::cout << "---------------------------" << std::endl;
		std::cout << std::endl;
	}
	/**
	 * Returns the percentage of ommitted data
	 *
	 * @return
	 */
	double getOmmited() {
		return 1 - (this->deduplicated / static_cast<double>(this->original));

	}
private:
	size_t original;
	size_t deduplicated;
	string name;
	set<string> hashesOfChunks;
};

#endif /* DEDUPLICATOR_H_ */
