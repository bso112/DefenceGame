#include "..\Headers\Base.h"

USING(Engine)

CBase::CBase()
{
}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
	
}

HRESULT CBase::OnKeyDown(_int KeyCode)
{
	return S_OK;
}

HRESULT CBase::OnKeyPressing(_int KeyCode)
{
	return S_OK;
}

HRESULT CBase::OnKeyUp(_int KeyCode)
{
	return S_OK;
}

HRESULT CBase::OnMoveTurn()
{
	return S_OK;
}
