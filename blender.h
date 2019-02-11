#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>

namespace blender
{
	void init();
	void tick(ola::DmxBuffer*, ola::client::StreamingClient*);
} // blender

struct patch {
	uint16_t *id;
	uint16_t *offset;
	uint16_t length;
	uint8_t *attributes;
};

struct attributeDescriptor {
	uint16_t *offset;
	uint16_t *value;
};

struct value {
	uint16_t channel;
	attributeDescriptor *attributes;
};

struct submaster {
	uint16_t *id;
	uint16_t length;
	value *values;
};
