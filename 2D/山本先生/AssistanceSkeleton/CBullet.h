#pragma once

#include <d3dx9.h>

class CBullet
{
public:
	CBullet();
	~CBullet();

	// メンバ変数

	// メンバ関数
	void Update();
	void Draw(LPD3DXSPRITE apSprite);
	void SetTexture(LPDIRECT3DTEXTURE9 apTexture);
	const bool CheckScreenOut();                      // 画面外チェック
	const LPDIRECT3DTEXTURE9 GetTexture();
	const bool CheckAlive();
	void SetIsAlive(bool IsAlive);
	const float GetTargetRadian();
	void SetTargetRadian(float TargetRadian);
	void SetPosition(D3DXVECTOR3 Position);
	const D3DXVECTOR3 GetPosition();

private:
	// メンバ変数
	LPDIRECT3DTEXTURE9   mpTexture;
	D3DXVECTOR3          mPosition;
	D3DXMATRIX           mMatrix;
	float                mTargetRadian;      // 目標への角度
	const float          mMoveSpeed;         // 速度
	bool                 mIsAlive;           // 生存フラグ
};