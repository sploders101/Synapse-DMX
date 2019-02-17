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

	// Create null fixture and submaster
	nullFixture.id = 0;
	nullSub.id = 0;

	// Parse patch.csv
	openers::patch(fixtures);

	// Parse subs.csv
	openers::submasters(submasters);

}

void blender::tick(ola::DmxBuffer* DMX, ola::client::StreamingClient* client)
{

}

