#include "..\Headers\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{

}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE g_hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	//Ű������ ��Ȯ�ϰ� �������. c_dfDIKeyboard �� ����������.
	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;
	/*
	���������̶� hWnd�� �ٸ� ������â�� �����ϴ� ������. 
	������� hWnd�� ���������� Ű���� ��ǲ�� �޴´ٰ� �ϸ� �ٸ� ������� Ű���� ��ǲ�� ���� ���Ѵ�.
	DISCL_NONEXCLUSIVE	: �ٸ� �����쵵 Ű���� ��ǲ�� ���� �� �ִ�.
	DISCL_BACKGROUND	: â�� �������� ��ǲ�� �����ϴ�. â�� ���������� ��ǲ�� �Ұ����ϴٰ� �ϸ� â�� �������� Ű���尡 �����̵�.
	���Է���ġ�� Ű���峪 ���콺�� �׻� DISCL_NONEXCLUSIVE | DISCL_BACKGROUND �̷��� ����Ѵ�.
	*/
	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND /*| DISCL_NOWINKEY*/)))
		return E_FAIL;
	//����̽��� ������ �� �ִ� ������� ��´�.
	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;


	if (FAILED(m_pSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND /*| DISCL_NOWINKEY*/)))
		return E_FAIL;
	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInput_Device::Set_Input_Device_State()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		return E_FAIL;

	//Ű������ ���¸� �޴´�.
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	//���콺�� ���¸� �޴´�.
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pSDK);
}
