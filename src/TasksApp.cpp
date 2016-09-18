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
	
	AddHandler(fsSync);
	AddHandler(gSync);
	
	fsSync->Init();
	fsSync->GetTasks();
	gSync->Init();
	gSync->GetTasks();
	

	
	//iterate throught all gSync Tasks and Add them to the fsSync
	//because fsSync is our basis
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
	switch (message->what) {
	case MERGE_TASKS:
		Task	*one;
		Task	*two;
		if ( message->FindPointer("TaskOne",(void **)&one) == B_OK &&
			message->FindPointer("TaskOne",(void **)&two) == B_OK ) {
				Task *mainTask=MergeTasks(one,two);
				BMessage *msg= new BMessage(MODIFY_TASK);
				msg->AddPointer("Task",mainTask);
				PostMessage(msg);
			}
	break;
	case ADD_TASK:		
	case ADD_TASK_LIST:
	case REMOVE_TASK:
	case REMOVE_TASK_LIST:
	case MODIFY_TASK:
	case ADD_TASK_SYNC:
	case REMOVE_TASK_SYNC:
	
	default:
		BApplication::MessageReceived(message);
		break;
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
