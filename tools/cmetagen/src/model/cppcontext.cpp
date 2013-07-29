#include "cppcontext.h"
#include "cppclass.h"
#include "cppnamespace.h"
#include "cppfile.h"
#include "util.h"

#include "cpgf/gassert.h"

#include <utility>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/DeclTemplate.h"
#include "clang/AST/DeclCXX.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;
using namespace std;

namespace metagen {

CppContext::CppContext(const Project * project)
	: project(project)
{
}

CppContext::~CppContext()
{
	clearPointerContainer(this->itemList);
}

void CppContext::process(const std::string & sourceFileName)
{
	this->parser.parse(this, sourceFileName.c_str());
}

void CppContext::beginFile(const char * fileName, clang::Decl * decl)
{
	GASSERT(! this->cppFile);

	this->cppFile.reset(new CppFile(fileName, decl));
}

void CppContext::endFile(const char * /*fileName*/)
{
	this->cppFile->prepare();
}

void CppContext::itemCreated(CppItem * item)
{
	this->itemList.push_back(item);
	item->setCppContext(this);
	
	GASSERT(this->itemDeclMap.find(item->getDecl()) == this->itemDeclMap.end());
	this->itemDeclMap.insert(make_pair(item->getDecl(), item));
}

CppItem * CppContext::findItemByDecl(const clang::Decl * decl) const
{
	if(decl == NULL) {
		return NULL;
	}

	ItemDeclMapType::const_iterator it = this->itemDeclMap.find(decl);
	if(it == this->itemDeclMap.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}

CppClass * CppContext::findClassByType(const CppType & cppType) const
{
	const clang::Decl * decl = cppType.getCXXRecordDecl();

	if(decl != NULL && dyn_cast<ClassTemplateSpecializationDecl>(decl) != NULL) {
		decl = dyn_cast<ClassTemplateSpecializationDecl>(decl)->getSpecializedTemplate();
	}

	CppItem * cppItem = this->findItemByDecl(decl);
	if(cppItem == NULL || ! cppItem->isClass()) {
		return NULL;
	}
	else {
		return static_cast<CppClass *>(cppItem);
	}
}

CppClassTraits CppContext::getClassTraits(const CppClass * cppClass) const
{
	return cppClass->getClassTraits();
}


} // namespace metagen