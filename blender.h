#ifndef BLENDER
#define BLENDER
#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>

namespace blender
{
	void init();
	void tick(ola::DmxBuffer*, ola::client::StreamingClient*);
} // blender
#endif
