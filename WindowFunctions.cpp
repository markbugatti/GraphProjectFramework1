#include "main.h"

INT nTopFigure, nTopMyFigure, nPtCounter;	// top of windows stack
RECT WndRect; // window size

			  // Objects windows
Figure *Window[MAX_FIGURES];
MyFigure myWindow[MAX_FIGURES];

/* mode selector */
BOOL bModeFigure = true; BOOL bModeMyFigure = false;
/**/

// dialog box
TCHAR szBuffEdtHeight[EDIT_BUFFSIZE], szBuffEdtWidth[EDIT_BUFFSIZE], // text buffers to dialogbox
szBuffEdtPenWidth[EDIT_BUFFSIZE], szBuffEdtCount[EDIT_BUFFSIZE];
INT  nEdtHeight, nEdtWidth, nEdtPenWidth, nEdtCount; // value of edits
BOOL bRBRectChecked = false, bRBElpsChecked = false, bRBTrglChecked = false, bErase = false;
COLORREF PenColor = RGB(0, 0, 0);
CHOOSECOLOR colorDlg;
COLORREF acrCustClr[16];

// to draw
HPEN hWindowPen;
HDC  hdc;
PAINTSTRUCT ps;

// difference between pointer and [x,y] position of figure 
INT nCrntDiffX;
INT nCrntDiffY;

// it shows whether figure is captured or no
BOOL bCptred = false;

// screen buffer buffer
HBITMAP hBuff = NULL;
HDC hMem = NULL;

POINT ptMouse;// current mouse pt
POINT ptMousePrev;
POINT ptMouseOld; // old mouse point to return figure in case 

ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProc, WCHAR *szClassName) {
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // WNDPROC
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance; // HINSTANCE
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHPROJECT_ICON));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDM_MENU);
	wcex.lpszClassName = szClassName; // WCHAR
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_GRAPHPROJECT_ICON));
	return RegisterClassExW(&wcex);
}

void CreateMainWnd(HWND &hWnd, HINSTANCE hInst, INT nCmdShow, LPWSTR ClassName, LPWSTR TitleName)
{
	hWnd = CreateWindowW(ClassName, TitleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

void ColorDlgInit(HWND &hWnd) {
	colorDlg.lStructSize = sizeof(CHOOSECOLOR);
	colorDlg.hwndOwner = hWnd;
	colorDlg.rgbResult = PenColor;
	colorDlg.Flags = CC_RGBINIT | CC_FULLOPEN;
	colorDlg.lpCustColors = (LPDWORD)acrCustClr;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		/* Radio for figure init*/
		CheckRadioButton(hWnd, IDC_RADIO_RECTANGLE, IDC_RADIO_TRIANGLE, IDC_RADIO_RECTANGLE);
		bRBRectChecked = true;
		/**/
		/* Radio for myFigure init*/
		if(bModeFigure)
			CheckRadioButton(hWnd, IDC_RADIO_FIGUREMODE, IDC_RADIO_MYFIGUREMODE, IDC_RADIO_FIGUREMODE);
		else
			CheckRadioButton(hWnd, IDC_RADIO_FIGUREMODE, IDC_RADIO_MYFIGUREMODE, IDC_RADIO_MYFIGUREMODE);
		bErase = false;
		/**/
		/* color dialog init*/
		ColorDlgInit(hWnd);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO_RECTANGLE:
			bRBRectChecked = true;
			bRBElpsChecked = false;
			bRBTrglChecked = false;
			break;
		case IDC_RADIO_ELLIPSE:
			bRBRectChecked = false;
			bRBElpsChecked = true;
			bRBTrglChecked = false;
			break;
		case IDC_RADIO_TRIANGLE:
			bRBRectChecked = false;
			bRBElpsChecked = false;
			bRBTrglChecked = true;
			break;
		case IDC_RADIO_FIGUREMODE:
			if (bModeMyFigure) {
				bModeMyFigure = false;
				bErase = true;
			}
			bModeFigure = true;
			break;
		case IDC_RADIO_MYFIGUREMODE:
			if (bModeFigure) {
				bModeFigure = false;
				bErase = true;
			}
			bModeMyFigure = true;
			break;
		case IDC_BUTTON_COLLOR:
			if (ChooseColor(&colorDlg)) {
				PenColor = colorDlg.rgbResult;
			}
			break;
		case IDOK:
			GetWindowText(GetDlgItem(hWnd, IDC_EDIT_HEIGHT), szBuffEdtHeight, EDIT_BUFFSIZE);
			GetWindowText(GetDlgItem(hWnd, IDC_EDIT_WIDTH), szBuffEdtWidth, EDIT_BUFFSIZE);
			GetWindowText(GetDlgItem(hWnd, IDC_EDIT_PENWIDTH), szBuffEdtPenWidth, EDIT_BUFFSIZE);
			GetWindowText(GetDlgItem(hWnd, IDC_EDIT_VERTEXCOUNT), szBuffEdtCount, EDIT_BUFFSIZE);
		case IDCANCEL:
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

SIZE sizeDelta;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		GetClientRect(hWnd, &WndRect);
		nTopFigure = nTopMyFigure = -1; // no Window
		hdc = GetDC(hWnd);
		hMem = CreateCompatibleDC(hdc); // virtual dc, a buffer
		hBuff = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT); // bitmap
		SelectObject(hMem, hBuff);  // select Bitmap on virtual dc
		PatBlt(hMem, 0, 0, WndRect.right, WndRect.bottom, WHITENESS);
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_FILE_CREATE:
			if (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_NEW), hWnd, DialogProc) == IDOK)
			{
				nEdtPenWidth = _ttoi(szBuffEdtPenWidth);
				if (bModeFigure)
				{
					nEdtHeight = _ttoi(szBuffEdtHeight);
					nEdtWidth = _ttoi(szBuffEdtWidth);
					if (bErase) {
						if (nTopFigure > 0)
							EnableMenuItem(GetMenu(hWnd), IDM_FILE_DELETE, MF_ENABLED);
						ObjectDraw(hMem, nTopFigure, hWindowPen, Window, hWnd, WndRect);
						bErase = false;
					}
					if (nEdtHeight && nEdtWidth && nEdtPenWidth) {	
						if (!++nTopFigure)
							EnableMenuItem(GetMenu(hWnd), IDM_FILE_DELETE, MF_ENABLED);
						if (bRBRectChecked)
						{
							Rectangles *tmp = new Rectangles;
							ObjectInit(tmp, nTopFigure);
							bRBRectChecked = false;
						}
						if (bRBElpsChecked)
						{
							Ellipses *tmp = new Ellipses;
							ObjectInit(tmp, nTopFigure);
							bRBElpsChecked = false;
						}
						if (bRBTrglChecked)
						{
							Triangles *tmp = new Triangles;
							ObjectInit(tmp, nTopFigure);
							bRBTrglChecked = false;
						}
						
					}
	 				ObjectDraw(hMem, nTopFigure, hWindowPen, Window, hWnd, WndRect);
				}
				if (bModeMyFigure)
				{
					if (bErase) {
						if(nTopMyFigure > 0)
							EnableMenuItem(GetMenu(hWnd), IDM_FILE_DELETE, MF_ENABLED);
						MyObjectDraw(hMem, nTopMyFigure, hWindowPen, myWindow, hWnd, WndRect);
						bErase = false;
					}
					if ((nEdtCount = _ttoi(szBuffEdtCount)) && nEdtPenWidth) {
						nPtCounter = 0;
						if (!++nTopMyFigure)
							EnableMenuItem(GetMenu(hWnd), IDM_FILE_DELETE, MF_ENABLED);
						myWindow[nTopMyFigure].setColor(PenColor);
						myWindow[nTopMyFigure].setFrameWidth(nEdtPenWidth);
					}

					MyObjectDraw(hMem, nTopMyFigure, hWindowPen, myWindow, hWnd, WndRect);
				}
			}
			break;
		case IDM_FILE_DELETE:			
			if (bModeFigure && nTopFigure > -1) {
				delete Window[nTopFigure];
				if(--nTopFigure == -1)
					EnableMenuItem(GetMenu(hWnd), IDM_FILE_DELETE, MF_GRAYED);
				ObjectDraw(hMem, nTopFigure, hWindowPen, Window, hWnd, WndRect);
			}
			if (bModeMyFigure && nTopMyFigure > -1) {
				myWindow[nTopMyFigure].init();//my Figure
				if(--nTopMyFigure == -1)
					EnableMenuItem(GetMenu(hWnd), IDM_FILE_DELETE, MF_GRAYED);
				MyObjectDraw(hMem, nTopFigure, hWindowPen, myWindow, hWnd, WndRect);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
		if ((nTopFigure > -1 && bModeFigure)
			|| (nTopMyFigure > -1 && bModeMyFigure)) {
			ptMousePrev.x = ptMouse.x = LOWORD(lParam);
			ptMousePrev.y = ptMouse.y = HIWORD(lParam);
			if (bModeFigure)
			{
				if (nTopFigure > 0)
				{
					for (int i = nTopFigure; i >= 0; i--) // changing place in stack
						if (Window[i]->isClicked(ptMouse)) {
							Figure *tmp = Window[i];
							for (int j = i; j < nTopFigure; j++)
							{
								Window[j] = Window[j + 1];
							}
							Window[nTopFigure] = tmp;
							i = -1;
						}
				}
				if (Window[nTopFigure]->isClicked(ptMouse))
				{
					ptMouseOld.x = Window[nTopFigure]->getX();
					ptMouseOld.y = Window[nTopFigure]->getY();
					nCrntDiffX = ptMouse.x - Window[nTopFigure]->getX();
					nCrntDiffY = ptMouse.y - Window[nTopFigure]->getY();
					bCptred = true;
					SetCapture(hWnd);
					ObjectDraw(hMem, nTopFigure, hWindowPen, Window, hWnd, WndRect);
				}
			}
			if (bModeMyFigure)
			{
				if (nPtCounter < nEdtCount) {
					if (nPtCounter == nEdtCount - 1) {
						myWindow[nTopMyFigure].setCurrPt(ptMouse, nPtCounter);
						myWindow[nTopMyFigure].setCurrPt(myWindow[nTopMyFigure].getCurrPt(0), ++nPtCounter);
						myWindow[nTopMyFigure].initRgn(hMem);
						MyObjectDraw(hMem, nTopMyFigure, hWindowPen, myWindow, hWnd, WndRect);
					}
					else {
						myWindow[nTopMyFigure].setCurrPt(ptMouse, nPtCounter);
						nPtCounter++;
					}
				}
				else {
					ptMouseOld = ptMouse;
					if (nTopMyFigure > 0)
					{
						for (int i = nTopMyFigure; i >= 0; i--) // changing place in stack
							if (myWindow[i].isClicked(ptMouse)) {
								MyFigure tmp = myWindow[i];
								for (int j = i; j < nTopMyFigure; j++)
									myWindow[j] = myWindow[j + 1];
								myWindow[nTopMyFigure] = tmp;
								i = -1;
							}
						}	
					if (myWindow[nTopMyFigure].isClicked(ptMouse)) { // перед сделать перестановку в стэке
						sizeDelta.cx = myWindow[nTopMyFigure].getCurrPt(0).x - ptMousePrev.x;
						sizeDelta.cy = myWindow[nTopMyFigure].getCurrPt(0).y - ptMousePrev.y;
						myWindow[nTopMyFigure].setDelta(sizeDelta);
						bCptred = true;
						SetCapture(hWnd);
						MyObjectDraw(hMem, nTopMyFigure, hWindowPen, myWindow, hWnd, WndRect);
					}	
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		if (bCptred)
		{
			ptMouse.x = LOWORD(lParam);
			ptMouse.y = HIWORD(lParam);
			if (!PtInRect(&WndRect, ptMouse))
			{
				if (bModeFigure)
				{
					Window[nTopFigure]->setX(ptMouseOld.x);
					Window[nTopFigure]->setY(ptMouseOld.y);
					ObjectDraw(hMem, nTopFigure, hWindowPen, Window, hWnd, WndRect);
				}
				if (bModeMyFigure)
				{
					myWindow[nTopMyFigure].move(ptMouseOld, ptMousePrev, hMem);
					myWindow[nTopMyFigure].initRgn(hMem);
					MyObjectDraw(hMem, nTopMyFigure, hWindowPen, myWindow, hWnd, WndRect);
				}
			}
			ReleaseCapture();
			bCptred = false;
		}
		break;
	case WM_MOUSEMOVE:
		if (bCptred)
		{	
			ptMouse.x = LOWORD(lParam);
			ptMouse.y = HIWORD(lParam);
			if (bModeFigure)
			{
				int tmpX = ptMouse.x - nCrntDiffX,
					tmpY = ptMouse.y - nCrntDiffY;
				Window[nTopFigure]->setX(tmpX);
				Window[nTopFigure]->setY(tmpY);
				ObjectDraw(hMem, nTopFigure, hWindowPen, Window, hWnd, WndRect);
			}
			if (bModeMyFigure)
			{
				myWindow[nTopMyFigure].move(ptMouse, ptMousePrev, hMem);
				myWindow[nTopMyFigure].initRgn(hMem);
				
				MyObjectDraw(hMem, nTopMyFigure, hWindowPen, myWindow, hWnd, WndRect);
				ptMousePrev = ptMouse;
			}
		}
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &WndRect);
		if (bModeFigure)
			ObjectDraw(hMem, nTopFigure, hWindowPen, Window, hWnd, WndRect);
		else
			MyObjectDraw(hMem, nTopMyFigure, hWindowPen, myWindow, hWnd, WndRect);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, WndRect.right, WndRect.bottom, hMem, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hMem);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}