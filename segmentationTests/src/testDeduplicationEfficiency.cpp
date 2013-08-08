/**
 * hello.cpp
 *
 *  Created on: Jul 8, 2013
 *      Author: Zahari Dichev <zaharidichev@gmail.com>
 */

#include "CPU_chunker/CPUChunker.h"
#include <iostream>
#include <fstream>      // std::ifstream
#include <vector>
#include <string>
#include <set>
#include <ostream>
#include <sstream>
#include "CPU_chunker/Deduplicator.h"
using namespace std;

typedef struct Experiment {
	string filename;
	size_t min;
	size_t max;
	size_t segmentSize;
	size_t fileSize;
} Experiment;

typedef struct ChunkLimits {
	size_t min;
	size_t max;
} ChunkLimits;

void runExp(Experiment ex) {

	std::ifstream infile(ex.filename.c_str(), std::ofstream::binary);

	stringstream name;
	name << ex.filename << " - " << ex.min << " - " << ex.max << " - " << (ex.segmentSize / 1024) << " Kb";
	CPUChunker chunker = CPUChunker(ex.min, ex.max);
	Deduplicator dedup = Deduplicator(name.str());

	int numLoops = ex.fileSize / ex.segmentSize;

	shared_ptr<BYTE> dataBuffer((BYTE*) malloc(ex.segmentSize));

	for (int var = 0; var < numLoops; ++var) {
		infile.read((char*) dataBuffer.get(), ex.segmentSize);
		dedup.addChunks(chunker.chunkData(dataBuffer, ex.segmentSize));
	}

	int residual = ex.fileSize % ex.segmentSize;
	if (residual != 0) {
		infile.read((char*) dataBuffer.get(), residual);
		dedup.addChunks(chunker.chunkData(dataBuffer, residual));

	}

	infile.close();
	std::cout << dedup.getOmmited() << std::endl;
	//dedup.printStats();
}

void runVaryingBounds(std::vector<ChunkLimits> bounds, std::vector<int> segmentSizes) {

	int cnt = 1;
	for (std::vector<ChunkLimits>::iterator it = bounds.begin(); it != bounds.end(); ++it) {
		size_t glob_MAX = (*it).max;
		size_t glob_MIN = (*it).min;

		std::cout << "RUNNING EXPERIMENT " << std::endl;
		std::cout << glob_MIN << " - " << glob_MAX << std::endl;

		Experiment exp;
		exp.fileSize = 2032221073;
		exp.filename = "/home/zahari/Desktop/kernels_big.dat";
		exp.max = glob_MAX;
		exp.min = glob_MIN;

		for (std::vector<int>::iterator it = segmentSizes.begin(); it != segmentSizes.end(); ++it) {
			exp.segmentSize = *(it);
			runExp(exp);

		}

		std::cout << "--------------------------------" << glob_MIN << " " << glob_MAX << std::endl;
		cnt++;
	}

}

int main() {

	std::vector<int> segSizes;
	segSizes.push_back(65536);
	segSizes.push_back(131072);
	segSizes.push_back(262144);
	segSizes.push_back(524288);
	segSizes.push_back(1048576);
	segSizes.push_back(2097152);
	segSizes.push_back(4194304);
	segSizes.push_back(8388608);
	segSizes.push_back(16777216);
	segSizes.push_back(33554432);
	segSizes.push_back(67108864);
	segSizes.push_back(134217728);
	segSizes.push_back(2032221073);

	ChunkLimits limit1;
	limit1.min = 2048;
	limit1.max = 8192;

	ChunkLimits limit2;
	limit2.min = 4096;
	limit2.max = 16384;

	ChunkLimits limit3;
	limit3.min = 8192;
	limit3.max = 32768;

	std::vector<ChunkLimits> bounds;
	bounds.push_back(limit1);
	bounds.push_back(limit2);
	bounds.push_back(limit3);

	runVaryingBounds(bounds, segSizes);

}
