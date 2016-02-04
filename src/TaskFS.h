#ifndef TASK_FS_H
#define TASK_FS_H

#include <Handler.h>
#include <FindDirectory.h>
#include <StorageKit.h>
#include <Locale.h>
#include <Catalog.h>

#include "TaskColumns.h"
#include "TaskSync.h"
#include "TaskList.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TaskApp"
	

struct attrib {
	const char*	attribute;
	type_code	type;
	const char*	displayAs;
	bool		isPublic;
	bool		editable;
	int32		width;
	const char*	name;
};


const attrib sDefaultAttributes[] = {
	{ "META:completed", B_BOOL_TYPE, "checkbox",true, true, 40, B_TRANSLATE("Completed") },
	{ "META:taskList",B_STRING_TYPE, NULL,false, false, 120, B_TRANSLATE("List") },
	{ "META:notes", B_STRING_TYPE, NULL,true, true, 50, B_TRANSLATE("Notes") },
	{ "META:priority", B_UINT32_TYPE,NULL, true, true, 50, B_TRANSLATE("Priority") },
	{ "META:due", B_TIME_TYPE, NULL,true, true, 50, B_TRANSLATE("Due Time") },
	{ "META:task_id", B_STRING_TYPE,NULL, false, false, 50, "Task ID" },
	{ "META:task_url", B_STRING_TYPE, NULL,false, false, 50, "Task URL" },
	{ NULL, 0,NULL,false,false,0, NULL }
};

class TaskFS : public TaskSync, public BHandler {
public:
									TaskFS();
									~TaskFS();
	virtual	status_t				Init(void);
	
	/* =========== Handel all Tasks related operations ========*/
	
	virtual	Task*					GetTask(BString forID);
	virtual	BObjectList<Task>*		GetTasks(void);
	virtual	BObjectList<Task>*		GetTasks(TaskList list);
	
	virtual status_t				AddTask(Task *tsk);
	
	virtual	status_t				UpdateTask(BString id, Task *tsk);
	
	virtual status_t				RemoveTask(BString id);
	
	
	/* ========= Handel all List related operations ======*/

	virtual	BObjectList<TaskList>*	GetTaskLists(void){return taskListList;};
	virtual	TaskList*				GetTaskList(BString id);

	virtual status_t				AddTaskList(TaskList *ctgr);
	
	virtual status_t				UpdateTaskList(BString id,TaskList *uList);

	//promt and ask if all Taks should be deleted or if the TaskList just be blank
	virtual status_t				RemoveTaskList(BString id);
	

	
private:
			status_t				SetUpMimeTyp(void);
			status_t				PrepareFirstStart(void);
			status_t				TaskToFile(Task *theTask, bool overwrite = true);
			Task*					FileToTask(entry_ref theEntryRef);
			TaskList*				DirectoryToList(BEntry *theEntry);
			
			entry_ref*				FileForId(Task *theTask);
			
			BDirectory				tasksDir;
			
			BObjectList<Task>*		taskList;
			BObjectList<TaskList>*	taskListList;
		
};

#endif
