#pragma once

#include <d3dx9.h>

class CEnemy
{
public:
	CEnemy();
	~CEnemy();

	// �����o�֐�
	void Update();                                           // �X�V
	void Draw(LPD3DXSPRITE apSprite);                        // �`��
	void SetTexture(LPDIRECT3DTEXTURE9 apTexture);           // �e�N�X�`���ݒ�
	const D3DXVECTOR3 GetPosition();
	void BulletHit();

private:
	// �����o�ϐ�
	LPDIRECT3DTEXTURE9   mpTexture;           // �摜
	D3DXVECTOR3          mPosition;           // ���W
	D3DXMATRIX           mMatrix;             // �s��
	bool                 mIsAlive;
};