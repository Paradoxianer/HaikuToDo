/*
 * Copyright 2016 Paradoxon <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
//*** This should be a Looper and all TaskSyncs should be
//**Handler wich are added to this

#include "TaskMerge.h"
#include "TaskFS.h"
#include "TaskGoogle.h"

TaskMerge::TaskMerge(void)
	:
	BLooper("MergeWorker")
{
	//start the Looper
	Run();
	taskList	= new BObjectList<Task>();
	AddHandler(new TaskFS());
	AddHandler(new TaskGoogle());
	Init();
}

TaskMerge::~TaskMerge(void){
}
		
status_t TaskMerge::Init(void)
{
	int32		i	= 0;
	TaskSync*	tmpSyncer;
	for (i=0; i<CountHandlers(); i++){
		tmpSyncer = dynamic_cast<TaskSync *>(HandlerAt(i));
		if (tmpSyncer != NULL)
			tmpSyncer->Init();
	}
}

	
BObjectList<Task>* TaskMerge::GetTasks(void)
{
	int32					i			= 0;
	TaskSync*				tmpSyncer	= NULL;
	BObjectList<Task>*		tmpTaskList	= NULL;	
	taskList->MakeEmpty();
	for (i=0; i<CountHandlers(); i++){
		tmpSyncer = dynamic_cast<TaskSync*>(HandlerAt(i));
		if (tmpSyncer != NULL)
			tmpTaskList = tmpSyncer->GetTasks();
		if (tmpTaskList != NULL)
			MergeIn(tmpTaskList);
		tmpTaskList = NULL;
		tmpSyncer	= NULL;
	}
	return taskList;
}


BObjectList<Task>* TaskMerge::GetTasks(TaskList ctgr)
{
	int32					i			= 0;
	TaskSync*				tmpSyncer	= NULL;
	BObjectList<Task>*		tmpTaskList	= NULL;
	taskList->MakeEmpty();
	for (i=0; i<CountHandlers(); i++){
		tmpSyncer = dynamic_cast<TaskSync*>(HandlerAt(i));
		if (tmpSyncer != NULL)
			tmpTaskList = tmpSyncer->GetTasks();
		if (tmpTaskList != NULL)
			MergeIn(tmpTaskList);
	}
	return taskList;
}

	
Task* TaskMerge::GetTask(BString forID)
{
	Task*	taskFound = NULL;
	int32 i;
	for (i=0;(i<taskList->CountItems() && taskFound==NULL) ;i++) {
		if (forID.Compare(taskList->ItemAt(i)->ID()) == 0)
			taskFound = taskList->ItemAt(i);
	}
	return taskFound;

}
	
	
status_t TaskMerge::AddTask(Task *tsk)
{
	//first check if we have the Task already..
	//add it to the TaskList and then call AddTask() of the TaskSyncer
}

	
status_t TaskMerge::UpdateTask(BString id,Task *tsk)
{
	//find all TaskSyncer of this Task wich need to be updated..
	//call all UpdateTasks of the TaskSyncer
}

	
status_t TaskMerge::RemoveTask(BString id)
{
	//find all TaskSyncer of this Task wich need to be updated..
	//call all UpdateTasks of the TaskSyncer
	//and remove it from the list
}
	
	
BObjectList<TaskList>* TaskMerge::GetTaskLists()
{
	//run throught all TaskSyncer and get the TaskLists
	
}


TaskList* TaskMerge::GetTaskList(BString id)
{
	//find the 
}


status_t TaskMerge::AddTaskList(TaskList *ctgr)
{
}	
	

status_t TaskMerge::UpdateTaskList(BString id,TaskList *ctgr)
{
}


status_t TaskMerge::RemoveTaskList(BString id)
{
}


status_t TaskMerge::AddTaskSync(TaskSync *sync)
{
	
}


status_t TaskMerge::RemoveTaskSync(TaskSync *sync)
{
	//remove this TaskSyncs from all Tasks
}
	

status_t TaskMerge::MergeIn(BObjectList<Task> *newTaskList)
{
	int32	i			= 0;
	Task	*tmpTask	= NULL;
	Task	*foundTask	= NULL;
	Task	*mergedTask	= NULL;
	//run throught all of the newTaskList ... 
	for (i=0;newTaskList->CountItems();i++){
		tmpTask = newTaskList->ItemAt(i);
		foundTask=GetTask(BString(tmpTask->ID()));
		if (foundTask!=NULL){
			mergedTask = MergeTasks(tmpTask, foundTask);
			UpdateTask(tmpTask->ID(),mergedTask);
		}
		else{
			//add it to all other Syncers as Add..
			taskList->AddItem(tmpTask);
		}
	}
}


Task* TaskMerge::MergeTasks(Task* firstTask, Task *secondTask)
{
	int32	i	= 0;
	BObjectList<TaskSync>* oldSyncs = NULL;
	if ((*firstTask) == (*secondTask))
		return firstTask;
	if ((*firstTask)>(*secondTask)){
		oldSyncs	= secondTask->Source();
		secondTask->SetTo(firstTask);
		for (i=0;i<oldSyncs->CountItems();i++)
				secondTask->AddSource(oldSyncs->ItemAt(i));
		return secondTask;
	}
	else{
		oldSyncs	= firstTask->Source();
		firstTask->SetTo(secondTask);
		for (i=0;i<oldSyncs->CountItems();i++)
				firstTask->AddSource(oldSyncs->ItemAt(i));
		return firstTask;
	}
}
