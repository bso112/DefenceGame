#pragma once
#include "GameObject.h"

#include "Management.h"
#include "Client_Defines.h"

BEGIN(Client)
class CWall;
class CImage3D;
class CTrigger : public CGameObject
{
public:
	typedef struct tagStatedesc
	{
		BASEDESC		tBaseDesc;
		SCENEID			eSceneID = SCENE_END;
		const _tchar*	pTextureTag = nullptr;
		SCENEID			eTextureSceneID = SCENE_END;
		_uint			iTextureID = 0;
		_uint			iShaderPass = 0;
	}STATEDESC;
protected:
	explicit CTrigger(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;


public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);

private:
	CTransform*	m_pTransform = nullptr;
	CVIBuffer*	m_pVIBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;
	CRenderer*	m_pRenderer = nullptr;
	CShader*		m_pShader = nullptr;
	CCollider_Box*	m_pBoxCollider = nullptr;
	CImage3D*		m_pLight[4];

private:
	STATEDESC	m_tDesc;

public:
	static CTrigger* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CTrigger* Clone_GameObject(void* pArg);
	virtual void Free();


};
END
