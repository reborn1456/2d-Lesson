#include "CBullet.h"

CBullet::CBullet()
	: mpTexture(nullptr)
	, mPosition()
	, mMatrix()
	, mTargetRadian()
	, mMoveSpeed()
	, mIsAlive(false)
{
}

CBullet::~CBullet()
{
}

void CBullet::Update()
{
	// �w��p�x�Ɉړ�
	mPosition.x += cos(mTargetRadian) * 10.f;
	mPosition.y += sin(mTargetRadian) * 10.f;

	// �ړ��s��쐬
	D3DXMatrixTranslation(&mMatrix, mPosition.x, mPosition.y, mPosition.z);

	// CheckScreenOut ���Ђ�����Ԃ��� mIsAlive �ɓ����
	mIsAlive = !CheckScreenOut();
}

void CBullet::Draw(LPD3DXSPRITE apSprite)
{
	// �e
	apSprite->SetTransform(&mMatrix);
	RECT rectBullet = { 0, 0, 16, 16 };
	apSprite->Draw(mpTexture, &rectBullet, &D3DXVECTOR3(8.f, 8.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBullet::SetTexture(LPDIRECT3DTEXTURE9 apTexture)
{
	if (apTexture == nullptr)
	{
		return;
		// �������^�[��
		// �e�N�X�`������Ȃ̂ŏ�������Ӗ�������
	}
	mpTexture = apTexture;
}

const bool CBullet::CheckScreenOut()
{
	// ��ʊO�Ȃ�true
	return mPosition.x < 0 || mPosition.x > 1280 || mPosition.y < 0 || mPosition.y > 720;
}

const LPDIRECT3DTEXTURE9 CBullet::GetTexture()
{
	return mpTexture;
}

const bool CBullet::CheckAlive()
{
	return mIsAlive;
}

const float CBullet::GetTargetRadian()
{
	return mTargetRadian;
}

void CBullet::SetTargetRadian(float TargetRadian)
{
	mTargetRadian = TargetRadian;
	return;
}

void CBullet::SetIsAlive(bool IsAlive)
{
	mIsAlive = IsAlive;
	return;
}

void CBullet::SetPosition(D3DXVECTOR3 Position)
{
	mPosition = Position;
	return;
}

const D3DXVECTOR3 CBullet::GetPosition()
{
	return mPosition;
}