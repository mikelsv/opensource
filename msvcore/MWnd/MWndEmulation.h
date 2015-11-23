#ifdef WIN32
HWND MWndCreateWindow(DWORD dwExStyle, VString lpClassName, VString lpWindowName, DWORD dwStyle, MRect rect, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam){
	return CreateWindowEx(dwExStyle, MODUNICODE(lpClassName), MODUNICODE(lpWindowName), dwStyle, rect.left, rect.top, rect.GetW(), rect.GetH(), hWndParent, hMenu, hInstance /*hinst*/, lpParam);
}

void MWndDestroyWindow(HWND hwnd){ DestroyWindow(hwnd); }
#else

#ifdef USEMSV_ANDROID
// Compile in WIN32, no linux types.
typedef void*	GC;
#endif

//typedef void* HDC;
typedef GC	  HDC;
typedef void* HFONT;
typedef void* HGDIOBJ;
typedef void* HBITMAP;
typedef HANDLE HMODULE;
typedef void* HBRUSH;
typedef void* HICON;
typedef void* HACCEL;
typedef void* HMENU;
typedef void VOID;
typedef void* HGLOBAL;
typedef void* HPALETTE;

#ifndef CONST
#define CONST const
#endif

#define VOID void

#define DEFAULT_GUI_FONT    17
#define DIB_RGB_COLORS      0 /* color table in RGBs */
#define DIB_PAL_COLORS      1 /* color table in palette indices */
#define OPAQUE              2

typedef WORD                ATOM;
typedef RECT*				LPRECT;
typedef POINT*				LPPOINT;
typedef const wchar_t*		LPCTSTR;
typedef CHAR*				LPSTR;
typedef float               FLOAT;
typedef int					INT;
typedef BYTE*				LPBYTE;

typedef const RECT			*LPCRECT;

typedef int INT_PTR, *PINT_PTR;
typedef unsigned int UINT_PTR, *PUINT_PTR;
typedef long LONG_PTR, *PLONG_PTR;
typedef unsigned long ULONG_PTR, *PULONG_PTR;

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;

typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef ULONG_PTR SIZE_T, *PSIZE_T;

#define CS_VREDRAW          0x0001
#define CS_HREDRAW          0x0002
#define CS_OWNDC            0x0020

#define HTERROR             (-2)
#define HTTRANSPARENT       (-1)
#define HTNOWHERE           0
#define HTCLIENT            1
#define HTCAPTION           2
#define HTSYSMENU           3
#define HTGROWBOX           4
#define HTSIZE              HTGROWBOX
#define HTMENU              5
#define HTHSCROLL           6
#define HTVSCROLL           7
#define HTMINBUTTON         8
#define HTMAXBUTTON         9
#define HTLEFT              10
#define HTRIGHT             11
#define HTTOP               12
#define HTTOPLEFT           13
#define HTTOPRIGHT          14
#define HTBOTTOM            15
#define HTBOTTOMLEFT        16
#define HTBOTTOMRIGHT       17
#define HTBORDER            18
#define HTREDUCE            HTMINBUTTON
#define HTZOOM              HTMAXBUTTON
#define HTSIZEFIRST         HTLEFT
#define HTSIZELAST          HTBOTTOMRIGHT
#define HTOBJECT            19
#define HTCLOSE             20
#define HTHELP              21

#define SC_MINIMIZE     0xF020
#define SC_MAXIMIZE     0xF030
#define SC_NEXTWINDOW   0xF040
#define SC_RESTORE      0xF120

#define CF_TEXT             1
#define CF_HDROP            15

// WM_SIZE message wParam values
#define SIZE_RESTORED       0
#define SIZE_MINIMIZED      1
#define SIZE_MAXIMIZED      2
#define SIZE_MAXSHOW        3
#define SIZE_MAXHIDE        4

#define SRCCOPY             (DWORD)0x00CC0020

// WINDOWS MESSAGES
#define WS_VISIBLE          0x10000000L
#define WS_CHILD            0x40000000L
#define WS_POPUP            0x80000000L

#define WS_EX_TOPMOST       0x00000008L

#define COLOR_BTNFACE           15
#define COLOR_BTNSHADOW         16
#define COLOR_BTNHIGHLIGHT      20

#define RDH_RECTANGLES  1

#define SWP_NOSIZE          0x0001
#define SWP_NOMOVE          0x0002

#define IMAGE_CURSOR        2

// WM_ACTIVATE state values
#define     WA_INACTIVE     0
#define     WA_ACTIVE       1
#define     WA_CLICKACTIVE  2

#define SWP_SHOWWINDOW      0x0040

#define BM_GETIMAGE        0x00F6
#define BM_SETIMAGE        0x00F7

#define PM_NOREMOVE         0x0000

#define MAKEINTRESOURCE(i) (LPSTR)((ULONG_PTR)((WORD)(i)))
#define IDC_ARROW           MAKEINTRESOURCE(32512)

#define HWND_TOPMOST    ((HWND)-1)


#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005
#define WM_ACTIVATE                     0x0006
#define WM_SETFOCUS                     0x0007
#define WM_KILLFOCUS                    0x0008
#define WM_ENABLE                       0x000A
#define WM_SETREDRAW                    0x000B
#define WM_SETTEXT                      0x000C
#define WM_GETTEXT                      0x000D
#define WM_GETTEXTLENGTH                0x000E
#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010
#define WM_QUIT                         0x0012
#define WM_ERASEBKGND                   0x0014

#define WM_MOUSEACTIVATE                0x0021
#define WM_GETMINMAXINFO                0x0024
#define WM_NCLBUTTONDOWN                0x00A1

#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102

#define WM_COMMAND                      0x0111
#define WM_SYSCOMMAND                   0x0112
#define WM_TIMER                        0x0113

#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#define WM_SIZING                       0x0214
#define WM_MOVING                       0x0216

#define WM_MOUSEWHEEL                   0x020A
#define WM_MOUSELEAVE                   0x02A3
#define WM_PRINT                        0x0317
#define WM_USER                         0x0400

#define PRF_CHECKVISIBLE    0x00000001L
#define PRF_NONCLIENT       0x00000002L
#define PRF_CLIENT          0x00000004L
#define PRF_ERASEBKGND      0x00000008L
#define PRF_CHILDREN        0x00000010L
#define PRF_OWNED           0x00000020L

#define CF_BITMAP           2

#define     GA_PARENT       1
#define     GA_ROOT         2
#define     GA_ROOTOWNER    3

#define COLORONCOLOR                 3

#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))

#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))


typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

typedef struct tagMSG{
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    POINT       pt;
} MSG, *PMSG, *NPMSG, *LPMSG;

typedef struct tagPAINTSTRUCT{
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;


#define CALLBACK
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagWNDCLASSEXA{
    UINT        cbSize;
    /* Win 3.x */
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
    /* Win 4.0 */
    HICON       hIconSm;
} WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;
typedef WNDCLASSEXA WNDCLASSEX;

typedef struct  tagXFORM{
    FLOAT   eM11;
    FLOAT   eM12;
    FLOAT   eM21;
    FLOAT   eM22;
    FLOAT   eDx;
    FLOAT   eDy;
} XFORM, *PXFORM, *LPXFORM;

typedef struct _RGNDATAHEADER{
    DWORD dwSize;
    DWORD iType;
    DWORD nCount;
    DWORD nRgnSize;
    RECT rcBound;
} 	RGNDATAHEADER;

typedef struct _RGNDATA{
    RGNDATAHEADER rdh;
    char Buffer[ 1 ];
} 	RGNDATA, *PRGNDATA, *NPRGNDATA, *LPRGNDATA;

typedef struct tagCREATESTRUCT{
    LPVOID      lpCreateParams;
    HINSTANCE   hInstance;
    HMENU       hMenu;
    HWND        hwndParent;
    int         cy;
    int         cx;
    int         y;
    int         x;
    LONG        style;
    LPCSTR      lpszName;
    LPCSTR      lpszClass;
    DWORD       dwExStyle;
} CREATESTRUCT, *LPCREATESTRUCT;

typedef struct tagBITMAP{
    LONG        bmType;
    LONG        bmWidth;
    LONG        bmHeight;
    LONG        bmWidthBytes;
    WORD        bmPlanes;
    WORD        bmBitsPixel;
    LPVOID      bmBits;
} BITMAP, *PBITMAP, *NPBITMAP, *LPBITMAP;

typedef struct tagMINMAXINFO {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;


BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax){ return 0; }
//LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){ return 0; }
BOOL PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg){ return 0; }

HMODULE GetModuleHandle(LPCSTR lpModuleName ){ return 0; }

int GetWindowText(HWND hWnd, LPSTR lpString, int nMaxCount){ return 0; }
BOOL SetWindowText(HWND hWnd, LPCSTR lpString){ return 0; }
//BOOL GetWindowRect(HWND hWnd, LPRECT lpRect){ return 0; }
BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags){ return 0; }
BOOL GetCursorPos(LPPOINT lpPoint){ return 0; }
HCURSOR SetCursor(HCURSOR hCursor){ return 0; }

int DrawText(HDC hDC, LPCSTR lpString, int nCount, LPRECT lpRect, UINT uFormat){ return 0;}
BOOL LineTo(HDC, int, int){ return 0; }
BOOL MoveToEx(HDC, int, int, LPPOINT){ return 0; }
BOOL Rectangle(HDC, int, int, int, int){ return 0; }

BOOL BitBlt(HDC, int, int, int, int, HDC, int, int, DWORD){ return 0; }
int GetDIBits(HDC, HBITMAP, UINT, UINT, LPVOID, LPBITMAPINFO, UINT){ return 0; }
HDC BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint){
//	return DefaultGC(mwnddisp, mwndscreen);
  return 0; }
BOOL EndPaint(HWND hWnd, CONST PAINTSTRUCT *lpPaint){ return 0; }


HRGN ExtCreateRegion(CONST XFORM *, DWORD, CONST RGNDATA *){ return 0; }
int SetWindowRgn(HWND hWnd, HRGN hRgn, BOOL bRedraw){ return 0; }
ATOM RegisterClassEx(CONST WNDCLASSEXA *){ return 0; }

BOOL OpenClipboard(HWND hWndNewOwner){ return 0; }
BOOL IsClipboardFormatAvailable(UINT format){ return 0; }
HANDLE GetClipboardData(UINT uFormat){ return 0; }
SIZE_T GlobalSize(HGLOBAL hMem){ return 0; }
LPVOID GlobalLock(HGLOBAL hMem){ return 0; }
#define CopyMemory RtlCopyMemory
VOID RtlCopyMemory(VOID *Destination, CONST VOID *Source, SIZE_T Length){ return ; }
BOOL GlobalUnlock(HGLOBAL hMem){ return 0; }
BOOL CloseClipboard(VOID){ return 0; }

HWND CreateWindowEx(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance,LPVOID lpParam){ return 0; }
BOOL CloseWindow(HWND hWnd){ return 0; }
BOOL ShowWindow(HWND hWnd, int nCmdShow){ return 0; }
LRESULT DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){ return 0; }
VOID SwitchToThisWindow( HWND hwnd, BOOL fUnknown){ return ; }
BOOL DestroyWindow( HWND hWnd){ return 0; }

BOOL TranslateMessage(CONST MSG *lpMsg){ return 0; }
LRESULT DispatchMessage(CONST MSG *lpMsg){ return 0; }
VOID PostQuitMessage(int nExitCode){ return ; }

HACCEL LoadAccelerators(HINSTANCE hInstance, LPCSTR lpTableName){ return 0; }
int TranslateAccelerator(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg){ return 0; }

void InvalidateRect(HWND hWnd, LPCRECT lpRect, BOOL bErase = TRUE ){  }
void ValidateRect(HWND hWnd, LPCRECT lpRect){  }

HWND GetAncestor(HWND hwnd, UINT gaFlags){ return 0; }
HBITMAP CreateCompatibleBitmap(HDC hdc, int cx, int cy){ return 0; }

BOOL PrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags){ return 0; }

LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){ return 0; }

BOOL EmptyClipboard(VOID){ return 0; }
HANDLE SetClipboardData(UINT uFormat, HBITMAP hMem){ return 0; }
BOOL GetClientRect(HWND hWnd, LPRECT lpRect){ return 0; }

//HDC GetWindowDC(HWND hWnd){ return 0; }

#endif

#if defined(__GNUC__)
	HDC CreateCompatibleDC(HDC){ return 0; }
	HFONT CreateFontIndirect(const tagLOGFONTA *){ return 0; }
	int GetObject(HGDIOBJ, int, LPVOID){ return 0; }
	HGDIOBJ SelectObject(HDC, HGDIOBJ){ return 0; }
	//HDC GetDC(HWND hWnd){ return 0; }
	HGDIOBJ GetStockObject(int){ return 0; }
	int ReleaseDC(HWND hWnd, HDC hDC){ return 0; }
	BOOL DeleteObject(HGDIOBJ){ return 0; }
	BOOL DeleteDC(HDC){ return 0; }

	COLORREF SetTextColor(HDC, COLORREF){ return 0; }
	DWORD GetSysColor(int nIndex){ return 0; }
	BOOL GetTextExtentPoint32(HDC, LPCSTR, int, LPSIZE){ return 0; }
	COLORREF SetBkColor(HDC, COLORREF){ return 0; }
	int SetBkMode(HDC, int){ return 0; }

	HBITMAP CreateDIBSection(HDC, CONST BITMAPINFO *, UINT, VOID **, HANDLE, DWORD){ return 0; }
	int SetStretchBltMode(HDC hdc, int mode){ return 0; }
	int StretchDIBits(HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight,
	CONST VOID * lpBits, CONST BITMAPINFO * lpbmi, UINT iUsage, DWORD rop){ return 0; }

	BOOL PrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags){ return 0; }
	VOID SwitchToThisWindow( HWND hwnd, BOOL fUnknown){ return ; }
#endif


#if defined(USEMSV_FLASH) || defined(USEMSV_ANDROID)
HWND MWndCreateWindow(DWORD dwExStyle, VString lpClassName, VString lpWindowName, DWORD dwStyle, MRect rect, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam){
	return 0;
}

void MWndDestroyWindow(HWND hwnd){ }

GC GetDC(HWND h){ return 0; }
HDC GetWindowDC(HWND hWnd){ return 0; }
BOOL GetWindowRect(HWND hWnd, LPRECT lpRect){ return 0; }
#endif





// Virtual CxImage
#ifdef USEMSV_CXIMAGE_EMULATE

/*class CxImage{

public:
	CxImage(){
		return ;
	}
	*/

	CxImage::CxImage(DWORD imagetype){}

	void* CxImage::Create(DWORD x, DWORD y, DWORD bpp, DWORD type){ return 0; }
	bool CxImage::Destroy(){ return 1; }

	bool CxImage::CreateFromArray(BYTE* pArray,DWORD dwWidth,DWORD dwHeight,DWORD dwBitsperpixel, DWORD dwBytesperline, bool bFlipImage){ return 0; }
	bool CxImage::CreateFromHBITMAP(HBITMAP hbmp, HPALETTE hpal){ return 0; }

	bool CxImage::Decode(BYTE*, DWORD, DWORD){ return 0; }

	bool CxImage::IsValid()const{ return 0; }
	WORD CxImage::GetBpp()const{ return 0; }
	DWORD CxImage::GetWidth()const{ return 0; }
	DWORD CxImage::GetHeight()const{ return 0; }
	DWORD CxImage::GetEffWidth()const{ return 0; }
//	BYTE* CxImage::GetInfo(){ return 0; }
//	BYTE* CxImage::GetData(){ return 0; }

	int Draw2(HDC, int x, int y, int cx, int cy){ return 0; }
	int Draw2(HDC, RECT r){ return 0; }
	int DrawString(HDC , int x, int y, const char* text, RGBQUAD rgb, const char *){ return 0; }

	RGBQUAD CxImage::GetPixelColor(long x, long y, bool bGetAlpha){ RGBQUAD rgb; rgb.rgbBlue=0; rgb.rgbGreen=0; rgb.rgbRed=0; return rgb; }
	void CxImage::SetPixelColor(long x, long y, RGBQUAD rgb, bool){ return ; }
	void CxImage::SetPixelColor(long x, long y, COLORREF col){ return ; }
	
	bool CxImage::Crop(const RECT &r, CxImage *){ return 0; }

	void CxImage::Clear(BYTE){ return; }

//	bool CxImage::CreateFromHBITMAP(HBITMAP, HPALETTE hpal){ return 0; }
//	HBITMAP CxImage::MakeBitmap(HDC hdc){ return 0; }

	bool CxImage::Load(const TCHAR* filename, DWORD imagetype){ return 0; }
	bool CxImage::Save(const TCHAR* filename, DWORD imagetype){ return 0; }
	/*
};

#define CXIMAGE_FORMAT_BMP 0
#define CXIMAGE_FORMAT_PNG 0
#define CXIMAGE_FORMAT_UNKNOWN 0
*/
#endif