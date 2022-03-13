#include "CPlayer.h"
#include "CGameFrame.h"
//#include <assert.h> /* this is the old version of assert which WILL appear when build in release mode */
#include <cassert>  /* this is the new version of assert which will NOT appear when build in release mode */

// コンストラクタ
CPlayer::CPlayer()
      // クラスの初期化、ゲームの初期化ではない
	: mpTexture(nullptr)
	, mPosition(200.f, 200.f, 0.f)   // mPosition = D3DXVECTOR3(200.f, 200.f, 0.f);
	, mMatrix()   // int, char etc. を含むものを()しただけで自動的にゼロ埋めされる
	, mBodyRotMatrix()
	, mBodyAngle(0.f)   // 0.0fは書かなく手も良い
	, mBarrelMatrix()
	, mBulletList()
	, mpBulletTexture(nullptr)
	, mpOwner(nullptr)
{
}

// デストラクタ
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
	float radian = atan2f(fy, fx)/* arc tangent with 2 points giving a float answer */;   //引数は縦、横の順番


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

	//車体・砲身、共通の移動座標行列を作成
	D3DXMatrixTranslation(&mMatrix, mPosition.x, mPosition.y, mPosition.z);

	//砲身の回転行列設定, rotating on the z axis
	D3DXMatrixRotationZ(&mBarrelMatrix, radian + D3DXToRadian(90));
	//砲身の回転行列と共通の移動行列を合成、必ずこの順番で掛け算（回転 * 移動）
	mBarrelMatrix = mBarrelMatrix * mMatrix;

	//車体の回転行列を設定, rotating on the z axis
	D3DXMatrixRotationZ(&mBodyRotMatrix, D3DXToRadian(mBodyAngle));
	//車体の回転行列と共通の移動行列を合成、必ずこの順番で掛け算（回転 * 移動）
	mMatrix = mBodyRotMatrix * mMatrix;




	// 弾
	/*//生配列
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (mBulletList[i].CheckAlive())
		{
			mBulletList[i].Update();
		}
	}*/
	// 敵の情報を取得 プレイヤー → オーナー → エネミー → 座標
	CEnemy* enemy = mpOwner->GetEnemy();
	const D3DXVECTOR3 enemyPos = enemy->GetPosition();
	//可変長配列ver
	for (int i = 0; i < mBulletList.size(); i++)
	{
		// 生存している弾だけ更新
		if (mBulletList[i]->CheckAlive())
		{
			// 弾の更新処理
			mBulletList[i]->Update();

			// 弾と敵の当たり判定処理
			// 敵と弾の２点間の距離を求める
			const D3DXVECTOR3 rangePos = mBulletList[i]->GetPosition() - enemyPos;
			const float length = D3DXVec3Length(&rangePos);

			// 距離が70未満なら直撃
			if (length < 70)
			{
				// 弾に当たった時の処理
				enemy->BulletHit();
			}
		}
	}


	//Left click
	/*//生配列
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (!mBulletList[i].CheckAlive())
			{
				// 弾の生存フラグを立てる
				mBulletList[i].SetIsAlive(true);

				// プレイヤーに用意させた弾テクスチャを設定
				mBulletList[i].SetTexture(mpBulletTexture);

				// 弾の初期位置を現在のプレイヤーの位置に設定
				mBulletList[i].SetPosition(mPosition);

				// マウスへの角度をコピー
				mBulletList[i].SetTargetRadian(radian);

				// 一発撃ったら break でループを抜ける
				break;
			}
		}
	}*/
	//可変長配列
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		CBullet* bullet = new CBullet();
		// 弾の生存フラグを立てる
		bullet->SetIsAlive(true);
		// プレイヤーに用意させた弾テクスチャを設定
		bullet->SetTexture(mpBulletTexture);
		// 弾の初期位置を現在のプレイヤーの位置に設定
		bullet->SetPosition(mPosition);
		// マウスへの角度をコピー
		bullet->SetTargetRadian(radian);
		// 配列の末尾に一個追加
		mBulletList.push_back(bullet);
	}
	// 不要な弾の要素を削除
	std::vector<CBullet*>::iterator it;
	it = mBulletList.begin();
	while (it != mBulletList.end())
	{
		// 画面外なら削除
		if ((*it)->CheckScreenOut() == true)
		{
			// 実体を削除
			delete (*it);

			// 要素（アドレス）を削除
			it = mBulletList.erase(it);
		}
		// 画面内なら次の要素へ
		else
		{
			it++;
		}
	}
}

void CPlayer::Draw(LPD3DXSPRITE apSprite)
{
	// 車体
	apSprite->SetTransform(&mMatrix);
	RECT rectBody = { 0, 140, 140, 280 };
	apSprite->Draw(mpTexture, &rectBody, &D3DXVECTOR3(70.f, 70.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 砲身
	apSprite->SetTransform(&mBarrelMatrix);
	RECT rectBarrel = { 0, 0, 140, 140 };
	apSprite->Draw(mpTexture, &rectBarrel, &D3DXVECTOR3(70.f, 110.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 弾
	/*//生配列
	for (int i = 0; i < BULLET_MAX; i++)
	{
		// 生存している弾だけ更新
		if (mBulletList[i].CheckAlive())
		{
			mBulletList[i].Draw(apSprite);
		}
	}*/
	//可変長配列
	for (int i = 0; i < mBulletList.size(); i++)
	{
		// 生存している弾だけ更新
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
		_ASSERT_EXPR(apTexture != nullptr, L"渡されたテクスチャ情報が空です\n確認ヨロ");



		return;
		// 早期リターン
		// テクスチャが空なので処理する意味が無い
	}
	mpTexture = apTexture;
}

void CPlayer::SetBulletTexture(LPDIRECT3DTEXTURE9 apTexture)
{
	if (apTexture == nullptr)
	{
		return;
		// 早期リターン
		// テクスチャが空なので処理する意味が無い
	}
	mpBulletTexture = apTexture;
}

void CPlayer::SetOwner(CGameFrame* apOwner)
{
	// new version assert which will NOT appear when build in release mode
	// Capital L is needed infront of the "" for showing sentance when you want to show japanese. If without, it will show corruption of text
	_ASSERT_EXPR(apOwner != nullptr, L"渡されたオーナー情報が空です\n確認ヨロ");

	mpOwner = apOwner;
	return;
}