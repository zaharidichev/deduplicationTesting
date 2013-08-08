/**
 * Deduplicator.cpp
 *
 *  Created on: Jul 8, 2013
 *      Author: Zahari Dichev <zaharidichev@gmail.com>
 */

#include "Deduplicator.h"

Deduplicator::Deduplicator(string name) {
	// TODO Auto-generated constructor stub
	this->name = name;
	this->original = 0;
	this->deduplicated = 0;
}

Deduplicator::~Deduplicator() {
	// TODO Auto-generated destructor stub
}

void Deduplicator::addChunk(shared_ptr<Chunk> chunk) {
	this->original = this->original + chunk.get()->getSize();
	unsigned char* uc;
	std::string s(reinterpret_cast<char const*>(chunk.get()->getHash().get()), 20);
	if (this->hashesOfChunks.insert(s).second == true) {
		this->deduplicated = this->deduplicated + chunk.get()->getSize();
	}

}

void Deduplicator::addChunks(vector<shared_ptr<Chunk> > chunks) {

	for (std::vector<shared_ptr<Chunk> >::iterator it = chunks.begin(); it != chunks.end(); ++it) {
		this->addChunk(*it);
	}
}

void Deduplicator::printStats() {
	std::cout << std::endl;
	std::cout << "---------------------------" << std::endl;
	std::cout << "Settings: " << this->name << std::endl << "Original: " << this->original << std::endl << "Wrote to disk: " << this->deduplicated << std::endl
			<< "Saved %: " << (this->deduplicated / static_cast<double>(this->original)) * 100 << std::endl << "Omitted %: "
			<< 100 - ((this->deduplicated / static_cast<double>(this->original)) * 100) << std::endl;
	std::cout << "---------------------------" << std::endl;
	std::cout << std::endl;

}

double Deduplicator::getOmmited() {

	return 1 - (this->deduplicated / static_cast<double>(this->original));
}
