/*
 * Copyright 2016 Paradoxon <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "TaskMerge.h"
#include "TaskFS.h"
#include "TaskGoogle.h"

TaskMerge::TaskMerge(void)
	: BLooper("MergeWorker")
{
	//start the Looper
	taskList	= new BObjectList<Task>();
	syncerList	= new BObjectList<TaskSync>();
	Init();
	Run();
}

TaskMerge::~TaskMerge(void){
	delete taskList;	
}
		
status_t TaskMerge::Init(void)
{
	status_t	err	= B_OK;
	return err;
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
	status_t	err = B_OK;
	if (GetTask(tsk->ID())!=NULL){
		//check if all TaskSyncer have this task in the ..list if not 
		//add it to the this syncer
	}
	else{
		//add it to all Syncers
		
	}
	return err;
}

	
status_t TaskMerge::UpdateTask(BString id,Task *tsk)
{
	status_t	err = B_OK;
	int32		i	= 0;
	for (i=0;i<syncerList->CountItems();i++){
		err = err || syncerList->ItemAt(i)->UpdateTask(id, tsk);
	}
	//find all TaskSyncer of this Task wich need to be updated..
	//call all UpdateTasks of the TaskSyncer
	return err;
}

	
status_t TaskMerge::RemoveTask(BString id)
{
	status_t	err	= B_OK;
	int32		i	= 0;
	for (i=0;i<syncerList->CountItems();i++){
		taskList->AddList(syncerList->ItemAt(i)->GetTasks());
	}
	//and remove it from the list
	return err;
}
	
	
BObjectList<TaskList>* TaskMerge::GetTaskLists()
{
	//run throught all TaskSyncer and get the TaskLists
	BObjectList<TaskList>		*listOfLists = new 	BObjectList<TaskList>();

	int32	i	= 0;
	for (i=0;i<syncerList->CountItems();i++){
		listOfLists->AddList(syncerList->ItemAt(i)->GetTaskLists());
	}
	return listOfLists;
}


TaskList* TaskMerge::GetTaskList(BString id)
{
	int32	i	= 0;
	TaskList *tskLst	= NULL;
	while (tskLst == NULL && i < syncerList->CountItems()) {
		tskLst = syncerList->ItemAt(i)->GetTaskList(id);
	}
	return tskLst;	
}


status_t TaskMerge::AddTaskList(TaskList *ctgr)
{
	status_t	err	=B_OK;
	int32		i	= 0;
	for (i=0;i<syncerList->CountItems();i++){
		syncerList->ItemAt(i)->AddTaskList(ctgr);
	}
	return err;
}	
	

status_t TaskMerge::UpdateTaskList(BString id,TaskList *ctgr)
{
	int32	i	= 0;
	for (i=0;i<syncerList->CountItems();i++){
		syncerList->ItemAt(i)->UpdateTaskList(id, ctgr);
	}
}


status_t TaskMerge::RemoveTaskList(BString id)
{
	int32	i	= 0;
	for (i=0;i<syncerList->CountItems();i++){
		syncerList->ItemAt(i)->RemoveTaskList(id);
	}

}


status_t TaskMerge::AddTaskSync(TaskSync *sync)
{
	//add this tasksyncer to all Tasks

	if (syncerList->AddItem(sync) == true)
		return B_OK;
	else 
		return B_ERROR;
}


status_t TaskMerge::RemoveTaskSync(TaskSync *sync)
{
	//remove this TaskSyncs from all Tasks
	if (syncerList->RemoveItem(sync) == true) 
		return B_OK;
	else
		return B_ERROR;
}
	

status_t TaskMerge::MergeIn(BObjectList<Task> *newTaskList)
{
	status_t	err			= B_OK;
	int32		i			= 0;
	Task		*tmpTask	= NULL;
	Task		*foundTask	= NULL;
	Task		*mergedTask	= NULL;
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
	return err;
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
