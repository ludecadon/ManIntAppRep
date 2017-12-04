
// ManIntDlg.h : header file
//

#pragma once

#include "ViewState.h"
#include "StateManager.h"
#include "afxmenubutton.h"
#include "afxwin.h"
#include "afxcmn.h"

// CManIntDlg dialog
class CManIntDlg : public CDialogEx
{
// Construction
public:
	CManIntDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MANINTAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	ViewModelLayer::ViewState		m_viewState;
	ViewModelLayer::StateManager	m_stateManager;

	HICON m_hIcon;

	void FillHierarchiesField();
	void FillEquipmentLevelsField();
	void FillEquipmentPropertyTreeField();
	void PopulatePropertyBranch(HTREEITEM hLast, ViewModelLayer::EquipmentPropertyNode *parentNode);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCMenuButton NewDropDownButton;
	CMFCMenuButton EditDropDownButton;
	CButton FindButton;
	CComboBox HierarchyScopesField;
	CListBox EquipmentLevelField;
	CEdit EquipmentIDField;
	afx_msg void OnBnClickedFind();
	afx_msg void OnCbnSelchangeHierarchiesBox();
	CEdit EquipDescriptionField;
	CTreeCtrl EquipmentPropertyTreeField;
	CEdit EquipPropValueField;
	CEdit EquipPropDescriptionField;
	afx_msg void OnTvnSelchangedPropertyTree(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit NewEquipLevelField;
	afx_msg void OnBnClickedAddLevelButton();
};
