#pragma once

#include "PipeLine.h"
#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
public:
	typedef struct tagStateDesc
	{
		_matrix		StateMatrix;
	}STATEDESC;
protected:
	explicit CCollider(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
	virtual _int Update_Collider(_matrix StateMatrix);
	virtual HRESULT Render_Collider();

public:
	virtual	_bool	Check_Collision(CCollider* pTarget) = 0;
protected:
	CPipeLine*			m_pPipeLine = nullptr;
	_bool				m_isColl = false;
public:
	virtual CComponent* Clone_Component(void* pArg) = 0;
	virtual void Free();
};

END