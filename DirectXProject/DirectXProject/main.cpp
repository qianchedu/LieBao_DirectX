#include <Windows.h>

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "ķķ��Ϸ����demo"

LRESULT WINAPI MsgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{

	//�û�ͨ������رճ���ť��
	//��Ϣ��������һ����ϢWM_CLOSE��
	//Ȼ��������Ϣ������ȡ��WM_CLOSE��
	//����DestroyWindow()��
	//��Ϣ��������WM_DESTROY��
	//Ӧ�ó����ٴ�ȡ�ߣ�
	//������PostQuitMessage()��
	//���յõ�WM_QUIT��ʹ��Ϣѭ���˳���
	//�����˳���
	case WM_DESTROY:
		PostQuitMessage(9);
		return 0;
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)PostQuitMessage(0);
		
		break;


	default:
		break;
	}

	//����Ϣ���ݸ�windowȥ����
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
int WINAPI WinMain(HINSTANCE hInst,
	HINSTANCE prevhInst,
	LPTSTR cmdLine,
	int show)
{
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgProc,
		0,
		0,
		hInst,
		NULL,				//ͼ��
		NULL,				//���
		(HBRUSH)GetStockObject(WHITE_BRUSH),//NULL,				//����ɫ
		NULL,				//�˵�
		WINDOW_CLASS,			//����������
		NULL				//�������ڵ�
	};

	//ע��
	RegisterClassEx(&wc);

	//���������� 
	HWND hWnd = CreateWindow(WINDOW_CLASS,			//����1�������Ĵ���������
			WINDOW_TITLE,						//����2��������ı���
			WS_OVERLAPPEDWINDOW,				//����3�����ڵ���ʽ
			100,								
			100,
			640,
			480,
			GetDesktopWindow(),
			NULL,
			hInst,
			NULL);

	ShowWindow(hWnd,SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	MSG msg;
	//���
	ZeroMemory(&msg,sizeof(msg));

	//��Ϣѭ��
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//ȡ��ע��-�ͷ���Դ
	UnregisterClass(WINDOW_CLASS,hInst);
}