#include "CollisionMgr.h"
#include "Collider.h"
#include "GameObject.h"
IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{

}

HRESULT CCollisionMgr::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CCollisionMgr::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CCollisionMgr::Add_CollisionGroup(COLLISION_GROUP eGroup, CGameObject* pGameObject)
{
	if (COL_END <= eGroup ||
		nullptr == pGameObject)
		return E_FAIL;

	m_CollisionGroup[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CCollisionMgr::CheckCollision()
{
	//���� ������Ʈ���� GameObject ���� �浹list���� �����Ѵ�.(�ݸ����Ŵ����� ����Ʈ���� �����ϴ°� �ƴϴ�!)
	for (auto& group : m_CollisionGroup)
	{
		for (auto& go : group)
		{
			go->Clear_DeadObject();
		}
	}

	if (FAILED(CheckCollision_Box()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollisionMgr::CheckCollision_Box()
{

	for (size_t i = 0; i < m_CollisionGroup[COL_BOX].size(); ++i)
	{
		for (size_t j = i + 1; j < m_CollisionGroup[COL_BOX].size(); ++j)
		{
			CGameObject* pSrc = m_CollisionGroup[COL_BOX][i];
			CGameObject* pDst = m_CollisionGroup[COL_BOX][j];

			CCollider* pColliderA = (CCollider*)pSrc->Find_Component(L"Com_Collider");
			CCollider* pColliderB = (CCollider*)pDst->Find_Component(L"Com_Collider");

			if (nullptr == pColliderA ||
				nullptr == pColliderB)
				return E_FAIL;

			if (pColliderA->Check_Collision(pColliderB))
			{

				//������ �������, �浹ó���� ���� �ʴ´�.
				if (pSrc->Get_Dead())
					pDst->Erase_Collided(pSrc);
				if (pDst->Get_Dead())
					pSrc->Erase_Collided(pDst);
				if (pSrc->Get_Dead() || pDst->Get_Dead())
					continue;


				//���� ���� �浹�� ���� ������ ������ �浹����Ʈ�� ���θ� �߰��Ѵ�.
				if (!pDst->Contain_Collided(pSrc))
				{
					pDst->Add_Collided(pSrc);
					pDst->Set_isCollided(true);
					pDst->OnCollisionEnter(pSrc);
				}
				if (!pSrc->Contain_Collided(pDst))
				{
					pSrc->Add_Collided(pDst);
					pSrc->Set_isCollided(true);
					pSrc->OnCollisionEnter(pDst);
				}


				pDst->OnCollisionStay(pSrc);
				pSrc->OnCollisionStay(pDst);
			}
			else
			{
				//���� ������ ���� �浹�� ����Ʈ�� ������ �����Ѵ�.
				pDst->Erase_Collided(pSrc);
				pSrc->Erase_Collided(pDst);

				//�浹����Ʈ�� �����, ���� �浹������ ������ collisionExit�� �����Ѵ�.
				if (pDst->Get_CollidedSize() == 0 && pDst->Get_isCollided())
				{
					pDst->Set_isCollided(false);
					pDst->OnCollisionExit(pSrc);
				}
				if (pSrc->Get_CollidedSize() == 0 && pSrc->Get_isCollided())
				{
					pSrc->Set_isCollided(false);
					pSrc->OnCollisionExit(pDst);
				}
			}
		}

		Safe_Release(m_CollisionGroup[COL_BOX][i]);
	}

	m_CollisionGroup[COL_BOX].clear();

	return S_OK;
}



void CCollisionMgr::Free()
{
	for (size_t i = 0; i < COL_END; ++i)
	{
		for (auto& pGameObject : m_CollisionGroup[i])
			Safe_Release(pGameObject);

		m_CollisionGroup[i].clear();
	}

}
