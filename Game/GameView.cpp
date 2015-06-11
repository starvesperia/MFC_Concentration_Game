// GameView.cpp : CGameView Ŭ������ ����
//
//#include <sstream>
//#include <fstream>
#include "stdafx.h"
#include "Game.h"

#include "GameDoc.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

BEGIN_MESSAGE_MAP(CGameView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CGameView ����/�Ҹ�

CGameView::CGameView()
	: m_nStage(-1)
	, m_nTimerCnt(0)
	, m_nTimeLimit(0)
	, m_nStartTimerCnt(0)
	, m_nStartTimerFin(0)
	, m_nTempScore(0)
	, m_nSuccessStreak(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	nMatchCount=0;
	m_bTimerStart=false;
}

CGameView::~CGameView()
{
}

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CGameView �׸���

void CGameView::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if(m_nStage==-1)
	{
		CString str, str2;
		str.Format(_T("Click to start game."));
		str2.Format(_T("The best record: %dPoint"), pDoc->GetHighScore());
		pDC->TextOutW(0,0,str);
		pDC->TextOutW(0,20,str2);
		return;
	}
	int nCount = 0;
	
	CDC memDC;
	CBitmap* pOldBmp;	

	// �����ϰ� �Ǿ����� �ʴٸ� �����Լ� ȣ��
	if(pDoc->m_bRandom)
	{
		nMatchCount=0;
		pDoc->OnRandom();
		pDoc->m_bRandom = false; 	
	}

	memDC.CreateCompatibleDC (pDC);        

	CString str,str2, str3;
	str.Format(_T("Current time : %dSec"), m_nTimerCnt);
	str2.Format(_T("Time limit : %dSec"), m_nTimeLimit);
	str3.Format(_T("Score : %dPoint"), m_nTempScore+pDoc->GetScore());
	pDC->TextOutW(0, 0, str2);
	pDC->TextOutW(0, 20, str);
	pDC->TextOutW(0, 40, str3);
  
	for(int n = 0; n < pDoc->m_nRow; n++)
	{
		for(int m = 0; m < pDoc->m_nCol; m++)
		{
			if(pDoc->m_bShow[n][m] == true)
			{
				pDoc->m_bmp[n][m].LoadBitmap(IDB_BITMAP1+pDoc->m_nRnd[nCount]);
				pOldBmp = memDC.SelectObject(&pDoc->m_bmp[n][m]);

				//���ʰ� ���ʿ� ������ �ֱ� ���ؼ� 1�� �� �ߴ�. 
				pDC->BitBlt(pDoc->m_bmCell.cx*(m+1), pDoc->m_bmCell.cy*(n+1), //��µ� ��ġ
							pDoc->m_bmCell.cx,		 pDoc->m_bmCell.cy,           //�׸��ϳ��� ũ��
							&memDC, 0, 0, SRCCOPY);
	   
				pDoc->m_bmp[n][m].DeleteObject();		
			}
			//���� �ٸ� �׸��� ��Ÿ������ �׸��� ���̵� ���� 1�� ������Ŵ
			nCount++; 
			if(pDoc->m_bShow[n][m] == false) 
			{
				CBitmap    bmp;
				bmp.LoadBitmap(IDB_BITMAP1);
				pOldBmp = memDC.SelectObject(&bmp);
	  
				pDC->BitBlt(pDoc->m_bmCell.cx*(m+1),pDoc->m_bmCell.cy*(n+1), //��µ� ��ġ
						pDoc->m_bmCell.cx, pDoc->m_bmCell.cy,           //�׸��ϳ��� ũ��
						&memDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}      
		}//for
	}//for
	memDC.SelectObject(pOldBmp);
	pDoc->ResizeWindow();
}


// CGameView �μ�

BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CGameView ����

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameDoc* CGameView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameView �޽��� ó����

void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nStage == -1)
	{
		ChangeLevel(++m_nStage);
		AfterStageEnd();
		return;
	}
	CGameDoc* pDoc = GetDocument();
  
	// ���� ���콺�� ������ �ִ� �ε���
	pDoc->m_nRowIndex = point.y / pDoc->m_bmCell.cy - 1;
	pDoc->m_nColIndex = point.x / pDoc->m_bmCell.cx - 1;

	// ���� Ŭ���� ���� �ٽ� Ŭ��, �ι� �̻� Ŭ��, ���� �ۿ� Ŭ���ߴٸ� ����
	if( pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] == true
		|| pDoc->m_nBmpSecondID != 0
		|| pDoc->m_nRowIndex > pDoc->m_nRow -1
		|| pDoc->m_nColIndex > pDoc->m_nCol -1
		|| pDoc->m_nRowIndex < 0
		|| pDoc->m_nColIndex < 0)
		return;

	if(m_bTimerStart==false)
	{
		SetTimer(0,1000, NULL);
		m_bTimerStart = true;
	}

	// ���콺�� ������ �ִ� �ε��� ���� �׸��� ������
	pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;

	CRect    rect;
	SetRect(&rect, pDoc->m_nRowIndex * (pDoc->m_bmCell.cx + 1),
				   pDoc->m_nColIndex * (pDoc->m_bmCell.cy + 1),
				   pDoc->m_nRowIndex * (pDoc->m_bmCell.cx + 2),
				   pDoc->m_nColIndex * (pDoc->m_bmCell.cy + 2));

  InvalidateRect(rect);

  // ���콺�� Ŭ���� ���� �ε��� �� �׸� ���̵� ����
	if(pDoc->m_nBmpFirstID == 0)
	{
		// ù��° ���콺 Ŭ��
		pDoc->m_bMouse = true;

		pDoc->m_nBmpFirstID =
					pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];

		m_nRowTempIndex = pDoc->m_nRowIndex;
		m_nColTempIndex = pDoc->m_nColIndex;    
	}
	else
	{
		// �ι�° ���콺 Ŭ��
		pDoc->m_bMouse = false;
    
		pDoc->m_nBmpSecondID =
				pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
	}

	// ���� �׸����� �Ǵ��Ѵ�
	OnMatching();

	CView::OnLButtonDown(nFlags, point);
}

void CGameView::OnMatching(void)
{
	CGameDoc* pDoc = GetDocument();

	// �׸��� ���ٸ�
	if(pDoc->m_nBmpFirstID == pDoc->m_nBmpSecondID 
		&& pDoc->m_bMouse == false)
	{
		m_nSuccessStreak++;
		if(m_nSuccessStreak>=3 && m_nSuccessStreak <6)
			m_nTempScore += 12;// ����
		else if(m_nSuccessStreak >= 6)
			m_nTempScore += 14;
		else
			m_nTempScore += 10;

		// Ŭ�� �̺�Ʈ �ʱ�ȭ
		pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = true;
		pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;
		pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
		m_nRowTempIndex = m_nColTempIndex = 0;

		//**********************************************************
		nMatchCount++;   
		//**********************************************************
	}
	else if(pDoc->m_nBmpFirstID != pDoc->m_nBmpSecondID 
		&& pDoc->m_bMouse==false)
	{
		// �ٸ� �׸��̶�� ��� ������
		CDC* pDC=GetDC();
		CDC memDC;
		CBitmap bmp;

		m_nSuccessStreak=0;
		m_nTempScore -= 1; // ����

		memDC.CreateCompatibleDC(pDC);
		bmp.LoadBitmap(IDB_BITMAP1 + pDoc->m_nBmpSecondID);

		CBitmap*  pOldBmp = memDC.SelectObject (&bmp);
		pDC->BitBlt(pDoc->m_bmCell.cx*(pDoc->m_nColIndex+1),
					pDoc->m_bmCell.cy * (pDoc->m_nRowIndex + 1),
					pDoc->m_bmCell.cx, pDoc->m_bmCell.cy,
					&memDC, 0, 0, SRCCOPY);

		bmp.DeleteObject();
		memDC.SelectObject(pOldBmp);
		Sleep(400);

		// Ŭ�� �̺�Ʈ �ʱ�ȭ
		pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = false;
		pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = false;
		pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
		m_nRowTempIndex = m_nColTempIndex = 0;
	}

	Invalidate();
	if(nMatchCount == (pDoc->m_nRow * pDoc->m_nCol)/2)
		OnSuccess();
}

void CGameView::AfterStageEnd(void)
{
	CGameDoc* pDoc = GetDocument();
	m_nSuccessStreak=0;
	pDoc->m_bRandom=true;// �����ϰ� �׸��� ID ����
	for(int n = 0; n < pDoc->m_nRow; n++)
		for(int m = 0; m < pDoc->m_nCol; m++)
			pDoc->m_bShow[n][m] = true;

	//�ƹ� �׸��� ���õ��� ���� ������ �ν��ϱ� ���ؼ� �ʱⰪ�� 0���� �����Ѵ�. 
	pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
	pDoc->m_bMouse = false;
	Invalidate();
	SetTimer(1,1000,NULL);
}

void CGameView::OnSuccess(void)
{
	KillTimer(0);
	CGameDoc* pDoc = GetDocument();
	pDoc->SetScore(m_nTempScore + pDoc->GetScore()); // ������ Doc���� ��� �ű�.
	m_nTempScore=0;
	
	if(m_nStage==7)
	{
		HighestRecord();	// �ְ��� Ȯ��.
		int res=AfxMessageBox(_T("All stage clear!\nBack to the title."), MB_OK);
		ChangeLevel(-1);
		AfterStageEnd();
		return;
	}
	int res=AfxMessageBox(_T("Stage clear! Continue?"), MB_YESNO);
	if(res==IDYES)
	{
		ChangeLevel(++m_nStage);
		AfterStageEnd();
	}
	else
    {
		HighestRecord();	// �ְ��� Ȯ��.
		ChangeLevel(-1);
		AfterStageEnd();
    }
}

void CGameView::OnFail(void)
{
	KillTimer(0);
	CGameDoc* pDoc = GetDocument();
	pDoc->SetScore(m_nTempScore + pDoc->GetScore()); // ������ Doc���� ��� �ű�.
	m_nSuccessStreak=0;
	HighestRecord();	// �ְ��� Ȯ��.

	int res=AfxMessageBox(_T("You lose! Restart?"), MB_YESNO);
	if(res==IDYES)
	{
		ChangeLevel(0);
		AfterStageEnd();
	}
	else
    {
		ChangeLevel(-1);
		AfterStageEnd();
    }
}

void CGameView::ChangeLevel(int n)
{
	m_bTimerStart=false;
	KillTimer(0);
	m_nStage=n;
	SwitchRandCandTime();
	m_nTimerCnt=0;
}

void CGameView::SwitchRandCandTime(void)
{
	CGameDoc* pDoc = GetDocument();

	switch(m_nStage)
	{
		case 0:
			pDoc->m_nRow = STAGE_ROW_0;
			pDoc->m_nCol = STAGE_COL_0;
			m_nTimeLimit = STAGE_TIME_0;
			m_nStartTimerFin = START_TIME_0;
			break;
		case 1:
			pDoc->m_nRow = STAGE_ROW_1;
			pDoc->m_nCol = STAGE_COL_1;
			m_nTimeLimit = STAGE_TIME_1;
			m_nStartTimerFin = START_TIME_1;
			break;
		case 2:
			pDoc->m_nRow = STAGE_ROW_2;
			pDoc->m_nCol = STAGE_COL_2;
			m_nTimeLimit = STAGE_TIME_2;
			m_nStartTimerFin = START_TIME_2;
			break;
		case 3:
			pDoc->m_nRow = STAGE_ROW_3;
			pDoc->m_nCol = STAGE_COL_3;
			m_nTimeLimit = STAGE_TIME_3;
			m_nStartTimerFin = START_TIME_3;
			break;
		case 4:
			pDoc->m_nRow = STAGE_ROW_4;
			pDoc->m_nCol = STAGE_COL_4;
			m_nTimeLimit = STAGE_TIME_4;
			m_nStartTimerFin = START_TIME_4;
			break;
		case 5:
			pDoc->m_nRow = STAGE_ROW_5;
			pDoc->m_nCol = STAGE_COL_5;
			m_nTimeLimit = STAGE_TIME_5;
			m_nStartTimerFin = START_TIME_5;
			break;
		case 6:
			pDoc->m_nRow = STAGE_ROW_6;
			pDoc->m_nCol = STAGE_COL_6;
			m_nTimeLimit = STAGE_TIME_6;
			m_nStartTimerFin = START_TIME_6;
			break;
		case 7:
			pDoc->m_nRow = STAGE_ROW_7;
			pDoc->m_nCol = STAGE_COL_7;
			m_nTimeLimit = STAGE_TIME_7;
			m_nStartTimerFin = START_TIME_7;
	}
}

void CGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	switch(nIDEvent)
	{
		case 0:
			m_nTimerCnt++;
			Invalidate();
			break;
		case 1:
			m_nStartTimerCnt++;
	}
	
	if(nIDEvent == 0 && m_nTimeLimit <= m_nTimerCnt)
	  OnFail();
	if(nIDEvent == 1 && m_nStartTimerFin <= m_nStartTimerCnt)
	{
		KillTimer(1);
		CGameDoc* pDoc = GetDocument();
		for(int n = 0; n < pDoc->m_nRow; n++)
			for(int m = 0; m < pDoc->m_nCol; m++)
				pDoc->m_bShow[n][m] = false;
	}
	Invalidate();
	CView::OnTimer(nIDEvent);
}


void CGameView::HighestRecord(void)
{
	CGameDoc* pDoc = GetDocument();
	if(pDoc->GetHighScore() < pDoc->GetScore())
	{	
		int res=AfxMessageBox(_T("New best record!"), MB_OK);
		pDoc->SetHighScore(pDoc->GetScore());
		/*
		std::stringstream ss;
		ss << pDoc->GetHighScore();
		std::ofstream ofs("highscore.dat");
		ofs << ss.str();
		ofs.close();
		*/
	}	
	pDoc->SetScore(0);
}
