#include <Windows.h>

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "姆姆游戏引擎demo"

LRESULT WINAPI MsgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{

	//用户通过点击关闭程序按钮后，
	//消息队列增加一条消息WM_CLOSE，
	//然后程序从消息队列中取走WM_CLOSE，
	//调用DestroyWindow()，
	//消息队列增加WM_DESTROY，
	//应用程序再次取走，
	//并调用PostQuitMessage()，
	//最终得到WM_QUIT而使消息循环退出，
	//程序退出。
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

	//将消息传递给window去处理
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
		NULL,				//图标
		NULL,				//鼠标
		(HBRUSH)GetStockObject(WHITE_BRUSH),//NULL,				//背景色
		NULL,				//菜单
		WINDOW_CLASS,			//窗口类名称
		NULL				//创建窗口的
	};

	//注册
	RegisterClassEx(&wc);

	//创建窗口类 
	HWND hWnd = CreateWindow(WINDOW_CLASS,			//参数1：创建的窗口类名称
			WINDOW_TITLE,						//参数2：窗口类的标题
			WS_OVERLAPPEDWINDOW,				//参数3：窗口的样式
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
	//清空
	ZeroMemory(&msg,sizeof(msg));

	//消息循环
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//取消注册-释放资源
	UnregisterClass(WINDOW_CLASS,hInst);
}