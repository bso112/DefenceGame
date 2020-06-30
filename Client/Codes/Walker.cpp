#include "stdafx.h"
#include "Walker.h"
#include "PickingMgr.h"
#include "KeyMgr.h"
#include "PickingMgr.h"
#include "AIStateController.h"
#include "AIState.h"
#include "Building.h"
#include "GameManager.h"
#include "StageUIMgr.h"

CWalker::CWalker(PDIRECT3DDEVICE9 pGraphic_Device)
	:CUnit(pGraphic_Device)
{
}

CWalker::CWalker(const CWalker & _rhs)
	: CUnit(_rhs)
{
}

HRESULT CWalker::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CWalker::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	CTransform::STATEDESC tTransformDesc;
	tTransformDesc.RotatePerSec = D3DXToRadian(90.f);
	tTransformDesc.SpeedPerSec = 8.0;
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
	
	m_iRecogRange = 10;
	m_tUnitStats.iAtt = CValue<int>(60);
	m_tUnitStats.iMaxHp = CValue<int>(30.f);
	m_tUnitStats.iCurrHp = m_tUnitStats.iMaxHp.GetValue();
	m_tUnitStats.bFriendly = false;

	return S_OK;
}

_int CWalker::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pBoxCollider ||
		nullptr == m_pTransform)
		return -1;

	m_pBoxCollider->Update_Collider(m_pTransform->Get_WorldMatrix());


	//컨트롤 모드
	if (m_bControlMode)
		return CUnit::Update_GameObject(TimeDelta);

#pragma region AIHunting

	//타깃이 있을떄
	if (nullptr != m_pTarget)
	{

		//타깃으로 간다.
		m_bMoving = true;



		CTransform* pTargetTransform = (CTransform*)m_pTarget->Find_Component(L"Com_Transform");
		if (nullptr == pTargetTransform) return -1;
		_float fDist = D3DXVec3Length(&(pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransform->Get_State(CTransform::STATE_POSITION)));
		//타깃과의 거리가 좁으면
		if (fDist < 2.f)
		{
			//공격
			CUnit* pUnit = dynamic_cast<CUnit*>(m_pTarget);
			CBuilding* pBuilding = dynamic_cast<CBuilding*>(m_pTarget);
			if (nullptr != pUnit)
			{
				pUnit->TakeDamage(m_tUnitStats.iAtt.GetValue(), m_tUnitStats.iAtt.GetValue());
			}
			else if (nullptr != pBuilding)
			{
				pBuilding->Get_Damage(m_tUnitStats.iAtt.GetValue());
			}


		}
	}

	//게임이 시작되었고, 타깃이 없을때만
	if (CGameManager::Get_Instance()->IsGameStart() && nullptr == m_pTarget)
	{

		CPickingMgr* pPickMgr = CPickingMgr::Get_Instance();
		if (nullptr == pPickMgr) return-1;
		if (nullptr == m_pTransform) return -1;


		//주변을 조사해서 게임오브젝트를 얻는다.
		vector<CGameObject*> pTargetVector = pPickMgr->OverlapSphere(m_pTransform->Get_State(CTransform::STATE_POSITION), FLT_MAX, this);
		if (pTargetVector.empty()) return 0;

		CUnit* pTargetUnit = nullptr;
		CBuilding* pTargetBuilding = nullptr;

		//최소거리 오브젝트 구하기
		_float fMindist = FLT_MAX;
		for (auto& go : pTargetVector)
		{
			//유닛일 경우
			CUnit*		pUnit = dynamic_cast<CUnit*>(go);
			if (nullptr != pUnit)
			{
				//아군일 경우 무시
				if (pUnit->Get_Friendly() == Get_Friendly())
					continue;
			}
			//빌딩일 경우
			else
			{
				//아군은 빌딩으로 갈 필요가 없다.
				if (m_tUnitStats.bFriendly)
					continue;
			}


			CTransform* pTargetTransform = (CTransform*)go->Find_Component(L"Com_Transform");
			if (nullptr == pTargetTransform) return -1;
			_float fDist = D3DXVec3Length(&(pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransform->Get_State(CTransform::STATE_POSITION)));
			if (fMindist > fDist)
			{
				fMindist = fDist;
				m_pTarget = go;
			}

		}

		if (nullptr == m_pTarget)
			return 0;

		//타깃과의 거리가 2보다 작으면
		if (fMindist < 2.f)
		{
			//공격
			if (pTargetUnit != nullptr)
				pTargetUnit->TakeDamage(m_tUnitStats.iAtt.GetValue(), m_tUnitStats.iAtt.GetValue());
			else if (pTargetBuilding != nullptr)
				pTargetBuilding->Get_Damage(m_tUnitStats.iAtt.GetValue());

		}
		//아니면
		else
		{
			//다가간다.
			CTransform* pTargetTransform = (CTransform*)m_pTarget->Find_Component(L"Com_Transform");
			if (nullptr == pTargetTransform) return -1;

			m_pTransform->Go_Dst(pTargetTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
			//A*
			//GoToDst(pTargetTransform->Get_State(CTransform::STATE_POSITION));


		}


	}

	//왠지 모르겠지만 타깃이 있는데 루트가 비면
	if (m_pTarget != nullptr && m_Route.empty())
	{
		CTransform* pTargetTransform = (CTransform*)m_pTarget->Find_Component(L"Com_Transform");
		if (nullptr == pTargetTransform) return -1;
		m_pTransform->Go_Dst(pTargetTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		//GoToDst(pTargetTransform->Get_State(CTransform::STATE_POSITION));
	}



#pragma endregion


	return CUnit::Update_GameObject(TimeDelta);
}

_int CWalker::Late_Update_GameObject(_double TimeDelta)
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

	if (nullptr != m_pTarget)
	{
		if (m_pTarget->Get_Dead())
			m_pTarget = nullptr;

	}

	return _int();
}

HRESULT CWalker::Render_GameObject()
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

HRESULT CWalker::OnKeyDown(_int KeyCode)
{

	return S_OK;
}

CWalker * CWalker::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CWalker*	pInstance = new CWalker(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CWalker");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject * CWalker::Clone_GameObject(void * pArg)
{
	CWalker*	pInstance = new CWalker(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CWalker");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CWalker::Free()
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

_bool CWalker::Picking(POINT _ViewPortPoint, _float3 * pHitPos)
{
	return m_pVIBuffer->Picking(_ViewPortPoint, g_hWnd, m_pTransform->Get_WorldMatrix(), pHitPos);
}

void CWalker::Interact()
{
	m_bControlMode = true;
	int a = 0;
	CUnit::Interact();
}
