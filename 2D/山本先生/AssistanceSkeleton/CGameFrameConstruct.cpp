/*
  DirextX9や画面の生成などを行っている関数を隔離したcppファイル
  基本的に見なくてよい。気になる人は見てみるのもアリ
*/

#include "CGameFrame.h"
#include <cassert>

LRESULT _stdcall WindowFunc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		return 0;

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CGameFrame::Create(HINSTANCE aHInst, const int aCmdShow)
{
	mWndClass = CreateWNDCLASS(aHInst);
	mpHwnd = CreateHWND(aHInst, aCmdShow);
	assert(mpHwnd != nullptr);
	CreateDirectX9();

	// スプライト作成
	D3DXCreateSprite(mpD3DDevice, &mpSprite);
	mpSprite->OnResetDevice();

	// フォント作成
	D3DXCreateFont(mpD3DDevice, 30, 30, FW_REGULAR, NULL, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "ＭＳ ゴシック", &mpFont);
	mpFont->OnResetDevice();
}

HWND CGameFrame::CreateHWND(HINSTANCE aHInst, const int aCmdShow)
{
	HWND result;
	result = CreateWindowEx(
		0,
		mAppName.c_str(),
		mAppName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		mScrollWidth, mScrollHeight,
		NULL, NULL, aHInst,
		NULL);

	if (!result)return nullptr;

	ShowWindow(result, aCmdShow);
	UpdateWindow(result);
	SetFocus(result);

	if (mIsFullScreen) {
		ShowCursor(FALSE);
	}
	else {
		RECT rc = { 0,0,mScrollWidth,mScrollHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		SetWindowPos(result, NULL, 50, 50, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_NOZORDER);
	}

	return result;
}

WNDCLASS CGameFrame::CreateWNDCLASS(HINSTANCE aHInst)
{
	WNDCLASS result;
	result.style = CS_DBLCLKS;
	result.lpfnWndProc = WindowFunc;
	result.cbClsExtra = 0;
	result.cbWndExtra = 0;
	result.hInstance = aHInst;
	result.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	result.hCursor = LoadCursor(NULL, IDC_ARROW);
	result.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	result.lpszMenuName = NULL;
	result.lpszClassName = mAppName.c_str();

	RegisterClass(&result);
	return result;
}

void CGameFrame::CreateDirectX9()
{
	// Direct3D オブジェクトを作成
	mpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (mpD3D == nullptr)
	{
		// オブジェクト作成失敗
		MessageBox(NULL, "Direct3D の作成に失敗しました。", "ERROR", MB_OK | MB_ICONSTOP);
		return;
	}
	int adapter;

	// 使用するアダプタ番号
	adapter = D3DADAPTER_DEFAULT;

	// ウインドウの作成が完了したので、Direct3D を初期化する
	ZeroMemory(&mD3Dpp, sizeof(D3DPRESENT_PARAMETERS));
	// Direct3D 初期化パラメータの設定
	if (mIsFullScreen)
	{
		// フルスクリーン : ほとんどのアダプタでサポートされているフォーマットを使用
		mD3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		// ウインドウ : 現在の画面モードを使用
		D3DDISPLAYMODE disp;
		// 現在の画面モードを取得
		mpD3D->GetAdapterDisplayMode(adapter, &disp);
		mD3Dpp.BackBufferFormat = disp.Format;
	}
	// 表示領域サイズの設定
	mD3Dpp.BackBufferWidth = mScrollWidth;
	mD3Dpp.BackBufferHeight = mScrollHeight;
	mD3Dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if (mIsFullScreen == false)
	{
		// ウインドウモード
		mD3Dpp.Windowed = 1;
	}

	// Z バッファの自動作成
	mD3Dpp.EnableAutoDepthStencil = 1;
	mD3Dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//ﾊﾞｯｸﾊﾞｯﾌｧをﾛｯｸ可能にする(GetDCも可能になる)
	mD3Dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// デバイスの作成 - T&L HAL
	if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, mpHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
	{
		// 失敗したので HAL で試行
		if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, mpHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
		{
			// 失敗したので REF で試行
			if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_REF, mpHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
			{
				// 結局失敗した
				MessageBox(NULL, "DirectX9が初期化できません。\n未対応のパソコンと思われます。", "ERROR", MB_OK | MB_ICONSTOP);
				mpD3D->Release();
				// 終了する
				return;
			}
		}
	}

	// レンダリング・ステートを設定
	// Z バッファ有効化->前後関係の計算を正確にしてくれる
	mpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	mpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファブレンディング有効化
	mpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// アルファブレンディング方法を設定
	mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// レンダリング時のアルファ値の計算方法の設定
	mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// テクスチャの色を使用
	mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	// 頂点の色を使用
	mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	// レンダリング時の色の計算方法の設定
	mpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//裏面カリング
	mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// フィルタ設定
	mpD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	mpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	mpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// ライト
	mpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	mpD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// 頂点フォーマットの設定
	mpD3DDevice->SetFVF(mFvFVertex);
}

void CGameFrame::GameLoop()
{
	// バックバッファと Z バッファをクリア
	mpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	Update();

	// 描画開始
	mpD3DDevice->BeginScene();

	D3DXMATRIX mView, mProj;

	// 視点行列の設定
	D3DXMatrixLookAtLH(&mView,
		&D3DXVECTOR3(0.0f, 0.0f, -10.0f),	// カメラの位置
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// カメラの視点
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)	// カメラの頭の方向
	);

	// 投影行列の設定
	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(60), (float)mScrollWidth / (float)mScrollHeight, 1.0f, 1000.0f);

	//行列設定
	mpD3DDevice->SetTransform(D3DTS_VIEW, &mView);
	mpD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);

	Draw();

	// 描画終了
	mpD3DDevice->EndScene();

	// バックバッファをプライマリバッファにコピー
	mpD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void CGameFrame::Draw()
{
	Draw3D();
	Draw2D();
}