#pragma once

#include <d3dx9.h>

class CXxxxx
{
public:
	CXxxxx();
	~CXxxxx();

	// メンバ関数
	void SetTexture(LPDIRECT3DTEXTURE9 apTexture);

private:
	// メンバ変数
	LPDIRECT3DTEXTURE9   mpTexture;
	D3DXVECTOR3          mPosition;
	D3DXMATRIX             mMatrix;
};