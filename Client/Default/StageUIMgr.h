#pragma once
#include "Base.h"

BEGIN(Client)
class CMyImage;
class CMyButton;
class CStageUIMgr : public CBase
{
	DECLARE_SINGLETON(CStageUIMgr)
public:
	explicit CStageUIMgr() {};
	virtual ~CStageUIMgr() = default;


public:
	virtual HRESULT Ready_StageUIMgr();

private:
	CMyButton*	m_pGameStartBtn = nullptr;



	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;

};
END
