// GameView.h : CGameView Ŭ������ �������̽�
//


#pragma once


enum STAGE_ROW
{
	STAGE_ROW_0 = 2,
	STAGE_ROW_1 = 3,
	STAGE_ROW_2 = 4,
	STAGE_ROW_3 = 4,
	STAGE_ROW_4 = 4,
	STAGE_ROW_5 = 5,
	STAGE_ROW_6 = 6,
	STAGE_ROW_7 = 6
};

enum STAGE_COL
{
	STAGE_COL_0 = 3,
	STAGE_COL_1 = 4,
	STAGE_COL_2 = 4,
	STAGE_COL_3 = 5,
	STAGE_COL_4 = 6,
	STAGE_COL_5 = 6,
	STAGE_COL_6 = 6,
	STAGE_COL_7 = 6
};

enum STAGE_TIME
{
	STAGE_TIME_0 = 5,
	STAGE_TIME_1 = 10,
	STAGE_TIME_2 = 20,
	STAGE_TIME_3 = 25,
	STAGE_TIME_4 = 30,
	STAGE_TIME_5 = 40,
	STAGE_TIME_6 = 55,
	STAGE_TIME_7 = 50
};

enum START_TIME
{
	START_TIME_0 = 1,
	START_TIME_1 = 4,
	START_TIME_2 = 8,
	START_TIME_3 = 12,
	START_TIME_4 = 16,
	START_TIME_5 = 20,
	START_TIME_6 = 23,
	START_TIME_7 = 21
};

class CGameView : public CView
{
public:
  //���� ���콺�� ��ǥ������ �׸��� ��ġ�� ����Ͽ� �����ϰ� �ִ� �ε���
  UINT  m_nRowTempIndex;
  UINT  m_nColTempIndex;

  int nMatchCount;

protected: // serialization������ ��������ϴ�.
	CGameView();
	DECLARE_DYNCREATE(CGameView)

// Ư���Դϴ�.
public:
	CGameDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMatching(void);
	void OnSuccess(void);
	int m_nStage;
private:
	void SwitchRandCandTime(void);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nTimerCnt;
	void OnFail(void);
	void ChangeLevel(int n);
	int m_nTimeLimit;
	boolean m_bTimerStart;
	void AfterStageEnd(void);
	int m_nStartTimerCnt;
	int m_nStartTimerFin;
	int m_nTempScore;
	void HighestRecord(void);
	int m_nSuccessStreak;
};

#ifndef _DEBUG  // GameView.cpp�� ����� ����
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

