#include "TasksApp.h"

int
main(int argc, const char *argv[])
{
	TaskApp app;
	app.Run();
	return 0;
}


TaskApp::TaskApp()
	:	BApplication(APP_SIG)
{
}


TaskApp::~TaskApp()
{
}


status_t TaskApp::FirstStart()
{
	/*This should go into the Settings Class
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path, true);

	BDirectory dir(path.Path());
	BEntry entry;
	if (dir.FindEntry("HaikuToDo", &entry) == B_OK) {
		//Read the Settings
	} else {
		//Create the Settings
	}
	*/
	
	// Read attributes from task mime type. If it does not exist,
	// or if it contains no attribute definitions, install a "clean"
	// person mime type from the hard-coded default attributes.

	bool valid = false;
	BMimeType mime(TASK_MIMETYPE);
	if (mime.IsInstalled()) {
		BMessage info;
		if (mime.GetAttrInfo(&info) == B_NO_ERROR) {
			int32 index = 0;
			while (true) {
				int32 type;
				if (info.FindInt32("attr:type", index, &type) != B_OK)
					break;
				bool editable;
				if (info.FindBool("attr:editable", index, &editable) != B_OK)
					break;

				// TODO: Support other types besides string attributes.
				if (type != B_STRING_TYPE || !editable)
					break;

				Attribute* attribute = new Attribute();
				ObjectDeleter<Attribute> deleter(attribute);
				if (info.FindString("attr:public_name", index,
						&attribute->name) != B_OK) {
					break;
				}
				if (info.FindString("attr:name", index,
						&attribute->attribute) != B_OK) {
					break;
				}

				if (!fAttributes.AddItem(attribute))
					break;

				deleter.Detach();
				index++;
			}
		}
		if (fAttributes.CountItems() == 0) {
			valid = false;
			mime.Delete();
		} else
			valid = true;
	}
	
	
}

void TaskApp::MessageReceived(BMessage *message)
{
}


int32 TaskApp::EventLoop(void *data)
{
	TaskApp *app = (TaskApp *)data;
	return 0;
}
