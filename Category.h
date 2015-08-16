#ifndef CATEGORY_H
#define CATEGORY_H


#include <Archivable.h>
#include <String.h>


class Category : public BArchivable {
public:
							Category();
							Category(const char* newName);
							Category(BMessage *archive);
							~Category() {};
							
							
			status_t		Archive(BMessage* archive, bool deep = true);
	static	BArchivable*	Instantiate(BMessage* archive);
	
	const	char*			Name(void) {return name.String();};
			void			SetName(BString newName){name.SetTo(newName);};
	
			time_t			LastUpdate(void){return updated;};
			void			SetLastUpdate(bigtime_t newTime){ updated=newTime; };

			int32			ID(void){return id;};
			void			SetID(int32 newID){id=newID;};

			BString			URL(void){return url.String();};
			void			SetURL(BString newURL){url=newURL;};

			
			bool			operator==(const Category& task) const;

protected:
			void			Init(void);

		
private:
			BString			name;
			time_t			updated; //this should be hidden at least in the tracker
			
			// we should implement the id, etag, selfLink
			int32			id;
			BString			url;
};

#endif
