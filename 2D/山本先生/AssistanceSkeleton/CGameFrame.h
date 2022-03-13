#include <windows.h>
#include <d3dx9.h>
#include <string>

/* GameFrameに持たせたい自作クラス等のヘッダーをインクルードする */
/* ここから===================================================== */
#include "CPlayer.h"
#include "CEnemy.h"
/* ここまで===================================================== */

/*
　GameFrameクラス
  DirectX9の初期化、Windowの作成などゲームループに必要な処理諸々をまとめたクラス
  ※ 本来1つのクラスに様々な機能を詰め込むのは設計ミスです
*/
class CGameFrame
{

public: // 以下の変数、関数は外部からアクセスできる

	CGameFrame();		// コンストラクタ
	~CGameFrame();		// デストラクタ

	void Create(HINSTANCE aHInst,const int aCmdShow);
	void Initialize();	// 初期化処理
	void Release();		// 終了処理
	void GameLoop();	// ゲームループ

	CEnemy* GetEnemy();


private: // 以下の変数、関数は外部からアクセスできない

	// 定数=================================================
	// 定数は全てコンストラクタで設定する必要がある
	const int			mScrollWidth;				// 画面横幅
	const int			mScrollHeight;				// 画面縦幅
	const std::string	mAppName;					// アプリケーション(ウィンドウ)名
	const bool			mIsFullScreen;				// フルスクリーンにするかどうか
	const int			mFvFVertex;					// ポリゴンの頂点情報

	// 変数=================================================
	HWND					mpHwnd;
	LPDIRECT3D9				mpD3D;
	LPDIRECT3DDEVICE9		mpD3DDevice;
	D3DPRESENT_PARAMETERS	mD3Dpp;
	WNDCLASS				mWndClass;
	LPD3DXSPRITE			mpSprite;				// スプライト
	LPD3DXFONT				mpFont;					// フォント

	CPlayer                 mPlayer;
	CEnemy                  mEnemy;

	// 関数=================================================
	void Update();									// 更新関数
	void Draw();									// 描画
	void Draw2D();									// 2D描画
	void Draw3D();									// 3D描画

	void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const std::string & aFilePath, int aWidth, int aHeight, D3DCOLOR aColor);
	HWND CreateHWND(HINSTANCE aHInst, const int aCmdShow);
	WNDCLASS CreateWNDCLASS(HINSTANCE aHInst);
	void CreateDirectX9();
};
