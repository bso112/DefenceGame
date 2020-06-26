#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Management.h"

BEGIN(Client)

class CBackGround final : public CGameObject
{
public:
	typedef struct tagStatedesc
	{
		BASEDESC			tBaseDesc;
		const _tchar*		pTextureTag = nullptr;
		SCENEID				eTextureSceneID = SCENE_END;
		_int				iTextureID = 0;

	}STATEDESC;
private:
	explicit CBackGround(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_ViewPort*	m_pVIBufferCom = nullptr;	
	CTexture*			m_pTextureCom = nullptr;

private:
	STATEDESC			m_tDesc;
	
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
public:
	static CBackGround* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END