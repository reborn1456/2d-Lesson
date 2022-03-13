#pragma once   // 多重インクルード防止

class CCounterMachine
{
public:
	CCounterMachine();      // コンストラクタ
	~CCounterMachine();     // デストラクタ

	// メンバ関数
	void AddCount();
	void ClearCount();
	void DrawCount();

	const int GetCount();

private:
	// メンバ変数
	int mCount;
};