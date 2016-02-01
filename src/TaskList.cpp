
#include "TaskList.h"

#include "TaskSync.h"
#include "Task.h"

TaskList::TaskList(): BArchivable()
{
	Init();
}

TaskList::TaskList(const char* newName)
	: BArchivable()
{
	Init();
	name=newName;
}

TaskList::TaskList(const char* newName, const char* newID, time_t lastUpdate, const char* newURL, TaskSync *newSource)
	: BArchivable()
{
	Init();
	name.SetTo(newName);
	id.SetTo(newID);
	updated	= lastUpdate;
	url.SetTo(newURL);
	SetSource(newSource);
}


TaskList::TaskList(BMessage *message)
	:BArchivable(message)
{
	Init();
	message->FindString("name",&name);
	message->FindInt32("update",(int32 *)&updated);
	message->FindString("id",&id);
	message->FindString("url",&url);
	
}

void TaskList::Init(void)
{
	source		= NULL;
	name		= "";
	updated		= real_time_clock();
	id			= (int32)real_time_clock();
	url			= "";
}


status_t TaskList::Archive(BMessage* archive, bool deep)
{
	BArchivable::Archive(archive, deep);
	archive->AddString("name",name.String());
	archive->AddInt32("update",(int32)updated);
	archive->AddString("id", id.String());
	archive->AddString("url",url.String());
	return B_OK;
}


BArchivable* TaskList::Instantiate(BMessage* archive)
{
   if ( !validate_instantiation(archive, "TaskList") )
      return NULL;
   return new TaskList(archive);
}

bool TaskList::operator==(const TaskList& other) const
{
	return     name == other.name 
			&& id == other.id
			&& url == other.url
			&& updated == other.updated;
}
