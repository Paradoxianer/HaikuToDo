#include "HaikuToDo.h"

int
main(int argc, const char *argv[])
{
	HaikuToDo app;
	app.Run();
	return 0;
}


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
