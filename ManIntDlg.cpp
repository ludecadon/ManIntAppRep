
// ManIntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ManIntApp.h"
#include "ManIntDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CManIntDlg dialog



CManIntDlg::CManIntDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManIntDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CManIntDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENU_NEW, NewDropDownButton);
	DDX_Control(pDX, IDC_MENU_EDIT, EditDropDownButton);
	DDX_Control(pDX, IDC_FIND, FindButton);
	DDX_Control(pDX, IDC_HIERARCHIES_BOX, HierarchyScopesField);
	DDX_Control(pDX, IDC_EQUIP_LEVEL_LIST, EquipmentLevelField);
	DDX_Control(pDX, IDC_EQUIP_ID_BOX, EquipmentIDField);
	DDX_Control(pDX, IDC_EQUIP_DESC_BOX, EquipDescriptionField);
	DDX_Control(pDX, IDC_PROPERTY_TREE, EquipmentPropertyTreeField);
	DDX_Control(pDX, IDC_EQUIP_PROP_VAL_FIELD, EquipPropValueField);
	DDX_Control(pDX, IDC_EQUIP_PROP_DESC_FIELD, EquipPropDescriptionField);
	DDX_Control(pDX, IDC_EDIT4, NewEquipLevelField);
}

BEGIN_MESSAGE_MAP(CManIntDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FIND, &CManIntDlg::OnBnClickedFind)
	ON_CBN_SELCHANGE(IDC_HIERARCHIES_BOX, &CManIntDlg::OnCbnSelchangeHierarchiesBox)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PROPERTY_TREE, &CManIntDlg::OnTvnSelchangedPropertyTree)
	ON_BN_CLICKED(IDC_ADD_LEVEL_BUTTON, &CManIntDlg::OnBnClickedAddLevelButton)
END_MESSAGE_MAP()


// CManIntDlg message handlers

BOOL CManIntDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Load Hierarchy Scopes and Equipment Levels, as these are generally applicable
	m_stateManager.ConnectToDatabase("ManIntDB", "sa", "30Chapman");
	m_stateManager.LoadFromDatabase(&m_viewState);

	FillEquipmentLevelsField();
	FillHierarchiesField();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManIntDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CManIntDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CManIntDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CManIntDlg::FillEquipmentLevelsField()
{
	for (StructureLayer::EquipmentLevel level : m_viewState.equipmentLevels)
	{
		int pos = EquipmentLevelField.AddString(CString(level.description.c_str()));
		EquipmentLevelField.SetItemData(pos, level.ID);
	}

	EquipmentLevelField.SetCurSel(0);
}

void CManIntDlg::FillHierarchiesField()
{
	for (StructureLayer::HierarchyScope scope : m_viewState.hierarchyScopes)
	{
		int pos = HierarchyScopesField.AddString(CString(scope.hierarchy.c_str()));
		HierarchyScopesField.SetItemData(pos, scope.ID);
	}

	HierarchyScopesField.SetCurSel(0);
}

void CManIntDlg::FillEquipmentPropertyTreeField()
{
	for (int i = 0; i < m_viewState.equipmentProperties.size(); i++)
	{
		HTREEITEM h = EquipmentPropertyTreeField.InsertItem(CString(m_viewState.equipmentProperties[i].GetRoot()->data.ID.c_str()), TVI_ROOT);
		PopulatePropertyBranch(h, m_viewState.equipmentProperties[i].GetRoot());
		EquipmentPropertyTreeField.Expand(h, TVE_EXPAND);
	}
}

void CManIntDlg::PopulatePropertyBranch(HTREEITEM hLast, ViewModelLayer::EquipmentPropertyNode *parentNode)
{
	for (int i = 0; i < parentNode->children.size(); i++)
	{
		HTREEITEM h = EquipmentPropertyTreeField.InsertItem(CString(parentNode->children[i]->data.ID.c_str()), hLast);
		if (parentNode->children[i]->children.size())
		{
			PopulatePropertyBranch(h, parentNode->children[i]);
		}
	}
}

void CManIntDlg::OnBnClickedFind()
{
	//clear certain fields first
	EquipDescriptionField.Clear();
	EquipmentPropertyTreeField.DeleteAllItems();
	EquipPropValueField.Clear();
	EquipPropDescriptionField.Clear();
	for (int i = 0; i < m_viewState.equipmentProperties.size(); i++)
	{
		m_viewState.equipmentProperties[i].DestroyTree();
	}

	m_viewState.equipmentProperties.clear();

	CString dlgText;
	EquipmentIDField.GetWindowTextW(dlgText);

	//validate data in hierarchy and ID field
	if (m_viewState.currentHierarchyScope->ID > 0 && dlgText.GetLength() > 0)
	{
		//find the item in the database
		char textmbcs[256] = { 0 };
		wcstombs(textmbcs, dlgText.GetBuffer(), 255);
		if (m_stateManager.GetViewForEquipment(m_viewState.currentHierarchyScope->hierarchy, textmbcs, &m_viewState))
		{
			//if it exists, update the UI from the view state
			EquipDescriptionField.SetWindowText(CString(m_viewState.equipment.description.c_str()));
			
			for (int i = 0; i < EquipmentLevelField.GetCount(); i++)
			{
				if (EquipmentLevelField.GetItemData(i) == m_viewState.currentEquipLevel->ID)
				{
					EquipmentLevelField.SetCurSel(i);
					break;
				}
			}

			FillEquipmentPropertyTreeField();
		}
		else
		{
			char errmsg[256] = { 0 };
			sprintf(errmsg, "Couldn't find '%s' in scope '%s'", textmbcs, m_viewState.currentHierarchyScope->hierarchy.c_str());
			MessageBox(CString(errmsg));
		}
	}
	else
	{
		MessageBox(CString("Please ensure you have selected a valid hierarchy and entered data into the ID field."));
	}
}


void CManIntDlg::OnCbnSelchangeHierarchiesBox()
{
	int sel = HierarchyScopesField.GetCurSel();
	int id = HierarchyScopesField.GetItemData(sel);

	for (int i = 0; i < m_viewState.hierarchyScopes.size(); i++)
	{
		if (m_viewState.hierarchyScopes[i].ID == id)
		{
			m_viewState.currentHierarchyScope = &m_viewState.hierarchyScopes[i];
			break;
		}
	}
}

void CManIntDlg::OnTvnSelchangedPropertyTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	HTREEITEM h = EquipmentPropertyTreeField.GetSelectedItem();
	CString dlgText = EquipmentPropertyTreeField.GetItemText(h);
	char mbcs[256] = { 0 };
	wcstombs(mbcs, dlgText.GetBuffer(), 255);

	for (int i = 0; i < m_viewState.equipmentProperties.size(); i++)
	{
		ViewModelLayer::EquipmentPropertyNode *found = m_viewState.equipmentProperties[i].FindNode(mbcs);
		if (found != nullptr)
		{
			EquipPropValueField.SetWindowText(CString(found->data.value.c_str()));
			EquipPropDescriptionField.SetWindowTextW(CString(found->data.description.c_str()));
		}
	}

	*pResult = 0;
}


void CManIntDlg::OnBnClickedAddLevelButton()
{
	//insert value from text field into the database and, if successful, the level field
	CString dlgText;
	NewEquipLevelField.GetWindowTextW(dlgText);
	char mbcs[256] = { 0 };
	wcstombs(mbcs, dlgText, 255);
	bool found = false;
	int highestId = -1;

	//first check that it isn't already in the level field before bothering the database
	for (int i = 0; i < m_viewState.equipmentLevels.size(); i++)
	{
		if (m_viewState.equipmentLevels[i].ID > highestId)
			highestId = m_viewState.equipmentLevels[i].ID;

		if (std::string(mbcs) == m_viewState.equipmentLevels[i].description)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		//now add it to the database
//		m_stateManager.AddRowsToDatabase()
	}
}
