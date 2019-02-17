#include "serialize.h"
#include "../structs.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

void parseLine(submasterConfig::submaster **submasters, char *pointer, size_t chars)
{

	unsigned int id;
	char *entries = (char*) malloc(chars - sizeof(char) * 2);

	sscanf(pointer, "%u,%s", &id, entries);

	submasterConfig::submaster *submaster = (submasterConfig::submaster*) malloc(sizeof(submasterConfig::submaster));

	submaster->id = id;

	// Count entries
	char *p = entries;
	uint16_t numEntries = 0;
	uint32_t strlength = 0;
	while(*p != '\0') {
		strlength++;
		p = p + sizeof(char);
	}
	p = entries;

}
