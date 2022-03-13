#pragma once

#include <d3dx9.h>

class CBullet
{
public:
	CBullet();
	~CBullet();

	// �����o�ϐ�

	// �����o�֐�
	void Update();
	void Draw(LPD3DXSPRITE apSprite);
	void SetTexture(LPDIRECT3DTEXTURE9 apTexture);
	const bool CheckScreenOut();                      // ��ʊO�`�F�b�N
	const LPDIRECT3DTEXTURE9 GetTexture();
	const bool CheckAlive();
	void SetIsAlive(bool IsAlive);
	const float GetTargetRadian();
	void SetTargetRadian(float TargetRadian);
	void SetPosition(D3DXVECTOR3 Position);
	const D3DXVECTOR3 GetPosition();

private:
	// �����o�ϐ�
	LPDIRECT3DTEXTURE9   mpTexture;
	D3DXVECTOR3          mPosition;
	D3DXMATRIX           mMatrix;
	float                mTargetRadian;      // �ڕW�ւ̊p�x
	const float          mMoveSpeed;         // ���x
	bool                 mIsAlive;           // �����t���O
};