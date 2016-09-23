#include "TaskSync.h"
#include "TaskDefs.h"
#include <Debug.h>

TaskSync::TaskSync(void):BHandler()
{
}

void TaskSync::MessageReceived(BMessage *message)
{
	TRACE();
	PRINT_OBJECT(*message);
	Task		*tmpTask;
	TaskList	*tmpTaskList;
	uint32		realWhat = 0;
	if (message->what ==  B_OBSERVER_NOTICE_CHANGE &&
	  message->FindInt32("be:observe_change_what", (int32*) &realWhat) == B_OK) {
		switch (realWhat) {
		case LOAD_TASKS:
			GetTasks();
			break;	
		case ADD_TASK:
			if (message->FindPointer("task",(void**)&tmpTask) == B_OK)
				if (tmpTask != NULL)
					AddTask(tmpTask);
				else
					printf("ERROR:add task - Task found was invalid\n");
			else
				printf("ERROR: add task - cant find task\n");
			break;
		case ADD_TASK_LIST:
			if (message->FindPointer("tasklist",(void**)&tmpTaskList) == B_OK)
				if (tmpTaskList != NULL)
					AddTaskList(tmpTaskList);
				else
					printf("ERROR: add task list - task list invalid\n");
			else{
				printf("ERROR: add task list - cant find task list\n");
			}		
			break;
		case REMOVE_TASK:
			if (message->FindPointer("task",(void**)&tmpTask) == B_OK)
				if (tmpTask != NULL)
					RemoveTask(tmpTask->ID());
				else
					printf("ERROR: remove task - Task found was invalid\n");
			else
				printf("ERROR: remove task - cant find task\n");
			break;
		case REMOVE_TASK_LIST:
			if (message->FindPointer("tasklist",(void**)&tmpTaskList) == B_OK)
				if (tmpTaskList != NULL)
					RemoveTaskList(tmpTaskList->ID());
				else
					printf("ERROR: remove task list - task list found was invalid\n");
			else
				printf("ERROR: remove task list - cant find task list\n");
			break;
		case MODIFY_TASK:
			if (message->FindPointer("task",(void**)&tmpTask) == B_OK)
				if (tmpTask != NULL)
					UpdateTask(tmpTask->ID(),tmpTask);
				else
					printf("ERROR: modfiy task - Task found was invalid\n");
			else
				printf("ERROR: modify task - cant find task\n");
			break;
		default:
			BHandler::MessageReceived(message);
			break;
		}
	}
	else
			BHandler::MessageReceived(message);
}
