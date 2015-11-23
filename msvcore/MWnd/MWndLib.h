// MWndLib

Versions mwnd_version[]={
	"0.4.0.0", "20.10.2013 03:11"
};

// You may add 
/*
#define USEMSV_GAMEBOX - add GameBox class
*/

#ifdef USEMSV_ANDROID
	#define USEMSV_CXIMAGE_NOLOADTAG
#endif

#ifdef __GNUC__
	//#define USEMSV_CXIMAGE_NOLOADTAG
	#define USEMSV_CXIMAGE_EMULATE
#endif

#ifndef USEMSV_MLIST
#error "Write #define USEMSV_MLIST to cpp!"
#endif

int main(int args, char* arg[]);
int RetType(const VString ext);
/////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#ifndef MSV_NOUSE_WINMAIN
#ifndef WINCE
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,	LPSTR lpCmdLine, int nCmdShow){
	return main(__argc, __argv);
}
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){
	return main(__argc, __argv);
}
#endif
#endif
#else

#endif


// Defines
#define MWND_DRAW_COPY 1
#define MWND_DRAW_RESIZE 2
#define MWND_DRAW_SLOW 4


// Structures
struct DINT{
	int x;
	int y;

	void Clear(){ x=0; y=0; }
};



// CxImage
#define MSV_USECXIMAGE
#include "../../../MSVImages/CxImage/ximage.h"
#include "../../../MSVImages/tiff/tiff.h"

// OpenGL data
int opengl_frame=0, opengl_frames=0;

#ifndef OPENGL_MAX_FREQ
#define OPENGL_MAX_FREQ       30   // максимальная частота обновления экрана
#endif

#ifndef OPENGL_MAX_TIME
#define OPENGL_MAX_TIME	500
#endif


// Includes
#include "MWndEmulation.h"
#include "MWndStyle.h"
#include "MWnd-font.h"
#include "MWndIPE-L2.h"
#include "MWndCore.h"
#include "MWndMenu.h"


#ifdef USEMSV_GAMEBOX
	#include "GameBox.h"
#endif

#ifdef USEMSV_HTMLRENDER
	#include "HtmlRender.h"
#endif


// CxImage Lib
#ifdef _DEBUG
#pragma comment(lib,"../../MSVImages/png/Debug/png.lib")
#pragma comment(lib,"../../MSVImages/jpeg/Debug/jpeg.lib")
#pragma comment(lib,"../../MSVImages/zlib/Debug/zlib.lib")
#pragma comment(lib,"../../MSVImages/tiff/Debug/tiff.lib")
#pragma comment(lib,"../../MSVImages/j2k/Debug/j2k.lib")
#pragma comment(lib,"../../MSVImages/j2k/Debug/j2k.lib")
#pragma comment(lib,"../../MSVImages/jbig/Debug/jbig.lib")
#pragma comment(lib,"../../MSVImages/jasper/Debug/jasper.lib")
#pragma comment(lib,"../../MSVImages/cximage/Debug/cximage.lib")
#else
#pragma comment(lib,"../../MSVImages/png/Release/png.lib")
#pragma comment(lib,"../../MSVImages/jpeg/Release/jpeg.lib")
#pragma comment(lib,"../../MSVImages/zlib/Release/zlib.lib")
#pragma comment(lib,"../../MSVImages/tiff/Release/tiff.lib")
#pragma comment(lib,"../../MSVImages/j2k/Release/j2k.lib")
#pragma comment(lib,"../../MSVImages/j2k/Release/j2k.lib")
#pragma comment(lib,"../../MSVImages/jbig/Release/jbig.lib")
#pragma comment(lib,"../../MSVImages/jasper/Release/jasper.lib")
#pragma comment(lib,"../../MSVImages/cximage/Release/cximage.lib")
#endif


// Return type
int RetType(const VString ext)
{	int type = 0;
	if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (ext == "gif")				type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (ext=="mng"||ext=="jng")	type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (ext == "ico")				type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")				type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (ext=="pcx")				type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
	else if (ext=="wbmp")				type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
	else if (ext=="wmf"||ext=="emf")	type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
	else if (ext=="j2k"||ext=="jp2")	type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
	else if (ext=="jbg")				type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
	else if (ext=="jp2"||ext=="j2k")	type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
	else if (ext=="jpc"||ext=="j2c")	type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
	else if (ext=="pgx")				type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
	else if (ext=="ras")				type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
	else if (ext=="pnm"||ext=="pgm"||ext=="ppm") type = CXIMAGE_FORMAT_PNM;
#endif
	else type = CXIMAGE_FORMAT_UNKNOWN;
	return type;
}