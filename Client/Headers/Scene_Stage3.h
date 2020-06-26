#pragma once

// �ε����� �ǹ��ϴ� Ŭ����.
// ���������� �ε���ü�� ����.
#include "Client_Defines.h"
#include "Scene.h"
#include "Loading.h"

BEGIN(Client)

class CScene_Stage3 final : public CScene
{
private:
	explicit CScene_Stage3(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Stage3() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();
public:
	static CScene_Stage3* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

END