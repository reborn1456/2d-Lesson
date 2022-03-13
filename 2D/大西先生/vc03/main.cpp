// プロジェクト名：skelton
// ファイル名：main.cpp
// スケルトン（骨組み）プログラム
// =================================== ヘッダファイルのインクルード ===================================
#include <windows.h>




// =================================== 関数プロトタイプ宣言 ===================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




// =================================== エントリポイント（出発点） ===================================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;           // メッセージ（構造体）変数を宣言
	WNDCLASS wc;       // ウィンドウクラス（構造体）変数を宣言
	HWND hWnd;         // ウィンドウハンドル変数を宣言

	// =================================== ウィンドウを作る ===================================
	// ①ウィンドウクラス（＝ウィンドウの設計図）の登録申請
	wc.style = CS_HREDRAW | CS_VREDRAW;                                 // スタイル
	wc.lpfnWndProc = WndProc;                                           // ■重要■ウィンドウプロシージャ名（アドレス）
	wc.cbClsExtra = 0;                                                  // ウィンドウクラスの保持するデータサイズ
	wc.cbWndExtra = 0;                                                  // ウィンドウの保持するデータサイズ
	wc.hInstance = hInst;                                               // ■重要■インスタンスハンドル
	wc.hIcon = NULL;                                                    // アイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);                           // カーソル（マウスポインタ）
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);             // ウィンドウの背景色
	wc.lpszMenuName = (LPSTR)NULL;                                      // メニュー
	wc.lpszClassName = "Test";                                          // ■重要■ウィンドウクラスの名前
	RegisterClass(&wc);                                                 // 登録する

	// ②ウィンドウクラスに従いウィンドウを（メモリ上に）作る
	hWnd = CreateWindow("Test", "デバイスコンテキスト", WS_OVERLAPPEDWINDOW, 10, 50, 1280, 720, NULL, NULL, hInst, NULL);

	ShowWindow(hWnd, nCmdShow);        // ③ウィンドウを表示する
	UpdateWindow(hWnd);                // ④クライアント領域を表示更新する

	// =================================== メッセージループ ===================================
	while (GetMessage(&msg, NULL, 0, 0))        // ⑤メッセージを取得する
	{
		TranslateMessage(&msg);                 // ⑥メッセージを変換する
		DispatchMessage(&msg);                  // ⑦ウィンドウプロシージャへメッセージを送る
	}

	return (int)msg.wParam;                     // 終了
}

// =================================== ウィンドウプロシージャ ===================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	char str[50];
	static char work[100];   // static = remembers what is inside
	static int x = 0, y = 50;
	int mouseX = 0, mouseY = 0;
	int i = 1200, j = 720;
	POINT Mouse;

	switch (uMsg)
	{
	case WM_DESTROY:                                                  // ウィンドウが破棄された
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		hdc = GetDC(hWnd);

		if (wParam == 'C')
		{
			PatBlt(hdc, 0, 0, 1280, 720, BLACKNESS);
		}

		SetTextColor(hdc, RGB(255, 0, 0));
		SetBkColor(hdc, RGB(255, 255, 0));
		TextOut(hdc, 0, 0, "こんにちは", 10);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_CHAR:
		if (wParam == '\r')   // 改行キーの時
		{
			x = 0;
			y += 16;
		}
		else
		{
			hdc = GetDC(hWnd);
			//wsprintf(str, "%cが入力された", wParam);
			//TextOut(hdc, 0, 0, str, 14);
			wsprintf(str, "%c", wParam);
			//		strcat_s(work, str);
			TextOut(hdc, x, y, str, strlen(str));
			x += 10;
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_LBUTTONDOWN:
		GetCursorPos(&Mouse);
		ScreenToClient(hWnd, &Mouse);

		hdc = GetDC(hWnd);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, Mouse.x, Mouse.y, "魯", 2);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_RBUTTONUP:
		hdc = GetDC(hWnd);
		SetTextColor(hdc, RGB(0, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 5, 5, "1106046", 7);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_MOUSEMOVE:
		GetCursorPos(&Mouse);
		ScreenToClient(hWnd, &Mouse);
		//mouseX = LOWORD(lParam);   //下位の１６桁を取る
		//mouseY = HIWORD(lParam);   //上位の１６桁を取る

		hdc = GetDC(hWnd);
		//SetPixel(hdc, Mouse.x, Mouse.y, RGB(0, 0, 0));
		while (i > 80 && j > 20)
		{
			Ellipse(hdc, 300, 100, 1200, 700);
		}
		ReleaseDC(hWnd, hdc);
		break;
	default:                                                          // それ以外の（何の処理もしない）メッセージのとき
		return DefWindowProc(hWnd, uMsg, wParam, lParam);             // OSに処理をまかせる
	}

	return 0;
}