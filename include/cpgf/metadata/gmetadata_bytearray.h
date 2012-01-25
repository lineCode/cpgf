#ifndef __GMETADATA_BYTEARRAY_H
#define __GMETADATA_BYTEARRAY_H


#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/metadata/gnamereplacer.h"


namespace cpgf {

namespace metadata_internal {


#define M(Type, Name) define.CPGF_MD_TEMPLATE _method(replaceName(# Name, replacer), &Type::Name);

template <typename T, typename MetaDefine>
void doBuildMetaData_ByteArray(MetaDefine define, const GMetaDataNameReplacer * replacer)
{
	M(T, getMemory)
	
	M(T, getPosition)
	M(T, setPosition)
	
	M(T, getLength)
	M(T, setLength)
	
	M(T, readInt8)
	M(T, readInt16)
	M(T, readInt32)
	M(T, readInt64)
	
	M(T, readUint8)
	M(T, readUint16)
	M(T, readUint32)
	M(T, readUint64)
	
	M(T, readBuffer)
	
	M(T, writeInt8)
	M(T, writeInt16)
	M(T, writeInt32)
	M(T, writeInt64)
	
	M(T, writeUint8)
	M(T, writeUint16)
	M(T, writeUint32)
	M(T, writeUint64)
	
	M(T, writeBuffer)
}

#undef M


} // namespace metadata_internal


template <typename MetaDefine>
void buildMetaData_ByteArray(MetaDefine define, const GMetaDataNameReplacer * replacer = NULL)
{
	metadata_internal::doBuildMetaData_ByteArray<typename MetaDefine::ClassType>(define, replacer);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
