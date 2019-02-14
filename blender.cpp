#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "blender.h"
#include "structs.h"
#include "serialize/serialize.h"
using std::cout;
using std::endl;

patchConfig::patch fixtures[65536];

void blender::init()
{
	openers::patch(fixtures);
}

void blender::tick(ola::DmxBuffer* DMX, ola::client::StreamingClient* client)
{

}

