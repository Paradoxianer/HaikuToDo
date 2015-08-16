#include <Mime.h>

#include "TasksApp.h"

int
main(int argc, const char *argv[])
{
	TaskApp app;
	app.Run();
	return 0;
}


TaskApp::TaskApp()
	:	BApplication(APP_SIG)
{
	FirstStart();
	
	fsSync = new TaskFS();
	fsSync->Init();
	fsSync->GetTasks();
}


TaskApp::~TaskApp()
{
}


status_t TaskApp::FirstStart()
{
	//set up setting files
	return B_OK;
}

void TaskApp::MessageReceived(BMessage *message)
{
}


int32 TaskApp::EventLoop(void *data)
{
	TaskApp *app = (TaskApp *)data;
	return 0;
}
