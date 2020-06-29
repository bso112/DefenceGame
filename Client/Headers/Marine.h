#pragma once
#include "Unit.h"
BEGIN(Client)
class CMarine : public CUnit
{
public:
	typedef	struct tagStateDesc
	{
		BASEDESC			tBaseDesc;
		UNITSTATS			tUnitStats;
		SCENEID				eTextureSceneID = SCENE_END;
		SCENEID				eSceneID = SCENE_END;
		const _tchar*		pTextureTag = nullptr;
		_uint				iShaderPass = 0;
		_uint				iTextureID = 0;

	}STATEDESC;
private:
	explicit CMarine(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMarine(const CMarine& _rhs);
	virtual ~CMarine() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	//pHitPos는 레이가 충돌한 지점의 위치다. (out 인자)
	virtual _bool		Picking(POINT _ViewPortPoint, _float3* pHitPos);
	virtual	void		Interact();

public:
	virtual HRESULT	OnKeyDown(_int KeyCode);

private:
	CVIBuffer*		m_pVIBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CShader*		m_pShader = nullptr;
	CCollider_Box*	m_pBoxCollider = nullptr;
	STATEDESC		m_tDesc;

public:
	static CMarine*	Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free();



};
END
