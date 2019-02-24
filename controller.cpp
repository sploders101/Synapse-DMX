#include <stdlib.h>
#include <unistd.h>
#include <ola/DmxBuffer.h>
#include <ola/Logging.h>
#include <ola/client/StreamingClient.h>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include "blender.hpp"

using std::cout;
using std::endl;

int main(int, char *[])
{

	unsigned int universe = 1; // DMX universe
	ola::DmxBuffer DMX; // Holds dmx frames

	// Enable logging OLA warnings
	ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);
	DMX.Blackout();	 // Set all channels to 0

	// Create a new client.
	ola::client::StreamingClient ola_client(
		(ola::client::StreamingClient::Options())
	);

	// Connect to the server
	if (!ola_client.Setup())
	{
		std::cerr << "Setup failed" << endl;
		exit(1);
	}

	blender::init();

	while (1)
	{
		blender::tick(&DMX, &ola_client);
		if (!ola_client.SendDmx(universe, DMX))
		{
			cout << "Send DMX failed" << endl;
			exit(1);
		}
	}
	
	return 0;
}
