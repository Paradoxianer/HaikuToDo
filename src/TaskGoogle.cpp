#include <stdio.h>

#include <Roster.h>

#include <NetworkKit.h>
#include <UrlRequest.h>
#include <UrlSynchronousRequest.h>
#include <UrlProtocolRoster.h>
#include <HttpHeaders.h>
#include <HttpRequest.h>

#include <Key.h>
#include <KeyStore.h>
#include <private/shared/Json.h>

#include "TaskGoogle.h"
#include "InputRequest.h"
#include "Internet.hpp"
#include "TasksApp.h"

TaskGoogle::TaskGoogle(){
	tLists=new BObjectList<TaskList>(20);
	taskList=new BObjectList<Task>(20);
}

TaskGoogle::~TaskGoogle()
{
	
}

status_t TaskGoogle::Init(void)
{
	//first try to load token	
	status_t err = B_OK;
	if (LoadToken()!=B_OK){
		char *accesString = RequestAccessString();
		err = RequestToken(accesString);
	}
	else
		err = RequestToken(NULL);
	if (err == B_OK)
		err = Load();
	return err;
}

status_t TaskGoogle::LoadToken(){
	BPasswordKey key;
	BKeyStore keyStore;
	if (keyStore.GetKey(tasksKeyring, B_KEY_TYPE_PASSWORD, "refresh_token",
			key) == B_OK) {
		refreshToken = key.Password();
		return B_OK;
	}
	return B_ERROR;	
}



char* TaskGoogle::RequestAccessString(){
	char *accesString = new char[512];
	BString endpoint("https://accounts.google.com/o/oauth2/auth");
	endpoint.Append("?response_type=code");
	endpoint.Append("&client_id=318709342848-0h9712v3kbpcv1r7oc8krdrfu22ohlld.apps.googleusercontent.com");
	endpoint.Append("&redirect_uri=urn:ietf:wg:oauth:2.0:oob");
	endpoint.Append("&scope=https://www.googleapis.com/auth/tasks");
	std::cout << endpoint.String() << std::endl;
	const char *args[] = { endpoint.String(), 0 };
	//remove old request_token
	RemoveToken();
	be_roster->Launch("application/x-vnd.Be.URL.http",1,const_cast<char **>(args));
	InputRequest* accesReturn=new InputRequest(	B_TRANSLATE("Google Code"),
												B_TRANSLATE("Input Code"),
												B_TRANSLATE("Type in the Code wich Google showed you"),
												B_TRANSLATE("OK"),
												B_TRANSLATE("OK"));
	if (accesReturn->Go(&accesString) == B_ERROR) {
		accesString = NULL;
	}
	return accesString;
}



status_t TaskGoogle::RequestToken(char *accesString)
{
	BHttpForm*	form=new BHttpForm();
	BString		newRefreshToken;
	status_t	err = B_OK;
	//if there is no refreshToken do the "First Time Autentiation Procedure
	if (refreshToken.Length()>0) {
		form->AddString("refresh_token",refreshToken);
		form->AddString("client_id",CLIENT_ID);
		form->AddString("client_secret",CLIENT_SECRET);
		form->AddString("grant_type","refresh_token");
	}
	else{
		if (accesString==NULL)
			form->AddString("code",RequestAccessString());
		else
			form->AddString("code",accesString);
		form->AddString("client_id",CLIENT_ID);
		form->AddString("client_secret",CLIENT_SECRET);
		form->AddString("grant_type","authorization_code");
		form->AddString("redirect_uri",REDIRECT_URI);
	}
	form->SetFormType(B_HTTP_FORM_URL_ENCODED);
	//**Need to check for internetconnection and return an error if there i none
	BString oauth2("https://www.googleapis.com/oauth2/v3/token");
	BString tokenResponse(HaikuHTTP::GET(oauth2,form));
	
	BMessage tokenJson;
	BPrivate::BJson::Parse(tokenJson,tokenResponse);
	tokenJson.PrintToStream();
	
	//this should start the counter how long the token stays valid	
	token			= BString(tokenJson.GetString("access_token",""));
	std::cout << "Token access " << token.String() << std::endl;
	
	newRefreshToken = BString(tokenJson.GetString("refresh_token",""));
	if (newRefreshToken.Length() != 0){
		RemoveToken();
		refreshToken.SetTo(newRefreshToken);
		SaveToken();
		std::cout << "Refresh Token" << refreshToken.String() << std::endl;
		return B_OK;
	}
	else
		return B_OK;
}


status_t TaskGoogle::Load(void){
	//this will also call atm the LoadTasks()... later we should move this here
	LoadTaskLists();
	//LoadTasks();
}

status_t TaskGoogle::LoadTaskLists(){
	
	BString listUrlString("https://www.googleapis.com/tasks/v1/users/@me/lists?access_token=");
	listUrlString.Append(token);
	
	BString listsResponse(HaikuHTTP::GET(listUrlString));
	
	BMessage listsJson;
	BPrivate::BJson::Parse(listsJson,listsResponse);
	listsJson.PrintToStream();
	
	BMessage userLists;
	if(listsJson.FindMessage("items",0,&userLists)!=B_OK)
	{
		std::cerr << "ERROR: 'items' not found" << std::endl;
	}
	int32 lists=userLists.CountNames(B_ANY_TYPE);
	
	for(int32 currentList=0;currentList<lists;currentList++)
	{
		BString nameString;
		nameString << currentList;
		BMessage userList;
		if(userLists.FindMessage(nameString.String(),&userList)!=B_OK)
		{
			std::cerr << "ERROR: No '0' list found " << std::endl;
		}
		userList.PrintToStream();
		BString*	title		= new BString();
		BString*	newID		= new BString();
		BString*	updateStr	= new BString();
		time_t		updated		= 0;
		BString*	selfLink	= new BString();
		if(userList.FindString("title",title)!=B_OK)
		{
			std::cerr << "ERROR: No 'title' found " << std::endl;
		}
		userList.FindString("id",newID);
		userList.FindString("updated",updateStr);
		updated=RFC3339ToTime(updateStr->String());
		userList.FindString("selfLink",selfLink);
		TaskList* cat=new TaskList(title->String(),newID->String(),updated,selfLink->String());
		tLists->AddItem(cat);
		this->LoadTasks(cat);
	}
}

status_t TaskGoogle::LoadTasks(TaskList* cat){
	
	//DO HTTP CONNECTION TO GOOGLE
	BMessage	taskJson;
	BList*		tks=new BList(20);
	const char*	catID = cat->ID();


	
	std::cout << "ID: " << catID << std::endl;
	BString url("https://www.googleapis.com/tasks/v1/lists/");
	url << catID;
	url.Append("/tasks?access_token=");
	url.Append(token);
	
	BString response(HaikuHTTP::GET(url));
	std::cout << response.String() << std::endl;
	BPrivate::BJson::Parse(taskJson,response);
	taskJson.PrintToStream();
	
	BMessage items;
	taskJson.FindMessage("items",&items);
	int32 lists=items.CountNames(B_ANY_TYPE);
	std::cout << "Lists found: " << lists << std::endl;
	
	for(int32 currentTask=0;currentTask<lists;currentTask++)
	{
		BString numberString("");
		numberString << currentTask;
		BMessage task;
		if(items.FindMessage(numberString.String(),&task)!=B_OK)
		{
			std::cerr << "ERROR: No '0' list found " << std::endl;
		}
		task.PrintToStream();
		const char* title;
		const char* taskId;
		const char*	newID;
		const char* notes;
		const char* status;
		task.FindString("title",&title);
		task.FindString("id",&newID);

		task.FindString("status",&status);
		Task* tk=new Task(title,cat,newID,false);
		if (task.FindString("notes",&notes) == B_OK && notes!=NULL )
			tk->SetNotes(notes);
		tks->AddItem(tk);
	}

}



status_t TaskGoogle::SaveToken(){
	if (refreshToken.Length() > 0) {
		BPasswordKey key(refreshToken, B_KEY_PURPOSE_WEB, "refresh_token");
		BKeyStore keyStore;
		keyStore.AddKeyring(tasksKeyring);
		keyStore.AddKey(tasksKeyring, key);
		return B_OK;
	}
	return B_ERROR;
}


status_t TaskGoogle::RemoveToken(void){
		BKeyStore keyStore;
		return keyStore.RemoveKeyring(tasksKeyring);
}

BObjectList<Task>* TaskGoogle::GetTasks(TaskList ctgr){
}
	
Task* TaskGoogle::GetTask(BString id){
}


status_t  TaskGoogle::AddTask(Task *tsk){
}

status_t TaskGoogle::UpdateTask(BString id,Task *tsk){
	//send and update to all changed Tasks	
}


status_t TaskGoogle::RemoveTask(BString id){
	//send delete Request to google
}
	
/* ======== TaskList related===*/
TaskList* TaskGoogle::GetTaskList(BString id) {
	return NULL;
}

status_t TaskGoogle::AddTaskList(TaskList *ctgr){
	//**implement
	return B_ERROR;
}
	
status_t TaskGoogle::UpdateTaskList(BString id,TaskList *ctgr){
}

		//what does google do if you call a delete on a TaskList?
status_t TaskGoogle::RemoveTaskList(BString id){
}
	

time_t TaskGoogle::RFC3339ToTime(const char *timeString){
	struct	tm timeinfo;
	int		year	= 0;
	int		month	= 0;
	int		day		= 0;
	int		hour	= 0;
	int		minute	= 0;
	int		second	= 0;
	//needs error check
	sscanf (timeString,"%d-%d-%dT%d:%d:%d", &year, &month, &day,
           &hour, &minute, &second);
	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_mday = day;
	timeinfo.tm_hour = hour;
	timeinfo.tm_min = minute;
	timeinfo.tm_sec = second;
	return mktime(&timeinfo);
}

char* TaskGoogle::TimeToRFC3339(time_t timeT){
	//30 should always be enought for a RFC3339 String
	char timeString[30];
	struct tm * timeinfo;
	
	timeinfo = localtime (&timeT);
	strftime (timeString,sizeof(timeString),"%Y-%m-%dT%T.%z",timeinfo);
	return timeString;
}
