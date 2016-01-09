/* HaikuToDo - a small tool for your ToDo List. It uses the Tracker
   as a front end. You create task by creating files with some special
   attributes. It syncs your ToDo List with your google account
   Based on the work done during gci 2014
 

   Copyright 2015 Matthias Lindner, released under the MIT licence.
*/

#include "Task.h"

Task::Task()
	: BArchivable()
{
	Init();
}


Task::Task(const char* title,TaskList *tList,const char* newID,bool completed, TaskSync *newParent)
	: BArchivable()
{
	Init();
	SetTitle(title);
	SetTaskList(tList);
	SetID(newID);
	Complete(completed);
	SetTaskSync(newParent);
}



Task::Task(BMessage *message)
	:BArchivable(message)
{
	Init();
	//read all Attributes from BMessage
	BString dueString;
	BString statusString;
	BString listID;
	
	message->FindString("title",&title);
	
	if (message->FindString("taskList",&listID) == B_OK){
		//**find ID
		belongTo=new TaskList("");
		belongTo->SetID(listID);
	}
	
	message->FindInt32("update", (int32 *)&updated);
	
	message->FindString("notes",&notes);
	
	
	message->FindInt32("due",(int32 *)&dueString);
	
	message->FindBool("status", &completed);
}



void Task::Init(void)
{
	parent		= NULL;
	title		= "";
	completed	= false;
	belongTo	= NULL;
	updated		= real_time_clock();
	notes		= "";
	due			= 0;
	priority	= 10;
	
}


status_t Task::Archive(BMessage* archive, bool deep)
{
	BArchivable::Archive(archive, deep);
	archive->AddString("title",title.String());
	archive->AddString("taskList",belongTo->ID());
	archive->AddInt32("update",(int32)updated);
	if (notes.CountChars()>0)
		archive->AddString("notes",notes.String());
	if (due != 0) {
		archive->AddInt32("due",(int32)due);
	}
	archive->AddBool("status", "needsAction");
	//just add priority
	archive->AddInt32("priority", priority);
	
	return B_OK;
}


BArchivable* Task::Instantiate(BMessage* archive)
{
   if ( !validate_instantiation(archive, "Task") )
      return NULL;
   return new Task(archive);
}


bool Task::operator==(const Task& other) const
{
	return     title == other.title 
			&& completed == other.completed
			&& *belongTo == *(other.belongTo)
			&& updated == other.updated
			&& notes == other.notes 
			&& due == other.due;
}
