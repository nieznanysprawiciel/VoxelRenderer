#pragma once

#include "swGraphicAPI/Resources/MeshResources.h"
#include "DX11Initializer/DX11APIObjects.h"


/**@brief Implementacja pixel shadera w DirectX 11.
@ingroup DX11API*/
class DX11GeometryShader : public GeometryShader, protected DX11APIObjects
{
	RTTR_ENABLE( GeometryShader );
private:

	ID3D11GeometryShader*			m_geometryShader;

protected:

	~DX11GeometryShader();

public:
	DX11GeometryShader( ID3D11GeometryShader* shader );

	inline ID3D11GeometryShader*		Get() { return m_geometryShader; }

	virtual bool						ReloadFromFile			() override;
	virtual bool						ReloadFromBinFile		() override;
	virtual void						SaveShaderBinFile		( const std::wstring& fileName ) override;

	static DX11GeometryShader*			CreateFromFile			( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "gs_4_0" );
	static DX11GeometryShader*			CreateFromBinFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "gs_4_0" );

	void								SetFileName				( const std::wstring& fileName ) { m_shaderFile = fileName; }
	void								SetShaderName			( const std::string& shaderName ) { m_shaderEntry = shaderName; }
};

