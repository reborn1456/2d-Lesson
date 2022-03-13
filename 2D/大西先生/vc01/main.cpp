// プロジェクト名：skelton
// ファイル名：main.cpp
// スケルトン（骨組み）プログラム
// =================================== ヘッダファイルのインクルード ===================================
#include <windows.h>




// =================================== 関数プロトタイプ宣言 ===================================
LRESULT CALLBACK xyz(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




// =================================== エントリポイント（出発点） ===================================
int WINAPI WinMain(HINSTANCE abc, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;           // メッセージ（構造体）変数を宣言
	WNDCLASS wc;       // ウィンドウクラス（構造体）変数を宣言
	HWND hWnd;         // ウィンドウハンドル変数を宣言

	// =================================== ウィンドウを作る ===================================
	// ①ウィンドウクラス（＝ウィンドウの設計図）の登録申請
	wc.style = CS_HREDRAW | CS_VREDRAW;                            // スタイル
	wc.lpfnWndProc = xyz;                                      // ■重要■ウィンドウプロシージャ名（アドレス）
	wc.cbClsExtra = 0;                                             // ウィンドウクラスの保持するデータサイズ
	wc.cbWndExtra = 0;                                             // ウィンドウの保持するデータサイズ
	wc.hInstance = abc;                                          // ■重要■インスタンスハンドル
	wc.hIcon = NULL;                                               // アイコン
	wc.hCursor = LoadCursor(NULL, IDC_NO);                         // カーソル（マウスポインタ）
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);         // ウィンドウの背景色
//	wc.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(250, 0, 0));        // RGBでウィンドウの背景色を変える
	wc.lpszMenuName = (LPSTR)NULL;                                 // メニュー
	wc.lpszClassName = "vc01class";                                     // ■重要■ウィンドウクラスの名前
	RegisterClass(&wc);                                            // 登録する

	// ②ウィンドウクラスに従いウィンドウを（メモリ上に）作る
	hWnd = CreateWindow("vc01class", "WinMainの学習", WS_OVERLAPPEDWINDOW, -10.0f, 0.0f, 1280, 720, NULL, NULL, abc, NULL);

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
LRESULT CALLBACK xyz(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:                                                  // ウィンドウが破棄された
		PostQuitMessage(0);
		break;
	default:                                                          // それ以外の（何の処理もしない）メッセージのとき
		return DefWindowProc(hWnd, uMsg, wParam, lParam);             // OSに処理をまかせる
	}

	return 0;
}