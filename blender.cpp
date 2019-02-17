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

patchConfig::patch *fixtures[65536];
submasterConfig::submaster *submasters[65536];

patchConfig::patch nullFixture;
submasterConfig::submaster nullSub;


void blender::init()
{

	// Nullify arrays to avoid segfaults
	nullFixture.exists = false;
	nullSub.exists = false;
	for(size_t i = 0; i < 65536; i++)
	{
		fixtures[i] = &nullFixture;
		submasters[i] = &nullSub;
	}
	

	// Fill in with real data
	openers::patch(fixtures);
	openers::submasters(submasters);

}

void blender::tick(ola::DmxBuffer* DMX, ola::client::StreamingClient* client)
{

}

