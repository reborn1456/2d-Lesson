#pragma once   // ���d�C���N���[�h�h�~

class CCounterMachine
{
public:
	CCounterMachine();      // �R���X�g���N�^
	~CCounterMachine();     // �f�X�g���N�^

	// �����o�֐�
	void AddCount();
	void ClearCount();
	void DrawCount();

	const int GetCount();

private:
	// �����o�ϐ�
	int mCount;
};