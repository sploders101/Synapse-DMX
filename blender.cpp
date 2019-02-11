#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "blender.h"
using std::cout;
using std::endl;

void blender::init()
{

	// Open patch file as read only
	int fd = open("./patch.bin", O_RDONLY);

	// Get file size
	off_t size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	// Map file to memory
	void *pFile = mmap(NULL, size, PROT_READ, 0, fd, 0);

	// Allocate memory for pointer maps pointing to patch settings
	patch *fixtures = (patch*) malloc(sizeof(patch) * 65535);
	mapPatch(pFile, size, fixtures);

}

void blender::tick(ola::DmxBuffer* DMX, ola::client::StreamingClient* client)
{

}

void mapPatch(void *pFile, off_t size, patch *fixtures)
{
	void *endOfFile = pFile + size;

	while (pFile < endOfFile)
	{
		patch *thisFixture = &fixtures[*((uint16_t*) pFile)];
		thisFixture->id = (uint16_t*) pFile;
		pFile = pFile + sizeof(uint16_t);
		thisFixture->offset = (uint16_t*) pFile;
		pFile = pFile + sizeof(uint16_t);
		thisFixture->length = (uint16_t *)pFile;
		pFile = pFile + sizeof(uint16_t);
		thisFixture->attributes = (uint8_t*) pFile;
		pFile = pFile + *thisFixture->length;
	}

}
