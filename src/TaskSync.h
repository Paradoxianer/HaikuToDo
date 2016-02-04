#ifndef TASK_SYNC_H
#define TASK_SYNC_H

#include <String.h>
#include <ObjectList.h>
#include <Handler.h>

#include "Task.h"
#include "TaskList.h"


class TaskSync  {
	public:
									TaskSync(void){};
	virtual							~TaskSync(void){};
		
	/**
	 *is called to prepare the TaskSync Object for Running
	 *here you should do things like prepare everything login and so on
	 *returns the status of the how far the Init came.
	 */
	virtual	status_t				Init(void)						= 0;
	
	
	/**
	 * returns a List of Task Objects 
	 */
	virtual	BObjectList<Task>*		GetTasks() 						= 0;
	virtual	BObjectList<Task>*		GetTasks(TaskList ctgr)			= 0;
	
	virtual	Task*					GetTask(BString id) 			= 0;
	
	
	virtual status_t				AddTask(Task *tsk)				= 0;
	
	virtual status_t				UpdateTask(BString id,Task *tsk)= 0;
	
	virtual status_t				RemoveTask(BString id)			= 0;
	
	
	/**
	 * returns a List of BString of all TaskLists
	 */
	virtual	BObjectList<TaskList>*	GetTaskLists()					= 0;

	virtual	TaskList*				GetTaskList(BString id)			= 0;

	virtual status_t				AddTaskList(TaskList *ctgr)		= 0;	
	
	virtual status_t				UpdateTaskList(BString id,TaskList *ctgr)	= 0;

	//what should we do? should we delete all task wich belong to this cateorie?
	virtual status_t				RemoveTaskList(BString id)		= 0;
	
};

#endif
