#include "stdafx.h"
#include "..\Headers\Marine.h"
#include "PickingMgr.h"
#include "KeyMgr.h"
#include "PickingMgr.h"
#include "AIStateController.h"
#include "AIState.h"
#include "Building.h"
#include "GameManager.h"

CMarine::CMarine(PDIRECT3DDEVICE9 pGraphic_Device)
	:CUnit(pGraphic_Device)
{
}

CMarine::CMarine(const CMarine & _rhs)
	: CUnit(_rhs)
{
}

HRESULT CMarine::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMarine::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	CTransform::STATEDESC tTransformDesc;
	tTransformDesc.RotatePerSec = D3DXToRadian(90.f);
	tTransformDesc.SpeedPerSec = 10.0;
	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(m_tDesc.eTextureSceneID, m_tDesc.pTextureTag, L"Com_Texture", (CComponent**)&m_pTexture)))
		return E_FAIL;


	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Shader_Cube", L"Com_Shader_Cube", (CComponent**)&m_pShader)))
		return E_FAIL;
	if (FAILED(Add_Component(SCENE_STATIC, L"Component_AIStateCon", L"Com_AIStateCon", (CComponent**)&m_pAICon)))
		return E_FAIL;


	if (FAILED(Add_Component(SCENE_STATIC, L"Component_VIBuffer_Cube", L"Com_VIBuffer_Cube", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_BoxCollider", L"Com_Collider", (CComponent**)&m_pBoxCollider)))
		return E_FAIL;

	m_pTransform->SetUp_Position(m_tDesc.tBaseDesc.vPos);
	m_pTransform->SetUp_Scale(m_tDesc.tBaseDesc.vSize);

	CKeyMgr::Get_Instance()->RegisterObserver(m_tDesc.eSceneID, this);
	//피킹매니저에 등록. 
	CPickingMgr::Get_Instance()->Register_Observer(this);

	//왜 릭이 나는지 모르겠음.
	//m_pAICon->Set_State(CAIState::STATE_IDLE, new CAIIdle(this));
	//m_pAICon->Set_State(CAIState::STATE_HUNTING, new CAIHunting(this));
	//m_pAICon->Set_State(CAIState::STATE_RETREAT, new CAIRetreat(this));
	//m_pAICon->Set_State(CAIState::STATE_WAIT, new CAIWait(this));

	//m_pAICon->Set_Default_State(CAIState::STATE_HUNTING, CAIState::STATEDESC(), CManagement::Get_Instance()->Get_TimeDelta(L"Timer_60"));


	m_iRecogRange = 10;

	m_tUnitStats.iAtt = CValue<int>(20);


	return S_OK;
}

_int CMarine::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pBoxCollider ||
		nullptr == m_pTransform)
		return -1;

	m_pBoxCollider->Update_Collider(m_pTransform->Get_WorldMatrix());


#pragma region AIHunting

	if (!CGameManager::Get_Instance()->IsGameStart())
		return 0;

	CPickingMgr* pPickMgr = CPickingMgr::Get_Instance();
	if (nullptr == pPickMgr) return-1;
	if (nullptr == m_pTransform) return -1;

	if (m_bControlMode)
		return 0;

	//주변을 조사해서 게임오브젝트를 얻는다.
	vector<CGameObject*> pTargetVector = pPickMgr->OverlapSphere(m_pTransform->Get_State(CTransform::STATE_POSITION), FLT_MAX, this);
	if (pTargetVector.empty()) return 0;

	for (auto& go : pTargetVector)
	{
		_float dist = 0.f;
		CTransform* pTargetTransform = (CTransform*)go->Find_Component(L"Com_Transform");
		if (nullptr == pTargetTransform) return -1;
		dist = D3DXVec3Length(&(pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransform->Get_State(CTransform::STATE_POSITION)));

		//유닛일 경우
		CUnit*		pUnit = dynamic_cast<CUnit*>(go);
		if (nullptr != pUnit)
		{
			//아군일 경우 무시
			if (pUnit->Get_Friendly() == Get_Friendly())
				continue;
			//적군일 경우
			else
			{
				//다가간다.
				CTransform* pTargetTransform = (CTransform*)pUnit->Find_Component(L"Com_Transform");
				if (nullptr == pTargetTransform) return -1;
				m_pTransform->Go_Dst(pTargetTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);


				//도착이면 공격
				if (dist < 2.f)
					pUnit->TakeDamage(m_tUnitStats.iAtt.GetValue(), m_tUnitStats.iAtt.GetValue());
			}
		}
		else
		{
			//아군은 빌딩을 공격하지 않는다
			if (m_tUnitStats.bFriendly)
				continue;

			//빌딩일 경우
			CBuilding* pBuilding = dynamic_cast<CBuilding*>(go);
			if (nullptr != pBuilding)
			{
				//다가간다.
				CTransform* pTargetTransform = (CTransform*)pBuilding->Find_Component(L"Com_Transform");
				if (nullptr == pTargetTransform) return -1;
				m_pTransform->Go_Dst(pTargetTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);

				//도착이면 공격
				if (dist < 2.f)
					pBuilding->Get_Damage(m_tUnitStats.iAtt.GetValue());

			}
		}




	}


#pragma endregion


	return CUnit::Update_GameObject(TimeDelta);
}

_int CMarine::Late_Update_GameObject(_double TimeDelta)
{


	//if (nullptr == m_pAICon) return -1;
	//m_pAICon->Update(CAIState::STATEDESC(), TimeDelta);


	//렌더러에 등록
	if (nullptr == m_pRenderer) return -1;
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	//콜리전 매니저에 등록
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement) return -1;
	pManagement->Add_CollisionGroup(CCollisionMgr::COL_BOX, this);


	return _int();
}

HRESULT CMarine::Render_GameObject()
{
	_matrix			matView, matProj;

	CManagement* pEnginMgr = CManagement::Get_Instance();
	if (nullptr == pEnginMgr) return E_FAIL;
	matView = pEnginMgr->Get_Transform(D3DTS_VIEW);
	matProj = pEnginMgr->Get_Transform(D3DTS_PROJECTION);

	if (FAILED(m_pTexture->Set_TextureOnShader(m_pShader, "g_DiffuseTexture", m_tDesc.iTextureID)))
		return E_FAIL;

	if (FAILED(m_pShader->Set_Value("g_matWorld", &m_pTransform->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matView", &matView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matProj", &matProj, sizeof(_matrix))))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Shader()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Pass(m_tDesc.iShaderPass)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pShader->End_Pass()))
		return E_FAIL;
	if (FAILED(m_pShader->End_Shader()))
		return E_FAIL;
	return S_OK;

}

HRESULT CMarine::OnKeyDown(_int KeyCode)
{
	if (KeyCode == VK_LBUTTON)
	{
		if (IsControllable())
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);
			GoToDst(pt);

		}
	}
	return S_OK;
}

CMarine * CMarine::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CMarine*	pInstance = new CMarine(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CMarine");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject * CMarine::Clone_GameObject(void * pArg)
{
	CMarine*	pInstance = new CMarine(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CMarine");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CMarine::Free()
{
	CPickingMgr::Get_Instance()->UnRegister_Observer(this);
	CKeyMgr::Get_Instance()->UnRegisterObserver(m_tDesc.eSceneID, this);

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pBoxCollider);
	CUnit::Free();
}

_bool CMarine::Picking(POINT _ViewPortPoint, _float3 * pHitPos)
{
	return m_pVIBuffer->Picking(_ViewPortPoint, g_hWnd, m_pTransform->Get_WorldMatrix(), pHitPos);
}

void CMarine::Interact()
{
	m_bControlMode = true;
	int a = 0;
}
