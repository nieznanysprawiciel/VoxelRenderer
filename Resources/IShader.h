#pragma once
/**
@file IShader.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "swGraphicAPI/Resources/ResourceObject.h"

#include <string>



class IShader	:	public ResourceObject
{
	RTTR_ENABLE( ResourceObject )
private:
protected:
	std::string				m_shaderEntry;			///< Nazwa g³ownej funkcji shadera.
	std::wstring			m_shaderFile;			///< Œcie¿ka do pliku z shaderem.
protected:
	IShader() : ResourceObject( 0 ) {}
	virtual ~IShader() = default;
public:
	const std::wstring&		GetShaderFile			() const		{ return m_shaderFile; }
	const std::string&		GetShaderEntry			() const		{ return m_shaderEntry; }


	virtual bool			ReloadFromFile			() = 0;
	virtual bool			ReloadFromBinFile		() = 0;
	virtual void			SaveShaderBinFile		( const std::wstring& file_name ) = 0;
};

