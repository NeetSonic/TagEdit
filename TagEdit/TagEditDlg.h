#pragma once
#include "AudioFile.h"
#include "afxwin.h"
#include "afxcmn.h"

extern bool g_bAddPic;

enum ChildThreadMessageWParam { 
	CTMsgW_UpdateFileCount, 
	CTMsgW_EndProc, 
	CTMsgW_DisableControls, 
	CTMsgW_ReceiveFileCount 
};

class CTagEditDlg : public CDialogEx {
public:
	CTagEditDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_TAGEDIT_DIALOG };

private:
	CButton btnAddPic;
	CProgressCtrl pcFileFinished;
	void EnableControls(const BOOL & bEnable = TRUE);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonReverseFileName();
	afx_msg void OnBnClickedButtonConvertToFlac();
	afx_msg void OnBnClickedButtonOpenMulFile();
	afx_msg LRESULT OnChildThreadMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckAddPicture();
};
