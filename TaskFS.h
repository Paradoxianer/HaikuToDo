#ifndef TASK_FS_H
#define TASK_FS_H

#include <FindDirectory.h>
#include <StorageKit.h>

#include "TaskManager.hpp"

class TaskFS : public TaskSync{
	public:
				TaskFS();
				~TaskFS();
		void	LoadTasks(const char* category,BListView* tasks);
		void	LoadCategories(BListView* categories);
		bool	AddCategory(const char* name, const char* filename);
		bool	AddTask(const char* title, const char* description, const char* category);
		bool	RemoveTask(const char* title, const char* description, const char* category);
		bool	MarkAsComplete(const char* title, const char* description, const char* category);
	private:
		BAlert* error;
		BString categories;
		BString tasks;
		int volume;
};

#endif
