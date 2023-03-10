#include "keyevent.h"

#include <iostream.h>
#include <dos.h>

#include <event.h>
#include "bounded.h"
#include "intLock.h"
#include "user.h"

PREPAREENTRY(9,0);

char keymap[128] = {
  0 , 27,'1','2','3','4','5','6','7','8','9','0','-','=', 8 , 9,
 'q','w','e','r','t','y','u','i','o','p','[',']', 13, 0 ,'a','s',
 'd','f','g','h','j','k','l',';',0,0,'`','\\','z','x','c','v','b',
 'n','m',',','.','/', 0 ,'*', 0 ,' '
};

KeyEvent::KeyEvent(BoundedBuffer* b) : Thread(), buffer(b) {
	end = 0;
}

void KeyEvent::run() {
	intLock
	Event event9(9);
	cout << "KeyListener started!" << endl;
	intUnlock
	char scancode, status, character;
	while (!end) {
		event9.wait();
		do {
			status = inportb(0x64);
			if(status & 0x01) {
				scancode = inportb(0x60);
				if(scancode == -127) {
					end = 1;
					myBuffer->append('!');
				}
				else {
					if (scancode&0x80) {
						buffer->append(keymap[scancode&0x7F]);
					}
				}

			};
			asm{
				cli
				in      al, 61h
				or      al, 10000000b
				out     61h, al
				and     al, 01111111b
				out     61h, al
				mov     al, 20h
				out     20h, al
				sti
			}
		 } while (!end && status & 0x01);
	intLock
		cout << endl << "KeyListener stopped!" << endl;
	intUnlock
}
