#include "Category.h"

Category::Category(): BArchivable()
{
	Init();
}

Category::Category(const char* newName)
	: BArchivable()
{
	Init();
	name=newName;
}

Category::Category(const char* newName, int32 newID, time_t lastUpdate, const char* newURL)
	: BArchivable()
{
	Init();
	name.SetTo(newName);
	id		= newID;
	updated	= lastUpdate;
	url.SetTo(newURL);
}


Category::Category(BMessage *message)
	:BArchivable(message)
{
	Init();
	message->FindString("name",&name);
	message->FindInt32("update",(int32 *)&updated);
	message->FindInt32("id",&id);
	message->FindString("url",&url);
	
}

void Category::Init(void)
{
	name		= "";
	updated		= real_time_clock();
	id			= (int32)real_time_clock();
	url			= "";
}


status_t Category::Archive(BMessage* archive, bool deep)
{
	BArchivable::Archive(archive, deep);
	archive->AddString("name",name.String());
	archive->AddInt32("update",(int32)updated);
	archive->AddInt32("id", id);
	archive->AddString("url",url.String());
	return B_OK;
}


BArchivable* Category::Instantiate(BMessage* archive)
{
   if ( !validate_instantiation(archive, "Category") )
      return NULL;
   return new Category(archive);
}

bool Category::operator==(const Category& other) const
{
	return     name == other.name 
			&& id == other.id
			&& url == other.url
			&& updated == other.updated;
}
