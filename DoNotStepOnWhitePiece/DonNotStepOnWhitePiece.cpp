#include <Windows.h>
#include "resource.h"
#include <time.h>

#define BOLCK 100

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //��Ϣ��� ��Ϣ��� ���Ӳ��� 
//���ڳ������ں���
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	//��ƴ�����
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ�ɰ�ɫ
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//���ع��
	wndClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;//����Ӧ�ó���ʵ��
	wndClass.lpfnWndProc = WindowProc;//���ڻص�����
	wndClass.lpszClassName = L"DoNotStepOnWhitePisces";//����������
	wndClass.lpszMenuName = NULL;//�˵�����
	wndClass.style = CS_VREDRAW | CS_HREDRAW;//���

	//ע�ᴰ��//
	RegisterClass(&wndClass);

	//��������
	HWND hWnd=CreateWindow(L"DoNotStepOnWhitePisces",
		L"��Ȱ׿��-����ģʽ",
		WS_SYSMENU|WS_CAPTION,
		50,
		50,
		BOLCK*4+16,
		BOLCK*4+34,
		NULL,
		NULL,
		hInstance,
		0
		);
	//��ʾ����
	ShowWindow(hWnd,SW_SHOW);
	//����
	UpdateWindow(hWnd);

	//��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return 0;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int bw[4];
	static int t=0;

	HDC hDC;        //��ͼ���
	PAINTSTRUCT ps; //�ṹ�� 
	HPEN hPen;      //���ʾ��
	HBRUSH hBrush;  //��ˢ���
	POINT point = {0,0};    //���������
	RECT rect;      //�����С
	static int Flag;
	static int n = 0;          //�������ĸ���
	wchar_t szTemp[100];//�ַ�����
	
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		srand((unsigned)time(NULL));
		for (int i = 0; i
			< 4; i++)
		{
			bw[i] = rand() % 4;
		}
		//������ʱ��
		SetTimer(hWnd,1,10,NULL);
		break;
	case WM_TIMER:
		t++;
		break;
	case WM_PAINT:
		hDC=BeginPaint(hWnd,&ps);
		for (int i = 0; i < 4; i++)
		{
			//ָ����������
			SetRect(&rect, bw[i]*BOLCK,i*BOLCK,bw[i]*BOLCK+BOLCK,i*BOLCK+BOLCK);
			//����һ֧��
			hPen=CreatePen(PS_SOLID,1,RGB(0,255,255));
			SelectObject(hDC, hPen);
			//����һ����ˢ
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC,hBrush);
			//��һ������
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			//�ͷ���Դ
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		EndPaint(hWnd,&ps);//������ͼ
		break;
	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		SendMessage(hWnd,WM_CHAR,point.x/BOLCK,0);
		break;
	case WM_CHAR:
		hDC=GetDC(hWnd);
		if (wParam!=bw[3])
		{
			Flag = -1;
			KillTimer(hWnd,0);//�رն�ʱ��
			wsprintf(szTemp,L"��Ϸ�����������ˣ���ʱ��%d.%d�� ������%d   ���ߣ�������",t/100,t-(t/100)*100,n);
			MessageBox(NULL,szTemp,L"��ʾ",MB_ICONWARNING);
			exit(0);
		}
		for (int i = 3; i >= 1; i--)
		{
			bw[i] = bw[i - 1];
		}
		bw[0] = rand() % 4;
		n++;//����ڿ�Ĵ���
		ScrollWindow(hWnd,0,BOLCK,NULL,NULL);
		//ָ����������
		SetRect(&rect, bw[0] * BOLCK,0,bw[0]*BOLCK+BOLCK,BOLCK);
		//����һ֧��
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		SelectObject(hDC, hPen);
		//����һ����ˢ
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrush);
		//��һ������
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		//�ͷ���Դ
		DeleteObject(hPen);
		DeleteObject(hBrush);
		ReleaseDC(hWnd,hDC);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}