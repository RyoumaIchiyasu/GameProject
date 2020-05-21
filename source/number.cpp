//==================================================================================================================
//
// 数字の描画 [number.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS										// 警告回避文

//==================================================================================================================
//	インクルードファイル
//==================================================================================================================
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "scene2D.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "number.h"

//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define TEXTURE_NUMBER "data/TEXTURE/number000.png"					// 数字テクスチャのソース先
#define SizeX 33													// 横のサイズ
#define SizeY 60													// 縦のサイズ
#define SIZE_WIDTH 0.1f												// 数字と数字の幅

//==================================================================================================================
//	静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;							// テクスチャ情報
D3DXVECTOR3 CNumber::m_pos = D3DXVECTOR3(0, 0, 0);						// 位置

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CNumber::CNumber()
{
	//初期化
	m_pos = D3DXVECTOR3(0, 0, 0);				// 位置
}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CNumber::~CNumber(void)
{

}

//==================================================================================================================
//	ポリゴン初期化処理
//==================================================================================================================
void CNumber::Init(void)
{
	//初期化
	m_pos = D3DXVECTOR3(0, 0, 0);							// 位置

	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラーの情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスを取得する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(右回りで設定する)
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-SizeX / 2), m_pos.y + (-SizeX / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (+SizeX / 2), m_pos.y + (-SizeX / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-SizeX / 2), m_pos.y + (+SizeX / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (+SizeX / 2), m_pos.y + (+SizeX / 2), 0.0f);

	// 同次座標(1.0で固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 色の設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//==================================================================================================================
//	ポリゴン終了処理
//==================================================================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファがあるとき
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();			// 頂点バッファの開放
		m_pVtxBuff = NULL;				// NULLにする
	}
}

//==================================================================================================================
//	ポリゴン更新処理
//==================================================================================================================
void CNumber::Update(void)
{

}

//==================================================================================================================
//	ポリゴン描画処理
//==================================================================================================================
void CNumber::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラーの情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスを取得する

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CNumber *CNumber::CreateObject(void)
{
	CNumber *pNumber = NULL;					// 数字変数をNULLにする

	pNumber = new CNumber;						// 動的に確保
	pNumber->Init();							// 初期化処理

	return pNumber;								// 値を返す
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CNumber::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラーの情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスを取得する

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_NUMBER,										// ファイルの名前
		&m_pTexture);										// 読み込むメモリー

	return S_OK;											// 値を返す
}

//==================================================================================================================
// テクスチャ情報破棄
//==================================================================================================================
void CNumber::Unload(void)
{
	// テクスチャ情報があるとき
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();						// テクスチャの開放
		m_pTexture = NULL;							// NULLにする
	}
}

//==================================================================================================================
// 数字の設定
//==================================================================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((float)nNumber * SIZE_WIDTH, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)nNumber * SIZE_WIDTH + SIZE_WIDTH, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)nNumber * SIZE_WIDTH, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)nNumber * SIZE_WIDTH + SIZE_WIDTH, 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//==================================================================================================================
// 位置の設定
//==================================================================================================================
void CNumber::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	m_pos = pos;					// 位置代入

	VERTEX_2D *pVtx;				// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(右回りで設定する)
	pVtx[0].pos.x = m_pos.x + (-fSizeX / 2);
	pVtx[0].pos.y = m_pos.y + (-fSizeY / 2);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + (+fSizeX / 2);
	pVtx[1].pos.y = m_pos.y + (-fSizeY / 2);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + (-fSizeX / 2);
	pVtx[2].pos.y = m_pos.y + (+fSizeY / 2);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + (+fSizeX / 2);
	pVtx[3].pos.y = m_pos.y + (+fSizeY / 2);
	pVtx[3].pos.z = 0.0f;

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

