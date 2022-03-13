#pragma once

#include <d3dx9.h>

class CEnemy
{
public:
	CEnemy();
	~CEnemy();

	// メンバ関数
	void Update();                                           // 更新
	void Draw(LPD3DXSPRITE apSprite);                        // 描画
	void SetTexture(LPDIRECT3DTEXTURE9 apTexture);           // テクスチャ設定
	const D3DXVECTOR3 GetPosition();
	void BulletHit();

private:
	// メンバ変数
	LPDIRECT3DTEXTURE9   mpTexture;           // 画像
	D3DXVECTOR3          mPosition;           // 座標
	D3DXMATRIX           mMatrix;             // 行列
	bool                 mIsAlive;
};