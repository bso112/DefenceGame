#include "stdafx.h"
#include "BombTower.h"
#include "Management.h"
#include "PickingMgr.h"
#include "CollisionMgr.h"
#include "Unit.h"
#include "Image3D.h"
CBombTower::CBombTower(PDIRECT3DDEVICE9 pGraphic_Device)
	: CBuilding(pGraphic_Device)
{
	m_tagStat.iPrice = 40;

}

CBombTower::CBombTower(const CBombTower & rhs)
	: CBuilding(rhs)
{
	m_iTileSize = rhs.m_iTileSize;
	m_fScale = rhs.m_fScale;
	m_tagStat.iPrice = rhs.m_tagStat.iPrice;
}

HRESULT CBombTower::Ready_GameObject_Prototype()
{
	//건축물별 기본 필요정보 수정

	//필요 타일 크기
	m_iTileSize = 1;

	//건축물 자체 크기
	m_fScale = 1.f;

	return S_OK;
}

HRESULT CBombTower::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tagStat.iMaxHP = CValue<int>(60);
	m_tagStat.iHp = m_tagStat.iMaxHP.GetValue();
	m_tagStat.iLevel = 1;
	//m_pVIBufferCom->SetAnchor(_float3(0.f,-0.5f,0.f));

	m_fRealScaleMag = m_fScale * m_iTileSize;
	m_pTransformCom->SetUp_Scale(_float3(m_fRealScaleMag, m_fRealScaleMag, m_fRealScaleMag));
	m_pTransformCom->SetUp_Position(((CBuilding::BUILDING_DESC*)pArg)->vPos);



	CPickingMgr::Get_Instance()->Register_Observer(this);

	return S_OK;
}

_int CBombTower::Update_GameObject(_double TimeDelta)
{
	if (m_bDead)
	{
		//폭발
		CPickingMgr* pickingMgr = CPickingMgr::Get_Instance();
		if (nullptr == pickingMgr)
			return -1;
		if (nullptr == m_pTransformCom)
			return -1;

		vector<CGameObject*> pObList = pickingMgr->OverlapSphere(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 5.f);
		for (auto& obj : pObList)
		{
			CUnit* pUnit = dynamic_cast<CUnit*>(obj);
			if (nullptr != pUnit)
			{
				pUnit->TakeDamage(100000, 1000);
			}
		}

		CImage3D* pEffect = nullptr;
		
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement) return -1;
		CImage3D::STATEDESC desc;
		desc.eTextureSceneID = SCENE_STATIC;
		desc.pTextureTag = L"Component_Texture_Explosion";
		desc.tBaseDesc = BASEDESC(m_pTransformCom->Get_State(CTransform::STATE_POSITION), _float3(100.f,100.f,1.f));
		CGameObject* pImage = pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Image3D",SCENE_STAGE1, L"Layer_Effect", &desc);
		return -1;
	}

	m_pBoxCollider->Update_Collider(m_pTransformCom->Get_WorldMatrix());
	return 0;
}

_int CBombTower::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this)))
		return -1;

	CCollisionMgr::Get_Instance()->Add_CollisionGroup(CCollisionMgr::COL_BOX, this);

	return _int();
}

HRESULT CBombTower::Render_GameObject()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(RENDER_BOMBTOWER);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

	return S_OK;
}

void CBombTower::Upgrade()
{
}

void CBombTower::Interact()
{
	CBuilding::Interact();
}

void CBombTower::OnCollisionEnter(CGameObject * _pOther)
{

}

void CBombTower::OnCollisionStay(CGameObject * _pOther)
{

}

HRESULT CBombTower::Add_Component()
{
	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	//// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Cube", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Brick", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Solid", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_BoxCollider", L"Com_Collider", (CComponent**)&m_pBoxCollider)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBombTower::SetUp_ConstantTable()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));


	if (FAILED(m_pTextureCom->Set_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;



	Safe_Release(pManagement);

	return S_OK;
}

CBombTower * CBombTower::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CBombTower*	pInstance = new CBombTower(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CBombTower");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBombTower::Clone_GameObject(void * pArg)
{
	CBombTower*	pInstance = new CBombTower(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CBombTower");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBombTower::Free()
{
	CPickingMgr::Get_Instance()->UnRegister_Observer(this);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBoxCollider);

	CGameObject::Free();
}
