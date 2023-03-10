#include <iostream.h>
#include <stdlib.h>

#include <event.h>
#include "bounded.h"
#include "intLock.h"
#include "keyevent.h"
#include "user.h"

Time TIME_SLICE = 2;   // 0 or defaultTimeSlice
int N = 3;            // 1 <= N <= 19
volatile int end = 0;

class Producer : public Thread {
public:
	Producer(BoundedBuffer* b, char c, Time time_slice);
	virtual ~Producer() { waitToComplete(); }

protected:
	virtual void run();
	char produce() { return c; };

private:
	BoundedBuffer* buffer;
	char c;
};

class Consumer : public Thread {
public:
	Consumer(BoundedBuffer* b) : Thread(defaultStackSize, 0), buffer(b) {}
	virtual ~Consumer() { waitToComplete(); }

protected:
	virtual void run();
	void consume(char c);

private:
	BoundedBuffer* buffer;
};

void tick() {}

Producer::Producer(BoundedBuffer* b, char c, Time time_slice) : Thread(defaultStackSize, time_slice), buffer(b), c(c) {}

void Producer::run () {
	while(!end) {
		char character = produce();
		buffer->append(character);
		Thread::sleep(10);
	}
}

void Consumer::consume(char c) {
	intLock
	cout<< c << " ";
	intUnlock
}

void Consumer::run() {
	int i = 0;
	while(!end) {
		char character = buffer->take();
		consume(character);
		if (i++ == 40) {
			Thread::sleep(5);
			i = 0;
		} else {
			for (int j = 0; j < 200; j++);
		}
	}
	while (buffer->fullCount()) {
		char character = buffer->take();
		consume(character);
		dispatch();
	}
}

int userMain (int argc, char* argv[]) {
	BoundedBuffer *buffer;
	Consumer *consumer;
	intLock
	if(argc < 2){
		cout << "Invalid input!" << endl;
		intUnlock
		return -1;
	}
	int size = atoi(argv[1]);
	N = atoi(argv[2]);
	N = N > 19 ? 19 : N;
	TIME_SLICE = atoi(argv[3]);
	if(size < N) {
		cout << "Number of produsers is larger than buffer size!" << endl;
		intUnlock
		return 1;
	}
	buffer = new BoundedBuffer(size);
	Producer **producers = new Producer*[N];
	KeyboardEvent* kevent;
	consumer = new Consumer(buffer);
	consumer->start();
	for (int i = 0; i < N; i++) {
		producers[i] = new Producer(buffer,'0' + i, TIME_SLICE);
		producers[i]->start();
	}
	kevent = new KeyboardEvent(buffer);
	intUnlock
	kev->start();
	for (int i = 0; i < N; i++) {
		delete producers[i];
	}
	delete[] producers;
	delete kevent;
	delete consumer;
	delete buffer;
	return 0;
}
