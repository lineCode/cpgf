/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderClass
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERCLASS_H
#define METADATA_META_BUILDERCLASS_H

#include "builderclass.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gselectFunctionByArity.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"

namespace metadata {

template <typename TsE1f>
metagen::BuilderClass &oPeRat0rWrapPer_metagen_BuilderClass_opAssign_0(TsE1f * sE1F, const metagen::BuilderClass &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_BuilderClass(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::BuilderContainer >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppItem *)>();

    _d.CPGF_MD_TEMPLATE _method("getCppClass", &D_d::ClassType::getCppClass);

    _d.CPGF_MD_TEMPLATE _operator<metagen::BuilderClass &(*)(cpgf::GMetaSelf, const metagen::BuilderClass &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_BuilderClass_opAssign_0<D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderClass();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
