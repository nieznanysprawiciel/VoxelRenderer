#pragma once
/**
@file IShader.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "swGraphicAPI/Resources/ResourceObject.h"



class IShaderInputLayout : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
private:
protected:
	IShaderInputLayout() : ResourceObject( 0 ) {}
	virtual ~IShaderInputLayout() = default;
public:

};

