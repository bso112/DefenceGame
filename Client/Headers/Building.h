#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Management.h"


BEGIN(Client)

class CBuilding abstract : public CGameObject
{
public:
	typedef struct tagBuildingDesc
	{
		_float3 vPos;
	}BUILDING_DESC;

	enum RENDER_COLOR
	{
		RENDER_RED,
		RENDER_WARNING,
		RENDER_END,
	};

protected:
	explicit CBuilding(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuilding(const CBuilding& rhs);
	virtual ~CBuilding() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual void Upgrade();
	virtual void Interact();

protected:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

protected:
	_int m_iTileSize;
	_float m_fScale;
	_float m_fRealScaleMag;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
public:
	virtual CGameObject* Clone_GameObject(void* pArg) = 0;
	virtual void Free();
};

END