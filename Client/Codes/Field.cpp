#include "stdafx.h"
#include "..\Headers\Field.h"
#include "Wall.h"
#include "Goal.h"
USING(Client)

CField::CField(PDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CField::CField(const CField & rhs)
	: CGameObject(rhs)
{

}

HRESULT CField::Ready_GameObject_Prototype()
{

	return S_OK;
}

HRESULT CField::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement) return E_FAIL;


#pragma region ÇÊµå¸¸µé±â
	_float3 vFieldPos = _float3(0.f, 0.f, 0.f);
	_float3	vFieldSize = _float3(m_tDesc.iBrickNumX + BRICK_MARGIN * (m_tDesc.iBrickNumX + 1), m_tDesc.iBrickNumY + BRICK_MARGIN * (m_tDesc.iBrickNumY + 1), m_tDesc.iBrickNumZ + BRICK_MARGIN * (m_tDesc.iBrickNumZ + 1));
	CWall::STATEDESC tWallDesc;
	tWallDesc.eTextureSceneID = SCENE_STATIC;
	tWallDesc.pTextureTag = L"Component_Texture_Wall";
	CGameObject* pWall = nullptr;
	CTransform* pTransform = nullptr;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;


	m_pTransform->SetUp_Position(vFieldPos);
	m_pTransform->SetUp_Scale(vFieldSize);

	//¿ÞÂÊ ¸é
	tWallDesc.tBaseDesc = BASEDESC(_float3(vFieldPos.x - vFieldSize.x * 0.5f, vFieldPos.y, vFieldPos.z), _float3(vFieldSize.x, vFieldSize.y, 0.01f));
	tWallDesc.pName = L"Left";
	if (nullptr == (pWall = pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Wall", m_tDesc.eSceneID, L"Layer_Wall", &tWallDesc)))
		return E_FAIL;
	if (nullptr == (pTransform = dynamic_cast<CTransform*>(pWall->Find_Component(L"Com_Transform"))))
		return E_FAIL;

	pTransform->SetUp_Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));

	if (nullptr == (m_Walls[0] = dynamic_cast<CWall*>(pWall)))
		return E_FAIL;

	

	//¿À¸¥ÂÊ¸é
	tWallDesc.tBaseDesc = BASEDESC(_float3(vFieldPos.x + vFieldSize.x * 0.5f, vFieldPos.y, vFieldPos.z), _float3(vFieldSize.x, vFieldSize.y, 0.01f));
	tWallDesc.pName = L"Right";
	if (nullptr == (pWall = pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Wall", m_tDesc.eSceneID, L"Layer_Wall", &tWallDesc)))
		return E_FAIL;
	if (nullptr == (pTransform = dynamic_cast<CTransform*>(pWall->Find_Component(L"Com_Transform"))))
		return E_FAIL;

	pTransform->SetUp_Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));

	if (nullptr == (m_Walls[1] = dynamic_cast<CWall*>(pWall)))
		return E_FAIL;

	//¾Õ¸é
	tWallDesc.tBaseDesc = BASEDESC(_float3(vFieldPos.x, vFieldPos.y, vFieldPos.z - vFieldSize.z * 0.5f), _float3(vFieldSize.x, vFieldSize.y, 0.01f));
	tWallDesc.pName = L"Fornt";
	if (nullptr == (pWall = pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Wall", m_tDesc.eSceneID, L"Layer_Wall", &tWallDesc)))
		return E_FAIL;

	if (nullptr == (m_Walls[2] = dynamic_cast<CWall*>(pWall)))
		return E_FAIL;

	//µÞ¸é
	tWallDesc.tBaseDesc = BASEDESC(_float3(vFieldPos.x, vFieldPos.y, vFieldPos.z + vFieldSize.z * 0.5f), _float3(vFieldSize.x, vFieldSize.y, 0.01f));
	tWallDesc.pName = L"Back";
	if (nullptr == (pWall = pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Wall", m_tDesc.eSceneID, L"Layer_Wall", &tWallDesc)))
		return E_FAIL;

	if (nullptr == (m_Walls[3] = dynamic_cast<CWall*>(pWall)))
		return E_FAIL;
	//À­¸é
	tWallDesc.tBaseDesc = BASEDESC(_float3(vFieldPos.x, vFieldPos.y + vFieldSize.y * 0.5f, vFieldPos.z), _float3(vFieldSize.x, vFieldSize.y, 0.01f));
	tWallDesc.pName = L"Top";
	if (nullptr == (pWall = pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Wall", m_tDesc.eSceneID, L"Layer_Wall", &tWallDesc)))
		return E_FAIL;
	if (nullptr == (pTransform = dynamic_cast<CTransform*>(pWall->Find_Component(L"Com_Transform"))))
		return E_FAIL;

	pTransform->SetUp_Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	if (nullptr == (m_Walls[4] = dynamic_cast<CWall*>(pWall)))
		return E_FAIL;

	//¹Ø¸é
	tWallDesc.tBaseDesc = BASEDESC(_float3(vFieldPos.x, vFieldPos.y - vFieldSize.y * 0.5f, vFieldPos.z), _float3(vFieldSize.x, vFieldSize.y, 0.01f));
	tWallDesc.pName = L"Bottom";
	if (nullptr == (pWall = pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Wall", m_tDesc.eSceneID, L"Layer_Wall", &tWallDesc)))
		return E_FAIL;
	if (nullptr == (pTransform = dynamic_cast<CTransform*>(pWall->Find_Component(L"Com_Transform"))))
		return E_FAIL;

	pTransform->SetUp_Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	if (nullptr == (m_Walls[5] = dynamic_cast<CWall*>(pWall)))
		return E_FAIL;
#pragma endregion

	CGoal::STATEDESC tGoalDesc;
	tGoalDesc.eTextureSceneID = SCENE_STATIC;
	tGoalDesc.iTextureID = 0;
	tGoalDesc.pTextureTag = L"Component_Texture_Goal";
	CTransform* pWallTransform = (CTransform*)m_Walls[m_tDesc.eWhereGoal]->Find_Component(L"Com_Transform");
	if (nullptr == pWallTransform)
		return E_FAIL;
	_float3 vBaseGoalPos = pWallTransform->Get_State(CTransform::STATE_POSITION);
	tGoalDesc.tBaseDesc = BASEDESC(_float3(vBaseGoalPos.x + m_tDesc.iGoalX, vBaseGoalPos.y + m_tDesc.iGoalY, vBaseGoalPos.z), _float3(1.f, 1.f, 0.01f));
	tGoalDesc.eSceneID = m_tDesc.eSceneID;
	pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Goal", m_tDesc.eSceneID, L"Layer_Goal", &tGoalDesc);


	return S_OK;
}

_int CField::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CField::Late_Update_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CField::Render_GameObject()
{
	return S_OK;
}

CWall** CField::Get_Walls()
{
	return m_Walls;
}

CField * CField::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CField*	pInstance = new CField(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CField * CField::Clone_GameObject(void * pArg)
{
	CField*	pInstance = new CField(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CField");
		Safe_Release(pInstance);
	}
	return pInstance;
}




void CField::Free()
{
	Safe_Release(m_pTransform);
	CGameObject::Free();
}
