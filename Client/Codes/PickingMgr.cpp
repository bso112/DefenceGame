#include "stdafx.h"
#include "..\Headers\PickingMgr.h"
#include "Terrain.h"
#include "Management.h"
#include "Interactable.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CPickingMgr)
CPickingMgr::CPickingMgr()
{
}

HRESULT CPickingMgr::Ready_PickingMgr(CTerrain * pTerrain)
{
	m_pTerrain = pTerrain;
	Safe_AddRef(m_pTerrain);

	CKeyMgr::Get_Instance()->RegisterObserver(SCENE_STATIC, this);
	return S_OK;
}



HRESULT CPickingMgr::Register_Observer(CInteractable * pObj)
{
	if (nullptr == pObj)
		return E_FAIL;

	m_listObject.push_back(pObj);
	return S_OK;
}

HRESULT CPickingMgr::UnRegister_Observer(CInteractable * pObj)
{
	auto& iter = m_listObject.begin();
	while (iter != m_listObject.end())
	{
		if (*iter == pObj)
		{
			iter = m_listObject.erase(iter);
		}
		else
			++iter;
	}
	return S_OK;
}

HRESULT CPickingMgr::Get_Route(_float3 _WorldSrc, _float3 _WorldDst, vector<_float3>& _out)
{
	if (nullptr == m_pTerrain)
		return E_FAIL;

	return m_pTerrain->Get_Route(_WorldSrc, _WorldDst, _out);
}

HRESULT CPickingMgr::Get_Route(_float3 _WorldSrc, POINT _ViewPortDst, vector<_float3>& _out)
{
	if (nullptr == m_pTerrain)
		return E_FAIL;

	_float3 vWorldPt;

	if (m_pTerrain->Picking(_ViewPortDst, &vWorldPt))
	{
		m_pTerrain->Get_Route(_WorldSrc, vWorldPt, _out);
		return S_OK;
	}
	return E_FAIL;
}

_bool CPickingMgr::Get_WorldMousePos(POINT _pt, _float3* _vWorldMouse)
{
	return m_pTerrain->Picking(_pt, _vWorldMouse);
}

vector<CGameObject*> CPickingMgr::OverlapSphere(_float3 vPos, _float _fRadius, CGameObject* pSelf)
{
	vector<CGameObject*> vecGameObject;
	for (auto& obj : m_listObject)
	{
		if (obj == pSelf)
			continue;

		CTransform* pObjTransform = (CTransform*)obj->Find_Component(L"Com_Transform");
		if (nullptr == pObjTransform) return vector<CGameObject*>();
		_float fDist = D3DXVec3Length(&(vPos - pObjTransform->Get_State(CTransform::STATE_POSITION)));
		if (fDist < _fRadius)
		{
			vecGameObject.push_back(obj);
		}
	}
	if(vecGameObject.empty())
		return vector<CGameObject*>();

	return vecGameObject;
}

CGameObject * CPickingMgr::OverlapSphere_Closest(_float3 vPos, _float _fRadius, _float* pDist, CGameObject* pSelf)
{
	_float minDist = FLT_MAX;
	CGameObject* pClosest = nullptr;
	for (auto& obj : m_listObject)
	{
		if (obj == pSelf)
			continue;
		CTransform* pObjTransform = (CTransform*)obj->Find_Component(L"Com_Transform");
		if (nullptr == pObjTransform) return nullptr;
		_float fDist = D3DXVec3Length(&(vPos - pObjTransform->Get_State(CTransform::STATE_POSITION)));
		if (fDist > _fRadius)
			continue;
		if (minDist > fDist)
		{
			minDist = fDist;
			pClosest = obj;
		}
	}

	*pDist = minDist;
	return pClosest;
}

HRESULT CPickingMgr::Pick_Object(POINT _ViewPortPoint, _float3* pHitPos)
{
	vector<CInteractable*> vecPicked;

	for (auto& obj : m_listObject)
	{
		if (obj->Picking(_ViewPortPoint, pHitPos))
		{
			vecPicked.push_back(obj);
		}

	}

	if (!vecPicked.empty())
	{
		//가장 z값이 작은 것을 구한다.
		sort(vecPicked.begin(), vecPicked.end(), [](CInteractable* pA, CInteractable* pB) {
			CTransform* pTransformA = (CTransform*)pA->Find_Component(L"Com_Transform");
			CTransform* pTransformB = (CTransform*)pB->Find_Component(L"Com_Transform");
			if (nullptr == pTransformA || nullptr == pTransformB)
				return false;
			return pTransformA->Get_State(CTransform::STATE_POSITION).z > pTransformB->Get_State(CTransform::STATE_POSITION).z;
		});


		vecPicked.front()->Interact();

	}

	return S_OK;
}


HRESULT CPickingMgr::OnKeyDown(_int KeyCode)
{
	//피킹
	if (KeyCode == VK_LBUTTON)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		_float3 vHitPos;
		Pick_Object(pt, &vHitPos);
	}

	return S_OK;
}

void CPickingMgr::Free()
{
	CKeyMgr::Get_Instance()->UnRegisterObserver(SCENE_STATIC, this);

	Safe_Release(m_pTerrain);

	m_listObject.clear();

}
