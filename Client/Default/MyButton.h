#pragma once
#include "GameObject.h"

#include "Management.h"

BEGIN(Client)
class CMyButton : public CGameObject
{
public:
	typedef struct tagStateDesc
	{
		BASEDESC				m_tBaseDesc;
		//Ű�Ŵ����� ����ؾ��ϴµ� ��ư�� ��������� ���� �� �����ϱ� ���ڷ� �޾ƾ���.
		SCENEID					m_eSceneID;
		const _tchar*			m_pTextureTag = L"";
		_uint					m_iTextureSceneID = SCENE_END;
		_uint					m_iTextureID = 0;
		_uint					m_iShaderPass = 0;
		_uint					m_iDepth = 0;
		

	}STATEDESC;

protected:
	explicit CMyButton(PDIRECT3DDEVICE9 _pGraphic_Device) :CGameObject(_pGraphic_Device) {};
	explicit CMyButton(CMyButton& _rhs);
	virtual ~CMyButton() = default;



public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();


public:
	void	Expand(_float3 _vMaxSize, _float3 _fExpandSpeed);





public:
	virtual HRESULT	Add_Listener(function<void()> _listener)
	{
		m_vecOnListener.push_back(_listener);
		return S_OK;
	}
	virtual HRESULT	Set_Listener(function<void()> _listener)
	{
		m_Listener = _listener;
		return S_OK;
	}


public:
	virtual HRESULT	OnKeyDown(_int KeyCode);

	virtual _uint Get_Depth();


protected:
	CVIBuffer_ViewPort*			m_pVIBuffer = nullptr;
	CTransform*			m_pTransform = nullptr;
	CTexture*			m_pTexture = nullptr;
	CShader*			m_pShader = nullptr;
	CRenderer*			m_pRenderer = nullptr;


public:
	_bool			m_bExpand = false;
	_float3			m_vMaxExpandSize;
	_float3			m_vExpandSpeed;

protected:
	STATEDESC		m_tDesc;

protected:
	//�������� ������
	vector<function<void()>> m_vecOnListener;
	//�ϳ��� ������(��� ����� �뵵)
	function<void()>	m_Listener;


public:
	static CMyButton*	Create(PDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
public:

	virtual void Free() override;

};

END