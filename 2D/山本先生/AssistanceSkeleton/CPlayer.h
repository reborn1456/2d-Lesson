#pragma once

#include <d3dx9.h>
#include "CBullet.h"

#include <vector>
using namespace std;


class CGameFrame;   // �N���X�̑O���錾
// 


class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	// �����o�֐�
	void Update(const POINT aMousePos);                      // �X�V
	void Draw(LPD3DXSPRITE apSprite);                        // �`��
	void SetTexture(LPDIRECT3DTEXTURE9 apTexture);           // �e�N�X�`���ݒ�
	void SetBulletTexture(LPDIRECT3DTEXTURE9 apTexture);     // �e�N�X�`���ݒ�

	void SetOwner(CGameFrame* apOwner);

private:

	// �����o�ϐ�
	LPDIRECT3DTEXTURE9   mpTexture;           // �摜
	D3DXVECTOR3          mPosition;           // ���W
	D3DXMATRIX           mMatrix;             // �s��
	D3DXMATRIX           mBodyRotMatrix;      // �ԑ̂̉�]�s��
	float                mBodyAngle;          // �ԑ̂̉�]�p�x
	D3DXMATRIX           mBarrelMatrix;       // �C�g�̍s��

	//static const int       BULLET_MAX = 999;           // static ���t���Ă�����v���C���[�����̂����Ă����� BULLET_MAX ���g��
	//CBullet                mBulletList[BULLET_MAX];
	std::vector<CBullet*>    mBulletList;
	LPDIRECT3DTEXTURE9       mpBulletTexture;

	CGameFrame*  mpOwner;   // �I�[�i�[
	// CGameFrame mOwner:
	// ��CGameScene�����̂Ń����o�ϐ��Ƃ��Ď��ꍇ��
	// �ϐ��錾�������s���R���X�g���N�^���Ă΂�邽��
	// #include "CGameFrame.h" ���K�{�ɂȂ�����A
	// mOwner�̒��g�𖈉�Update�֐����ɃR�s�[���Ȃ��Ă͂Ȃ�Ȃ�������
	// �f�����b�g�������Ȃ�̂ŁA��{�I�ɃN���X�������o�ϐ���
	// ���ꍇ�̓|�C���^�Ŏ��悤�ɂ���

	D3DXVECTOR3          mEnemyPosition;           // enemy���W
};