/**
 * testSpeed.cpp
 *
 *	This file tests the data skipping percentage on the CPU
 *	for more flexible results. Same results apply when GPU chunker is
 *	used
 *
 *  Created on: Jul 22, 2013
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
#include <boost/shared_ptr.hpp>
#include <ctime>

using namespace std;
typedef unsigned char BYTE;

int testSkippingData(size_t min, boost::shared_ptr<BYTE> data, size_t dataLen) {
	size_t maxSize = 1048576;
	size_t minSize = min;

	CPUChunker chunker = CPUChunker(minSize, maxSize);
	vector<boost::shared_ptr<Chunk> > chunks = chunker.chunkData(data, dataLen);

	int numChunks = chunks.size();

	int amountOfDataSkipped = numChunks * min;

	double percentSkipped = double(amountOfDataSkipped) / (double) dataLen;
	printf("%d %.4lf\n", min, percentSkipped);

}

int test() {

	size_t sizeOfData = 2032221073;
	boost::shared_ptr<BYTE> data((BYTE*) malloc(sizeOfData * sizeof(BYTE)));

	std::ifstream inStream("/home/zahari/Desktop/kernels_big.dat");
	clock_t begin = clock();

	inStream.read((char*) data.get(), sizeOfData);

	size_t minSize = 32768;
	size_t maxSize = 1048576;

	CPUChunker chunker = CPUChunker(minSize, maxSize);
	using namespace std;
	vector<boost::shared_ptr<Chunk> > chunks = chunker.chunkData(data, sizeOfData);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	//printf("%d %.2lf\n", min, elapsed_secs);

	/*	for(vector<boost::shared_ptr<Chunk> >::iterator it = chunks.begin(); it != chunks.end(); ++it) {
	 cout<< *((*it).get()) << std::endl;
	 }*/

	return 0;
}

int main() {

	size_t sizeOfData = 2032221073;
	boost::shared_ptr<BYTE> data((BYTE*) malloc(sizeOfData * sizeof(BYTE)));
	std::ifstream inStream("/home/zahari/Desktop/kernels_big.dat");

	inStream.read((char*) data.get(), sizeOfData);

	testSkippingData(4096, data, sizeOfData);
	testSkippingData(8192, data, sizeOfData);
	testSkippingData(12288, data, sizeOfData);
	testSkippingData(16384, data, sizeOfData);
	testSkippingData(20480, data, sizeOfData);
	testSkippingData(24576, data, sizeOfData);
	testSkippingData(28672, data, sizeOfData);
	testSkippingData(32768, data, sizeOfData);
	testSkippingData(36864, data, sizeOfData);
	testSkippingData(40960, data, sizeOfData);
	testSkippingData(45056, data, sizeOfData);
	testSkippingData(49152, data, sizeOfData);
	testSkippingData(53248, data, sizeOfData);
	testSkippingData(57344, data, sizeOfData);
	testSkippingData(61440, data, sizeOfData);
	testSkippingData(65536, data, sizeOfData);
	testSkippingData(69632, data, sizeOfData);
	testSkippingData(73728, data, sizeOfData);
	testSkippingData(77824, data, sizeOfData);
	testSkippingData(81920, data, sizeOfData);
	testSkippingData(86016, data, sizeOfData);
	testSkippingData(90112, data, sizeOfData);
	testSkippingData(94208, data, sizeOfData);
	testSkippingData(98304, data, sizeOfData);

}

