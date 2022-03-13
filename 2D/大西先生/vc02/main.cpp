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
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);             // ウィンドウの背景色
//	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(250, 0, 0));        // RGBでウィンドウの背景色を変える, Releaseしてないからあんまり使わない方がいい
	wc.lpszMenuName = (LPSTR)NULL;                                      // メニュー
	wc.lpszClassName = "Test";                                          // ■重要■ウィンドウクラスの名前
	RegisterClass(&wc);                                                 // 登録する

	// ②ウィンドウクラスに従いウィンドウを（メモリ上に）作る
	hWnd = CreateWindow("Test", "ウィンドウプロシージャの学習", WS_OVERLAPPEDWINDOW, 10, 50, 800, 600, NULL, NULL, hInst, NULL);

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
	int m = 0;
	char str[50];
	int mouseX = 0, mouseY = 0;
	POINT Mouse;

	switch (uMsg)
	{
	case WM_CREATE:
		MessageBeep(MB_ICONASTERISK);
		while (1)   // 無限ループ
		{
			m = MessageBox(hWnd, "A Window Has Been Created", "Message Box", MB_ABORTRETRYIGNORE | MB_DEFBUTTON3 | MB_ICONEXCLAMATION);   // メッセージボックスを表示する
			if (m == IDABORT)   // 中止を押したら
			{
				DestroyWindow(hWnd);   // ウィンドウが破棄される
				return 0;
			}
			if (m == IDIGNORE)   // 無視を押したら
				break;   // ループを抜ける
		}
		break;

	case WM_MOUSEMOVE:
		for (int i = 0; i < 10; i++)
		{
			FlashWindow(hWnd, TRUE);  // if false then it just simply does't flash
		}
		break;

	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		case 'B':
			MessageBeep(MB_ICONASTERISK);
			break;
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
			FlashWindow(hWnd, TRUE);  // if false then it just simply does't flash
			break;
		}
		break;

	case WM_CHAR:
//		str[0] = wParam;
//		str[1] = '\0';
		wsprintf(str, "%c", wParam);
		MessageBox(hWnd, str, TEXT("Message Box"), MB_OK);
		break;

	case WM_LBUTTONUP:
//		mouseX = lParam & 0x0000FFFF;
		mouseX = LOWORD(lParam);   //下位の１６桁を取る
//		mouseY = (lParam >> 16) & 0x0000FFFF;   // 一つ目の方法。シフトしてから＆（アンド）
//		mouseY = (lParam & 0xFFFF0000) >> 16;   // 二つ目の方法。＆（アンド）からシフトする
		mouseY = HIWORD(lParam);   //上位の１６桁を取る
		wsprintf(str, "マウス座標( %d, %d )", mouseX, mouseY);
		//GetCursorPos(&Mouse);
		//ScreenToClient(hWnd, &Mouse);
		//wsprintf(str, "マウス座標( %d, %d )", Mouse.x, Mouse.y);
		SetWindowText(hWnd, str);
		break;

	case WM_CLOSE:
		if (IDYES == MessageBox(hWnd, TEXT("Are you sure you want to quit this program?"), TEXT("Confirmation"), MB_YESNO | MB_DEFBUTTON2))
		{
			DestroyWindow(hWnd);
			return 0;
		}
		break;

	case WM_RBUTTONDOWN:
		if (IDYES == MessageBox(hWnd, TEXT("Are you sure you want to quit this program?"), TEXT("Confirmation"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			DestroyWindow(hWnd);
			return 0;
		}
		break;

	case WM_DESTROY:                                                  // ウィンドウが破棄された
		PostQuitMessage(0);
		return 0;

	default:                                                          // それ以外の（何の処理もしない）メッセージのとき
		return DefWindowProc(hWnd, uMsg, wParam, lParam);             // OSに処理をまかせる
	}

	return 0;
}