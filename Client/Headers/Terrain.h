#pragma once
#include "GameObject.h"
#include "Management.h"

BEGIN(Client)

class CTerrain final : public CGameObject
{

public:
	typedef struct tagStateDesc
	{
		SCENEID _eSceneID = SCENE_END;
	}STATEDESC;

private:
	typedef struct tagNode
	{
		_int Gcost = INT_MAX; // distance from staring node
		_int Hcost = INT_MAX; // distance from end node
		_int Fcost = INT_MAX;	// Gcost + Hcost
								//À§Ä¡
		_float3		vPos;
		//ÀÎµ¦½º
		_uint		X;
		_uint		Z;
		tagNode*	pParent = nullptr;
		_bool		bMovable = false;

		tagNode() {}
		tagNode(_int _Gcost, _int _Hcost, _int _Fcost, _int _X, _int _Z, tagNode* _pParent)
		{
			Gcost = _Gcost;
			Hcost = _Hcost;
			Fcost = _Fcost;
			X = _X;
			Z = _Z;
			pParent = _pParent;
		}

		void Set_Node(_int _Gcost, _int _Hcost, _int _Fcost, _int _X, _int _Z, tagNode* _pParent)
		{
			Gcost = _Gcost;
			Hcost = _Hcost;
			Fcost = _Fcost;
			X = _X;
			Z = _Z;
			pParent = _pParent;
		}
		void Set_Pos(_float3 _vPos) { vPos = _vPos; }
		void Set_Movable(_bool _bMovable) { bMovable = _bMovable; }
		void Clear()
		{
			Gcost = INT_MAX;	// distance from staring node
			Hcost = INT_MAX;	// distance from end node
			Fcost = INT_MAX;	// Gcost + Hcost

			X = 0;
			Z = 0;
			pParent = nullptr;

		}

	}NODE;
private:
	explicit CTerrain(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT	Get_Route(_float3 _src, _float3 _dst, vector<_float3>& _out);
	_bool	 Picking(POINT pt, _float3 * pOut);

public:
	virtual HRESULT	OnKeyDown(_int KeyCode);

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Terrain*	m_pVIBufferCom = nullptr;
	STATEDESC			m_tDesc;
private:
	NODE*				m_Nodes[TILEZ][TILEX];

private:
	HRESULT Add_Component();
	HRESULT	Ready_Nodes();
	HRESULT SetUp_ConstantTable();
public:
	static CTerrain* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END