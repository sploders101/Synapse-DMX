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

void parseLine(patchConfig::patch* fixtures, char *pointer, size_t chars)
{

	unsigned int id;
	unsigned int offset;
	unsigned int length;
	char *attributes = (char*) malloc(chars - sizeof(char) * 4);

	sscanf(pointer, "%u,%u,%s", &id, &offset, attributes);

	patchConfig::patch *fixture = &fixtures[id];

	fixture->id = id;
	fixture->offset = offset;

	// Count attributes
	char *p = attributes;
	int8_t numAttrs = 0;
	uint16_t strlength = 0;
	while(*p != '\0') {
		strlength++;
		p = p + sizeof(char);
		cout << strlength;
	}
	p = attributes;
	while(p < attributes + ( sizeof(char) * strlength)) {
		if(numAttrs == 0) numAttrs++;
		if(*p == ':') {
			numAttrs++;
		}
		p = p + sizeof(char);
	}

	// Set number of fixtures
	fixture->length = numAttrs;

	// Reset pointer
	p = attributes;
	
	// Allocate space for all attributes
	fixture->attributes = (patchConfig::attribute *) malloc(sizeof(patchConfig::attribute) * numAttrs);
	int8_t currAttr = 0;
	while(p < attributes + ( sizeof(char) * strlength)) {
		unsigned int type;
		unsigned int blendMode;
		sscanf(p, "%u-%u", &type, &blendMode);
		while(*p != ':') p = p + sizeof(char);
		p = p + sizeof(char);
		fixture->attributes[currAttr].blendType = blendMode;
		fixture->attributes[currAttr].type = type;
		currAttr++;
	}

}

void openers::patch(patchConfig::patch* fixtures)
{
	int file = open("./patch.csv", O_RDONLY);

	off_t size = lseek(file, 0, SEEK_END);
	lseek(file, 0, SEEK_SET);

	char *pointer = (char*) mmap(NULL, size, PROT_READ, MAP_PRIVATE, file, 0);

	char *newPointer = (char*) malloc(size);
	char *EOFPointer = newPointer + size;

	memcpy(newPointer, pointer, size);

	while(newPointer < EOFPointer) {

		size_t chars = 0;

		while(*newPointer != '\n') {
			newPointer = newPointer + sizeof(char);
			chars++;
		}

		*newPointer = '\0';

		parseLine(fixtures, newPointer - (sizeof(char) * chars), chars);

		cout << "DMX Offset: " << fixtures[1].offset << endl;
		cout << "DMX Length: " << fixtures[1].length << endl;
		cout << "Intensity blend type: " << fixtures[1].attributes[0].blendType << endl;

		newPointer = newPointer + sizeof(char);

	}
	
}
