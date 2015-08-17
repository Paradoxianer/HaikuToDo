#ifndef TASK_SYNC_H
#define TASK_SYNC_H

#include <String.h>
#include <ObjectList.h>
#include "Task.h"
#include "Category.h"


class TaskSync {
	public:
			  						TaskSync(void){};
	virtual							~TaskSync(void){};
		
	/**
	 *is called to prepare the TaskSync Object for Running
	 *here you should do things like prepare everything login and so on
	 *returns the status of the how far the Init came.
	 */
	virtual	status_t				Init(void)		= 0;
	
	/**
	 * returns a List of BString of all Categories
	 */
	virtual	BObjectList<Category>*	GetCategories()				= 0;
	
	/**
	 * returns a List of Task Objects 
	 */
	virtual	BObjectList<Task>*		GetTasks() 					= 0;
	
	virtual status_t				UpdateTasks(BObjectList<Task>*) = 0;
	
	virtual status_t				UpdateCategories(BObjectList<Category>*) = 0;
	
	
};

#endif
