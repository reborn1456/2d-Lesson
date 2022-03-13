#include "CGameFrame.h"
#include "CPlayer.h"
#include "CBullet.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

LRESULT APIENTRY WndFunc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch(wParam){
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		return 0;

	}
	return DefWindowProc(hwnd,msg,wParam,lParam);

}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,
				   LPSTR lpszCmdParam,int nCmdshow)
{
	MSG msg;

	// GameFrame 生成
	CGameFrame * pGameFrame = new CGameFrame();	// 生成(メモリ確保)
	pGameFrame->Create(hInst, nCmdshow);		// 固定機能作成
	pGameFrame->Initialize();					// 初期化

	while(1){
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if(!GetMessage(&msg,NULL,0,0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			pGameFrame->GameLoop();
		}
	}

	// GameFrame 終了
	delete pGameFrame;	// 破棄(メモリ解放)

	return (int)msg.wParam;
}
