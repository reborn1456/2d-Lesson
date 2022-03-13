#include<windows.h>
#include<stdio.h>
#include<d3dx9.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#pragma warning (disable: 4996)

#define	SCRW		1280	// ウィンドウ幅（Width
#define	SCRH		768		// ウィンドウ高さ（Height

LPDIRECT3D9				lpD3D;		// Direct3Dインターフェイス

LPDIRECT3DDEVICE9		lpD3DDevice;	// Direct3DDeviceインターフェイス

D3DPRESENT_PARAMETERS d3dpp;


// 頂点フォーマットの定義
struct VERTEX
{
	D3DXVECTOR3 Pos;
	D3DCOLOR Color;
	D3DXVECTOR2 Tex;
};

//エネミー状態
enum eEnemyState
{
	eNone, eRightMove, eLeftMove
};

typedef struct tENEMY
{
	D3DXMATRIX mMatrix;   //最終行列用
	D3DXMATRIX mEnemyTransMat;   //移動行列用
	D3DXMATRIX mEnemyScaleMat;
	D3DXVECTOR2 mPosition;   //Float型の x と y がまとまった構造体
	eEnemyState mState;   //敵の状態
	const float mEnemyMoveSpeed = 2;
	float mEnemyMoveMax;
	float mEnemyScaleX;
	bool mFlg;   //生存フラグ
}tEnemy;

// 頂点用の構造体を宣言
typedef struct VERTEX2D
{
	float x;			// x座標
	float y;			// y座標
	float z;			// 常に0
	float rhw;			// 常に1
	D3DCOLOR color;		// 色
}Vertex2D;



//関数プロトタイプ宣言
void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color);
void CreateConsole();
void LoadMapFile();
void EnemyInitialise();
void EnemyUpdate();
void EnemyRender2D();


////  グローバル変数宣言

LPD3DXSPRITE lpSprite;	// スプライト
LPD3DXFONT lpFont;		// フォント

//Player
LPDIRECT3DTEXTURE9 playerTex;
D3DXMATRIX playerMat;
D3DXMATRIX playerTransMat;
D3DXMATRIX playerScaleMat;
float playerX;
float playerY;
float playerMoveX;
float playerScaleX;
float playerMoveY;
bool playerFlg;

bool playerJumpFlg;   //true(1)   false(0)
/* if (Flg == true){～}   same as   if (Flg){～}
   if (Flg == false){～}   same as   if (!Flg){～} */

const int mapChipHeightSize = 12;
const int mapChipWidthSize = 32;
int mapChipList[mapChipHeightSize][mapChipWidthSize];
LPDIRECT3DTEXTURE9 mapChipTex;
D3DXMATRIX mapChipMat[mapChipHeightSize][mapChipWidthSize];
float mapChipX[mapChipHeightSize][mapChipWidthSize];
float mapChipY[mapChipHeightSize][mapChipWidthSize];

char mapStage;


tEnemy enemy;

float CameraX;
float CameraY;

const float scrollWidth = 250.0f;
const float leftLine = scrollWidth;
const float rightLine = SCRW - scrollWidth;



bool gameFullScreen;	// フルスクリーン（true,false)





#define	FVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)


void DrawLine(float x1, float y1, float x2, float y2, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x1, y1, 0, 1, colour }, { x2, y2, 0, 1, colour } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST /* primitive type, this draws a line between points */, 1 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//Function to format the game
void Initialise(void)
{
	//CreateConsole();


	LoadTexture(&playerTex, "Chara.png", 64, 64, NULL);
	LoadTexture(&mapChipTex, "MapChip.png", 320, 320, NULL);
	

	playerX = 300;
	playerY = 100;

	playerMoveX = 0.0f;
	playerMoveY = 0.0f;

	playerScaleX = 1.0f;

	CameraX = 0.0f;
	CameraY = 0.0f;

	playerFlg = true;

	for (int i = 0; i < mapChipHeightSize; i++)
	{
		for (int j = 0; j < mapChipWidthSize; j++)
		{
			mapChipX[i][j] = j * 64.0f;
			mapChipY[i][j] = i * 64.0f;
		}
	}

	LoadMapFile();

	EnemyInitialise();
}

void Terminate(void)
{
	playerTex->Release();
	mapChipTex->Release();

	//FreeConsole();
}

void EnemyInitialise()
{
	enemy.mPosition.x = 200.0f;
	enemy.mPosition.y = 600.0f;
	enemy.mFlg = true;
	enemy.mState = eEnemyState::eRightMove;   //最初は右移動
	enemy.mEnemyScaleX = 1.0f;
//	enemy.mEnemyMoveSpeed = 2;
}

void EnemyRightMove()
{
	enemy.mEnemyMoveMax = 400;

	if (enemy.mPosition.x < enemy.mEnemyMoveMax)
	{
		enemy.mPosition.x += enemy.mEnemyMoveSpeed;
		enemy.mEnemyScaleX = 1.0f;
	}
	else
	{
		enemy.mState = eEnemyState::eLeftMove;
	}
}

void EnemyLeftMove()
{
	enemy.mEnemyMoveMax = 92;

	bool result = enemy.mPosition.x > enemy.mEnemyMoveMax;
	if (result)
	{
		enemy.mPosition.x -= enemy.mEnemyMoveSpeed;
		enemy.mEnemyScaleX = -1.0f;
	}
	else
	{
		enemy.mState = eEnemyState::eRightMove;
	}
}

void EnemyUpdate()
{
	switch (enemy.mState)
	{
	case eEnemyState::eNone:
		break;
	case eEnemyState::eRightMove:
		EnemyRightMove();
		break;
	case eEnemyState::eLeftMove:
		EnemyLeftMove();
		break;
	default:
		break;
	}


	D3DXMatrixScaling(&enemy.mEnemyScaleMat, enemy.mEnemyScaleX/* Xの拡大 */, 1.0f/* Yの拡大 */, 0.0f/* Zの拡大 */);
	D3DXMatrixTranslation(&enemy.mEnemyTransMat, enemy.mPosition.x, enemy.mPosition.y, 0.0f);

	enemy.mMatrix = enemy.mEnemyScaleMat * enemy.mEnemyTransMat;
}

void EnemyRender2D()
{
	RECT rcEnemy = { 0, 0, 64, 64 };
	if (enemy.mFlg == true)
	{
		lpSprite->SetTransform(&enemy.mMatrix);
		lpSprite->Draw(playerTex, &rcEnemy, &D3DXVECTOR3(32.0f, 32.0f, 0.0f), NULL, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
}

void PlayerAndEnemyHit()
{
	/*  const RECT rc = { 左辺(.left), 上辺(.top), 右辺(.right), 下辺(.bottom) };  */
	const RECT playerRect = { playerX - 32, playerY - 32, playerX + 32, playerY + 32 };
	const RECT nextPlayerRect = { playerRect.left + playerMoveX, playerRect.top + playerMoveY, playerRect.right + playerMoveX, playerRect.bottom + playerMoveY };
	const RECT enemyRect = { enemy.mPosition.x - 32, enemy.mPosition.y - 32, enemy.mPosition.x + 32, enemy.mPosition.y + 32 };

	//自機と敵が生存していたら判定
	if (playerFlg && enemy.mFlg)
	{
		//左右判定
		if (playerRect.left < enemyRect.right && playerRect.right > enemyRect.left)
		{
			//上判定
			if (nextPlayerRect.bottom > enemyRect.top && nextPlayerRect.top < enemyRect.top)
			{
				enemy.mFlg = false;
				if (playerJumpFlg == false || playerJumpFlg)
				{
					playerMoveY = -13.0f;
					playerJumpFlg = true;
				}
				return;
			}
			//下判定
			if (nextPlayerRect.top < enemyRect.bottom && nextPlayerRect.bottom > enemyRect.bottom)
			{
				playerFlg = false;
				return;
			}
		}

		//上下判定
		if (playerRect.bottom > enemyRect.top && playerRect.top < enemyRect.bottom)
		{
			//左判定
			if (nextPlayerRect.right > enemyRect.left && nextPlayerRect.left < enemyRect.left)
			{
				playerFlg = false;
				return;
			}
			//右判定
			if (nextPlayerRect.left < enemyRect.right && nextPlayerRect.right > enemyRect.right)
			{
				playerFlg = false;
				return;
			}
		}
	}
}

// 更新処理
void Update(void)
{

	playerMoveX = 0;   //playerMoveX goes back to 0 every frame, so the player won't keep on moving after pressing once
	playerMoveY += 1.0f;  //this gives it a +1 of gravity
	////playerX movement
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	//	playerMoveX += 5.0f;   //don't fidget with the playerX directly
	//	playerScaleX = 1.0f;
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	playerMoveX -= 5.0f;
	//	playerScaleX = -1.0f;
	//}
	////playerY movement
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	playerMoveY += 5.0f;   //don't fidget with the playerX directly
	//}
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//{
	//	if (playerJumpFlg == false)
	//	{
	//		playerMoveY = -20.0f;
	//		playerJumpFlg = true;
	//	}
	//}

	
	//自機固定スクロール、プレイヤーではなくマップチップを移動
	CameraX = 0.0f;   //移動量
	CameraY = 0.0f;   //移動量
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (playerX + 32.f + 5.f > rightLine)
		{
			CameraX -= 5.0f;
		}
		else
		{
			playerMoveX += 5.0f;   //don't fidget with the playerX directly
		}
		playerScaleX = 1.0f;
	}


	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (playerX - 32.f - 5.f < leftLine)
		{
			CameraX += 5.0f;
		}
		else
		{
			playerMoveX -= 5.0f;
		}
		playerScaleX = -1.0f;
	}


	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		playerMoveY += 5.0f;   //don't fidget with the playerX directly
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		if (playerJumpFlg == false)
		{
			playerMoveY = -20.0f;
			playerJumpFlg = true;
		}
	}





	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		Initialise();
	}



	PlayerAndEnemyHit();

	//the x and y of player in the next frame, future x and y
	float nextPlayerX = playerX + playerMoveX;
	float nextPlayerY = playerY + playerMoveY;

	for (int i = 0; i < mapChipHeightSize; i++)
	{
		for (int j = 0; j < mapChipWidthSize; j++)
		{
			if (mapChipList[i][j] != 0)
			{
				continue;
			}

			//short form of the x and y of the mapChip
			const float chipX = mapChipX[i][j]/* + CameraX */;
			const float chipY = mapChipY[i][j]/* + CameraY */;

			//player's 下 and block's 上
			if (playerY + 32 > chipY && playerY - 32 < chipY + 64)
			{
				//blockの左からの判定
				if (nextPlayerX + 32 > chipX && nextPlayerX - 32 < chipX)
				{
					playerX = chipX - 32;
					playerMoveX = 0;
				}
				//blockの右からの判定
				else if (nextPlayerX - 32 < chipX + 64 && nextPlayerX + 32 > chipX + 64)
				{
					playerX = chipX + 64 + 32;
					playerMoveX = 0;
				}
			}
			////player's 上 and block's 下
			//else if (playerY - 32 < chipY + 64 && playerY + 32 > chipY)
			//{
			//	//左からの判定
			//	if (nextPlayerX - 32 < chipX + 64 && nextPlayerX + 32 > chipX + 64)
			//	{
			//		playerX = chipX + 64 + 32;
			//		playerMoveX = 0;
			//	}
			//	//右からの判定
			//	else if (nextPlayerX + 32 > chipX && nextPlayerX - 32 < chipX)
			//	{
			//		playerX = chipX + 32;
			//		playerMoveX = 0;
			//	}
			//}

			//player's 左 and block's 右
			if (playerX - 32 < chipX + 64 && playerX + 32 > chipX)
			{
				//blockの上からの判定
				if (nextPlayerY + 32 > chipY && nextPlayerY - 32 < chipY)
				{
					playerY = chipY - 32;
					playerMoveY = 0;
					playerJumpFlg = false;
				}
				//blockの下からの判定
				else if (nextPlayerY - 32 < chipY + 64 && nextPlayerY + 32 > chipY + 64)
				{
					playerY = chipY + 64 + 32;
					playerMoveY = 0;
				}
			}
			////player's 右 and block's 左
			//else if (playerX + 32 > chipX && playerX - 32 < chipX + 64)
			//{
			//	//上からの判定
			//	if (nextPlayerY + 32 > chipY && nextPlayerY - 32 < chipY)
			//	{
			//		playerY = chipY - 32;
			//		playerMoveY = 0;
			//		playerJumpFlg = false;
			//	}
			//	//下からの判定
			//	else if (nextPlayerY - 32 < chipY + 64 && nextPlayerY + 32 > chipY + 64)
			//	{
			//		playerY = chipY + 64 + 32;
			//		playerMoveY = 0;
			//	}
			//}

		}
	}



	playerX += playerMoveX;
	playerY += playerMoveY;

	for (int i = 0; i < mapChipHeightSize; i++)
	{
		for (int j = 0; j < mapChipWidthSize; j++)
		{
			mapChipX[i][j] += CameraX;
			mapChipY[i][j] += CameraY;
		}
	}




	//Changing the scale of the player here
	D3DXMatrixScaling(&playerScaleMat, playerScaleX/* Xの拡大 */, 1.0f/* Yの拡大 */, 0.0f/* Zの拡大 */);
	//Changing the translation of player, for the movement of player
	D3DXMatrixTranslation(&playerTransMat, playerX, playerY, 0.0f);

	//最終的な行列　＝　拡大　＊　移動     the multiplication has to be in this order or else there will be a bug
	playerMat = playerScaleMat * playerTransMat;

	//map translation
	for (int i = 0; i < mapChipHeightSize; i++)
	{
		for (int j = 0; j < mapChipWidthSize; j++)
		{
			D3DXMatrixTranslation(&mapChipMat[i][j], mapChipX[i][j]/* + CameraX */, mapChipY[i][j]/* + CameraY */, 0.0f);
		}
	}
	
	EnemyUpdate();
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

	//render map
	RECT rcMapChip;
	for (int i = 0; i < mapChipHeightSize; i++)
	{
		for (int j = 0; j < mapChipWidthSize; j++)
		{
			if (mapChipList[i][j] == 0)
			{
				rcMapChip = { 64, 0, 128, 64 };
			}
			if (mapChipList[i][j] == 1)
			{
				rcMapChip = { 0, 0, 64, 64 };
			}
			if (mapChipList[i][j] == 2)
			{
				rcMapChip = { 128, 0, 192, 64 };
			}
			lpSprite->SetTransform(&mapChipMat[i][j]);
			lpSprite->Draw(mapChipTex, &rcMapChip, &D3DXVECTOR3(0.0f, 0.0f, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	EnemyRender2D();

	//render player
	RECT rcPlayer = { 0, 0, 64, 64 };
	if (playerFlg == true)
	{	
		lpSprite->SetTransform(&playerMat);
		lpSprite->Draw(playerTex, &rcPlayer, &D3DXVECTOR3(32.0f, 32.0f, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}



	// 描画終了
	lpSprite->End();

	//文字表示はここから

	DrawLine(leftLine, 0, leftLine, SCRH, D3DCOLOR_ARGB(255, 255, 0, 0));
	DrawLine(rightLine, 0, rightLine, SCRH, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void GameFrame(void)
{
	// バックバッファと Z バッファをクリア
	//change the rgb here to change the background color
	lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);


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
		"Direct X",
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

	// LoadTexture(&playerTex, "dinoChrome.png", 2404, 130, D3DCOLOR_XRGB(255, 255, 255 /*change the rgb here to not show "a" specific colour from the sprite*/ ));


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

	lpSprite->Release();	// スプライト
	lpFont->Release();		// フォント


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

void LoadMapFile()
{
	FILE *fp;
	//to set the numbers in map.txt to mapChipList array
	if ((fp = fopen("Map.txt", "r")) != NULL)  //ファイルを開く
	{
		printf("OK \n");

		char ch;
		int i = 0, j = 0;

		//doesn't work
		//while ((ch = fgetc(fp)) != EOF)  //1文字読む
		//{
		//	for (int i = 0; i < 12; i++)
		//	{
		//		for (int j = 0; j < 33; j++)
		//		{
		//			if (ch != '\n')
		//			{
		//				printf("%d \n", ch);
		//				mapChipList[i][j] = ch - 0x30;
		//			}
		//		}
		//	}
		//}

		while ((ch = fgetc(fp)) != EOF)   //1文字読む
		{
			if (ch == '\n')
			{
				i++;
				j = 0;
			}
			else
			{
				/*printf("%d \n", ch);*/
				mapChipList[i][j] = ch - 0x30;
				j++;
			}
		}

		fclose(fp);
	}
	else
	{
		printf("NG \n");
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			printf("%d", mapChipList[i][j]);
		}
		printf("\n");
	}
}