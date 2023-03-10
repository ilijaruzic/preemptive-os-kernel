#ifndef __BOUNDED_BUFFER__
#define __BOUNDED_BUFFER__

#include <semaphor.h>

class BoundedBuffer {
public:

	BoundedBuffer(unsigned size);
	virtual ~BoundedBuffer();
	int append(char);
	char take();
	int fullCount() { return itemAvailable.val(); };

private:
	unsigned Size;
	Semaphore mutexa, mutext;
	Semaphore spaceAvailable, itemAvailable;
	char* buffer;
	int head, tail;
};

#endif
