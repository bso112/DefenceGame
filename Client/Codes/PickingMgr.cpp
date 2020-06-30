#include "stdafx.h"
#include "..\Headers\PickingMgr.h"
#include "Terrain.h"
#include "Management.h"
#include "Interactable.h"
#include "CommandCenter.h"
#include "Barricade.h"
#include "KeyMgr.h"
#include "Marine.h"
IMPLEMENT_SINGLETON(CPickingMgr)
CPickingMgr::CPickingMgr()
{
}

HRESULT CPickingMgr::Ready_PickingMgr(CTerrain * pTerrain)
{
	m_pTerrain = pTerrain;
	Safe_AddRef(m_pTerrain);

	CKeyMgr::Get_Instance()->RegisterObserver(SCENE_STATIC, this);
	m_eMode = MODE_NORMAL;
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
	if (vecGameObject.empty())
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
	//설치
	if (m_eMode == MODE_NORMAL)
	{
		switch (KeyCode)
		{
		case '1':
			m_eMode = MODE_BARRICADE;
			break;
		case '2':
			m_eMode = MONE_COMMANDCENTER;
			break;
		case '3':
			m_eMode = MODE_UNIT;
			break;
		default:
			break;
		}

	}

	if (KeyCode == VK_LBUTTON)
	{
		//피킹
		PickObject();
		//설치
		InstallObject();

		return S_OK;
	}
}

HRESULT CPickingMgr::PickObject()
{

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_float3 vHitPos;
	//오브젝트 피킹 후 인터렉트
	Pick_Object(pt, &vHitPos);


	return S_OK;
}

HRESULT CPickingMgr::InstallObject()
{


	_float3 vDest;
	POINT tTemp;
	GetCursorPos(&tTemp);
	ScreenToClient(g_hWnd, &tTemp);
	Get_WorldMousePos(tTemp, &vDest);
	vDest.y = 0;

	CManagement* pManagement = CManagement::Get_Instance();
	//_float3 vDest = vHitPos;
	CTerrain* pTerrain = Get_Terrain();
	int iTileSize = 0;
	CBuilding::BUILDING_DESC BuildingDesc;

	switch (m_eMode)
	{
	case MODE_BARRICADE:
		iTileSize = ((CBuilding*)pManagement->Find_Prototype(SCENE_STATIC, L"GameObject_Barricade"))->Get_TileSize();

		if (pTerrain->BuildCheck(&vDest, iTileSize) == false)
			return S_OK;//설치 실패

		BuildingDesc.vPos = vDest;
		if (FAILED(pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Barricade", SCENE_STAGE1, L"Layer_Barricade", &BuildingDesc)))
			return E_FAIL;

		m_eMode = MODE_NORMAL;
		pTerrain->Set_Occupation(&vDest, iTileSize, 1);
		break;
	case MONE_COMMANDCENTER:
		iTileSize = ((CBuilding*)pManagement->Find_Prototype(SCENE_STATIC, L"GameObject_CommandCenter"))->Get_TileSize();

		if (pTerrain->BuildCheck(&vDest, iTileSize) == false)
			return S_OK;//설치 실패

		BuildingDesc.vPos = vDest;
		if (FAILED(pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_CommandCenter", SCENE_STAGE1, L"Layer_CommandCenter", &BuildingDesc)))
			return E_FAIL;

		m_eMode = MODE_NORMAL;
		pTerrain->Set_Occupation(&vDest, iTileSize, 1);
		break;
	case MODE_UNIT:
	{
		CMarine::STATEDESC tMarineDesc;
		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(vDest, _float3(2.f, 2.f, 2.f));
		
		CMarine* pMarine = nullptr;
		if (nullptr == (pMarine = (CMarine*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Marine", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		pMarine->Set_Friendly(true);
	}
	default:
		break;
	}


	
}

void CPickingMgr::Check_Mouse()
{
	if (m_eMode == MODE_NORMAL)
		return;

	_float3 vDest;
	POINT tTemp;
	GetCursorPos(&tTemp);
	ScreenToClient(g_hWnd, &tTemp);
	Get_WorldMousePos(tTemp, &vDest);
	vDest.y = 0;

	CManagement* pManagement = CManagement::Get_Instance();
	CTerrain* pTerrain = Get_Terrain();
	int iTileSize = 0;

	switch (m_eMode)
	{
	case MODE_BARRICADE:
		iTileSize = ((CBuilding*)pManagement->Find_Prototype(SCENE_STATIC, L"GameObject_Barricade"))->Get_TileSize();
		pTerrain->BuildCheck(&vDest, iTileSize);
		break;

	case MONE_COMMANDCENTER:
		iTileSize = ((CBuilding*)pManagement->Find_Prototype(SCENE_STATIC, L"GameObject_CommandCenter"))->Get_TileSize();
		pTerrain->BuildCheck(&vDest, iTileSize);
		break;

	default:
		break;
	}
}

void CPickingMgr::Free()
{
	CKeyMgr::Get_Instance()->UnRegisterObserver(SCENE_STATIC, this);

	Safe_Release(m_pTerrain);

	m_listObject.clear();

}
