#ifndef ENTITY_H
#define ENTITY_H

#include <Archivable.h>
#include <Message.h>
#include <time.h>



/** " 
  *	Baseclass for Task and TaskList
  * ID
  * Titel / Name
  * LastUpdated
  * URL
  */
class Entity : public BArchivable{
public:
							Entity();
							Entity(const char* title, const char* newID, BString newURL);
							Entity(BMessage *message);
						
			status_t		Archive(BMessage* archive, bool deep = true);
	static	BArchivable*	Instantiate(BMessage* archive);
	

	const	char*			Title(void)							= 0;
			void			SetTitle(const char* newTitle)		= 0;
		
			
			time_t			LastUpdate(void)					= 0;
			void			SetLastUpdate(bigtime_t newTime)	= 0;

			
	const	char*			ID(void)							= 0;
			void			SetID(const char* newID)			= 0;

	const	char*			URL(void)							= 0;;
			void			SetURL(BString newURL)				= 0;


			
			bool			operator==(const Task& task) const	= 0;
			bool			operator<(const Task& task) const	= 0;
			bool			operator>(const Task& task) const	= 0;

};

#endif
