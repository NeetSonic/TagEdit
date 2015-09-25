#include "stdafx.h"
#include "TagEdit.h"
#include "TagEditDlg.h"
#include "afxdialogex.h"
#include "cderr.h" // ���ڵ�ͬʱ�򿪵��ļ�������ʱ���׳�FNERR_BUFFERTOOSMALL����
#include "Standard.h"

#ifdef _DEBUG
#define new DEBUG_NEW            
#endif

bool g_bAddPic;

#define MAX_FILE_COUNT 500
#define DIALOG_CAPTION _T("TagEdit v1.0")
#define CONVERT_FLAC_BAT_SAVEPATH _T("F:\\convert.bat")
#define WriteFlacEncodeConmand(srcPath)\
	file.Write("flac -8 \"", 9);\
	file.Write(srcPath, strlen(srcPath));\
	file.Write("\"\r\n", 3)

unsigned int __stdcall threadTagEdit(PVOID pM) {
	CFileDialog fileDlg(true, NULL, L"*.mp3", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY, L"MP3�ļ�(*.mp3);WAVE�ļ�(*.wav);APE�ļ�(*.ape);FLAC�ļ�(*.flac)|*.mp3;*.wav;*.ape;*.flac||");
	fileDlg.m_ofn.nMaxFile = static_cast<DWORD>(MAX_FILE_COUNT * MAX_PATH);
	wchar_t buffer[MAX_FILE_COUNT * MAX_PATH];
	fileDlg.m_ofn.lpstrFile = buffer;
	memset(fileDlg.m_ofn.lpstrFile, 0, sizeof(buffer));
	int ret = fileDlg.DoModal();
	if(ret == IDCANCEL){
		if(CommDlgExtendedError() == FNERR_BUFFERTOOSMALL){ MessageBox(NULL, _T("�򿪵��ļ����������˸������ƣ�500����"), DIALOG_CAPTION, MB_OK); }
		return 0;
	}
	else if(ret == IDOK){
		SendMessage((HWND)pM, CHILD_THREAD_MESSAGE, CTMsgW_DisableControls, NULL);
		POSITION filePos = fileDlg.GetStartPosition();
		UINT nFileCount(0);
		CString chFilePath(_T(""));
		while(filePos != NULL){
			nFileCount++;
			fileDlg.GetNextPathName(filePos);
		}
		SendMessage((HWND)pM, CHILD_THREAD_MESSAGE, (WPARAM)CTMsgW_ReceiveFileCount, (LPARAM)nFileCount);
		nFileCount = 0;
		filePos = fileDlg.GetStartPosition();
		while(filePos != NULL){
			AudioFile a;
			chFilePath = fileDlg.GetNextPathName(filePos);
			if(a.Open(chFilePath) && (++nFileCount)){
				if(g_bAddPic){
					CString chPicPath(chFilePath.Left(chFilePath.ReverseFind('.') + 1));
					chPicPath += _T("png");
					if(PathFileExists(chPicPath)){ a.AddCoverPicture(chPicPath); }
					else{
						chPicPath = chPicPath.Left(chPicPath.ReverseFind('.') + 1);
						chPicPath += _T("jpg");
						if(PathFileExists(chPicPath)){ a.AddCoverPicture(chPicPath); }
					}
				}
				a.StartEdit();
				SendMessage((HWND)pM, CHILD_THREAD_MESSAGE, (WPARAM)CTMsgW_UpdateFileCount, (LPARAM)nFileCount);
			}
			else MessageBox(NULL, _T("�Ƿ��ļ�·��"), DIALOG_CAPTION, MB_OK);
		}
		SendMessage((HWND)pM, CHILD_THREAD_MESSAGE, (WPARAM)CTMsgW_EndProc, NULL);
	}
	return 0;
}

CTagEditDlg::CTagEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTagEditDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CTagEditDlg::OnPaint() {
	if(IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
HCURSOR CTagEditDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}
void CTagEditDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ADD_PICTURE, btnAddPic);
	DDX_Control(pDX, IDC_PROGRESS_FILE_FINISHED, pcFileFinished);
}
void CTagEditDlg::EnableControls(const BOOL & bEnable /*= TRUE*/) {
	GetDlgItem(IDC_BUTTON_CONVERT_TO_FLAC)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_OPEN_MUL_FILE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_REVERSE_FILE_NAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ADD_PICTURE)->EnableWindow(bEnable);
}

BEGIN_MESSAGE_MAP(CTagEditDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_MUL_FILE, &CTagEditDlg::OnBnClickedButtonOpenMulFile)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE_FILE_NAME, &CTagEditDlg::OnBnClickedButtonReverseFileName)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT_TO_FLAC, &CTagEditDlg::OnBnClickedButtonConvertToFlac)
	ON_BN_CLICKED(IDC_CHECK_ADD_PICTURE, &CTagEditDlg::OnBnClickedCheckAddPicture)
	ON_MESSAGE(CHILD_THREAD_MESSAGE, &CTagEditDlg::OnChildThreadMessage)
END_MESSAGE_MAP()

BOOL CTagEditDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//----------�Զ����ʼ��----------
	btnAddPic.SetCheck(TRUE);
	g_bAddPic = true;
	SetSysFont();
	GetDlgItem(IDC_STATIC_DISPLAY)->SetFont(&g_sysFont);
	pcFileFinished.SetPos(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
inline void ReverseFileName(const wchar_t* chFilePath) {
	CString csFilePath(chFilePath);
	int nFlag = csFilePath.ReverseFind('\\');
	CString csFileDir(csFilePath.Left(nFlag + 1));
	CString csFileName(csFilePath.Right(csFilePath.GetLength() - nFlag - 1));
	nFlag = csFileName.ReverseFind('.');
	CString csFileType(csFileName.Right(csFileName.GetLength() - nFlag));
	csFileName = csFileName.Left(nFlag);
	nFlag = csFileName.Find('-', 0);
	if(nFlag == -1){ return; }
	CString csNewName(csFileName.Right(csFileName.GetLength() - nFlag - 1));
	DropSpace(csNewName);
	csNewName += _T(" - ");
	CString csTemp(csFileName.Left(nFlag));
	DropSpace(csTemp);
	csNewName += csTemp;
	CString csNewPath(csFileDir);
	csNewPath += csNewName;
	csNewPath += csFileType;
	try{
		CFile::Rename(csFilePath, csNewPath);
	}
	catch(CFileException* pEx){
		pEx->ReportError();
		pEx->Delete();
	}
}
afx_msg LRESULT CTagEditDlg::OnChildThreadMessage(WPARAM wParam, LPARAM lParam) {
	switch(wParam){
		case CTMsgW_UpdateFileCount:{
			pcFileFinished.SetPos((int)lParam);
			CString cs;
			cs.Format(L"�Ѵ����ļ����� %d", (UINT)lParam);
			SetDlgItemText(IDC_STATIC_DISPLAY, cs);
		}break;
		case CTMsgW_EndProc:{
			SetDlgItemText(IDC_STATIC_DISPLAY, L"�������");
			EnableControls();
		}break;
		case CTMsgW_DisableControls:{
			EnableControls(FALSE);
		}break;
		case CTMsgW_ReceiveFileCount:{
			pcFileFinished.SetRange(0, (short)lParam);
			pcFileFinished.SetPos(0);
		}break;
	}
	return 0;
}
void CTagEditDlg::OnBnClickedButtonOpenMulFile() {
	_beginthreadex(NULL, 0, threadTagEdit, (void*)(this->m_hWnd), 0, NULL);
}
void CTagEditDlg::OnBnClickedButtonReverseFileName() {
	CFileDialog fileDlg(true, NULL, L"*.mp3", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY, L"MP3�ļ�(*.mp3);WAVE�ļ�(*.wav);APE�ļ�(*.ape);FLAC�ļ�(*.flac)|*.mp3;*.wav;*.ape;*.flac||");
	fileDlg.m_ofn.nMaxFile = static_cast<DWORD>(MAX_FILE_COUNT * MAX_PATH);
	wchar_t buffer[MAX_FILE_COUNT * MAX_PATH];
	fileDlg.m_ofn.lpstrFile = buffer;
	memset(fileDlg.m_ofn.lpstrFile, 0, sizeof(buffer));
	int ret = fileDlg.DoModal();
	if(ret == IDCANCEL){
		if(CommDlgExtendedError() == FNERR_BUFFERTOOSMALL){ MessageBox(_T("�򿪵��ļ����������˸������ƣ�500����"), DIALOG_CAPTION); }
		return;
	}
	else if(ret == IDOK){
		POSITION filePos = fileDlg.GetStartPosition();
		UINT nFileCount(0);
		CString chFileCount(_T(""));
		CString csFilePath(_T(""));
		while(filePos != NULL){
			csFilePath = fileDlg.GetNextPathName(filePos);
			if(PathFileExists(csFilePath) && (++nFileCount)){
				ReverseFileName(csFilePath);
				chFileCount.Format(_T("�Ѵ����ļ����� %d"), nFileCount);
				SetDlgItemText(IDC_STATIC_DISPLAY, chFileCount);
			}
			else MessageBox(_T("�Ƿ��ļ�·��"), DIALOG_CAPTION, MB_OK);
		}
		MessageBox(_T("��ת��ɣ�"), DIALOG_CAPTION, MB_OK);
		SetDlgItemText(IDC_STATIC_DISPLAY, L"������ϣ�");
	}
}
void CTagEditDlg::OnBnClickedButtonConvertToFlac() {
	CFileDialog fileDlg(true, NULL, L"*.wav", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY, L"WAVE�ļ�(*.wav)|*.wav||");
	fileDlg.m_ofn.nMaxFile = static_cast<DWORD>(MAX_FILE_COUNT * MAX_PATH);
	wchar_t buffer[MAX_FILE_COUNT * MAX_PATH];
	fileDlg.m_ofn.lpstrFile = buffer;
	memset(fileDlg.m_ofn.lpstrFile, 0, sizeof(buffer));
	int ret = fileDlg.DoModal();
	if(ret == IDCANCEL){
		if(CommDlgExtendedError() == FNERR_BUFFERTOOSMALL){ MessageBox(_T("�򿪵��ļ����������˸������ƣ�500����"), DIALOG_CAPTION); }
		return;
	}
	else if(ret == IDOK){
		POSITION filePos = fileDlg.GetStartPosition();
		CString csFilePath(_T(""));
		CFile file;			   
		file.Open(CONVERT_FLAC_BAT_SAVEPATH, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate);
		file.SetLength(0); 
		char chPathAnsi[MAX_PATH];
		while(filePos != NULL){
			csFilePath = fileDlg.GetNextPathName(filePos);
			if(PathFileExists(csFilePath)){
				memset(chPathAnsi, 0, sizeof(chPathAnsi));
				UnicodeToAnsi(csFilePath, chPathAnsi, MAX_PATH);
				WriteFlacEncodeConmand(chPathAnsi);
			}
			else MessageBox(_T("�Ƿ��ļ�·��"), DIALOG_CAPTION, MB_OK);
		}
		file.Write("pause", 5);
		file.Close();
		MessageBox(_T("ת���ļ�convert.bat�����ɣ�˫�����м���"), DIALOG_CAPTION, MB_OK);
		ShellExecute(NULL, L"open", L"F:\\", NULL, NULL, SW_SHOW);
	}
}
void CTagEditDlg::OnBnClickedCheckAddPicture() {
	g_bAddPic = !!(btnAddPic.GetCheck()); // ���������Ա������ܾ���C4800��intתbool
}
