#include "serialize.h"
#include "../structs.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void parseLine(submasterConfig::submaster **submasters, char *pointer, size_t chars)
{

	// Parse variables out of the text
	unsigned int id;
	char *entries = (char*) malloc(chars - sizeof(char) * 2);
	sscanf(pointer, "%u,%s", &id, entries);

	submasters[id] = (submasterConfig::submaster*)  malloc(sizeof(patchConfig::patch));
	submasters[id]->exists = true;
	submasters[id]->id = id;
	
	// Put in entries
	char *p = entries;
	int8_t currEntry = 0;
	while(*p != '\0') {

		// Parse out entry parameters
		unsigned int fixtureID;
		unsigned int attrID;
		unsigned int value;
		sscanf(p, "%u-%u-%u", &fixtureID, &attrID, &value);

		// Create entry and fill it in
		printf("%u", (unsigned int) currEntry);
		submasters[id]->entries[currEntry] = (submasterConfig::blendEntry*) malloc(sizeof(submasterConfig::blendEntry));
		submasters[id]->entries[currEntry]->fixtureID = fixtureID;
		submasters[id]->entries[currEntry]->attrID = attrID;
		submasters[id]->entries[currEntry]->value = value;
		currEntry++;

		// Skip to the next attribute
		while(*p != ':' && *p != '\0') p = p + sizeof(char);
		p = p + sizeof(char);

	}
	submasters[id]->length = currEntry;

}

void openers::submasters(submasterConfig::submaster **submasters)
{

	int file = open("./submasters.csv", O_RDONLY);

	off_t size = lseek(file, 0, SEEK_END);
	lseek(file, 0, SEEK_SET);

	char *pointer = (char*) mmap(NULL, size, PROT_READ, MAP_PRIVATE, file, 0);

	char *newPointer = (char*) malloc(size);
	char *EOFPointer = newPointer + size;

	memcpy(newPointer, pointer, size);
	// munmap(pointer, size);

	while(newPointer < EOFPointer) {
		
		size_t chars = 0;

		while(*newPointer != '\n') {
			newPointer = newPointer + sizeof(char);
			chars++;
		}

		*newPointer = '\0';

		parseLine(submasters, newPointer - (sizeof(char) * chars), chars);

		newPointer = newPointer + sizeof(char);

	}

}
