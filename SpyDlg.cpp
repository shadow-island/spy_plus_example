///////////////////////////////////////////////////////////////////////////////
// File: spydlg.cpp                 
// Author : Marius C.
// Last Changed:25-02-1998  by MCO
//
//                                                               
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spy.h"
#include "SpyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char *GTSZCLTRS[32] ={"STATIC",
							"EDIT",
							"LISTBOX",
							"COMBOBOX",
							"BUTTON",
							"DIALOG",
							"SYSTREEVIEW32",
							"SYSLISTVIEW32",
							"SYSHEADER32",
							"SCROLLBAR",
							"MDICLIENT",
							"SYSTABCONTROL32",
							"#",0};


////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CAboutDlg : public CDialog
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    Standard MFC Function (see help)
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
    public:
        CAboutDlg();
    
    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
    protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION: CAboutDlg::CAboutDlg
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CAboutDlg::DoDataExchange
//
//    PARAMETERS: 
//              CDataExchange* pDX
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CSpyDlg::CSpyDlg(CWnd* pParent /*=NULL*/)
////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CDialog
//
//    PARAMETERS: 
//              CSpyDlg::IDD
//               pParent
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
: CDialog(CSpyDlg::IDD, pParent),m_capture(0)
{
    //{{AFX_DATA_INIT(CSpyDlg)
	m_x = 400;
	m_y = 450;
	//}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    memset(m_counts,0,32*sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::DoDataExchange
//
//    PARAMETERS: 
//              CDataExchange* pDX
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSpyDlg)
    DDX_Control(pDX, IDC_LIST1, m_lb);
    DDX_Control(pDX, ST_FINDER, m_finder);
    DDX_Control(pDX, IDC_TREE1, m_wndtree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpyDlg, CDialog)
//{{AFX_MSG_MAP(CSpyDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, OnEndlabeleditTree1)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnInitDialog
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
BOOL CSpyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    // Add "About..." menu item to system menu.
    
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);   // Set big icon
    SetIcon(m_hIcon, FALSE);  // Set small icon
    
    // TODO: Add extra initialization here
    m_pWindow = NULL;
    m_imgList.Create(BMP_CONTROLS, 20, 0,RGB(255,255,255));//CImageList
    m_wndtree.SetImageList(&m_imgList,TVSIL_NORMAL);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnSysCommand
//
//    PARAMETERS: 
//              UINT nID
//               LPARAM lParam
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnPaint
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting
        
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnQueryDragIcon
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
HCURSOR CSpyDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnLButtonDown
//
//    PARAMETERS: 
//              UINT nFlags
//               CPoint point
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    
    
    RECT rt; m_finder.GetWindowRect(&rt);
    
    ScreenToClient(&rt);
    if(PtInRect(&rt,point))
    {
        m_capture = TRUE;
        SetCapture();
    }
    CDialog::OnLButtonDown(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnMouseMove
//
//    PARAMETERS: 
//              UINT nFlags
//               CPoint point
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_capture)
    {
        ClientToScreen(&point);
        CWnd*  pWindow = WindowFromPoint(point);
        if(pWindow && m_pWindow != pWindow && pWindow != this)
        {
            RECT rt;
            pWindow->GetClientRect(&rt);
            //pWindow->ScreenToClient(&rt);
            CDC *pdc= pWindow->GetDC();
            //pdc->DrawFocusRect(&rt);
            pdc->PatBlt(rt.left,rt.top,rt.right,4,PATINVERT);
            pdc->PatBlt(rt.left,rt.top,4,rt.bottom,PATINVERT);
            pdc->PatBlt(rt.right-4,rt.top,4,rt.bottom,PATINVERT);
            pdc->PatBlt(rt.left,rt.bottom-4,rt.right,4,PATINVERT);
            ReleaseDC(pdc);
            
            if(m_pWindow  && m_pWindow!= this )
            {
                m_pWindow->GetClientRect(&rt);
                CDC *pdc= m_pWindow->GetDC();
                pdc->PatBlt(rt.left,rt.top,rt.right,4,PATINVERT);
                pdc->PatBlt(rt.left,rt.top,4,rt.bottom,PATINVERT);
                pdc->PatBlt(rt.right-4,rt.top,4,rt.bottom,PATINVERT);
                pdc->PatBlt(rt.left,rt.bottom-4,rt.right,4,PATINVERT);
                
            }
            m_pWindow = pWindow;
            if(m_pWindow && m_pWindow!=this )
            {
                char ss[32];
                GetClassName(m_pWindow->m_hWnd,ss,32);
                m_wclass = ss;
                m_pWindow->GetWindowText(m_wname);
                
            }
            
        }
		SetCursor(::LoadCursor(NULL,IDC_CROSS));
    }
    CDialog::OnMouseMove(nFlags, point);
}


////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::ScanWindows
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::ScanWindows()
{
    CWnd* pcurent = m_pWindow;
    CWnd* pLast   = m_pWindow;
	CString cs;
	char ss[128];
	pLast->GetWindowText(ss,128);
	cs.Format("Walk [Chlidren Of '%s %X']",ss,pLast->m_hWnd);
	SetWindowText(cs);
    if(pLast = pLast->GetWindow(GW_CHILD))
	{
		 FillTree(pLast);
	}
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::FillTree
//
//    PARAMETERS: 
//              CWnd* pLast
//              HTREEITEM level
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
HTREEITEM CSpyDlg::FillTree(CWnd* pLast,HTREEITEM level)
{
    HTREEITEM htr   = level;
    CWnd*  pNext = pLast;
    CWnd*  pChild= pLast;
    do
    {
        htr = AddITreetem(pNext, level);
        if(pChild = pNext->GetWindow(GW_CHILD))
        {
            FillTree(pChild,htr);
        }
        m_wndtree.Expand( htr, TVE_EXPAND);
    }while(pNext = pNext->GetWindow(GW_HWNDNEXT));
    return htr;
}


////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::AddITreetem
//
//    PARAMETERS: 
//              CWnd* pLast
//              HTREEITEM level
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
HTREEITEM CSpyDlg::AddITreetem(CWnd* pLast,HTREEITEM level)
{
    HTREEITEM	htr;
    CString		cs;
    char		text[128];
    char		cname[32];
    char		wtext[32];
	char*		psz  ;
    int			index  = 0;
    int			id;
    
    pLast->GetWindowText(wtext,32);
    id = pLast->GetDlgCtrlID();
    GetClassName(pLast->m_hWnd,cname,32);
    AnsiUpper(cname);
    cs = cname;
    while(psz = GTSZCLTRS[index++])
    {
        if(cs == psz)
          break;
    }
    if((cs.GetLength()>=3) && cs[0]=='A' && cs[1]=='F' && cs[2]=='X')
	      index = 0; // MFC CRAPS
    if(cs[0]=='#')   // DIALOG
		  index = 6;
    m_counts[index]++;
    sprintf(text,"'%s'ID:%d HWND:%X)",wtext,id,pLast->m_hWnd);
    htr = m_wndtree.InsertItem(text,index,index,level);
    m_wndtree.SetItemData(htr,(DWORD)pLast->m_hWnd);
    return htr;
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnLButtonUp
//
//    PARAMETERS: 
//              UINT nFlags
//               CPoint point
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    RECT	rt;
    char	text[64];
    int		i = 0;
	char*	p ;
    
	ReleaseCapture();
    if(m_capture)
    {
        memset(m_counts,0,32*sizeof(int));
        m_wndtree.DeleteAllItems();//CTreeCtrl
        ClientToScreen(&point);
        m_pWindow = WindowFromPoint(point);
        if(m_pWindow!=this && m_pWindow)
        {
            ScanWindows();
            m_lb.ResetContent();
            while(p = GTSZCLTRS[i++])
            {
                sprintf(text,"%s \t\t\t   %d",p, m_counts[i]);
                m_counts[i]=0;
                m_lb.AddString(text);
            }
            m_pWindow->GetClientRect(&rt);
            CDC *pdc= m_pWindow->GetDC();
            pdc->PatBlt(rt.left,rt.top,rt.right,4,PATINVERT);
            pdc->PatBlt(rt.left,rt.top,4,rt.bottom,PATINVERT);
            pdc->PatBlt(rt.right-4,rt.top,4,rt.bottom,PATINVERT);
            pdc->PatBlt(rt.left,rt.bottom-4,rt.right,4,PATINVERT);
            ReleaseDC(pdc);
        }
    }
    m_capture=FALSE;
    CDialog::OnLButtonUp(nFlags, point);
}



////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnDestroy
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnDestroy()
{
    CDialog::OnDestroy();
    m_imgList.DeleteImageList();
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnButton1
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnButton1()
{
    HWND hw =(HWND)m_wndtree.GetItemData(m_wndtree.GetSelectedItem());
    if(::IsWindowEnabled(hw))
      ::EnableWindow(hw,FALSE);
    else
       ::EnableWindow(hw,TRUE);
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnButton2
//
//    PARAMETERS: 
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnButton2()
{
    // TODO: Add your control notification handler code here
    HWND hw =(HWND)m_wndtree.GetItemData(m_wndtree.GetSelectedItem());
    if(::IsWindowVisible(hw))
      ::ShowWindow(hw,SW_HIDE);
    else
        ::ShowWindow(hw,SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CSpyDlg::OnEndlabeleditTree1
//
//    PARAMETERS: 
//              NMHDR* pNMHDR
//               LRESULT* pResult
//    DESCRIPTION: 
//    RETURN:   <see prototype>
////////////////////////////////////////////////////////////////////////////////
void CSpyDlg::OnEndlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
    TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
    *pResult = 0;
}


BOOL CSpyDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
