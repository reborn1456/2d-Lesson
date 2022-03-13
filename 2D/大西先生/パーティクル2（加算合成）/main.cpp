#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#define	SCRW		1280	// ウィンドウ幅（Width
#define	SCRH		720		// ウィンドウ高さ（Height











////関数プロトタイプ宣言
void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color);
void Update(void);
void CreateConsole();





////構造体宣言

// 頂点フォーマットの定義
struct VERTEX
{
	D3DXVECTOR3 Pos;
	D3DCOLOR Color;
	D3DXVECTOR2 Tex;
};

// 頂点用の構造体を宣言
typedef struct VERTEX2D
{
	float x;			// x座標
	float y;			// y座標
	float z;			// 常に0
	float rhw;			// 常に1
	D3DCOLOR color;		// 色
}Vertex2D;

// パーティクル用データ構造体
struct PARTICLE
{
	float  mPosX;          // 座標
	float  mPosY;          // 座標
	float  mMoveX;         // 移動量
	float  mMoveY;         // 移動量
	float        mSize;         // サイズ
	int          mLifespan;     // 要綱機関（活動限界）
	D3DXMATRIX   mMatrix;       // 行列
};




//class PARTICLE
//{
//public:
//	void EmitParticle(PARTICLE *p, D3DXVECTOR3 aPos, D3DXVECTOR3 aMove, float aSize, int aLifespan);
//	void UpdateParticle(PARTICLE *p);
//	void DrawParticle(PARTICLE *p);
//
//private:
//	D3DXVECTOR3  mPos;          // 座標
//	D3DXVECTOR3  mMove;         // 移動量
//	float        mSize;         // サイズ
//	int          mLifespan;     // 要綱機関（活動限界）
//	D3DXMATRIX   mMatrix;       // 行列
//};




////  定数宣言



//// グローバル変数宣言, default as 0 when not specified

LPDIRECT3D9				lpD3D;		// Direct3Dインターフェイス

LPDIRECT3DDEVICE9		lpD3DDevice;	// Direct3DDeviceインターフェイス

D3DPRESENT_PARAMETERS d3dpp;

LPD3DXSPRITE lpSprite;	// スプライト
LPD3DXFONT lpFont;		// フォント

bool gameFullScreen;	// フルスクリーン（true,false)

int frame;   //variable to count the number of frames

LPDIRECT3DTEXTURE9 playerTex;  //画像を入れる入れ物
D3DXMATRIX playerMat;  //行列(座標)
float playerX;
float playerY;

// パーティクル用変数宣言
const int           particleNum = 100;     // 粒子の個数
PARTICLE            smoke[particleNum];   // 煙用
LPDIRECT3DTEXTURE9  particleTex;          // テクスチャ






#define	FVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 0~1以下のランダム
#define RND (rand()/(double)(RAND_MAX))


//void PARTICLE::EmitParticle(PARTICLE *p, D3DXVECTOR3 aPos, D3DXVECTOR3 aMove, float aSize, int aLifespan)
//{
//	p->mPos.x = aPos.x;
//	p->mPos.y = aPos.y;
//	p->mMove.x = aMove.x;
//	p->mMove.y = aMove.y;
//	p->mSize = aSize;
//	p->mLifespan = aLifespan;
//}
//
//void PARTICLE::UpdateParticle(PARTICLE *p)
//{
//	D3DXMATRIX matPos;   // 移動行列
//	D3DXMATRIX matSize;   //拡大縮小行列
////	D3DXMatrixScaling();
//	D3DXMatrixTranslation(&matPos, p->mPos.x, p->mPos.y, p->mPos.z);
//	p->mMatrix = matPos;
//}
//
//void PARTICLE::DrawParticle(PARTICLE *p)
//{
//	//表示範囲
//	RECT rcParticle = { 0, 0, 32, 32 };
//	//移動のMatrix, 座標を行列にセット
//	//行列をセット
//	lpSprite->SetTransform(&p->mMatrix);
//	//表示
//	lpSprite->Draw(particleTex,   //何を表示しますか
//		&rcParticle,   //表示範囲を指定
//		&D3DXVECTOR3(16.0f, 16.0f, 0.0f),   //中心点
//		NULL,   //注視点
//		D3DCOLOR_ARGB(255, 255, 255, 255));   //表示させる色
//}


void EmitParticle(PARTICLE *p, float aPosX, float aPosY, float aMoveX, float aMoveY, float aSize, int aLifespan)
{
	p->mPosX = aPosX;
	p->mPosY = aPosY;
	p->mMoveX = aMoveX;
	p->mMoveY = aMoveY;
	p->mSize = aSize;
	p->mLifespan = aLifespan;
}

void UpdateParticle(PARTICLE *p)
{
	// 座標更新
	p->mPosX += p->mMoveX;
	p->mPosY += p->mMoveY;

	// サイズ変動
	p->mSize *= .99f;

	// 活動限界
	p->mLifespan--;
	if (p->mLifespan <= 0)
	{
		EmitParticle(p, rand()%1280, rand()%700, RND*2-1, RND*1-10, RND*2, RND*90+10);
	}

	// 行列作成
	D3DXMATRIX matPos;   // 移動行列
	D3DXMATRIX matSize;   //拡大縮小行列
	D3DXMatrixScaling(&matSize, p->mSize, p->mSize, 0.f);
	D3DXMatrixTranslation(&matPos, p->mPosX, p->mPosY, 0.f);
	p->mMatrix = matSize * matPos;
}

void DrawParticle(PARTICLE *p)
{
	//表示範囲
	RECT rcParticle = { 0, 0, 32, 32 };
	//移動のMatrix, 座標を行列にセット
	//行列をセット
	lpSprite->SetTransform(&p->mMatrix);
	//表示
	lpSprite->Draw(particleTex,   //何を表示しますか
		&rcParticle,   //表示範囲を指定
		&D3DXVECTOR3(16.0f, 16.0f, 0.0f),   //中心点
		NULL,   //注視点
		D3DCOLOR_ARGB(128, rand() % 265, rand() % 265, rand()%265));   //表示させる色
}




//初期化関数　Function to format the game
void Initialise(void)
{
	/*//画像読み込み
	LoadTexture(&playerTex,    //何処に読み込みますか
		"player.png",   //何を読み込みますか
		256,   //画像の幅
		64,   //画像の高さ
		D3DCOLOR_XRGB(0, 0, 0));   //読み込みたくない色, change the rgb here to not show "a" specific colour from the sprite */

	////コンソールの作成
	//CreateConsole();

	//frame = 0;	//start counting the frames from 0

	srand(time(0));

	LoadTexture(&playerTex, "player.png", 256, 64, D3DCOLOR_XRGB(0, 0, 0));
	LoadTexture(&particleTex, "star.png", 32, 32, D3DCOLOR_XRGB(0, 0, 0));
	playerX = 300.f;
	playerY = 200.f;

	for (int i = 0; i < particleNum; i++)
	{
		EmitParticle(&smoke[i], 
			playerX, playerY + 30,      // 座標
			RND * 2-1, RND * 1 -10,   // 移動量
			RND * 2,                 // サイズ
			RND*90+10);						// 活動限界
	}


}

//終了関数　Function after ending the program
void Terminate(void)
{
	//////画像解放
	playerTex->Release(); 
	particleTex->Release(); 




	lpSprite->Release();	// スプライト
	lpFont->Release();		// フォント

	////コンソールの解放
	//FreeConsole();
}

//角度算出, Degree
float GetDegreeAngle(float x, float y, float tx, float ty)
{
	float fx = tx - x;
	float fy = ty - y;

	float rad = atan2f(fy, fx)/* arc tangent with 2 points giving a float answer */;

	float deg = D3DXToDegree(rad);

	if (deg < 0)
	{
		deg += 360.0f;
	}

	return deg;
}

//角度算出, Radian
float GetRadianAngle(float x, float y, float tx, float ty)
{
	float fx = tx - x;
	float fy = ty - y;

	float rad = atan2f(fy, fx)/* arc tangent with 2 points giving a float answer */;

	return rad;
}

//直線距離算出
float GetDistance(float x, float y, float tx, float ty)
{
	float fx = tx - x;
	float fy = ty - y;

	//斜辺
	float dist = sqrt((fx * fx) + (fy * fy));

	return dist;
}

// 更新処理
void Update(void)
{
	////最終的な行列　＝　拡大　＊　移動     the multiplication has to be in this order or else there will be a bug
	//playerMat = playerScaleMat * playerTransMat;
	////拡大のMatrix
	//D3DXMatrixScaling(&playerScaleMat, -1.0f /* Xの拡大 */, 1.0f /* Yの拡大 */, 0.0f /* Zの拡大 */);

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		playerX += 10;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		playerX -= 10;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		playerY -= 10;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		playerY += 10;
	}
	D3DXMatrixTranslation(&playerMat, playerX, playerY, 0.f);

	for (int i = 0; i < particleNum; i++)
	{
		// パーティクル更新
		UpdateParticle(&smoke[i]);
	}

}

// 3D描画
void Render3D(void)
{

}

// 2D描画
void Render2D(void)
{
	//////////////////////////////////////////////////
	///	スプライトの描画処理
	//////////////////////////////////////////////////
	// 描画開始
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//表示範囲
	RECT rcPlayer = { 0, 0, 64, 64 };  
/*	//移動のMatrix, 座標を行列にセット
	D3DXMatrixTranslation(&playerMat,   //どの行列にセットしますか
							playerX,   // X座標の移動
							playerY,   // Y座標の移動
							0.0f);   // Z座標の移動(2Dゲームは0固定) */
	//行列をセット
	lpSprite->SetTransform(&playerMat);
	//表示
	lpSprite->Draw(playerTex,   //何を表示しますか
					&rcPlayer,   //表示範囲を指定
					&D3DXVECTOR3(32.0f, 32.0f, 0.0f),   //中心点
					NULL,   //注視点
					D3DCOLOR_ARGB(255, 255, 255, 255));   //表示させる色


	//// 加算合成
	//lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 加算合成（描画元半透明）
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < particleNum; i++)
	{
		// パーティクル描画
		DrawParticle(&smoke[i]);
	}

	lpSprite->Flush();

	// 半透明（元の設定に戻しておく）
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);




	// 描画終了
	lpSprite->End();



	//文字表示はここから



}

void GameFrame(void)
{
	// バックバッファと Z バッファをクリア
	//change the rgb here to change the background color
	lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);


	// 更新処理
	Update();


	// 描画開始
	lpD3DDevice->BeginScene();

	D3DXMATRIX mView, mProj;

	// 視点行列の設定
	D3DXMatrixLookAtLH(&mView,
		&D3DXVECTOR3(0.0f, 0.0f, -10.0f),	// カメラの位置
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// カメラの視点
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)	// カメラの頭の方向
	);

	// 投影行列の設定
	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(60), (float)SCRW / (float)SCRH, 1.0f, 1000.0f);

	//行列設定
	lpD3DDevice->SetTransform(D3DTS_VIEW, &mView);
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);



	// 3D描画
	Render3D();

	// 2D描画
	Render2D();



	// 描画終了
	lpD3DDevice->EndScene();

	// バックバッファをプライマリバッファにコピー
	lpD3DDevice->Present(NULL, NULL, NULL, NULL);

	//frame++;	//+1 every frame
}

LRESULT APIENTRY WndFunc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color)
{
	if (W == 0)W = D3DX_DEFAULT;
	if (H == 0)H = D3DX_DEFAULT;
	D3DXCreateTextureFromFileEx(lpD3DDevice, fname, W, H, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, Color, NULL, NULL, lpTex);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev,
	LPSTR lpszCmdParam, int nCmdshow)
{
	//---------------------Window作成関連-----------------------

	MSG msg;

	HWND hwnd;
	WNDCLASS wc;
	char szAppName[] = "Generic Game SDK Window";

	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WndFunc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		szAppName,
		"Direct X",		//this is the title shown on the window bar
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCRW, SCRH,
		NULL, NULL, hInst,
		NULL);

	if (!hwnd)return FALSE;

	ShowWindow(hwnd, nCmdshow);
	UpdateWindow(hwnd);
	SetFocus(hwnd);

	gameFullScreen = false;	// ウィンドウモード

	if (gameFullScreen) {
		ShowCursor(FALSE);
	}
	else {
		RECT rc = { 0,0,SCRW,SCRH };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		SetWindowPos(hwnd, NULL, 50, 50, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_NOZORDER);
	}

	//---------------------DirectX Graphics関連-----------------------

	// Direct3D オブジェクトを作成
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		// オブジェクト作成失敗
		MessageBox(NULL, "Direct3D の作成に失敗しました。", "ERROR", MB_OK | MB_ICONSTOP);
		// 終了する
		return 0;
	}
	int adapter;

	// 使用するアダプタ番号
	adapter = D3DADAPTER_DEFAULT;

	// ウインドウの作成が完了したので、Direct3D を初期化する
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	// Direct3D 初期化パラメータの設定
	if (gameFullScreen)
	{
		// フルスクリーン : ほとんどのアダプタでサポートされているフォーマットを使用
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		// ウインドウ : 現在の画面モードを使用
		D3DDISPLAYMODE disp;
		// 現在の画面モードを取得
		lpD3D->GetAdapterDisplayMode(adapter, &disp);
		d3dpp.BackBufferFormat = disp.Format;
	}
	// 表示領域サイズの設定
	d3dpp.BackBufferWidth = SCRW;
	d3dpp.BackBufferHeight = SCRH;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if (!gameFullScreen)
	{
		// ウインドウモード
		d3dpp.Windowed = 1;
	}

	// Z バッファの自動作成
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//ﾊﾞｯｸﾊﾞｯﾌｧをﾛｯｸ可能にする(GetDCも可能になる)
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// デバイスの作成 - T&L HAL
	if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
	{
		// 失敗したので HAL で試行
		if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
		{
			// 失敗したので REF で試行
			if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_REF, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
			{
				// 結局失敗した
				MessageBox(NULL, "DirectX9が初期化できません。\n未対応のパソコンと思われます。", "ERROR", MB_OK | MB_ICONSTOP);
				lpD3D->Release();
				// 終了する
				return 0;
			}
		}
	}

	// レンダリング・ステートを設定
	// Z バッファ有効化->前後関係の計算を正確にしてくれる
	lpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファブレンディング有効化
	lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// アルファブレンディング方法を設定
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// レンダリング時のアルファ値の計算方法の設定
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// テクスチャの色を使用
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	// 頂点の色を使用
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	// レンダリング時の色の計算方法の設定
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//裏面カリング
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// フィルタ設定
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// ライト
	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	lpD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// 頂点フォーマットの設定
	lpD3DDevice->SetFVF(FVF_VERTEX);

	timeBeginPeriod(1);

	// ゲームに関する初期化処理 ---------------------------


	//Init();   is the short form of initialise
	Initialise();





	// スプライト作成
	D3DXCreateSprite(lpD3DDevice, &lpSprite);
	lpSprite->OnResetDevice();

	// フォント作成
	D3DXCreateFont(lpD3DDevice, 30/* 高さ */, 30/* 幅 */, FW_REGULAR, NULL, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "ＭＳ ゴシック", &lpFont);

	lpFont->OnResetDevice();

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			GameFrame();
		}
	}

	// ゲームに関する終了処理 ---------------------------

	//End();  is a easier word for terminate
	Terminate();


	timeEndPeriod(1);

	// Direct3D オブジェクトを解放
	lpD3DDevice->Release();
	lpD3D->Release();

	return (int)msg.wParam;
}

//コンソール作成
void CreateConsole()
{
	//コンソールを作成する
	AllocConsole();

	FILE *fp_c = NULL;

	freopen_s(&fp_c, "CONOUT$", "w", stdout);
	freopen_s(&fp_c, "CONIN$", "r", stdin);
}