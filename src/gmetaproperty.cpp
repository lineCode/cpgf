#include "cpgf/gmetaproperty.h"
#include "cpgf/gexception.h"


namespace cpgf {


namespace meta_internal {


void GMetaPropertyDataBase::deleteObject()
{
	this->virtualFunctions->deleteObject(this);
}

bool GMetaPropertyDataBase::canGet() const
{
	return this->virtualFunctions->canGet(this);
}

bool GMetaPropertyDataBase::canSet() const
{
	return this->virtualFunctions->canSet(this);
}

GVariant GMetaPropertyDataBase::get(void * instance) const
{
	return this->virtualFunctions->get(this, instance);
}

void GMetaPropertyDataBase::set(void * instance, const GVariant & v) const
{
	this->virtualFunctions->set(this, instance, v);
}

size_t GMetaPropertyDataBase::getPropertySize() const
{
	return this->virtualFunctions->getPropertySize(this);
}

void * GMetaPropertyDataBase::getPropertyAddress(void * instance) const
{
	return this->virtualFunctions->getPropertyAddress(this, instance);
}

GMetaExtendType GMetaPropertyDataBase::getItemExtendType(uint32_t flags) const
{
	return this->virtualFunctions->getItemExtendType(this, flags);
}


} // namespace meta_internal


bool GMetaProperty::canGet() const
{
	return this->baseData->canGet();
}

bool GMetaProperty::canSet() const
{
	return this->baseData->canSet();
}

GVariant GMetaProperty::get(void * instance) const
{
	if(!this->baseData->canGet()) {
		raiseCoreException(Error_Meta_ReadDenied);
	}

	return this->baseData->get(instance);
}

void GMetaProperty::set(void * instance, const GVariant & value) const
{
	if(!this->baseData->canSet()) {
		raiseCoreException(Error_Meta_WriteDenied);
	}

	this->baseData->set(instance, value);
}

void * GMetaProperty::getAddress(void * instance) const
{
	return this->baseData->getPropertyAddress(instance);
}

size_t GMetaProperty::getSize() const
{
	return this->baseData->getPropertySize();
}

const GMetaType & GMetaProperty::getItemType() const
{
	return super::getItemType();
}

GMetaExtendType GMetaProperty::getItemExtendType(uint32_t flags) const
{
	return this->baseData->getItemExtendType(flags);
}



} // namespace cpgf
