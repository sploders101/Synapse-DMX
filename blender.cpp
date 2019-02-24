#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "blender.hpp"
#include "structs.hpp"
#include "serialize/serialize.hpp"
using std::cout;
using std::endl;

struct blenderValue {
	uint16_t subID;
	uint8_t value;
};

patchConfig::patch *fixtures[65536];
submasterConfig::submaster *submasters[65536];

patchConfig::patch nullFixture;
submasterConfig::submaster nullSub;

blenderValue layers[65536];

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

	// Empty layers
	for(size_t i = 0; i < 65536; i++)
	{
		layers[i].subID = 0;
		layers[i].value = 0;
	}
	

}

void blender::tick(ola::DmxBuffer* DMX, ola::client::StreamingClient* client)
{
	DMX->Blackout();
	for(size_t i = 0; i < 65535; i++)
	{
		blenderValue layer = layers[i];

		if(layer.value == 0) {
			break;
		}

		submasterConfig::submaster sub = *submasters[layer.subID];
		
		if(sub.exists) {
			for(size_t i = 0; i < sub.length; i++)
			{

				submasterConfig::blendEntry entry = *sub.entries[i];
				uint16_t fixtureID = entry.fixtureID;
				uint16_t newVal = entry.value;
				fixtures[fixtureID]->attributes;

				for(size_t j = 0; j < fixtures[fixtureID]->length; i++)
				{
					if(fixtures[fixtureID]->attributes[j]->type == entry.attrID) {
						uint16_t DMXAdr = fixtures[fixtureID]->offset + j;
						uint8_t currVal = DMX->Get(DMXAdr);
						uint8_t blendType = fixtures[fixtureID]->attributes[j]->blendType;

						if(blendType == 0 && currVal < newVal) {
							DMX->SetChannel(DMXAdr, newVal);							
						} else if(blendType == 1) {
							DMX->SetChannel(DMXAdr,
								(
									(
										(
											(newVal * layer.value) + 
											(currVal * layer.value)
										) /
										255
									) +
									currVal
								)
							);
						}
						break;
					}
				}

			}
		}

	}
	
}

