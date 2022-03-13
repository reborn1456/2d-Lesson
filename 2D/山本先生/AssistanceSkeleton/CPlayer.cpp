#include "CPlayer.h"
#include "CGameFrame.h"
//#include <assert.h> /* this is the old version of assert which WILL appear when build in release mode */
#include <cassert>  /* this is the new version of assert which will NOT appear when build in release mode */

// �R���X�g���N�^
CPlayer::CPlayer()
      // �N���X�̏������A�Q�[���̏������ł͂Ȃ�
	: mpTexture(nullptr)
	, mPosition(200.f, 200.f, 0.f)   // mPosition = D3DXVECTOR3(200.f, 200.f, 0.f);
	, mMatrix()   // int, char etc. ���܂ނ��̂�()���������Ŏ����I�Ƀ[�����߂����
	, mBodyRotMatrix()
	, mBodyAngle(0.f)   // 0.0f�͏����Ȃ�����ǂ�
	, mBarrelMatrix()
	, mBulletList()
	, mpBulletTexture(nullptr)
	, mpOwner(nullptr)
{
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
	//safe release
	if (mpTexture != nullptr)   // if it is notyet destroyed, not 0
	{
		mpTexture->Release();
		mpTexture = nullptr;   // put null(0) inside it just incase it contains any rubbish which is still inside after release
	}

	//safe release
	if (mpBulletTexture)   // if it is notyet destroyed, not 0
	{
		mpBulletTexture->Release();
		mpBulletTexture = nullptr;   // put null(0) inside it just incase it contains any rubbish which is still inside after release
	}
}

void CPlayer::Update(const POINT aMousePos)
{
	const float fx = aMousePos.x - mPosition.x;
	const float fy = aMousePos.y - mPosition.y;
	float radian = atan2f(fy, fx)/* arc tangent with 2 points giving a float answer */;   //�����͏c�A���̏���


	if (GetAsyncKeyState('D') & 0x8000)
	{
		mPosition.x += 3.0f;
		mBodyAngle = 270.f;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		mPosition.x += -3.0f;
		mBodyAngle = 90.f;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		mPosition.y += -3.0f;
		mBodyAngle = 180.f;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		mPosition.y += 3.0f;
		mBodyAngle = 0.f;
	}

	//�ԑ́E�C�g�A���ʂ̈ړ����W�s����쐬
	D3DXMatrixTranslation(&mMatrix, mPosition.x, mPosition.y, mPosition.z);

	//�C�g�̉�]�s��ݒ�, rotating on the z axis
	D3DXMatrixRotationZ(&mBarrelMatrix, radian + D3DXToRadian(90));
	//�C�g�̉�]�s��Ƌ��ʂ̈ړ��s��������A�K�����̏��ԂŊ|���Z�i��] * �ړ��j
	mBarrelMatrix = mBarrelMatrix * mMatrix;

	//�ԑ̂̉�]�s���ݒ�, rotating on the z axis
	D3DXMatrixRotationZ(&mBodyRotMatrix, D3DXToRadian(mBodyAngle));
	//�ԑ̂̉�]�s��Ƌ��ʂ̈ړ��s��������A�K�����̏��ԂŊ|���Z�i��] * �ړ��j
	mMatrix = mBodyRotMatrix * mMatrix;




	// �e
	/*//���z��
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (mBulletList[i].CheckAlive())
		{
			mBulletList[i].Update();
		}
	}*/
	// �G�̏����擾 �v���C���[ �� �I�[�i�[ �� �G�l�~�[ �� ���W
	CEnemy* enemy = mpOwner->GetEnemy();
	const D3DXVECTOR3 enemyPos = enemy->GetPosition();
	//�ϒ��z��ver
	for (int i = 0; i < mBulletList.size(); i++)
	{
		// �������Ă���e�����X�V
		if (mBulletList[i]->CheckAlive())
		{
			// �e�̍X�V����
			mBulletList[i]->Update();

			// �e�ƓG�̓����蔻�菈��
			// �G�ƒe�̂Q�_�Ԃ̋��������߂�
			const D3DXVECTOR3 rangePos = mBulletList[i]->GetPosition() - enemyPos;
			const float length = D3DXVec3Length(&rangePos);

			// ������70�����Ȃ璼��
			if (length < 70)
			{
				// �e�ɓ����������̏���
				enemy->BulletHit();
			}
		}
	}


	//Left click
	/*//���z��
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (!mBulletList[i].CheckAlive())
			{
				// �e�̐����t���O�𗧂Ă�
				mBulletList[i].SetIsAlive(true);

				// �v���C���[�ɗp�ӂ������e�e�N�X�`����ݒ�
				mBulletList[i].SetTexture(mpBulletTexture);

				// �e�̏����ʒu�����݂̃v���C���[�̈ʒu�ɐݒ�
				mBulletList[i].SetPosition(mPosition);

				// �}�E�X�ւ̊p�x���R�s�[
				mBulletList[i].SetTargetRadian(radian);

				// �ꔭ�������� break �Ń��[�v�𔲂���
				break;
			}
		}
	}*/
	//�ϒ��z��
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		CBullet* bullet = new CBullet();
		// �e�̐����t���O�𗧂Ă�
		bullet->SetIsAlive(true);
		// �v���C���[�ɗp�ӂ������e�e�N�X�`����ݒ�
		bullet->SetTexture(mpBulletTexture);
		// �e�̏����ʒu�����݂̃v���C���[�̈ʒu�ɐݒ�
		bullet->SetPosition(mPosition);
		// �}�E�X�ւ̊p�x���R�s�[
		bullet->SetTargetRadian(radian);
		// �z��̖����Ɉ�ǉ�
		mBulletList.push_back(bullet);
	}
	// �s�v�Ȓe�̗v�f���폜
	std::vector<CBullet*>::iterator it;
	it = mBulletList.begin();
	while (it != mBulletList.end())
	{
		// ��ʊO�Ȃ�폜
		if ((*it)->CheckScreenOut() == true)
		{
			// ���̂��폜
			delete (*it);

			// �v�f�i�A�h���X�j���폜
			it = mBulletList.erase(it);
		}
		// ��ʓ��Ȃ玟�̗v�f��
		else
		{
			it++;
		}
	}
}

void CPlayer::Draw(LPD3DXSPRITE apSprite)
{
	// �ԑ�
	apSprite->SetTransform(&mMatrix);
	RECT rectBody = { 0, 140, 140, 280 };
	apSprite->Draw(mpTexture, &rectBody, &D3DXVECTOR3(70.f, 70.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// �C�g
	apSprite->SetTransform(&mBarrelMatrix);
	RECT rectBarrel = { 0, 0, 140, 140 };
	apSprite->Draw(mpTexture, &rectBarrel, &D3DXVECTOR3(70.f, 110.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// �e
	/*//���z��
	for (int i = 0; i < BULLET_MAX; i++)
	{
		// �������Ă���e�����X�V
		if (mBulletList[i].CheckAlive())
		{
			mBulletList[i].Draw(apSprite);
		}
	}*/
	//�ϒ��z��
	for (int i = 0; i < mBulletList.size(); i++)
	{
		// �������Ă���e�����X�V
		if (mBulletList[i]->CheckAlive())
		{
			mBulletList[i]->Draw(apSprite);
		}
	}
}

void CPlayer::SetTexture(LPDIRECT3DTEXTURE9 apTexture)
{

	if (apTexture == nullptr)
	{

		//// old version of assert which WILL appear even when build in release mode
		//assert(apTexture != nullptr);

		// new version assert which will NOT appear when build in release mode
		// Capital L is needed infront of the "" for showing sentance when you want to show japanese. If without, it will show corruption of text
		_ASSERT_EXPR(apTexture != nullptr, L"�n���ꂽ�e�N�X�`����񂪋�ł�\n�m�F����");



		return;
		// �������^�[��
		// �e�N�X�`������Ȃ̂ŏ�������Ӗ�������
	}
	mpTexture = apTexture;
}

void CPlayer::SetBulletTexture(LPDIRECT3DTEXTURE9 apTexture)
{
	if (apTexture == nullptr)
	{
		return;
		// �������^�[��
		// �e�N�X�`������Ȃ̂ŏ�������Ӗ�������
	}
	mpBulletTexture = apTexture;
}

void CPlayer::SetOwner(CGameFrame* apOwner)
{
	// new version assert which will NOT appear when build in release mode
	// Capital L is needed infront of the "" for showing sentance when you want to show japanese. If without, it will show corruption of text
	_ASSERT_EXPR(apOwner != nullptr, L"�n���ꂽ�I�[�i�[��񂪋�ł�\n�m�F����");

	mpOwner = apOwner;
	return;
}