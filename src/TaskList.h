#ifndef TASK_LIST_H
#define TASK_LIST_H


#include <Archivable.h>
#include <String.h>
#include <ObjectList.h>


class Task;
class TaskSync;

class TaskList : public BArchivable {
public:
								TaskList();
								TaskList(const char* newName);
								TaskList(const char* newName, const char* newID, time_t lastUpdate  = 0, const char* newURL = NULL, TaskSync *newSource=NULL);
								TaskList(BMessage *archive);
								~TaskList() {};
							
							
			status_t			Archive(BMessage* archive, bool deep = true);
	static	BArchivable*		Instantiate(BMessage* archive);
	
	const	char*				Name(void) {return name.String();};
			void				SetName(BString newName){name.SetTo(newName);};
	
			time_t				LastUpdate(void){return updated;};
			void				SetLastUpdate(bigtime_t newTime){ updated=newTime; };

	const	char*				ID(void){return id.String();};
			void				SetID(const char* newID){id.SetTo(newID);};
			
			BString				URL(void){return url.String();};
			void				SetURL(BString newURL){url=newURL;};

			TaskSync*			Source(void){return source;};
			void				SetSource(TaskSync *newSource){source=newSource;};


			BObjectList<Task>*	GetTasks(void) {return taskList;};
			
			bool				operator==(const TaskList& task) const;

protected:
			void				Init(void);

		
private:
			TaskSync*			source;
			BString				name;
			time_t				updated;
			
			//should  we implement  etag?
			BString				id;
			BString				url;
			BObjectList<Task>*	taskList;
			
};

#endif
