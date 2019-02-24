#ifndef LIGHTINGSTRUCTS
#define LIGHTINGSTRUCTS
#include <inttypes.h>
namespace patchConfig
{

	struct attribute
	{
		uint8_t type;
		uint8_t blendType;
	};

	struct patch
	{
		bool exists;
		uint16_t id;
		uint16_t offset;
		uint8_t length;
		attribute *attributes[256];
	};

}

namespace submasterConfig
{

	struct blendEntry
	{
		uint16_t fixtureID;
		uint16_t attrID;
		uint16_t value;
	};

	struct submaster
	{
		bool exists;
		uint16_t id;
		uint16_t length;
		blendEntry *entries[256];
	};

}
#endif
