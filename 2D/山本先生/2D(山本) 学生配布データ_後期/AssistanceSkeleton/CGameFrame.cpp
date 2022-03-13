/*
  ゲーム本体に関連した関数のみのcppファイル
  基本的にここを改良していく
*/

#include "CGameFrame.h"

// テクスチャ読み込み
void CGameFrame::LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const std::string & aFilePath, int aWidth, int aHeight, D3DCOLOR aColor)
{
	if (aWidth == 0)aWidth = D3DX_DEFAULT;
	if (aHeight == 0)aHeight = D3DX_DEFAULT;
	D3DXCreateTextureFromFileEx(mpD3DDevice, aFilePath.c_str(), aWidth, aHeight, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, aColor, NULL, NULL, lpTex);
}

/*
  コンストラクタ
  クラスのインスタンス(==実体)を生成(メモリ確保)時に呼ばれる
*/
CGameFrame::CGameFrame()
	// 初期化子リスト
	// コンストラクタ()の続きに[:]コロンを
	// つけることで記述できる、区切りは[,]カンマ
	// 変数宣言と同じ順番で書かないとバグの原因になる
	: mScrollWidth(1280)
	, mScrollHeight(720)
	, mAppName("Skeleton")
	, mIsFullScreen(false)
	, mFvFVertex(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	, mpHwnd(nullptr)
	, mpD3D(nullptr)
	, mpD3DDevice(nullptr)
	, mD3Dpp()
	, mWndClass()
	, mpSprite(nullptr)
	, mpFont(nullptr)
{
}

/*
  デストラクタ
  クラスのインスタンス(==実体)を破棄(メモリ開放)時に呼ばれる
*/
CGameFrame::~CGameFrame()
{
	Release();
}

// 終了処理
void CGameFrame::Release()
{
	mpSprite->Release();
	mpFont->Release();
	mpD3D->Release();
	mpD3DDevice->Release();
}

// 初期化
void CGameFrame::Initialize()
{
}

// 更新
void CGameFrame::Update()
{
}

// 2D描画
void CGameFrame::Draw2D()
{
	// 描画開始
	mpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// 描画終了
	mpSprite->End();
}

// 3D描画
void CGameFrame::Draw3D()
{
}
