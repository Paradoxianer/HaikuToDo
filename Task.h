#ifndef TASK_H
#define TASK_H

#include <Archivable.h>
#include <Message.h>
#include <File.h>
#include <String.h>

#include <time.h>

class Task : public BArchivable{
public:
							Task();
							Task(const char* title,const char* category,bool completed);
							Task(BMessage *message);
						
			status_t		Archive(BMessage* archive, bool deep = true);
	static	BArchivable*	Instantiate(BMessage* archive);
	

	const	char*			Title(void) {return title.String();};
			void			SetTitle(BString newTitle){title.SetTo(newTitle);};
	
			bool			IsCompleted(void) { return completed;};
			void			Complete(bool newCompleted=true) { completed=newCompleted;};
	
	const	char*			Category(void) { return category.String();};
			void			SetCategory(BString newCategory) {category.SetTo(newCategory);}
			
			time_t			LastUpdate(void){return updated;};
			void			SetLastUpdate(bigtime_t newTime){ updated=newTime; };

	const	char*			Notes(void){return notes.String();};
			void			SetNotes(BString newNotes) { notes.SetTo(newNotes);};

			time_t			DueTime(void){return due;};
			void			SetDueTime(bigtime_t newDueTime){due=newDueTime; };
			
			
			uint32			Priority(void){return priority;};
			void			SetPriority(uint32 newPriority){priority=newPriority;};
			
			
			bool			operator==(const Task& task) const;


protected:
			void			Init(void);

		
private:
			BString			title;
			bool			completed;
			BString			category;
			time_t			updated; //this should be hidden at least in the tracker
			
			//this are optional
			BString			notes;
			time_t			due;  //time when this todo should be finished
			
			//should we implement the id, etag, selfLink
			
			
			/*this is haiku only supported ... we maybe blend this into notes or title or better calculate
			  the position (from the google task out of this
			*/
			uint32			priority;
		
};

#endif
