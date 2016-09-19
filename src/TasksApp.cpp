#include <Mime.h>

#include "TasksApp.h"
#include "Task.h"

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
	gSync	= new TaskGoogle();
}


TaskApp::~TaskApp()
{
}

void TaskApp::ReadyToRun()
{
	/*fsSync->Init();
	gSync->Init();*/
	AddHandler(fsSync);
	AddHandler(gSync);
	fsSync->StartWatchingAll(this);
	gSync->StartWatchingAll(this);
	PostMessage(LOAD_TASKS);
}

status_t TaskApp::FirstStart()
{
	//set up setting files
	return B_OK;
}

void TaskApp::MessageReceived(BMessage *message)
{
	PRINT_OBJECT(*message);
	switch (message->what) {
	case MERGE_TASKS:
		Task	*one;
		Task	*two;
		if ( message->FindPointer("TaskOne",(void **)&one) == B_OK &&
			message->FindPointer("TaskOne",(void **)&two) == B_OK ) {
				Task *mainTask=MergeTasks(one,two);
				BMessage *msg= new BMessage(MODIFY_TASK);
				msg->AddPointer("Task",mainTask);
				BroadCast(msg);
			}
		break;
	case LOAD_TASKS:
	case ADD_TASK:		
	case ADD_TASK_LIST:
	case REMOVE_TASK:
	case REMOVE_TASK_LIST:
	case MODIFY_TASK:
	case ADD_TASK_SYNC:
	case REMOVE_TASK_SYNC:
		bool broadcast;
		broadcast = false;
		message->FindBool("BroadCast", &broadcast);
		//onyl Broadcast this Message if this message has not yet been broadcasted
		if (broadcast == false)
			BroadCast(message);
		break;
	default:
		BApplication::MessageReceived(message);
		break;
	}
}

void TaskApp::BroadCast(BMessage *message)
{
	//start a 1 so that we dont include the looper itself
	message->AddBool("BroadCast",true);
	for (int i = 1; i<CountHandlers();i++){
		PostMessage(message,HandlerAt(i));
	}
}

int32 TaskApp::EventLoop(void *data)
{
	TaskApp *app = (TaskApp *)data;
	return 0;
}


Task* TaskApp::MergeTasks(Task* firstTask, Task *secondTask)
{
	int32	i	= 0;
	if ((*firstTask) == (*secondTask))
		return firstTask;
	if ((*firstTask)>(*secondTask)){
		secondTask->SetTo(firstTask);
		return secondTask;
	}
	else{
		firstTask->SetTo(secondTask);
		return firstTask;
	}	
}

