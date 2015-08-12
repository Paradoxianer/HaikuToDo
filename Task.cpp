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


Task::Task(const char* title,const char* category,bool completed)
	: BArchivable()
{
	Init();
	SetTitle(title);
	SetCategory(category);
	Complete(completed);
}



Task::Task(BMessage *message)
	:BArchivable(message)
{
	Init();
	//read all Attributes from BMessage
	BString kind;
	BString updateString;
	BString dueString;
	BString statusString;
	if (message->FindString("kind",&kind) == B_OK)
		if (kind.Compare("task#task") != B_OK)
			return;
	
	message->FindString("title",&title);
	
	message->FindString("category",&category);
	
	if (message->FindString("update",&updateString) == B_OK) {
		//convert updateString into timeformat
	}
	
	message->FindString("notes",&notes);
	
	
	if (message->FindString("due",&dueString)  == B_OK) {
		//convert dueString into time format
	}
	
	if (message->FindString("status", &statusString)  == B_OK)
		if (statusString.ICompare("completed") == B_OK)
			completed = true;
		else
			completed = false;
}


Task::Task(BFile toDoFile)
{
	Init();
	//Read all Attributes from File
}


void Task::Init(void)
{
	title		= "";
	completed	= false;
	category	= "";
	updated		= real_time_clock();
	notes		= "";
	due			= 0;
	priority	= 10;
	
}


status_t Task::Archive(BMessage* archive, bool deep)
{
	BString updateString;
	BString dueString;
	BArchivable::Archive(archive, deep);
	archive->AddString("kind","task#task");
	archive->AddString("title",title.String());
	archive->AddString("category",category.String());
	archive->AddString("update",updateString);
	if (notes.CountChars()>0)
		archive->AddString("notes",notes.String());
	if (due != 0) {
		//generate dueString
		archive->AddString("due",dueString.String());
	}
	if (completed==false)
		archive->AddString("status", "needsAction");
	else
		archive->AddString("status", "completed");
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


status_t Task::WriteToFile(BFile toDoFile)
{
	//write Attributes
}


bool Task::operator==(const Task& other) const
{
	return     title.Compare(other.Title()) 
			&& completed == other.IsCompleted()
			&& category.Compare(other.Category()) 
			&& updated == other.LastUpdate()
			&& notes.Compare(other.Notes()) 
			&& due == other.DueTime());
}
