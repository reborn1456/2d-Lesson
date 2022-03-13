#pragma once

#include <d3dx9.h>
#include "CBullet.h"

#include <vector>
using namespace std;


class CGameFrame;   // クラスの前方宣言
// 


class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	// メンバ関数
	void Update(const POINT aMousePos);                      // 更新
	void Draw(LPD3DXSPRITE apSprite);                        // 描画
	void SetTexture(LPDIRECT3DTEXTURE9 apTexture);           // テクスチャ設定
	void SetBulletTexture(LPDIRECT3DTEXTURE9 apTexture);     // テクスチャ設定

	void SetOwner(CGameFrame* apOwner);

private:

	// メンバ変数
	LPDIRECT3DTEXTURE9   mpTexture;           // 画像
	D3DXVECTOR3          mPosition;           // 座標
	D3DXMATRIX           mMatrix;             // 行列
	D3DXMATRIX           mBodyRotMatrix;      // 車体の回転行列
	float                mBodyAngle;          // 車体の回転角度
	D3DXMATRIX           mBarrelMatrix;       // 砲身の行列

	//static const int       BULLET_MAX = 999;           // static が付いていたらプレイヤーが何体あっても同じ BULLET_MAX を使う
	//CBullet                mBulletList[BULLET_MAX];
	std::vector<CBullet*>    mBulletList;
	LPDIRECT3DTEXTURE9       mpBulletTexture;

	CGameFrame*  mpOwner;   // オーナー
	// CGameFrame mOwner:
	// ↑CGameSceneを実体でメンバ変数として持つ場合は
	// 変数宣言をした行たコンストラクタが呼ばれるため
	// #include "CGameFrame.h" が必須になったり、
	// mOwnerの中身を毎回Update関数毎にコピーしなくてはならなかったり
	// デメリットが多くなるので、基本的にクラスをメンバ変数で
	// 持つ場合はポインタで持つようにする

	D3DXVECTOR3          mEnemyPosition;           // enemy座標
};