#define _USE_MATH_DEFINES

#define	SCRW		1280	// ウィンドウ幅（Width
#define	SCRH		720		// ウィンドウ高さ（Height



#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <dsound.h>
#include "wavread.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")


////////////////////////////////////////////////// 列挙型 //////////////////////////////////////////////////

enum SceneName
{
	//default starting as 0 and then +1 for each after it
	/* when use:
	   int scene = SceneName::Game_PROC; */

	TITLE_PROC, GAME_PROC
};






////////////////////////////////////////////////// 関数プロトタイプ宣言 //////////////////////////////////////////////////

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color);
void Update(void);
void CreateConsole();
void Title_Init(void);
void Title_Update(void);
void Title_Render(void);
void Game_Init(void);
void Game_Update(void);
void Game_Render(void);






////////////////////////////////////////////////// クラス宣言, default as private when not specified //////////////////////////////////////////////////
class C_CHARACTER
{
public:
	LPDIRECT3DTEXTURE9 Tex;
	D3DXMATRIX Mat;
	float X;
	float Y;
	float MoveX;
	float MoveY;

	void Init(void);
};






////////////////////////////////////////////////// 構造体宣言 //////////////////////////////////////////////////

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

typedef struct T_CHARACTER
{
	LPDIRECT3DTEXTURE9 Tex;
	D3DXMATRIX Mat;
	float X;
	float Y;
	float MoveX;
	float MoveY;
}T_chara;






////////////////////////////////////////////////// 名前空間 (namespace) //////////////////////////////////////////////////







////////////////////////////////////////////////// グローバル変数宣言, default as 0 when not specified //////////////////////////////////////////////////

//音関係
LPDIRECTSOUND8 lpDSound;	//DirectSoundオブジェクト
LPDIRECTSOUNDBUFFER lpSPrimary;

LPDIRECT3D9				lpD3D;		// Direct3Dインターフェイス

LPDIRECT3DDEVICE9		lpD3DDevice;	// Direct3DDeviceインターフェイス

D3DPRESENT_PARAMETERS d3dpp;

//音を鳴らすのに必要
LPDIRECTSOUNDBUFFER8 lpBGM; //音を入れる入れ物
LPDIRECTSOUNDBUFFER8 lpSE; //音を入れる入れ物
LPDIRECTSOUND3DBUFFER8 lp3DBGM; //音の設定データ

LPD3DXSPRITE lpSprite;	// スプライト
LPD3DXFONT lpFont;		// フォント

bool gameFullScreen;	// フルスクリーン（true,false)
/* true(1)   false(0)
   if (Flg == true){～}   same as   if (Flg){～}
   if (Flg == false){～}   same as   if (!Flg){～} */

int frame;   //variable to count the number of frames

//場面管理
int Scene;    //0 = Title    1 = Game  etc.

LPDIRECT3DTEXTURE9 TmpTex;   //① 動的テクスチャ
LPDIRECT3DSURFACE9 lpTmpbuf;   //② サーフェイス
LPDIRECT3DSURFACE9 lpTmpZbuf;   //③ Zバッファ
LPDIRECT3DSURFACE9 lpBackbuf;   //④ バックアップ1
LPDIRECT3DSURFACE9 lpZbuf;   //⑤ バックアップ2

/*
LPDIRECT3DTEXTURE9 playerTex;  //画像を入れる入れ物
//座標
float playerX;
float playerY;
D3DXMATRIX PosMat;   //座標行列
//移動量
float playerMoveX;
float playerMoveY;

//拡大縮小に必要
float sizeW;
float sizeH;
D3DXMATRIX ScaleMat;   //拡縮行列

//回転に必要
float angle;
D3DXMATRIX RotMat;   //回転行列

//合成行列
D3DXMATRIX playerMat;  */






////////////////////////////////////////////////// 定数宣言 (const) //////////////////////////////////////////////////









#define	FVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)



void LoadWAVE3D(LPDIRECTSOUNDBUFFER8 &pDSData, LPDIRECTSOUND3DBUFFER8 &pDSData3D, char *fname)
{
	HRESULT hr;

	// WAVEファイルを開く
	CWaveSoundRead WaveFile;
	WaveFile.Open(fname);

	// セカンダリ・バッファを作成する
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC dsbdesc;
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);

	//(DSBCAPS_CTRL3D=３Ｄサウンドを使用)
	dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRL3D |
		DSBCAPS_CTRLVOLUME | /*DSBCAPS_CTRLPAN |*/ DSBCAPS_CTRLFREQUENCY;
	dsbdesc.dwBufferBytes = WaveFile.m_ckIn.cksize;
	dsbdesc.lpwfxFormat = WaveFile.m_pwfx;

	//3DサウンドのHELアルゴリズムを選択
	dsbdesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	//	dsbdesc.guid3DAlgorithm=DS3DALG_DEFAULT;

		// バッファを作る
	LPDIRECTSOUNDBUFFER pDSTmp;
	//	lpDSound->CreateSoundBuffer(&dsbdesc, &pDSData, NULL); 
	lpDSound->CreateSoundBuffer(&dsbdesc, &pDSTmp, NULL);
	pDSTmp->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pDSData);
	pDSTmp->Release();

	// セカンダリ・バッファにWaveデータを書き込む
	LPVOID lpvPtr1;		// 最初のブロックのポインタ
	DWORD dwBytes1;		// 最初のブロックのサイズ
	LPVOID lpvPtr2;		// ２番目のブロックのポインタ
	DWORD dwBytes2;		// ２番目のブロックのサイズ

	hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);

	// DSERR_BUFFERLOSTが返された場合，Restoreメソッドを使ってバッファを復元する
	if (DSERR_BUFFERLOST == hr)
	{
		pDSData->Restore();
		hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
	}
	if (SUCCEEDED(hr))
	{
		// ロック成功

		// ここで，バッファに書き込む
		// バッファにデータをコピーする
		UINT rsize;
		WaveFile.Read(dwBytes1, (LPBYTE)lpvPtr1, &rsize);
		if (0 != dwBytes2)
			WaveFile.Read(dwBytes2, (LPBYTE)lpvPtr2, &rsize);

		// 書き込みが終わったらすぐにUnlockする．
		hr = pDSData->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
	}

	//3Dのセカンダリバッファを作る
	pDSData->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&pDSData3D);
}

////角度算出, Degree
////相手の角度を求める関数,  Degree    Arctan Function
////キャラ1から見たキャラ2（ターゲット）の角度
float GetDegreeAngle(float x, float y, float tx, float ty)
{
	float fx = tx - x;
	float fy = ty - y;

	if ((int)fx == 0.f && (int)fy == 0.f)
	{
		return 270.f;
	}

	float rad = atan2f(fy, fx)/* arc tangent with 2 points giving a float answer */;   //引数は縦、横の順番

	float deg = D3DXToDegree(rad);

	if (deg < 0)
	{
		deg += 360.0f;
	}

	/*double atan2(double y, double x);
	float atan2f(float y, float x);*/

	return deg;
}

////角度算出, Radian
////相手の角度を求める関数,  Radian    Arctan Function
////キャラ1から見たキャラ2（ターゲット）の角度
float GetRadianAngle(float x, float y, float tx, float ty)
{
	float fx = tx - x;
	float fy = ty - y;

	if ((int)fx == 0.f && (int)fy == 0.f)
	{
		return 270.f;
	}

	float rad = atan2f(fy, fx)/* arc tangent with 2 points giving a float answer */;   //引数は縦、横の順番

	return rad;
}

////直線距離算出
float GetDistance(float x, float y, float tx, float ty)
{
	float fx = tx - x;
	float fy = ty - y;

	//斜辺
	float dist = sqrt((fx * fx) + (fy * fy));

	return dist;
}

//// 点を描く関数
void DrawPixel(float x, float y, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x, y, 0, 1, colour } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST /* primitive type, this draws a point */, 1 /* the amount of points */, vertex, sizeof(Vertex2D));
	//lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST /* primitive type, this draws a line between points */, 1 /* the amount of lines */, vertex, sizeof(Vertex2D));
	//lpD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP /* primitive type, this draws lines */, 3 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//// 2点間の線を描く関数
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

void DrawLineColGrad(float x1, float y1, D3DCOLOR colour1, float x2, float y2, D3DCOLOR colour2)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x1, y1, 0, 1, colour1 }, { x2, y2, 0, 1, colour2 } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST /* primitive type, this draws a line between points */, 1 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//// 三角形を線で描く関数
void DrawLineTriangle(float x1, float y1, float x2, float y2, float x3, float y3, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x1, y1, 0, 1, colour }, { x2, y2, 0, 1, colour }, { x2, y2, 0, 1, colour }, { x3, y3, 0, 1, colour }, { x3, y3, 0, 1, colour }, { x1, y1, 0, 1, colour } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST /* primitive type, this draws a line between points */, 3 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

void DrawLineTriColGrad(float x1, float y1, D3DCOLOR colour1, float x2, float y2, D3DCOLOR colour2, float x3, float y3, D3DCOLOR colour3)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x1, y1, 0, 1, colour1 }, { x2, y2, 0, 1, colour2 }, { x2, y2, 0, 1, colour2 }, { x3, y3, 0, 1, colour3 }, { x3, y3, 0, 1, colour3 }, { x1, y1, 0, 1, colour1 } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST /* primitive type, this draws a line between points */, 3 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//// 三角形を描く関数
void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x1, y1, 0, 1, colour }, { x2, y2, 0, 1, colour }, { x3, y3, 0, 1, colour } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST /* primitive type, this draws a line between points */, 1 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

void DrawTriColGrad(float x1, float y1, D3DCOLOR colour1, float x2, float y2, D3DCOLOR colour2, float x3, float y3, D3DCOLOR colour3)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x1, y1, 0, 1, colour1 }, { x2, y2, 0, 1, colour2 }, { x3, y3, 0, 1, colour3 } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST /* primitive type, this draws a line between points */, 1 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//// 四角形を線で描く関数
void DrawLineSquare(float x, float y, int width, int height, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x, y, 0, 1, colour }, { x + width, y, 0, 1, colour }, { x + width, y, 0, 1, colour }, { x + width, y + height, 0, 1, colour }, { x + width, y + height, 0, 1, colour }, { x, y + height, 0, 1, colour }, { x, y + height, 0, 1, colour }, { x, y, 0, 1, colour } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST /* primitive type, this draws a line between points */, 4 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

void DrawLineSquColGrad(float x, float y, int width, int height, D3DCOLOR colour1, D3DCOLOR colour2, D3DCOLOR colour3, D3DCOLOR colour4)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x, y, 0, 1, colour1 }, { x + width, y, 0, 1, colour2 }, { x + width, y, 0, 1, colour2 }, { x + width, y + height, 0, 1, colour3 }, { x + width, y + height, 0, 1, colour3 }, { x, y + height, 0, 1, colour4 }, { x, y + height, 0, 1, colour4 }, { x, y, 0, 1, colour1 } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST /* primitive type, this draws a line between points */, 4 /* the amount of lines */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//// 四角形を描く関数
void DrawSquare(float x, float y, int width, int height, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x, y, 0, 1, colour }, { x + width, y, 0, 1, colour }, { x, y + height, 0, 1, colour }, { x + width, y + height, 0, 1, colour } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP /* primitive type, this draws a line between points */, 2 /* the amount of primitives */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

void DrawSquColGrad(float x, float y, int width, int height, D3DCOLOR colour1, D3DCOLOR colour2, D3DCOLOR colour3, D3DCOLOR colour4)
{
	// 頂点用変数の宣言
	Vertex2D vertex[] = { { x, y, 0, 1, colour1 }, { x + width, y, 0, 1, colour2 }, { x, y + height, 0, 1, colour3 }, { x + width, y + height, 0, 1, colour4 } };

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP /* primitive type, this draws a line between points */, 2 /* the amount of primitives */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//// 円を点で描く関数
void DrawDotCircle(float x, float y, float rx, float ry, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[360 + 1];

	float deg;   //Degree(0∼360)
	float rad;   //Radian(0∼2π)

	for (int i = 0; i < 360; i++)
	{
		deg = i;
		rad = D3DXToRadian(deg);
		vertex[i].x = x + cos(rad) * rx;
		vertex[i].y = y + sin(rad) * ry;
		vertex[i].z = 0.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].color = colour;
	}

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST /* primitive type, this draws a point */, 360 /* the amount of points */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);

}

//// 円を線で描く関数
void DrawLineCircle(float x, float y, float rx, float ry, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[60 + 1];

	float deg;   //Degree(0∼360)
	float rad;   //Radian(0∼2π)

	for (int i = 0; i < 60 + 1; i++)
	{
		deg = i * 6;
		rad = D3DXToRadian(deg);
		vertex[i].x = x + cos(rad) * rx;
		vertex[i].y = y + sin(rad) * ry;
		vertex[i].z = 0.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].color = colour;
	}

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP /* primitive type, this draws a line between points */, 60 /* the amount of primitives */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);

}

//// 円を描く関数
void DrawCircle(float x, float y, float rx, float ry, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[60 + 1];

	float deg;   //Degree(0∼360)
	float rad;   //Radian(0∼2π)

	for (int i = 0; i < 60 + 1; i++)
	{
		deg = i * 6;
		rad = D3DXToRadian(deg);
		vertex[i].x = x + cos(rad) * rx;
		vertex[i].y = y + sin(rad) * ry;
		vertex[i].z = 0.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].color = colour;
	}

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN /* primitive type, this draws a line between points */, 60 - 1 /* the amount of primitives */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
}

//// 多角形を点で描く関数
void DrawDotPoly(float x, float y, float r, float poly, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[360 + 1];

	float deg;   //Degree(0∼360)
	float rad;   //Radian(0∼2π)

	for (int i = 0; i < poly + 1; i++)
	{
		deg = i * (360 / poly);
		rad = D3DXToRadian(deg);
		vertex[i].x = x + cos(rad) * r;
		vertex[i].y = y + sin(rad) * r;
		vertex[i].z = 0.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].color = colour;
	}

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST /* primitive type, this draws a point */, 360 /* the amount of points */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);

}

//// 多角形を線で描く関数
void DrawLinePoly(float x, float y, float r, float poly, float dir, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[60 + 1];

	float deg;   //Degree(0∼360)
	float rad;   //Radian(0∼2π)

	for (int i = 0; i < poly + 1; i++)
	{
		deg = i * (360 / poly) + dir;
		rad = D3DXToRadian(deg);
		vertex[i].x = x + cos(rad) * r;
		vertex[i].y = y + sin(rad) * r;
		vertex[i].z = 0.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].color = colour;
	}

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP /* primitive type, this draws a line between points */, poly /* the amount of primitives */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);

}

//// 多角形を描く関数
void DrawPolygon(float x, float y, float r, float poly, float dir, D3DCOLOR colour)
{
	// 頂点用変数の宣言
	Vertex2D vertex[60 + 1];

	float deg;   //Degree(0∼360)
	float rad;   //Radian(0∼2π)

	for (int i = 0; i < poly + 1; i++)
	{
		deg = i * (360 / poly) + dir;
		rad = D3DXToRadian(deg);
		vertex[i].x = x + cos(rad) * r;
		vertex[i].y = y + sin(rad) * r;
		vertex[i].z = 0.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].color = colour;
	}

	// FVF設定（頂点構造体の情報を伝える）
	lpD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	// 描画
	lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN /* primitive type, this draws a line between points */, (poly - 1) /* the amount of primitives */, vertex, sizeof(Vertex2D));

	// 元々のFVF設定に戻す
	lpD3DDevice->SetFVF(FVF_VERTEX);
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

	/*LoadWAVE3D(lpSE, lp3DBGM, "Test.WAV");
	LoadWAVE3D(lpBGM, lp3DBGM, "BGM.WAV");
	lp3DBGM->SetMode(DS3DMODE_DISABLE, DS3D_IMMEDIATE);*/

	//① テクスチャ作成
	D3DXCreateTexture(lpD3DDevice, SCRW, SCRH, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TmpTex);
	//② サーフェイス取得
	TmpTex->GetSurfaceLevel(0, &lpTmpbuf);
	//③ Zバッファ取得
	lpD3DDevice->GetDepthStencilSurface(&lpTmpZbuf);
	//④ バックアップ1
	lpD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &lpBackbuf);
	//⑤ バックアップ2
	lpD3DDevice->GetDepthStencilSurface(&lpZbuf);



}

//終了関数　Function after ending the program
void Terminate(void)
{
	//////画像解放
	//playerTex->Release(); 


	/*lpBGM->Release();
	lpSE->Release();
	lp3DBGM->Release();*/

	TmpTex->Release();

	lpSprite->Release();	// スプライト
	lpFont->Release();		// フォント

	////コンソールの解放
	//FreeConsole();
}





// タイトルの初期化
void Title_Init(void)
{
	Scene = TITLE_PROC;
}
void Title_Update(void)
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		Game_Init();
	}
}
void Title_Render(void)
{

}
// ゲームの初期化
void Game_Init(void)
{
	Scene = GAME_PROC;

	/*lpBGM->Play(0, 0, DSBPLAY_LOOPING);*/

}
void Game_Update(void)
{
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		Title_Init();
	}

	/*if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		lpSE->Play(0, 0, 0);
	}
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		lpBGM->Stop();
	}*/






	/*
	//回転行列を作成, rotating on the z axis
	D3DXMatrixRotationZ(&RotMat, D3DXToRadian(angle));
	//拡縮行列を作成
	D3DXMatrixScaling(&ScaleMat, sizeW, sizeH, 1.0f);
	//座標行列を作成
	D3DXMatrixTranslation(&PosMat, playerX, playerY, 0.0f);
	//行列合成、この順番で掛け算（拡縮 * 回転 * 移動）
	playerMat = ScaleMat * RotMat * PosMat;
	//行列合成、この順番で掛け算（拡縮 * 移動 * 回転）,衛星みたいにくるくる回る
	playerMat = ScaleMat * PosMat * RotMat;   */
}
void Game_Render(void)
{

}

void C_CHARACTER::Init(void)
{

	LoadTexture(&Tex, "player.png", 256, 64, D3DCOLOR_XRGB(0, 0, 0));

	X = SCRW / 2;
	Y = SCRH / 2;
}







// 更新処理
void Update(void)
{
	////最終的な行列　＝　拡大　＊　移動     the multiplication has to be in this order or else there will be a bug
	//playerMat = playerScaleMat * playerTransMat;
	////拡大のMatrix
	//D3DXMatrixScaling(&playerScaleMat, -1.0f /* Xの拡大 */, 1.0f /* Yの拡大 */, 0.0f /* Zの拡大 */);

	/*switch (Scene)
	{
	case TITLE_PROC:
		Title_Update();
		break;
	case GAME_PROC:
		Game_Update();
		break;
	}*/



}

// 3D描画
void Render3D(void)
{

}

// 動的テクスチャ作成
void DynamicRender2D(void)
{
	//描画先切り替え
	lpD3DDevice->SetRenderTarget(0, lpTmpbuf);
	lpD3DDevice->SetDepthStencilSurface(lpTmpZbuf);


	//////////////////////////////////////////////////
	///	スプライトの描画処理
	//////////////////////////////////////////////////
	// 描画開始
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	/*//表示範囲
	RECT rcPlayer = { 0, 0, 64, 64 };
	//移動のMatrix, 座標を行列にセット
	D3DXMatrixTranslation(&playerMat,   //どの行列にセットしますか
							playerX,   // X座標の移動
							playerY,   // Y座標の移動
							0.0f);   // Z座標の移動(2Dゲームは0固定)
	//行列をセット
	lpSprite->SetTransform(&playerMat);
	//表示
	lpSprite->Draw(playerTex,   //何を表示しますか
					&rcPlayer,   //表示範囲を指定
					&D3DXVECTOR3(32.0f, 32.0f, 0.0f),   //中心点
					NULL,   //注視点
					D3DCOLOR_ARGB(255, 255, 255, 255));   //表示させる色  */

					/*switch (Scene)
					{
					case TITLE_PROC:
						Title_Render();
						break;
					case GAME_PROC:
						Game_Render();
						break;
					}*/








					// 描画終了
	lpSprite->End();



	//文字表示はここから

	/*RECT rcScoreText = { 570, 0, 0, 0 };
	char scoreText[50];
	sprintf_s(scoreText, sizeof(scoreText), "HI %05d    score %05d", highScore, gameScore);
	lpFont->DrawText(NULL, scoreText, -1, &rcScoreText, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 100, 100, 100));*/



	//基本図形はここから



}

// 2D描画
void Render2D(void)
{
	//描画先切り替え
	lpD3DDevice->SetRenderTarget(0, lpBackbuf);
	lpD3DDevice->SetDepthStencilSurface(lpZbuf);


	//////////////////////////////////////////////////
	///	スプライトの描画処理
	//////////////////////////////////////////////////
	// 描画開始
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	/*//表示範囲
	RECT rcPlayer = { 0, 0, 64, 64 };  
	//移動のMatrix, 座標を行列にセット
	D3DXMatrixTranslation(&playerMat,   //どの行列にセットしますか
							playerX,   // X座標の移動
							playerY,   // Y座標の移動
							0.0f);   // Z座標の移動(2Dゲームは0固定)
	//行列をセット
	lpSprite->SetTransform(&playerMat);
	//表示
	lpSprite->Draw(playerTex,   //何を表示しますか
					&rcPlayer,   //表示範囲を指定
					&D3DXVECTOR3(32.0f, 32.0f, 0.0f),   //中心点
					NULL,   //注視点
					D3DCOLOR_ARGB(255, 255, 255, 255));   //表示させる色  */

	/*switch (Scene)
	{
	case TITLE_PROC:
		Title_Render();
		break;
	case GAME_PROC:
		Game_Render();
		break;
	}*/








	// 描画終了
	lpSprite->End();



	//文字表示はここから

	/*RECT rcScoreText = { 570, 0, 0, 0 };
	char scoreText[50];
	sprintf_s(scoreText, sizeof(scoreText), "HI %05d    score %05d", highScore, gameScore);
	lpFont->DrawText(NULL, scoreText, -1, &rcScoreText, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 100, 100, 100));*/



	//基本図形はここから



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

	// 動的テクスチャ作成
	DynamicRender2D();

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


	//初期化-------------------------------------------------------------
	//---------------------DirectSound関連-----------------------
	DirectSoundCreate8(NULL, &lpDSound, NULL);

	//協調レベルを設定
	lpDSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	// プライマリ・バッファの作成
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC dsbdesc;
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	// プライマリ・バッファを指定
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// バッファを作る
	lpDSound->CreateSoundBuffer(&dsbdesc, &lpSPrimary, NULL);

	// プライマリ・バッファのWaveフォーマットを設定
	// 　　　優先協調レベル以上の協調レベルが設定されている必要があります．
	WAVEFORMATEX pcmwf;
	ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX));
	pcmwf.wFormatTag = WAVE_FORMAT_PCM;
	pcmwf.nChannels = 2;		// ２チャンネル（ステレオ）
	pcmwf.nSamplesPerSec = 44100;	// サンプリング・レート　44.1kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
	pcmwf.wBitsPerSample = 16;		// 16ビット
	lpSPrimary->SetFormat(&pcmwf);

	CoInitialize(NULL);





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




	// 終了処理
	lpSPrimary->Release();
	lpDSound->Release();

	CoUninitialize();


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