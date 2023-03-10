#ifndef __KEY_EVENT__
#define __KEY_EVENT__

#include <thread.h>

class BoundedBuffer;

class KeyEvent :public Thread{
public:
	KeyEvent(BoundedBuffer*);
	virtual ~KeyEvent() { waitToComplete(); }

protected:
	virtual void run();
	char character;

private:
	BoundedBuffer* buffer;
};

#endif  
