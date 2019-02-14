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
		uint16_t id;
		uint16_t offset;
		uint8_t length;
		attribute *attributes;
	};

}
#endif
