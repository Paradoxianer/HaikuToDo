#ifndef TASK_H
#define TASK_H

#include <Archivable.h>
#include <Message.h>
#include <File.h>
#include <String.h>

#include <time.h>

#include "TaskList.h"

class TaskSync;


/** Maybe we should derive this and TaskList from one "Parent" 
  *	wich just contains the 
  * ID
  * Titel / Name
  * LastUpdated
  * URL
  */
class Task : public BArchivable{
public:
							Task();
							Task(const char* title,TaskList *newList,const char* newID,bool completed = false, TaskSync *newSource=NULL);
							Task(BMessage *message);
						
			status_t		Archive(BMessage* archive, bool deep = true);
	static	BArchivable*	Instantiate(BMessage* archive);
	

	const	char*			Title(void) {return title.String();};
			void			SetTitle(const char* newTitle){title.SetTo(newTitle);};
	
			bool			IsCompleted(void) { return completed;};
			void			Complete(bool newCompleted=true) { completed=newCompleted;};
	
			TaskList*		GetTaskList(void) { return belongTo;};
			void			SetTaskList(TaskList *newTaskList) {belongTo=newTaskList;}
			
			time_t			LastUpdate(void){return updated;};
			void			SetLastUpdate(bigtime_t newTime){ updated=newTime; };

	const	char*			Notes(void){return notes.String();};
			void			SetNotes(BString newNotes) { notes.SetTo(newNotes);};

			time_t			DueTime(void){return due;};
			void			SetDueTime(bigtime_t newDueTime){due=newDueTime; };
			
			
			uint32			Priority(void){return priority;};
			void			SetPriority(uint32 newPriority){priority=newPriority;};
			
	const	char*			ID(void){return id.String();};
			void			SetID(const char* newID){id.SetTo(newID);};

	const	char*			URL(void){return url.String();};
			void			SetURL(BString newURL){url=newURL;};


			TaskSync*		Source(void){return source;};
			void			SetSource(TaskSync *newSource){source=newSource;};

			
			bool			operator==(const Task& task) const;


protected:
			void			Init(void);

		
private:
			TaskSync*		source;
			BString			title;
			bool			completed;
			TaskList*		belongTo;
			time_t			updated; //this should be hidden at least in the tracker
			
			//this are optional
			BString			notes;
			time_t			due;  //time when this todo should be finished
			
			
			/*this is haiku only supported ... we maybe blend this into notes or title or better calculate
			  the position (from the google task out of this
			*/
			uint32			priority;
			
			// we should implement the id, etag, selfLink
			BString			id;
			BString			url;
			
			
};

#endif
