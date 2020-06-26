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
	//죽은 오브젝트들은 GameObject 안의 충돌list에서 제거한다.(콜리전매니저의 리스트에서 제거하는게 아니다!)
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

				//상대방이 죽은경우, 충돌처리를 하지 않는다.
				if (pSrc->Get_Dead())
					pDst->Erase_Collided(pSrc);
				if (pDst->Get_Dead())
					pSrc->Erase_Collided(pDst);
				if (pSrc->Get_Dead() || pDst->Get_Dead())
					continue;


				//만약 서로 충돌한 적이 없으면 서로의 충돌리스트에 서로를 추가한다.
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
				//만약 상대방이 전에 충돌한 리스트에 있으면 삭제한다.
				pDst->Erase_Collided(pSrc);
				pSrc->Erase_Collided(pDst);

				//충돌리스트가 비었고, 전에 충돌한적이 있으면 collisionExit을 실행한다.
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
