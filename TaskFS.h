#ifndef TASK_FS_H
#define TASK_FS_H

#include <FindDirectory.h>
#include <StorageKit.h>

#include "TaskColumns.h"
#include "TaskSync.h"
#include "TasksApp.h"


struct attrib {
	const char*	attribute;
	type_code	type;
	bool		isPublic;
	bool		editable;
	int32		width;
	const char*	name;
};

const attrib sDefaultAttributes[] = {
	{ "META:completed", B_BOOL_TYPE, true, true, 40, B_TRANSLATE("Completed") },
	{ "META:title", B_STRING_TYPE, true, true, 120, B_TRANSLATE("Titel") },
	{ "META:category",B_STRING_TYPE, true, true, 120, B_TRANSLATE("Category") },
	{ "META:notes", B_STRING_TYPE, true, true, 50, B_TRANSLATE("Notes") },
	{ "META:priority", B_UINT32_TYPE, true, true, 50, B_TRANSLATE("Priority") },
	{ "META:due", B_TIME_TYPE, true, true, 50, B_TRANSLATE("Due Time") },
	{ "META:task_id", B_INT32_TYPE, false, false, 50, "Task ID" },
	{ "META:task_url", B_STRING_TYPE, false, false, 50, "Task URL" },
	{ NULL, 0,false,false,0, NULL }
};

class TaskFS : public TaskSync {
public:
									TaskFS();
									~TaskFS();
	virtual	status_t				Init(void);
	
	virtual	BObjectList<BString>*	GetCategories(void);
	virtual	BObjectList<Task>*		GetTasks(void);
	virtual	BObjectList<Task>*		GetTasks(BString categorie);
	
	virtual status_t				UpdateTasks(BObjectList<Task>*);
	virtual status_t				UpdateCategories(BObjectList<BString>*);
	
private:
			status_t				PrepareFirstStart(void);
			status_t				TaskToFile(Task *theTask);
			Task*					FileToTask(BFile *theFile);
			
			BDirectory				tasksDir;
			
		
};

#endif
