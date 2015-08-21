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

TaskGoogle::TaskGoogle()
{
	categoryList=new BObjectList<Category>(20);
	taskList=new BObjectList<Task>(20);
}

TaskGoogle::~TaskGoogle()
{
	
}

status_t TaskGoogle::Init(void)
{
	//first try to load token
	if (LoadToken()!=B_OK)
		RequestAccessString();
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



status_t TaskGoogle::RequestTocken(BString code)
{
	BHttpForm* form=new BHttpForm();
	//if there is no refreshToken do the "First Time Autentiation Procedure
	if (refreshToken.Length()>0) {
		const char * accesString = RequestAccessString();
		form->AddString("code",accesString);
		form->AddString("client_id",CLIENT_ID);
		form->AddString("client_secret",CLIENT_SECRET);
		form->AddString("grant_type","authorization_code");
		form->AddString("redirect_uri",REDIRECT_URI);
	}
	else{
		form->AddString("refresh_token",refreshToken);
		form->AddString("client_id",CLIENT_ID);
		form->AddString("client_secret",CLIENT_SECRET);
		form->AddString("grant_type","refresh_token");
	}
	form->SetFormType(B_HTTP_FORM_URL_ENCODED);
	BString oauth2("https://www.googleapis.com/oauth2/v3/token");
	BString tokenResponse(HaikuHTTP::GET(oauth2,form));
	
	BMessage tokenJson;
	BPrivate::BJson::Parse(tokenJson,tokenResponse);
	tokenJson.PrintToStream();
	
	//this should start the counter how long the token stays valid	
	token=BString(tokenJson.GetString("access_token","NOT_FOUND"));
	if (refreshToken.Length() == 0){
		refreshToken=new BString(tokenJson.GetString("refreshToken","NOT_FOUND"));
		SaveToken(void);
		std::cout << "Refresh Token" << refreshToken.String() << std::endl;
	}
	
	std::cout << "Token access " << token.String() << std::endl;
}


status_t TaskGoogle::Load(void){

	BString listUrlString("https://www.googleapis.com/tasks/v1/users/@me/lists?access_token=");
	listUrlString.Append(token);
	BString listsResponse(HaikuHTTP::GET(listUrlString));
	
	BMessage listsJson;
	std::cout << listsResponse << std::endl;
	BPrivate::BJson::Parse(listsJson,listsResponse);
	listsJson.PrintToStream();
	
	BMessage userLists;
	if(listsJson.FindMessage("items",0,&userLists)!=B_OK) {
		std::cerr << "ERROR: 'items' not found" << std::endl;
	}

	
	int32 lists=userLists.CountNames(B_ANY_TYPE);
	std::cout << "Lists found: " << lists << std::endl;
	
	for(int32 currentList=0;currentList<lists;currentList++)
	{
		std::ostringstream ss;
		ss << currentList;
		BMessage userList;
		if(userLists.FindMessage(ss.str().c_str(),&userList)!=B_OK)
		{
			std::cerr << "ERROR: No '0' list found " << std::endl;
		}
		userList.PrintToStream();
		const char* title;
		const char* id;
		if(userList.FindString("title",&title)!=B_OK)
		{
			std::cerr << "ERROR: No 'title' found " << std::endl;
		}
		userList.FindString("id",&id);
		Category* cat=new Category(title,"MIME_DATABASE",id);
		categories->AddItem(cat);
		this->GetTasks(cat);
	}
	
	//DISPATCH SYNC_CATEGORIES
	
	int32 count=be_app->CountWindows();
	for(int32 i=0;i<count;i++)
	{
		be_app->WindowAt(i)->PostMessage(new BMessage(SYNC_CATEGORIES));
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


BList* TaskGoogle::GetTasks(Category* cat)
{
	//DO HTTP CONNECTION TO GOOGLE
	BList* tks=new BList(20);
	
	BString id=cat->GetID();
	std::cout << "ID: " << id.String() << std::endl;
	BString url("https://www.googleapis.com/tasks/v1/lists/");
	url.Append(id);
	url.Append("/tasks?access_token=");
	url.Append(token);
	
	BString response(HaikuHTTP::GET(url));
	std::cout << response.String() << std::endl;
	BMessage taskJson;
	BPrivate::BJson::Parse(taskJson,response);
	taskJson.PrintToStream();
	
	BMessage items;
	taskJson.FindMessage("items",&items);
	int32 lists=items.CountNames(B_ANY_TYPE);
	std::cout << "Lists found: " << lists << std::endl;
	
	for(int32 currentTask=0;currentTask<lists;currentTask++)
	{
		std::ostringstream ss;
		ss << currentTask;
		BMessage task;
		if(items.FindMessage(ss.str().c_str(),&task)!=B_OK)
		{
			std::cerr << "ERROR: No '0' list found " << std::endl;
		}
		task.PrintToStream();
		const char* title;
		const char* taskId;
		const char* notes;
		const char* status;
		task.FindString("title",&title);
		task.FindString("id",&taskId);
		task.FindString("notes",&notes);
		task.FindString("status",&status);
		Task* tk=new Task(title,notes,id.String(),false);
		tks->AddItem(tk);
	}
	return tks;
}
