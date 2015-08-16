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
	SetCategory(Category(category));
	Complete(completed);
}



Task::Task(BMessage *message)
	:BArchivable(message)
{
	Init();
	//read all Attributes from BMessage
	BString dueString;
	BString statusString;
	BString categoryString;
	
	message->FindString("title",&title);
	
	if (message->FindString("category",&categoryString) == B_OK){
		category=Category(categoryString.String());
	}
	
	message->FindInt32("update", (int32 *)&updated);
	
	message->FindString("notes",&notes);
	
	
	message->FindInt32("due",(int32 *)&dueString);
	
	message->FindBool("status", &completed);
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
	BArchivable::Archive(archive, deep);
	archive->AddString("title",title.String());
	archive->AddString("category",category.Name());
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
			&& category == other.category 
			&& updated == other.updated
			&& notes == other.notes 
			&& due == other.due;
}
