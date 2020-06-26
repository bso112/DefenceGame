#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();


public:
	//충돌리스트안에 있지만 죽은 오브젝트들을 리스트에서 제거한다.
	void Clear_DeadObject();
	bool Get_isCollided() { return m_bCollided; }
	void Set_isCollided(bool _bCollided) { m_bCollided = _bCollided; }
	//충돌한 오브젝트를 저장한다.
	void Add_Collided(CGameObject* _pColided) { m_listCollided.push_back(_pColided); }
	//충돌한 오브젝트 셋의 사이즈를 가져온다.
	size_t Get_CollidedSize() { return m_listCollided.size(); }
	//충돌한 오브젝트 셋에 _pCollied가 있는지 검사한다.
	bool Contain_Collided(CGameObject* _pCollided)
	{
		for (auto& go : m_listCollided)
		{
			if (go == _pCollided)
				return true;
		}
		return false;
	}

	bool Erase_Collided(CGameObject* _pCollided)
	{
		auto& iter = m_listCollided.begin();
		while (iter != m_listCollided.end())
		{
			if (*iter == _pCollided)
			{
				iter = m_listCollided.erase(iter);
				return true;
			}
			else
				++iter;
		}
		return false;
	}
	void Set_Dead() { m_bDead = true; }
	const bool& Get_Dead() const { return m_bDead; }
	void Set_Active(_bool bActive) { m_bActive = bActive; OnSetActive(bActive); }
	virtual void OnSetActive(_bool bActive) {}
protected:
	HRESULT Add_Component(_uint iPrototypeSceneID, const _tchar* pPrototypeTag, const _tchar * pComponentTag, CComponent** ppOut, void* pArg = nullptr);
public:
	CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionStay(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

protected:
	PDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	_bool						m_bActive = true;
	_bool						m_bDead = false;
	const _tchar*				m_pName = nullptr;

public:
	const _tchar*				Get_Name() { return m_pName; }

protected:
	map<const _tchar*, CComponent*>			m_Components;
	typedef map<const _tchar*, CComponent*>	COMPONENTS;

protected:
	//충돌했나
	bool	m_bCollided = false;
	//충돌한 오브젝트 셋
	list<CGameObject*> m_listCollided;

public:
	virtual CGameObject* Clone_GameObject(void* pArg) = 0;
	virtual void Free();
};

END