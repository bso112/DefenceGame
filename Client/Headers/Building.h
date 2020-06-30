#pragma once
#include "Client_Defines.h"
#include "Interactable.h"
#include "Management.h"
#include "Value.h"
//#include "StageUIMgr.h"


BEGIN(Client)

class CBuilding abstract : public CInteractable
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

	typedef struct tagBuildingStat
	{
		CValue<int> iMaxHP = CValue<int>(0);
		_int iHp = 0;
		_int iPrice = 0;
		_int iLevel = 1;
	}BUILDING_STAT;

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
	_int Get_TileSize() { return m_iTileSize; }
	void Get_Damage(_int iDmg);
	void virtual Interact();

public:
	virtual void Upgrade();
	virtual _bool Picking(POINT _ViewPortPoint, _float3* _vHitPos) override;

protected:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
	BUILDING_STAT		m_tagStat;

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