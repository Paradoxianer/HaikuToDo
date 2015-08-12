#include "HaikuToDo.h"


HaikuToDo::HaikuToDo()
	:	BApplication(APP_SIG)
{
}


HaikuToDo::~HaikuToDo()
{
}


void HaikuToDo::MessageReceived(BMessage *message)
{
}


int32 HaikuToDo::EventLoop(void *data)
{
	HaikuToDo *app = (HaikuToDo *)data;
	return 0;
}
