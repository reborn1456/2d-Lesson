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
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);             // ウィンドウの背景色
//	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(250, 0, 0));        // RGBでウィンドウの背景色を変える, Releaseしてないからあんまり使わない方がいい
	wc.lpszMenuName = (LPSTR)NULL;                                      // メニュー
	wc.lpszClassName = "Test";                                          // ■重要■ウィンドウクラスの名前
	RegisterClass(&wc);                                                 // 登録する

	// ②ウィンドウクラスに従いウィンドウを（メモリ上に）作る
	hWnd = CreateWindow("Test", "TITLE", WS_OVERLAPPEDWINDOW, 200, 100, 500, 400, NULL, NULL, hInst, NULL);

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
	static int x = 0;
	int mx, my;

	switch (uMsg)
	{
	case WM_DESTROY:                                                  // ウィンドウが破棄された
		PostQuitMessage(0);
		break;
		// =============================== New added in this lesson =====================================
	case WM_KEYDOWN:
		hdc = GetDC(hWnd);
		if (wParam == VK_SPACE)
		{
			SetTextColor(hdc, RGB(255, 255, 0));
			SetBkColor(hdc, RGB(0, 0, 255));
			TextOut(hdc, x, 50, "こんにちは", 10);
			x += 1;
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		mx = LOWORD(lParam);   //下位の１６桁を取る
		my = HIWORD(lParam);   //上位の１６桁を取る
		SetPixel(hdc, mx, my, RGB(255, 255, 255));
		ReleaseDC(hWnd, hdc);
		break;
		// =============================== New added in this lesson =====================================
	default:                                                          // それ以外の（何の処理もしない）メッセージのとき
		return DefWindowProc(hWnd, uMsg, wParam, lParam);             // OSに処理をまかせる
	}

	return 0;
}