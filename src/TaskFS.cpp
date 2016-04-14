
#include <iostream>
#include <stdio.h>

#include <kernel/fs_index.h>


#include <Resources.h>

#include "Task.h"
#include "TaskFS.h"
#include "TaskColumns.h"
#include "TasksApp.h"


TaskFS::TaskFS(void):BHandler("FS_Sync")
{
	taskList		= NULL;
	taskListList	= NULL;
}


TaskFS::~TaskFS()
{
	delete taskListList;
	delete taskList;
}

status_t TaskFS::Init(void)
{
	taskList		= new BObjectList<Task>();
	taskListList	= new BObjectList<TaskList>();
	return PrepareFirstStart();
}

Task* TaskFS::GetTask(BString forID)
{
	Task	*tmpTask	= NULL;
	int32	i			= 0;
	bool	found		= false;
	//grab a up to date List of Items
	GetTasks();
	while (i<taskList->CountItems() && found!=true)
	{
		tmpTask=taskList->ItemAt(i);
		found = tmpTask->ID() == forID;
		i++;
	}
	
	if (found == true)
		return tmpTask;
	else
		return NULL;
}

BObjectList<Task>* TaskFS::GetTasks(void)
{
	BEntry *tmpEntry = new BEntry();
	taskList->MakeEmpty();
	taskListList->MakeEmpty();
	while (tasksDir.GetNextEntry(tmpEntry, false) == B_OK)
		if (tmpEntry->IsDirectory())
			taskListList->AddItem(DirectoryToList(tmpEntry));
	return taskList;
	tasksDir.Rewind();
}


BObjectList<Task>* TaskFS::GetTasks(TaskList forCategorie)
{
	Task				*tmpTask		= NULL;
	BObjectList<Task>	*taskTaskList	= new BObjectList<Task>();
	int32	i;
	//grab a up to date List of Items
	GetTasks();
	for (i = 0; i<taskList->CountItems();i++) {
		tmpTask=taskList->ItemAt(i);
		if (*(tmpTask->GetTaskList()) == forCategorie)
			taskTaskList->AddItem(tmpTask);
	}
	return taskTaskList;
}


status_t TaskFS::AddTask(Task *tsk)
{
	return TaskToFile(tsk, true);
}


status_t TaskFS::UpdateTask(BString id, Task *tsk)
{
	//it should only write a file if there is 
	return TaskToFile(tsk, false);
}


status_t TaskFS::RemoveTask(BString id)
{
	entry_ref *ref = FileForId(GetTask(id));
	BEntry deleter(ref);
	deleter.Remove();
}


TaskList* TaskFS::GetTaskList(BString id)
{
	//**.. try to find the right ... id
}


status_t TaskFS::AddTaskList(TaskList *ctgr)
{
	//**create folder
	
	return B_OK;
}
	
status_t  TaskFS::UpdateTaskList(BString id,TaskList *ctgr)
{
	//find the name for the given id  then replace all File Attribs with the 
	//new TaskList Name
}


status_t TaskFS::RemoveTaskList(BString id)
{
	//find all entrys with categories... and delete it....
}


status_t TaskFS::PrepareFirstStart()
{
	status_t err = B_OK;
	//first create the task directory
	BPath homeDir;
	find_directory(B_USER_DIRECTORY, &homeDir);

	BString tasksDirString = homeDir.Path();
	tasksDirString << "/" << TASK_DIRECTORY;

	tasksDir = BDirectory(tasksDirString.String());
	//if the folder was not found create it
	if (tasksDir.InitCheck() != B_OK) {
		BDirectory homeDirectory(homeDir.Path());
		err = homeDirectory.CreateDirectory(TASK_DIRECTORY,
			&tasksDir);
		if (err != B_OK)
			printf("Failed to create tasks directory (%s)\n",
				strerror(err));
		ssize_t written = tasksDir.WriteAttr("_trk/columns_le", B_RAW_TYPE,
			0, task_columns, sizeof(task_columns));
		if (written < 0)
			printf("Failed to write column info (%s)\n", strerror(written));
	}
	SetUpMimeTyp();
	return err;
}

status_t TaskFS::SetUpMimeTyp(void)
{
	status_t err;
	//set the MimeType
	BMimeType mime(TASK_MIMETYPE);
	//later do better check
	bool valid = mime.IsInstalled();
	if (!valid) {
		mime.Install();
		mime.SetShortDescription(B_TRANSLATE_CONTEXT("Tasks",
			"Short mimetype description"));
		mime.SetLongDescription(B_TRANSLATE_CONTEXT("Tasks",
			"Long mimetype description"));
		//get the icon from our Ressources
		BResources* res = BApplication::AppResources();
		if (res != NULL){
			size_t size;
			const void* data = res->LoadResource(B_VECTOR_ICON_TYPE, "TASK_ICON", &size);
			if (data!=NULL)
				mime.SetIcon(reinterpret_cast<const uint8*>(data), size);
		}
		mime.SetPreferredApp(APP_SIG);

		// add default task fields to meta-mime type
		BMessage fields;
		for (int32 i = 0; sDefaultAttributes[i].attribute; i++) {
			fields.AddString("attr:public_name", sDefaultAttributes[i].name);
			fields.AddString("attr:name", sDefaultAttributes[i].attribute);
			fields.AddInt32("attr:type", sDefaultAttributes[i].type);
			fields.AddString("attr:display_as", sDefaultAttributes[i].displayAs);
			fields.AddBool("attr:viewable", sDefaultAttributes[i].isPublic);
			fields.AddBool("attr:editable", sDefaultAttributes[i].editable);
			fields.AddInt32("attr:width", sDefaultAttributes[i].width);
			fields.AddInt32("attr:alignment", B_ALIGN_LEFT);
			fields.AddBool("attr:extra", false);
		}
		mime.SetAttrInfo(&fields);
			// create indices on all volumes for the found attributes.
		int32 count = 8;
		BVolumeRoster volumeRoster;
		BVolume volume;
		while (volumeRoster.GetNextVolume(&volume) == B_OK) {
			for (int32 i = 0; i < count; i++) {
				if (sDefaultAttributes[i].isPublic == true)
					fs_create_index(volume.Device(), sDefaultAttributes[i].attribute,
						sDefaultAttributes[i].type, 0);
			}
		}
	}
	else
		err = B_OK;
	return err;
}



status_t TaskFS::TaskToFile(Task *theTask, bool overwrite)
{
	BFile		taskFile;
	BEntry		entry;
	status_t	err;
	TaskList	*tskLst	= theTask->GetTaskList();
	
	BDirectory	dir		= BDirectory();

	bool	completed	= theTask->IsCompleted();
	uint32	priority	= theTask->Priority();
	time_t	due			= theTask->DueTime();


	//first find directory.. then create files in this directory
	if (tskLst!=NULL)
		dir.SetTo(&tasksDir,tskLst->Name());
	else
		dir.SetTo(&tasksDir,".");

	
	//first check if the File already exists..
	//if not and overwrite is on check the ids..
	// and search for the correspondending file...

	if (dir.FindEntry(theTask->Title(),&entry) == B_OK) {
		taskFile.SetTo((const BEntry*)&entry,B_READ_WRITE);
		err = B_OK;
	} 
	else {
		entry_ref *ref= FileForId(theTask);
		if (ref==NULL){
			dir.CreateFile(theTask->Title(),&taskFile,overwrite);
			dir.FindEntry(theTask->Title(),&entry);
		}
		else {
			entry.SetTo(ref);
			taskFile.SetTo((const BEntry*)ref,B_READ_WRITE);
		}
	}
	if (taskFile.InitCheck() == B_OK){
		taskFile.WriteAttr("META:completed",B_BOOL_TYPE, 0, &completed, sizeof(completed));
		entry.Rename(theTask->Title());
		taskFile.WriteAttrString("META:taskList",new BString(theTask->GetTaskList()->ID()));
		taskFile.WriteAttrString("META:notes",new BString(theTask->Notes()));
		taskFile.WriteAttr("META:priority", B_UINT32_TYPE, 0, &priority, sizeof(priority));
		taskFile.WriteAttr("META:due", B_TIME_TYPE, 0, &due, sizeof(due));
		taskFile.WriteAttrString("META:task_id",  new BString(theTask->ID()));
		taskFile.WriteAttrString("META:task_url",new BString(theTask->URL()));
	}
	else
		err=B_ERROR;
	return err; 
}


Task* TaskFS::FileToTask(entry_ref theEntryRef)
{
	Task*	newTask		=new Task();
	//needed for the "attribute stuff
	BFile	theFile(&theEntryRef,B_READ_ONLY);
	//needed to get out the name
	BEntry	theEntry(&theEntryRef);
	
	bool	completed;
	char	name[B_FILE_NAME_LENGTH];
	BString	taskListID;
	BString	notes;
	uint32	priority;
	time_t	due;
	BString	id;
	BString	url;	
	
	//maby do a check if everything went ok and only if so set the values
	theFile.ReadAttr("META:completed",B_BOOL_TYPE, 0, &completed, sizeof(completed));
	theEntry.GetName(name);
	theFile.ReadAttrString("META:taskList",&taskListID);
	theFile.ReadAttrString("META:notes",&notes);
	theFile.ReadAttr("META:priority", B_UINT32_TYPE, 0, &priority, sizeof(priority));
	theFile.ReadAttr("META:due", B_TIME_TYPE, 0, &due, sizeof(due));
	theFile.ReadAttrString("META:task_id", &id);
	theFile.ReadAttrString("META:task_url",&url);
	
	//** find TaskList from ID//
	
	TaskList *newTaskList = new TaskList("");
	newTaskList->SetID(taskListID);
	newTask->Complete(completed);
	newTask->SetTitle(BString(name));
	newTask->SetTaskList(newTaskList);
	newTask->SetNotes(notes);
	newTask->SetPriority(priority);
	newTask->SetDueTime(due);
	newTask->SetID(id);
	newTask->SetURL(url);
	return newTask;
}


BEntry* TaskFS::ListToDirectory(TaskList *theList)
{
	BEntry		*entry	= NULL;
	BDirectory	*dir	= new BDirectory();
	int32		i		= 0;
	if (tasksDir.InitCheck()==B_OK) {
		if (tasksDir.CreateDirectory(theList->Name(),dir)==B_OK)
		{
			if (dir->InitCheck()==B_OK){
				dir->WriteAttrString("META:task_id",  new BString(theList->ID()));
				dir->WriteAttrString("META:task_url",new BString(theList->URL()));
				ssize_t written = tasksDir.WriteAttr("_trk/columns_le", B_RAW_TYPE,
					0, task_columns, sizeof(task_columns));
				if (written < 0)
					printf("Failed to write column info (%s)\n", strerror(written));
				for (i=0;i<theList->GetTasks()->CountItems();i++)
				{
					TaskToFile(theList->GetTasks()->ItemAt(i));
				}
				dir->SetModificationTime(theList->LastUpdate());
			}
		}
	}
	delete dir;
	return entry;
}


TaskList* TaskFS::DirectoryToList(BEntry *theEntry)
{
	TaskList*	newTaskList		=new TaskList();
	//needed for the "attribute stuff
	BFile	theFile(theEntry,B_READ_ONLY);
	//needed to get out the name
	
	char	name[B_FILE_NAME_LENGTH];
	BString	taskListID;
	BString	notes;
	time_t	mtime;
	BString	id;
	BString	url;	
	
	//maby do a check if everything went ok and only if so set the values
	theEntry->GetName(name);
	theEntry->GetModificationTime(&mtime);
	theFile.ReadAttrString("META:task_id", &id);
	theFile.ReadAttrString("META:task_url",&url);
	
	newTaskList->SetID(taskListID);
	newTaskList->SetName(BString(name));
	newTaskList->SetLastUpdate(mtime);
	newTaskList->SetID(id);
	newTaskList->SetURL(url);
	return newTaskList;
}



entry_ref* TaskFS::FileForId(Task *theTask)
{
	//**scan through all files for the ID or shell we do a querry??
	BString		fileID;
	entry_ref	*ref;
	BNode		theNode;
	
	while (tasksDir.GetNextRef(ref) == B_OK){
		theNode.SetTo(ref);
		if (theNode.ReadAttrString("META:task_id", &fileID) == B_OK)
			if (fileID==theTask->ID() && fileID.Length() != 0)
				return ref;
	}
	return NULL;
}
