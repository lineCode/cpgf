#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

class A
{
public:
	A() : na(0) {}
	
	int na;
};

class B
{
public:
	B() : nb(0) {}

	A a;
	int nb;
};

class C
{
public:
	C() : nc(0) {}

	void init() {
		this->a.na = 1;
		this->b.nb = 2;
		this->nc = 3;
		this->b.a.na = 5;
	}

	A a;
	B b;
	int nc;
};

bool operator == (const A & a1, const A & a2)
{
	return a1.na == a2.na;
}

bool operator == (const B & b1, const B & b2)
{
	return b1.a == b2.a && b2.nb == b2.nb;
}

bool operator == (const C & c1, const C & c2)
{
	return c1.a == c2.a && c1.b == c2.b && c1.nc == c2.nc;
}

template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<A> classDefineA = GDefineMetaClass<A>::declare("TestSerializeClassA");
	
	classDefineA
		FIELD(A, na)
	;
	define._class(classDefineA);
	
	GDefineMetaClass<B> classDefineB = GDefineMetaClass<B>::declare("TestSerializeClassB");
	
	classDefineB
		FIELD(B, a)
		FIELD(B, nb)
	;
	define._class(classDefineB);

	GDefineMetaClass<C> classDefineC = GDefineMetaClass<C>::declare("TestSerializeClassC");
	
	classDefineC
		FIELD(C, a)
		FIELD(C, b)
		FIELD(C, nc)
	;
	define._class(classDefineC);
	
}

template <typename READER, typename AR>
void doTestNestedObject(IMetaWriter * writer, const READER & reader, const AR & ar)
{
	const char * const serializeObjectName = "nestedObject";
	
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassC"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	C instance;
	instance.init();

	C * pinstance = &instance;
	
	serializeWriteObjectPointer(archiveWriter.get(), serializeObjectName, pinstance, metaClass.get());
	// should error
//	GBEGIN_EXCEPTION
//		serializeWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());
//	GEND_EXCEPTION(...)

	ar.rewind();

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader.get(service.get())));
	
	C readInstance;
	
	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());
	
	GCHECK(instance == readInstance);
}

GTEST(testNestedObject_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	
	doTestNestedObject(writer.get(), MetaReaderGetterStream(stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testNestedObject_Xml)
{
	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestNestedObject(writer.get(), MetaReaderGetterXml(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


GTEST(testNestedObject_Json)
{
	GMetaJsonArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(outputArchive));
	
	doTestNestedObject(writer.get(), MetaReaderGetterJson(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


} // unnamed namespace