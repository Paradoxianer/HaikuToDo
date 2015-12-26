#ifndef TASK_LIST_H
#define TASK_LIST_H


#include <Archivable.h>
#include <String.h>
class Task;

class TaskList : public BArchivable {
public:
							TaskList();
							TaskList(const char* newName);
							TaskList(const char* newName, const char* newID, time_t lastUpdate, const char* newURL);
							TaskList(BMessage *archive);
							~TaskList() {};
							
							
			status_t		Archive(BMessage* archive, bool deep = true);
	static	BArchivable*	Instantiate(BMessage* archive);
	
	const	char*			Name(void) {return name.String();};
			void			SetName(BString newName){name.SetTo(newName);};
	
			time_t			LastUpdate(void){return updated;};
			void			SetLastUpdate(bigtime_t newTime){ updated=newTime; };

	const	char*			ID(void){return id.String();};
			void			SetID(const char* newID){id.SetTo(newID);};
			
			BString			URL(void){return url.String();};
			void			SetURL(BString newURL){url=newURL;};

			
			bool			operator==(const TaskList& task) const;

protected:
			void			Init(void);

		
private:
			BString			name;
			time_t			updated;
			
			// we should implement  etag
			BString			id;
			BString			url;
			
};

#endif
