#ifndef __GMETACLASSTRAVELLER_H
#define __GMETACLASSTRAVELLER_H


#include "cpgf/gclassutil.h"

#include <deque>

#include <stddef.h>


namespace cpgf {

struct IMetaClass;

class GMetaClassTraveller : public GNoncopyable
{
private:
	struct Node {
		Node(IMetaClass * metaClass, void * instance, IMetaClass * derived)
			: metaClass(metaClass), instance(instance), derived(derived) {
		}

		IMetaClass * metaClass;
		void * instance;
		IMetaClass * derived;
	};
	
	typedef std::deque<Node> ListType;
	
public:
	GMetaClassTraveller(IMetaClass * metaClass, void * instance);
	~GMetaClassTraveller();
	
	IMetaClass * next(void ** outInstance, IMetaClass ** outDerived);
	IMetaClass * next(void ** outInstance);

private:
	ListType traversal;
};



} // namespace cpgf


#endif

