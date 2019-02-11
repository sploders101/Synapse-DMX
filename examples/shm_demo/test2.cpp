#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>
using namespace std;

#define SHMSIZE sizeof(sem_t) * 2 + 512

int main()
{

	// Open shared memory created by parent
	int shm_id = shm_open("/DMXTest", O_RDWR, 0777);

	// Map shared memory into local address space and point variables at it
	sem_t *toParent = (sem_t *)mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
	sem_t *fromParent = toParent + sizeof(sem_t);
	char *test = (char *) fromParent + sizeof(sem_t);

	// Tell parent we are ready
	cout << "Ready to accept response\n";
	sem_post(toParent);

	// Wait for response from parent
	cout << "Waiting for response...\n";
	sem_wait(fromParent);

	// Print message and acknowledge
	cout << "Parent said " << test << "\n";
	sem_post(toParent);

}
