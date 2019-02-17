#include "serialize.h"
#include "../structs.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

void parseLine(patchConfig::patch **fixtures, char *pointer, size_t chars)
{

	// Parse variables out of the text
	unsigned int id;
	unsigned int offset;
	unsigned int length;
	char *attributes = (char*) malloc(chars - sizeof(char) * 4);
	sscanf(pointer, "%u,%u,%s", &id, &offset, attributes);

	// Create fixture
	fixtures[id] = (patchConfig::patch*) malloc(sizeof(patchConfig::patch));
	fixtures[id]->exists = true;
	fixtures[id]->id = id;
	fixtures[id]->offset = offset;
	
	// Put in attributes
	char *p = attributes;
	int8_t currAttr = 0;
	while(*p != '\0') {

		// Parse out attribute parameters
		unsigned int type;
		unsigned int blendMode;
		sscanf(p, "%u-%u", &type, &blendMode);

		// Create attribute and fill it in
		fixtures[id]->attributes[currAttr] = (patchConfig::attribute*) malloc(sizeof(patchConfig::attribute));
		fixtures[id]->attributes[currAttr]->blendType = blendMode;
		fixtures[id]->attributes[currAttr]->type = type;
		currAttr++;
		
		// Skip to the next attribute
		while(*p != ':' && *p != '\0') p = p + sizeof(char);
		p = p + sizeof(char);

	}
	fixtures[id]->length = currAttr;

}

void openers::patch(patchConfig::patch **fixtures)
{
	int file = open("./patch.csv", O_RDONLY);

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

		parseLine(fixtures, newPointer - (sizeof(char) * chars), chars);

		newPointer = newPointer + sizeof(char);

	}
	
}
