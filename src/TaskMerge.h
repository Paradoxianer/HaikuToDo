/*
 * Copyright 2016 Paradoxon <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TASK_MERGE_H
#define TASK_MERGE_H


#include <Looper.h>

#include <SupportDefs.h>
#include "TaskSync.h"
#include "Task.h"


class TaskMerge: public BLooper, public TaskSync {
public:
	 						TaskMerge(void);
							~TaskMerge(void);
		
	/**
	 *is called to prepare the TaskSync Object for Running
	 *here you should do things like prepare everything login and so on
	 *returns the status of the how far the Init came.
	 */
	status_t				Init(void);
	
	
	/**
	 * returns a List of Task Objects 
	 */
	BObjectList<Task>*		GetTasks();
	BObjectList<Task>*		GetTasks(TaskList ctgr);
	
	Task*					GetTask(BString id);
	
	
	status_t				AddTask(Task *tsk);
	
	status_t				UpdateTask(BString id,Task *tsk);
	
	status_t				RemoveTask(BString id);
	
	
	/**
	 * returns a List of BString of all TaskLists
	 */
	BObjectList<TaskList>*	GetTaskLists();

	TaskList*				GetTaskList(BString id);

	status_t				AddTaskList(TaskList *ctgr);	
	
	status_t				UpdateTaskList(BString id,TaskList *ctgr);

	//what should we do? should we delete all task wich belong to this cateorie?
	status_t				RemoveTaskList(BString id);

	/**
	 *
	 *
	 */
	status_t				AddTaskSync(TaskSync *sync);
	status_t				RemoveTaskSync(TaskSync *sync);
	
	status_t				MergeIn(BObjectList<Task> *newTaskList);
	Task*					MergeTasks(Task* firstTask, Task *secondTask);	
	 
								
private:
	BObjectList<Task>		*taskList;
	BObjectList<TaskSync>	*syncerList;

};


#endif // _H
