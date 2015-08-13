#ifndef TASKSYNC_H
#define TASKSYNC_H

#include <vector>
#include "Task.h"

const int32 SYNC_CATEGORIES=800;

class TaskSync {
	public:
							TaskSync(){};
							~TaskSync(){};
		virtual bool		Login(){};
		virtual void		NextStep(BString code){};
		virtual	BList*		GetCategories(){};
		virtual	BList*		GetTasks(Category* category){};
};

#endif
