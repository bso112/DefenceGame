#pragma once
#include "GameObject.h"

#include "Management.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCube : public CGameObject
{
public:
	typedef struct tagStatedesc
	{
		BASEDESC		tBaseDesc;
		SCENEID			eSceneID = SCENE_END;
		const _tchar*	pTextureTag = nullptr;
		SCENEID			eTextureSceneID = SCENE_END;
		_uint			iTextureID = 0;
	}STATEDESC;
protected:
	explicit CCube(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCube(const CCube& rhs);
	virtual ~CCube() = default;


public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();


private:
	CTransform*		m_pTransform = nullptr;
	CVIBuffer*		m_pVIBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CShader*		m_pShader = nullptr;
	CCollider_Box*	m_pBoxCollider = nullptr;
private:
	STATEDESC	m_tDesc;


public:
	static CCube* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END
