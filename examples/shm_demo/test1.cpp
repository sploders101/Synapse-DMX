#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <iostream>
using namespace std;

#define SHMSIZE sizeof(sem_t) * 2 + 512

int main()
{

	// Initialize shared memory
	int shm_id = shm_open("/DMXTest", O_CREAT | O_RDWR, 0777);
	ftruncate(shm_id, SHMSIZE);

	// Map shared memory into local address space and point variables at it
	sem_t *fromChild = (sem_t*) mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
	sem_t *toChild = fromChild + sizeof(sem_t);
	char *test = (char*) toChild + sizeof(sem_t);

	// Initialize semaphores
	sem_init(fromChild, 1, 0);
	sem_init(toChild, 1, 0);

	// Wait for child to post
	cout << "Ready\n";
	sem_wait(fromChild);

	// Write hello world to shared memory, and tell child we are ready
	cout << "Writing \"Hello world\" to shm...\n";
	strcpy(test, "Hello world");
	sem_post(toChild);

	// Wait for child, print acknowledgement, then exit
	sem_wait(fromChild);
	cout << "Child acknowledged and finished.\n";

}
