#include "CEnemy.h"

CEnemy::CEnemy()
	: mpTexture(nullptr)
	, mPosition(600.f, 300.f, 0.f)
	, mMatrix()
	, mIsAlive(true)
{
}

CEnemy::~CEnemy()
{
	//safe release
	if (mpTexture != nullptr)   // if it is notyet destroyed, not 0
	{
		mpTexture->Release();
		mpTexture = nullptr;   // put null(0) inside it just incase it contains any rubbish which is still inside after release
	}
}

void CEnemy::Update()
{
	if (mIsAlive == false)
	{
		return;
		// 早期リターン
		// テクスチャが空なので処理する意味が無い
	}

	

	//車体・砲身、共通の移動座標行列を作成
	D3DXMatrixTranslation(&mMatrix, mPosition.x, mPosition.y, mPosition.z);
}

void CEnemy::Draw(LPD3DXSPRITE apSprite)
{
	if (mIsAlive == false)
	{
		return;
		// 早期リターン
		// テクスチャが空なので処理する意味が無い
	}
	// 車体
	apSprite->SetTransform(&mMatrix);
	RECT rectBody = { 0, 140, 140, 280 };
	apSprite->Draw(mpTexture, &rectBody, &D3DXVECTOR3(70.f, 70.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 砲身
	RECT rectBarrel = { 0, 0, 140, 130 };
	apSprite->Draw(mpTexture, &rectBarrel, &D3DXVECTOR3(70.f, 110.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CEnemy::SetTexture(LPDIRECT3DTEXTURE9 apTexture)
{
	if (apTexture == nullptr)
	{
		return;
		// 早期リターン
		// テクスチャが空なので処理する意味が無い
	}
	mpTexture = apTexture;
}

const D3DXVECTOR3 CEnemy::GetPosition()
{
	return mPosition;
}

void CEnemy::BulletHit()
{
	mIsAlive = false;
}