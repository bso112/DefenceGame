#include "stdafx.h"
#include "..\Headers\Marine.h"
#include "PickingMgr.h"
#include "KeyMgr.h"
#include "PickingMgr.h"
#include "AIStateController.h"
#include "AIState.h"

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
	tTransformDesc.SpeedPerSec = 5.0;
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

	m_pAICon->Set_State(CAIState::STATE_IDLE, new CAIIdle(this));
	m_pAICon->Set_State(CAIState::STATE_HUNTING, new CAIHunting(this));
	m_pAICon->Set_State(CAIState::STATE_RETREAT, new CAIRetreat(this));
	m_pAICon->Set_State(CAIState::STATE_WAIT, new CAIWait(this));

	m_pAICon->Set_Default_State(CAIState::STATE_HUNTING, CAIState::STATEDESC(), CManagement::Get_Instance()->Get_TimeDelta(L"Timer_60"));



	m_bFriendly = true;
	m_iRecogRange = 5;
	
	m_tUnitStats.iAtt = CValue<int>(20);

	//피킹매니저에 등록. 
	CPickingMgr::Get_Instance()->Register_Observer(this);

	return S_OK;
}

_int CMarine::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pBoxCollider ||
		nullptr == m_pTransform)
		return E_FAIL;

	m_pBoxCollider->Update_Collider(m_pTransform->Get_WorldMatrix());

	return CUnit::Update_GameObject(TimeDelta);
}

_int CMarine::Late_Update_GameObject(_double TimeDelta)
{


	if (nullptr == m_pAICon) return -1;
	m_pAICon->Update(CAIState::STATEDESC(), TimeDelta);


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
