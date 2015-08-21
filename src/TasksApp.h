#ifndef TaskApp_H
#define TaskApp_H

#include <Application.h>
#include <Locale.h>
#include <Catalog.h>

#include "Task.h"
#include "TaskFS.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TaskApp"

#define	APP_SIG	"application/x-vnd.Tasks"
#define TASK_DIRECTORY "tasks"

#define TASK_MIMETYPE "application/x-tasks"


static const char* tasksKeyring = "Tasks";


class TaskApp : public BApplication {
public:
					TaskApp();
					~TaskApp();

	/*creates a Settings File and a Settings Folder
	  */
		status_t	FirstStart(void);
	
	/*called on StartUp scan through all files
	  if google sync is on download the latest list and
	  sync it with the google list if google sync is on 
	  start the Sync Loop wich monitors all nodes...
	*/
		status_t	StartUp(void);
	
		void		MessageReceived(BMessage *message);
	
static	int32		EventLoop(void *data);

private:
		
		TaskFS				*fsSync;
		
		sem_id				fEventLock;
		BList				fEvents;
		BList				fAllEvents;
		sem_id				fNotify;
		thread_id			fEventLoop;
		bool				fQuiting;
};

#endif
