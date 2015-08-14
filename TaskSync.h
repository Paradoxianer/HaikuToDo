#ifndef TASKSYNC_H
#define TASKSYNC_H

#include <vector>
#include "Task.h"
#include "TaskManager.h"


class TaskSync {
	public:
									TaskSync(void){};
									~TaskSync(void){};
		
	/**
	 *is called to prepare the TaskSync Object for Running
	 *here you should do things like prepare everything login and so on
	 *returns the status of the how far the Init came.
	 */
	virtual	status_t				Init(void)		= 0;
	
	/**
	 * returns a List of BString of all Categories
	 */
	virtual	BObjectList<String>*	GetCategories()				= 0;
	
	/**
	 * returns a List of Task Objects 
	 */
	virtual	BObjectList<Task>*		GetTasks() 					= 0;
	
	virtual status_t				UpdateTasks(BObjectList<Task>*) = 0;
	
	virtual status_t				UpdateCategories(BObjectList<String>*) = 0;
	
	
};

#endif
