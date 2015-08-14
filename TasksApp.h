#ifndef TaskApp_H
#define TaskApp_H

#include <Application.h>
#include <Locale.h>
#include <Catalog.h>

#include "Task.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TaskApp"

#define	TASK_MIMETYPE	"text/task"
#define APP_SIG			"application/x-vnd.Tasks"

#define TASK_DIRECTORY	"Tasks"



struct attrib {
	const char*	attribute;
	type_code	type;
	bool		isPublic;
	bool		editable;
	int32		width;
	const char*	name;
};

struct attrib sDefaultAttributes[] = {
	{ "META:title", B_STRING_TYPE, true, true, 120, B_TRANSLATE("Titel") },
	{ "META:completed", B_BOOL_TYPE, true, true, 120, B_TRANSLATE("Completed") },
	{ "META:category",B_STRING_TYPE, true, true, 120, B_TRANSLATE("Category") },
	{ "META:due", B_TIME_TYPE, true, true, 50, B_TRANSLATE("Due Time") },
	{ "META:priority", B_UINT32_TYPE, true, true, 50, B_TRANSLATE("Priority") },
	{ NULL, NULL,true,true,0, NULL }
};





class TaskApp : public BApplication {
public:
					TaskApp();
					~TaskApp();

	/*setting up the folder and Mimetype
	  if the user want ot login into google we need to
	  do the authentification here over google sync
	  and store the token and necessary informations
	  in the keymanager from haiku
	  */
		status_t	FirstStart(void);
	
	/*called on StartUp scan through all files
	  if google sync is on download the latest list and
	  sync it with the google list if google sync is on 
	  start the Sync Loop wich monitors all nodes...
	*/
		status_t	StartUp(void);
	
		Task		Merge(Task firstTask,Task secondTask);

		void		MessageReceived(BMessage *message);
	
static	int32		EventLoop(void *data);

private:
		BString				taskDir;
		sem_id				fEventLock;
		BList				fEvents;
		BList				fAllEvents;
		sem_id				fNotify;
		thread_id			fEventLoop;
		bool				fQuiting;
};

#endif
