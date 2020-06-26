#pragma once

// 1. 내가 로드해야할 다수의 이미지를 모아 놓는다.
// 2. Ctexture클래스르 ㄹ객체화했다. == 플레이어의 대시에 사용되는 이미지들. 
// 3. Ctexture클래스르 ㄹ객체화했다. == 플레이어의 아이들에 사용되는 이미지들. 
#include "Component.h"

BEGIN(Engine)

class CShader;
class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_GENERAL, TYPE_CUBE, TYPE_END };
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	//텍스쳐 타입마다 로드하는 방식이 다르다.
	HRESULT Set_TextureOnShader(CShader* pShader, D3DXHANDLE hParameter, _uint iIndex = 0);
public:
	virtual HRESULT Ready_Component_Prototype(TYPE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Ready_Component(void* pArg);
private:	
	vector<IDirect3DBaseTexture9*>			m_Textures;
	typedef vector<IDirect3DBaseTexture9*>	TEXTURES;
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device,const _tchar* pTextureFilePath, _uint iNumTextures = 1, TYPE eType = TYPE_GENERAL);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END