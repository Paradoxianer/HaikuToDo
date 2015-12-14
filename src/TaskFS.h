#ifndef TASK_FS_H
#define TASK_FS_H

#include <FindDirectory.h>
#include <StorageKit.h>
#include <Locale.h>
#include <Catalog.h>

#include "TaskColumns.h"
#include "TaskSync.h"
#include "Category.h"

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
	{ "META:category",B_STRING_TYPE, NULL,true, true, 120, B_TRANSLATE("Category") },
	{ "META:notes", B_STRING_TYPE, NULL,true, true, 50, B_TRANSLATE("Notes") },
	{ "META:priority", B_UINT32_TYPE,NULL, true, true, 50, B_TRANSLATE("Priority") },
	{ "META:due", B_TIME_TYPE, NULL,true, true, 50, B_TRANSLATE("Due Time") },
	{ "META:task_id", B_STRING_TYPE,NULL, false, false, 50, "Task ID" },
	{ "META:task_url", B_STRING_TYPE, NULL,false, false, 50, "Task URL" },
	{ NULL, 0,NULL,false,false,0, NULL }
};

class TaskFS : public TaskSync {
public:
									TaskFS();
									~TaskFS();
	virtual	status_t				Init(void);
	
	/* =========== Handel all Tasks related operations ========*/
	
	virtual	BObjectList<Task>*		GetTasks(void);
	virtual	Task*					GetTask(BString forID);
	virtual	BObjectList<Task>*		GetTasks(Category ctgr);
	
	virtual status_t				AddTask(Task *tsk);
	
	virtual	status_t				UpdateTask(BString id, Task *tsk);
	
	virtual status_t				RemoveTask(BString id);
	
	
	/* ========= Handel all Category related operations ======*/

	virtual	BObjectList<Category>*	GetCategories(void){return categoryList;};
	virtual	Category*				GetCategorie(BString id);

	virtual status_t				AddCategorie(Category *ctgr);
	
	virtual status_t				UpdateCategorie(BString id,Category *ctgr);

	//promt and ask if all Taks should be deleted or if the category just be blank
	virtual status_t				RemoveCategorie(BString id);
	

	
private:
			status_t				PrepareFirstStart(void);
			status_t				TaskToFile(Task *theTask, bool overwrite = true);
			Task*					FileToTask(entry_ref theEntryRef);
			
			entry_ref*				FileForId(Task *theTask);
			
			BDirectory				tasksDir;
			
			BObjectList<Task>*		taskList;
			BObjectList<Category>*	categoryList;
		
};

#endif
