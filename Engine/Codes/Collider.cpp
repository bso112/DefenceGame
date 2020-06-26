#include "..\Headers\Collider.h"


USING(Engine)

CCollider::CCollider(PDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pPipeLine(CPipeLine::Get_Instance())
{
	Safe_AddRef(m_pPipeLine);
}


CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCollider::Ready_Component_Prototype()
{


	return S_OK;
}

HRESULT CCollider::Ready_Component(void * pArg)
{


	return S_OK;
}

_int CCollider::Update_Collider(_matrix StateMatrix)
{


	return _int();
}

HRESULT CCollider::Render_Collider()
{
	return S_OK;
}

void CCollider::Free()
{
	Safe_Release(m_pPipeLine);

	CComponent::Free();
}
