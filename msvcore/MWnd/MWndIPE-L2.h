// Mini Image
struct minimimg{ int x, y; };

GLock xliblock;

class MWndIPE;

#ifdef MSV_USEXLIB

//#define USE_SHM
//#define USE_XV

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef USE_MEMFILE
 #ifndef USE_SHM
  #include <sys/mman.h>
 #endif
 #include <unistd.h>
 #include <fcntl.h>
#endif

class XGrabWnd{ public:
	unsigned int width, height, depth;

	Display *disp; int scr, init;
	Window hwnd;
	XVisualInfo vinfo;

	XImage *image;

#define __BEGIN_DECLS	extern "C" {
//#define __END_DECLS }

#include <sys/cdefs.h>
//#include <sys/ipc.h>
//#include <sys/shm.h>  

#ifdef USE_SHM
 #include <sys/ipc.h>
 #include <sys/shm.h>   
 #include <X11/extensions/XShm.h>

 XShmSegmentInfo Shminfo;
 bool Using_SHM;
#endif

#ifdef USE_XV
 #include <X11/extensions/Xvlib.h>
 
	XvPortID XVport;
	bool Using_XV;
#endif

XGrabWnd(){ disp=0; scr=0; hwnd=0; width=0; height=0; depth=0; init=0; }

void Failed(const char *what){
	print("Failed: ", what, "\r\n");
}

void GetWindowGeometry(){
    unsigned border_width;
    int xpos, ypos;
    Window root;

	//scr=DefaultScreen(disp);
	//hwnd=RootWindow(disp, scr);

	if(!disp || !hwnd){ return ; }
	
    
    //fprintf(stderr, "XGetGeometry...\n");
    XGetGeometry(disp, hwnd, &root, &xpos, &ypos, &width, &height, &border_width, &depth);
       // Failed("XGetGeometry");
    //}
    fprintf(stderr, "width=%u, height=%u, depth=%u\n", width, height, depth);
}

void Init(Display *d, Window h, unsigned int s){
	disp=d; hwnd=h; scr=s;

	print("G disp: ", disp ? "ok" : "FAIL", " window: ", hwnd ? "ok" : "FAIL", "\r\n");
	if(!disp || !hwnd){ return ; }

    XFlush(disp);
    XSync(disp, False);
    
    GetWindowGeometry();

#ifdef USE_SHM
    {
        Using_SHM = XShmQueryExtension(disp);
        if(!Using_SHM)
            Failed("XShmQueryExtension");
    }
#endif

#ifdef USE_XV
    {
        unsigned ver,rel,req,ev,err;
        Using_XV = XvQueryExtension(disp, &ver,&rel,&req,&ev,&err) == Success;
        if(!Using_XV){
            Failed("XvQueryExtension");
        }
        
        if(Using_XV){
            unsigned adaptors;
            XvAdaptorInfo *ai;
            XvQueryAdaptors(disp, mRootWin, &adaptors, &ai);
            
            Using_XV = false;
            for(unsigned a=0; a<adaptors; ++a)
                if(ai[a].type & (XvOutputMask | XvVideoMask)){
                    fprintf(stderr, "Considering XV adapter %u\n", a);
                    for(unsigned b=0; b<ai[a].num_ports; ++b){
                        XVport = ai[a].base_id + b;
                        if(!XvGrabPort(disp, XVport, CurrentTime)){
                            fprintf(stderr, "Using XV port %d\n", XVport);
                            Using_XV = true;
                        }
                    }
                }
        }
            
        if(Using_XV){
            Using_XV = false;
            
            unsigned formats;
            XvImageFormatValues *fo = XvListImageFormats(disp, XVport, (int *)&formats);
            for(unsigned a=0; a<formats; ++a)
                if(fo[a].type == XvRGB)
                    Using_XV = true;
            
            if(!Using_XV) fprintf(stderr, "No suitable colour format, not using XV\n");
        }
        
        if(Using_XV){
            XGCValues xgcv;
            GC gc = XCreateGC(disp, hwnd, 0, &xgcv);
        
            XvGetVideo(disp, XVport, hwnd, gc,
                       0,0,             width,height,
                       0,0, width,height);
        }
    }
#endif

    if(!XMatchVisualInfo(disp, scr, depth, DirectColor, &vinfo)){
        Failed("XMatchVisualInfo");
        return;
    }
}

const unsigned char *GetImage(Display *d, Window h, unsigned int s){	print("GetImage(),b  ");
	if(!init){ Init(d, h, s); init=1; }

	if(!d || !h){ print("FAIL  "); return 0; }

#ifdef USE_MEMFILE
    static struct MemoryFile
    {
#ifndef USE_SHM
        int MemoryFD;
#endif
        unsigned char *buf;
    public:
        MemoryFile(): buf(NULL)
        {
#ifdef USE_SHM
            FILE *fp = fopen(MEMORY_FILE.c_str(), "rt");
            if(!fp) return;
            
            int shmid = -1;
            fscanf(fp, "%d", &shmid);
            fclose(fp);
            buf = (unsigned char*) shmat(shmid, NULL, 0);
#else
            MemoryFD = open(MEMORY_FILE.c_str(), O_RDONLY);
            if(MemoryFD < 0) return;

            unsigned size = width*height*2;
            buf = (unsigned char*) mmap
                        (NULL, size, PROT_READ,
                         MAP_SHARED, MemoryFD,
                         0
                        );
            if(buf == (unsigned char*)(MAP_FAILED))
                buf = NULL;
#endif
        }
        ~MemoryFile()
        {
#ifdef USE_SHM
            shmdt(buf);
#else
            close(MemoryFD);
#endif
        }
    } MemoryFile;
    
    if(MemoryFile.buf) return MemoryFile.buf;
#endif


#ifdef USE_SHM
    if(!image && Using_SHM){
        fprintf(stderr, "XShmCreateImage...\n");
        image = XShmCreateImage(disp, vinfo.visual,
                  depth,
                  ZPixmap,
                  NULL,
                  &Shminfo,
                  width,
                  height);
        if(!image){
            Failed("XShmCreateImage");
        CancelSHM:
            Using_SHM = false;
            return GetImage();
        }
        
        fprintf(stderr, "shmget...\n");
        Shminfo.shmid = shmget(IPC_PRIVATE,
                               image->bytes_per_line * image->height,
                               IPC_CREAT | 0777);
        if(Shminfo.shmid < 0){
            perror("shmget");
        CancelSHM2:
            XDestroyImage(image); image=NULL;
            goto CancelSHM;
        }
        fprintf(stderr, "shmat...\n");
        Shminfo.shmaddr = (char *) shmat(Shminfo.shmid, 0, 0);
        if(Shminfo.shmaddr == (char *)-1){
            perror("shmat");
            goto CancelSHM2;
        }
        fprintf(stderr, "shmctl...\n");
        shmctl(Shminfo.shmid, IPC_RMID, 0),

        image->data = Shminfo.shmaddr;
        Shminfo.readOnly = False;
        fprintf(stderr, "XShmAttach...\n");
        if(!XShmAttach(disp, &Shminfo)){
            Failed("XShmAttach");
            goto CancelSHM2;
        }
        
        XSync(disp, False);
    }
#endif

	//print("GetImage(),XG=", image ? "1" : "0", "   ");

    /* If SHM failed or was disabled, try non-SHM way */
    if(!image){
        /*fprintf(stderr, "XGetImage...\n");
        fflush(stderr);*/
        
        image = XGetImage(disp, hwnd, 0, 0, width, height, AllPlanes, ZPixmap);
        if(!image){
            Failed("XGetImage");
        } else depth=image->bits_per_pixel;

		//print("d: ", itos(image->depth), " bitmap_pad: ", itos(image->bitmap_pad), " bitmap_unit: ", itos(image->bitmap_unit),
		//	" bits_per_pixel: ", itos(image->bits_per_pixel), "\r\n");
    }

	//print("GetImage(),XG>", image ? "1" : "0", "   ");
    
#ifdef USE_SHM
    if(Using_SHM){
        /* COPIES an image from the server */
    
        /*fprintf(stderr, "XSHMGetImage...\n");
        fflush(stderr);*/
        if(!XShmGetImage(disp, hwnd, image, 0, 0, AllPlanes)){
            Failed("XSHMGetImage");
        }
    }
#endif
    return image ? (unsigned char *)image->data : NULL;
}

void DeleteImage(void){
    if(!image) return;

#ifdef USE_SHM
    if(Using_SHM) return;
#endif

	XDestroyImage(image);
    image = NULL;
}

void XDone(void){
    if(!image) return;

	DeleteImage();
    
#ifdef USE_SHM
    if(Using_SHM){
        fprintf(stderr, "Destroying image\n");
        fflush(stderr);

        fprintf(stderr, "XShmDetach...\n");
        XShmDetach(disp, &Shminfo);
        fprintf(stderr, "XDestroyImage...\n");
        XDestroyImage(image);
        image = NULL;
        fprintf(stderr, "shmdt...\n");
        shmdt(Shminfo.shmaddr);
    }
#endif
}

}XGrabWnd;

#endif



// Virtual Image
class MWndVxImage{
	int width, height, bpp;
	BYTE *data;

public:
	MWndVxImage(){ width=0; height=0; bpp=0; data=0; }

	MWndVxImage(int x, int y, int b, BYTE *d){
		width=x; height=y; bpp=b; data=d;
	}

	void Create(int x, int y, int b, BYTE *d){
		width=x; height=y; bpp=b; data=d;
	}

	int IsValid(){ return data!=0; }
	int GetBpp(){ return bpp; }
	int GetWidth(){ return width; }
	int GetHeight(){ return height; }
	int GetEffWidth(){ return (((24 * width) + 31) / 32) * 4; }
	BYTE* GetData(){ return data; }
};

// Container
class MWndIPE_C{
	CxImage *img;
	MWndIPE *ipe;
	MWndVxImage *vim;

	friend class MWndIPE;

public:
	MWndIPE_C(CxImage &i){ img=&i; ipe=0; vim=0; }
	MWndIPE_C(MWndIPE &i){ img=0; ipe=&i; vim=0; }
	MWndIPE_C(MWndVxImage &i){ img=0; ipe=0; vim=&i; }

	int IsValid();
//	int GetBpp();
	int GetWidth();
	int GetHeight();
	int GetEffWidth();
	int GetAllSize();
	BYTE* GetData();
	SRect GetRect();
	//operator TLock*();
};


/////////////////// IPE
class MWndIPE : public TLock{
	BYTE *data;
//	HDC dc;
	int width, height, ewidth, dsize;
//	TLock lock;
#ifdef MSV_USEXLIB
	 XImage *ximage;
#else
	void *ximage;
#endif

public:
	MWndIPE(){ data=0; width=0; height=0; ewidth=0; dsize=0; ximage=0; }
	~MWndIPE(){ Lock(); _deln(data); UnLock(); }

	void TestRect(MRect &rc);

	int DrawImage(MWndIPE_C iim, MRect rc, MRect to, int cnf);
	int DrawImageTransp(MWndIPE_C iim, MRect rc, MRect to, MRGB rgb);
	int DrawImageTextM(VString line, MyFontCur &cur, MRect mr, MRGB rgb);

	// Draw
	int DrawBox(SRect rc, MRGB rgb);
	int DrawImage2(MWndIPE_C iim, SRect rc, SRect to, int cnf=0);

	// Draw text
	static SIZE GetTextSize(unsigned char *ln, unsigned char *to, const MyFontString *str, int rls=0);
	int DrawTextChar(const MyFontChar *chr, const MRect rc, MRect to, const MRGB rgb);
	int DrawText(const VString text, const MyFont2 &mf, const MRect rc, const MRGB rgb, int fontsize=18, const int style=0);

	//memset2
	void memset2(void *_to, void *from, int fs, int sz){
		unsigned char *to=(unsigned char*)_to, *p=to; int s=fs, ms;
		memcpy(to, from, min(fs, sz)); to+=fs; sz-=fs;
		while(sz){
			ms=min(s, sz);
			memcpy(to, p, ms); s*=2;
			to+=ms; sz-=ms;
		}

		return ;
	}

	// Check
	int Check(BYTE *p){
		if(p<data || p>=data+dsize){
			return 0;
		}
		return 1;
	}


	int New(int _x, int _y){
		if(width==_x && height==_y) return 2;
//		x-=50; y-=50;
#ifdef MSV_USEXLIB
		// return 0;
#endif
		UGLOCK(*this);
		BYTE *odata=data;

		width=_x; height=_y; ewidth=((((24 * width) + 31) / 32) * 4); dsize=ewidth*height;
		data=_newn(dsize); memset(data, 204, dsize); // normal: sz

#ifdef MSV_USEXLIB
		if(mwnddisp){
			print("NEW! ", mwnddisp ? "1" : "0", ximage ? "1" : "0",  data ? "1" : "0", ". ", itos(width), "-", itos(height), "\r\n");
			if(ximage){ XDestroyImage(ximage); ximage=0; odata=0; }
		}
#endif
		_del(odata);

		//for(int i=0; i<sz; i++){
		//	if(i<sz/2) data[i]=i/2;
		//	else data[i]=0;
		//}		
		return 1;
	}

	int NewHImage(HBITMAP hbmp, HDC dc_){
		if(!hbmp) return 0;
		//Del();
#ifndef NOUSEMSV_WINAPI
		BITMAP bm; BITMAPINFO bi;
		GetObject(hbmp, sizeof(BITMAP), (LPSTR) &bm);
		
        if(!New(bm.bmWidth, bm.bmHeight)) return 0;
		GetBI(bi);

        HDC dc = ::GetDC(NULL);
		if(!dc) return false;

        int ret=GetDIBits(dc, hbmp, 0, height, GetData(), (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
		
		//if(bm.bmBitsPixel==32){
		//	BYTE *a=GetData(), *e=a, *t=a+bi.bmiHeader.biSizeImage-4;
		//	for(a; a<t; a){
		//		*e++=*a++;
		//		*e++=*a++;
		//		*e++=*a++;
		//		a++;
		//	}
		//}


        ::ReleaseDC(NULL, dc);
		return ret;
#endif //NOUSEMSV_WINAPI
		return 1;
	}

	//int NewHImageText(HBITMAP hbmp, MRGB rgb){
	//	if(!NewHImage(hBmp)) return 0;

	//	for(int i=0; i<GetHeight(); i++){ 
	//		BYTE *f=GetData(), *t=f+x*3;
	//		while(f<t){
	//			if(*f==1)
	//		}
	//	}
	//}

//	int SetDC(HDC d){ dc=d; }

	void Del(){
		UGLOCK(*this);
		width=0; height=0;
#ifdef MSV_USEXLIB
		if(ximage){ XDestroyImage(ximage); ximage=0; data=0; }
		_deln(data);
#else
		_deln(data);
#endif
		//print("IPEDel\r\n");
	}

	int Is()const{ return data!=0; }
	int IsValid()const{ return data!=0; }
	int GetWidth()const{ return width; }
	int GetHeight()const{ return height; }
	int GetEffWidth()const{ return ewidth; }
	int GetAllSize()const{ return ewidth*height; }
	BYTE* GetData()const{ return data; }

	SRect GetRect()const{ SRect rc(0, 0, width, height); return rc; }

	int GetPixel(int x, int y){
		if(x<0 || y<0 || x>=width || y>=height) return 0;
		return (*(int*)(GetData()+x*3+y*ewidth))&16777215;
	}

#ifndef NOUSEMSV_WINAPI
	void GetBI(BITMAPINFO &bmInfo)const{
		memset(&bmInfo,0,sizeof(BITMAPINFO));
		bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		bmInfo.bmiHeader.biWidth=width;
		bmInfo.bmiHeader.biHeight=height;
		bmInfo.bmiHeader.biPlanes=1;
		bmInfo.bmiHeader.biBitCount=24;		
		bmInfo.bmiHeader.biSizeImage=dsize;
	}
#endif //NOUSEMSV_WINAPI
	

	int Clear(const MRGB rgb){
		UGLOCK(*this);
		if(!data) return 0;
		
		BYTE *l=data, *t=data+width*3;
		unsigned int d=*(unsigned int*)&rgb;

		for(l; l<t; l+=3){
			*(unsigned int*)l=d; // *l++=rgb.rgbRed; *l++=rgb.rgbGreen; *l++=rgb.rgbBlue;
		}

		for(int i=1; i<height; i++){
			memcpy(data+ewidth*i, data, ewidth);
		}

		return 1;
	}

	const MWndIPE& operator=(const MWndIPE &copy){
		if(New(copy.GetWidth(), copy.GetHeight())){
			memcpy(GetData(), copy.GetData(), copy.GetHeight()*copy.GetEffWidth());
		}
		return *this;
	}

	void Copy(const MWndIPE &copy){
		if(New(copy.GetWidth(), copy.GetHeight())){
			memcpy(GetData(), copy.GetData(), copy.GetHeight()*copy.GetEffWidth());
		}
		return ;
	}


#ifndef MSV_USEXLIB
	void Draw(HWND hwnd, HDC dc){ return Draw(hwnd, dc, MRect(0, 0, width, height)); }

	void Draw(HWND hwnd, HDC dc, MRect rc){
		UGLOCK(*this);
#ifndef NOUSEMSV_WINAPI
		rc.Crop(MRect(0, 0, width, height));

		BITMAPINFO bmInfo;
		GetBI(bmInfo);

		//Itos it;
		//print(it.Format("DR: %d,%d-%d,%d\r\n", rc.left, rc.top, rc.right, rc.bottom));

		SetStretchBltMode(dc, COLORONCOLOR);
		int e=StretchDIBits(dc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, rc.left, height-rc.bottom, rc.right-rc.left, rc.bottom-rc.top, data, (BITMAPINFO*)&bmInfo, DIB_RGB_COLORS, SRCCOPY);
		//int e=StretchDIBits(dc, rc.left, rc.top, rc.right, rc.bottom, rc.left, rc.top, rc.right, rc.bottom, data, (BITMAPINFO*)&bmInfo, DIB_RGB_COLORS, SRCCOPY);
#endif // NOUSEMSV_WINAPI
	}
#else
	void Draw(Window hwnd, GC dc){  return Draw(hwnd, dc, MRect(0, 0, width, height)); }

	void Draw(Window hwnd, GC dc, MRect rc){
		UGLOCK(*this);// Itos it;
		if(width<=0 || height<=0) return ;

		// no x XYBitmap 8,16,32
		// 24 XYPixmap 8,16,32 - not normal draw
		// 24 ZPixmap 8,32 - hmm, not bad + ls = NODRAW
		// 24 ZPixmap 24 - NOT DRAW
		// 32 ZPixmap 24 - NOT DRAW
		// 32 ZPixmap 8,32 - invalid attr

		UGLOCK2(xliblock);

		if(!ximage) ximage=XCreateImage(mwnddisp, 0, 24, ZPixmap, 0, (char*)data, width, height, 32, 0);

		print("DRAW ", mwnddisp ? "1" : "0", ximage ? "1" : "0",  data ? "1" : "0", dc ? "1" : "0", ". ", itos(width), "-", itos(height));
		//print(". ", itos(ls), ". ", itos(ximage->bytes_per_line), "\r\n");
		//print(itos(ximage->bits_per_pixel));
		print("\r\n");
		
		if(!ximage){ print("NODRAW\r\n"); return ; }
		
		ximage->bytes_per_line=ewidth; ximage->bits_per_pixel=24;
		print("ewidth: ", itos(ewidth), "\r\n");
		
		//print("IPE_Draw 1\r\n");

		Itos it; it.Format("w: %d iw:%d ih:%d d:%d tid:%d\r\n", (int)hwnd, ximage->width, ximage->height, ximage->data ? 1 : 0, (int)GetCurrentThreadId());
		print(it); 
	
		int tm=time();
static int ttt;

if(ttt!=tm)
		if(ximage) XPutImage(mwnddisp, hwnd, dc, ximage, 0, 0, 0, 0, width, height);
ttt=tm;
		print("IPE_Draw 3\r\n");
	}
#endif


	int Grab2(HWND hWnd){
		if(!hWnd) return 0;
		UGLOCK(*this);

		RECT rt;
		RECT rt2;
		RECT rt3;
		HWND hwnd;
		HDC hdcm;
		HBITMAP hbm;
		DWORD hob;
		HDC hdc;

		HDC hdcmem=0;
		HGDIOBJ hobj=0;
		HBITMAP hbmp=0;

		HWND hw_fix=hWnd;

		HDC hdc_s=GetDC(hWnd);
		HDC hwdc_s=GetWindowDC(hWnd);
		HDC hdc_scr=GetDC(0);

		HDC hwdc_fix=hwdc_s;
		
		SelectObject(hdcmem, hobj);
		DeleteDC(hdcmem);
		DeleteObject(hbmp);
		GetWindowRect(hw_fix, &rt);

		hdcmem=CreateCompatibleDC(hwdc_fix);
		hbmp=CreateCompatibleBitmap(hwdc_fix, rt.right-rt.left, rt.bottom-rt.top);
		hobj=SelectObject(hdcmem, hbmp);

		PrintWindow(hw_fix,hdcmem,0);
		BitBlt(hdc_s, rt.left, rt.right, rt.right-rt.left, rt.bottom-rt.top, hdcmem, 0, 0, SRCCOPY);

		NewHImage(hbmp, 0);
		
		ReleaseDC(0,hdc_scr);
		ReleaseDC(hWnd,hwdc_s);
		ReleaseDC(hWnd,hdc_s);
		SelectObject(hdcmem,hobj);
		DeleteDC(hdcmem);
		DeleteObject(hbmp);



		return 0;
/*
		// Create compatible DC
		HDC cdc = CreateCompatibleDC(dc);
		HDC hdc = CreateCompatibleDC(dc);

		// Create a compatible bitmap
		HBITMAP bmp = CreateCompatibleBitmap(dc, rc.GetW(), rc.GetH()); 

		// Select Bitmap
        HGDIOBJ odc=SelectObject(cdc, bmp);

		//SendMessage(hwnd, WM_PRINT, (WPARAM)cdc, PRF_CHILDREN|PRF_CLIENT|PRF_NONCLIENT);
		PrintWindow(hwnd, cdc, 0);
		//PAINTSTRUCT ps;
		//BeginPaint(hwnd, &ps);
		
		// Copy pixels from screen to the BITMAP
		int ret=BitBlt(hdc, 0, 0, rc.GetW(), rc.GetH(), cdc, 0, 0, SRCCOPY);

		// Restore bitmap
		SelectObject(cdc, odc);

		//EndPaint(hwnd, &ps);

		// Create image
		if(ret) ret=NewHImage(bmp, cdc);

		DeleteObject(bmp);
		// Delete the hBitmapdc as you no longer need it
		DeleteDC(cdc);
		// Release the desktop device context handle
		//ReleaseDC(hwnd, dc); 
		return ret;
		*/
	}

	int Grab3(HWND hwnd){
		if(!hwnd) return 0;
		UGLOCK(*this);

		MRect rc; GetWindowRect(hwnd, rc);
		HDC dc = GetWindowDC(hwnd);
		if(!dc) return 0;

		// Create compatible DC
		HDC cdc = CreateCompatibleDC(dc);

		// Create a compatible bitmap
		HBITMAP bmp = CreateCompatibleBitmap(dc, rc.GetW(), rc.GetH()); 

		// Select Bitmap
        HGDIOBJ odc=SelectObject(cdc, bmp);

		//PostMessage(hwnd, WM_PRINT, (WPARAM)cdc, 0);
		//PAINTSTRUCT ps;
		//BeginPaint(hwnd, &ps);

		HDC hdcmem=CreateCompatibleDC(dc);

		HDC hdc=GetWindowDC(hwnd);
		HDC hdcm=CreateCompatibleDC(hdc);
		HBITMAP hbm=CreateCompatibleBitmap(hdc, rc.GetW(), rc.GetH()); 
		HGDIOBJ hob=SelectObject(hdcm,hbm);
		PrintWindow(hwnd,hdcm,0);
		BitBlt(hdcmem, 0, 0, rc.GetW(), rc.GetH(), hdcm, 0, 0, SRCCOPY);
		NewHImage(hbm, hdcmem);
		SelectObject(hdcm, hob);
		DeleteDC(hdcm);
		DeleteObject(hbm);

		
		
		// Copy pixels from screen to the BITMAP
		int ret=BitBlt(cdc, 0, 0, rc.GetW(), rc.GetH(), dc, 0, 0, SRCCOPY);

		// Restore bitmap
		SelectObject(cdc, odc);

		//EndPaint(hwnd, &ps);

		// Create image
		//if(ret) ret=NewHImage(bmp, cdc);

		DeleteObject(bmp);
		// Delete the hBitmapdc as you no longer need it
		DeleteDC(cdc);
		// Release the desktop device context handle
		ReleaseDC(hwnd, dc); 
		return ret;
		return 0;
}

	int Grab4(HWND hWnd){
		if(!hWnd) return 0;
		UGLOCK(*this);

	HDC hDCMem = CreateCompatibleDC(NULL);

RECT rect;

GetWindowRect(hWnd, & rect);

HBITMAP hBmp = NULL;

{

      HDC hDC = GetDC(hWnd);

      hBmp = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);

      ReleaseDC(hWnd, hDC);

}

    HGDIOBJ hOld = SelectObject(hDCMem, hBmp);

    SendMessage(hWnd, WM_PRINT, (WPARAM) hDCMem, PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND | PRF_NONCLIENT | PRF_OWNED);

    SelectObject(hDCMem, hOld);

    DeleteObject(hDCMem);

    OpenClipboard(hWnd);

    EmptyClipboard();

    SetClipboardData(CF_BITMAP, hBmp);

    CloseClipboard();




		return 0;
	}

	int GrabM(HWND hwnd){
		if(!hwnd) return 0;
		UGLOCK(*this);

#ifndef NOUSEMSV_WINAPI
		HWND hwndr=GetAncestor(hwnd, GA_ROOT);

		MRect rc, rcr; int ret=0;
		GetWindowRect(hwnd, rc);
		GetWindowRect(hwndr, rcr);
		HDC dc = GetWindowDC(hwnd);
		HDC dcr = GetWindowDC(hwndr);
//		if(!dc) return 0;

		// Create compatible DC
		HDC cdc = CreateCompatibleDC(dc);
//		HDC cdc2 = CreateCompatibleDC(dc);
		HDC cdcr = CreateCompatibleDC(dcr);

		// Create a compatible bitmap
		HBITMAP bmpr = CreateCompatibleBitmap(cdcr, rcr.GetW(), rcr.GetH()); 
//		HBITMAP bmp = CreateCompatibleBitmap(dc, rc.GetW(), rc.GetH()); 

		// Select Bitmap
 //       HGDIOBJ odc=SelectObject(cdc, bmp);
		HGDIOBJ odcr=SelectObject(cdcr, bmpr);

		//PostMessage(hwnd, WM_PRINT, (WPARAM)cdc, 0);
		//PAINTSTRUCT ps;
		//BeginPaint(hwnd, &ps);
		int r=PrintWindow(hwndr, cdcr, 0);
		
		// Copy pixels from screen to the BITMAP
		ret=BitBlt(cdc, 0, 0, rc.GetW(), rc.GetH(), cdcr, rc.left-rcr.left, rc.top-rcr.top, SRCCOPY);
//		ret=BitBlt(cdc2, 0, 0, rc.GetW(), rc.GetH(), cdc, 0, 0, SRCCOPY);
		//int ret=BitBlt(dc, 0, 0, rc.GetW(), rc.GetH(), cdc, 0, 0, SRCCOPY);

		// Restore bitmap
//		SelectObject(cdc, odc);
		SelectObject(cdcr, odcr);

		//EndPaint(hwnd, &ps);

		// Create image
		NewHImage(bmpr, cdcr);
		//MWndIPE img;
		//ret=img.NewHImage(bmp, dc);
		//New(rc.GetW(), rc.GetH());
		//DrawImage(img, rc, GetRect(), 0);
		Save("tmp-t.png");

//		DeleteObject(bmp);
		DeleteObject(bmpr);
		// Delete the hBitmapdc as you no longer need it
		DeleteDC(cdc);
//		DeleteDC(cdc2);
		DeleteDC(cdcr);
		// Release the desktop device context handle
		ReleaseDC(hwnd, dc); 
		ReleaseDC(hwndr, dcr); 
		return ret;
#endif // NOUSEMSV_WINAPI
		return 0;
	}


	int GrabM2(HWND hwnd){
		if(!hwnd) return 0;
		UGLOCK(*this);

#ifndef NOUSEMSV_WINAPI
		HWND hwndr=GetAncestor(hwnd, GA_ROOT);

		{
			MRect rc, rcr; int ret=0;
			GetWindowRect(hwnd, rc);
			GetWindowRect(hwndr, rcr);

			HDC dc = GetWindowDC(hwnd);
			HDC cdc = CreateCompatibleDC(dc);
			HDC cdc2 = CreateCompatibleDC(dc);
			HBITMAP bmp = CreateCompatibleBitmap(dc, rc.GetW(), rc.GetH()); 
			HGDIOBJ odc=SelectObject(cdc, bmp);

			HDC dcr = GetWindowDC(hwndr);
			HDC cdcr = CreateCompatibleDC(dcr);
			HBITMAP bmpr = CreateCompatibleBitmap(dcr, rcr.GetW(), rcr.GetH()); 
			HGDIOBJ odcr=SelectObject(cdcr, bmpr);

			//SendMessage(hwndr, WM_PRINT, (WPARAM)cdcr, PRF_CLIENT);

			int r=PrintWindow(hwndr, cdcr, 0);
			ret=BitBlt(cdc, 0, 0, rc.GetW(), rc.GetH(), cdcr, rc.left-rcr.left, rc.top-rcr.top, SRCCOPY);

			
			SelectObject(cdcr, odcr);
			DeleteObject(bmpr);
			DeleteDC(cdcr);
			ReleaseDC(hwndr, dcr); 

			ret=BitBlt(cdc2, 0, 0, rc.GetW(), rc.GetH(), cdc, 0, 0, SRCCOPY);
			NewHImage(bmp, cdcr);

			//Save("tmp-t.png");
			//Sleep(5000);

			SelectObject(cdc, odc);
			DeleteObject(bmp);
			DeleteDC(cdc);
			DeleteDC(cdc2);
			ReleaseDC(hwnd, dc); 
			return 1;
		}



		MRect rc, rcr; int ret=0;
		GetWindowRect(hwnd, rc);
		GetWindowRect(hwndr, rcr);
		HDC dc = GetWindowDC(hwnd);
		HDC dcr = GetWindowDC(hwndr);
//		if(!dc) return 0;

		// Create compatible DC
		HDC cdc = CreateCompatibleDC(dc);
//		HDC cdc2 = CreateCompatibleDC(dc);
		HDC cdcr = CreateCompatibleDC(dcr);

		// Create a compatible bitmap
		HBITMAP bmpr = CreateCompatibleBitmap(cdcr, rcr.GetW(), rcr.GetH()); 
//		HBITMAP bmp = CreateCompatibleBitmap(dc, rc.GetW(), rc.GetH()); 

		// Select Bitmap
 //       HGDIOBJ odc=SelectObject(cdc, bmp);
		HGDIOBJ odcr=SelectObject(cdcr, bmpr);

		//PostMessage(hwnd, WM_PRINT, (WPARAM)cdc, 0);
		//PAINTSTRUCT ps;
		//BeginPaint(hwnd, &ps);
		int r=PrintWindow(hwndr, cdcr, 0);
		
		// Copy pixels from screen to the BITMAP
		ret=BitBlt(cdc, 0, 0, rc.GetW(), rc.GetH(), cdcr, rc.left-rcr.left, rc.top-rcr.top, SRCCOPY);
//		ret=BitBlt(cdc2, 0, 0, rc.GetW(), rc.GetH(), cdc, 0, 0, SRCCOPY);
		//int ret=BitBlt(dc, 0, 0, rc.GetW(), rc.GetH(), cdc, 0, 0, SRCCOPY);

		// Restore bitmap
//		SelectObject(cdc, odc);
		SelectObject(cdcr, odcr);

		//EndPaint(hwnd, &ps);

		// Create image
		NewHImage(bmpr, cdcr);
		//MWndIPE img;
		//ret=img.NewHImage(bmp, dc);
		//New(rc.GetW(), rc.GetH());
		//DrawImage(img, rc, GetRect(), 0);
		Save("tmp-t.png");

//		DeleteObject(bmp);
		DeleteObject(bmpr);
		// Delete the hBitmapdc as you no longer need it
		DeleteDC(cdc);
//		DeleteDC(cdc2);
		DeleteDC(cdcr);
		// Release the desktop device context handle
		ReleaseDC(hwnd, dc); 
		ReleaseDC(hwndr, dcr); 
		return ret;
#endif // NOUSEMSV_WINAPI
		return 0;
	}


	int Grab(HWND hwnd){
		if(!hwnd) return 0;
		UGLOCK(*this);

#ifndef MSV_USEXLIB
#ifndef NOUSEMSV_WINAPI

		// GeabM
		//return GrabM2(hwnd);
		//hwnd=GetAncestor(hwnd, GA_ROOT);

		MRect rc; GetWindowRect(hwnd, rc);
		HDC dc = GetWindowDC(hwnd);
		if(!dc) return 0;

		// Create compatible DC
		HDC cdc = CreateCompatibleDC(dc);
		HDC cdc2 = CreateCompatibleDC(dc);

		// Create a compatible bitmap
		HBITMAP bmp = CreateCompatibleBitmap(dc, rc.GetW(), rc.GetH()); 

		// Select Bitmap
        HGDIOBJ odc=SelectObject(cdc, bmp);

		//PostMessage(hwnd, WM_PRINT, (WPARAM)cdc, 0);
		//PAINTSTRUCT ps;
		//BeginPaint(hwnd, &ps);
		
		// Copy pixels from screen to the BITMAP
		int ret=BitBlt(cdc, 0, 0, rc.GetW(), rc.GetH(), dc, 0, 0, SRCCOPY);

		// Restore bitmap
		SelectObject(cdc, odc);

		//EndPaint(hwnd, &ps);

		// Create image
		if(ret) ret=NewHImage(bmp, cdc);

		DeleteObject(bmp);
		// Delete the hBitmapdc as you no longer need it
		DeleteDC(cdc);
		DeleteDC(cdc2);
		// Release the desktop device context handle
		ReleaseDC(hwnd, dc); 
		return ret;
#endif // NOUSEMSV_WINAPI
		return 0;
#else

		Window ww=hwnd;
		print("Window: ", itos64(sizeof(Window)), ".  ");

		print("HWND: ", itos64(ww), ".  ");
		ww=RootWindow(mwnddisp, mwndscreen);
		print("HWND: ", itos64(ww), ".  ");
		//ww=RootWindow(mwnddisp, mwndscreen);
		ww=MWndGetDesktopWindow();
		print("HWND: ", itos64(ww), ".  ");
		ww=RootWindow(mwnddisp, mwndscreen);
		print("HWND: ", itos64(ww), ".  ");

		if(!hwnd) print("First Window Fail\r\n");
		hwnd=RootWindow(mwnddisp, mwndscreen);
		if(!hwnd) print("Second Window Fail\r\n");

		XGrabWnd.DeleteImage();
		if(!New(XDisplayWidth(mwnddisp, 0), XDisplayHeight(mwnddisp, 0))) return 0;

		const unsigned char *data=XGrabWnd.GetImage(mwnddisp, hwnd, mwndscreen);

		//for(int a=0; a<height*width; a++){}

		if(data){ print("G++! ");
			for(int i=0; i<height; i++){

				if(XGrabWnd.depth==32)
				for(int k=0; k<width; k++){
					//memcpy(GetData()+i*k*3, data+(width*(height-i-1)+k-1)*4, 3);
					memcpy(GetData()+(i*width+k)*3, data+((height-i-1)*width+k)*4, 3);
				//	memcpy(GetData()+i*GetEffWidth(), data+i*depth*GetWidth(), depth*GetWidth());
				}
					
				if(XGrabWnd.depth==24)
					memcpy(GetData()+i*GetEffWidth(), data+(height-i-1)*GetEffWidth(), GetEffWidth());
			}
			return 1;
		}
		else return 0;
#endif
		return 0;
	}



	// Draw Line, Box, Circle, objects

	int DrawLine(int x, int y, int cx, int cy, MRect to, MRGB rgb){
		// r[name] - return. new 18.08.2011 11:13
		int rx=GetWidth(), ry=GetHeight(), rew=GetEffWidth();
		BYTE *rd=GetData();

		int fx, fy, tx, ty, bx, by; // from, to, begin: x, y
		fx = to.left>=0 ? to.left : 0; tx = to.right<rx ? to.right : rx;
		fy = to.top>=1 ? to.top : 1; ty = to.bottom<=ry ? to.bottom : ry;
		bx=fx; by=fy;// int fk=fy;//, fi=fx;

		// Draw
		BYTE *rdp=rd+fx*3+(ry-ty)*rew;
		int rs=(tx-fx)*3;
		if(fx>tx) return 0;

		int step=tx-fx, stepy=ty-fy;//, tk=0, lk=0; // step, this-last k
		//float gr=sqrt((float)(cx-x)*(cx-x)+(cy-y)*(cy-y))/(cx-x);

		int x1=x, x2=cx, y1=y, y2=cy;

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		int sx = x1 < x2 ? 1 : -1;
		int sy = y1 < y2 ? 1 : -1;
		int r1 = dx - dy, r2;

		int test=0;
	 
		while(1){ if(test++>S128K) break;
			BYTE *e=rd+(ry-ty)*rew+(ry-y1)*rew+x1*3;
			if(fx<=x1 && x1<tx && fy<=y1 && y1<ty){
				*e++=rgb.blue; *e++=rgb.green; *e=rgb.red;
				Check(e);
			}
	 
			if(x1 == x2 && y1 == y2) break;
			r2 = r1 * 2;
	 
			if(r2 > -dy){ r1 -= dy; x1 += sx; }	 
			if(r2 < dx){ r1 += dx; y1 += sy; }
		}

		return 1;


		//for(fy; fy<ty; fy++){ Check(rdp);
		//	for(int i=0; i<step; i++){
		//		//BYTE *e=rdp+i*3, *a=idp+(i*100/step)*(rc.right-rc.left)/100*3;
		//		BYTE *e=rdp+i*3; //(i*100/step)*(rc.right-rc.left)/100*3;
		//		
		//		float grm=sqrt((float)(i+fx)*(i+fx)+(fy-y)*(fy-y))/(i+fx);
		//		if(abs(gr-grm)<1){
		//			*e++=rgb.blue; *e++=rgb.green; *e=rgb.red;
		//		}else {
		//			e+=3;
		//		}
		//	}
		//	//tk=(fy-by+1)*(rc.bottom-rc.top)/stepy;
		//	//tk=(fy-by+1)*(rc.bottom-rc.top)/stepy;
		//	//lk=tk;
		//	rdp+=rew;
		//}

//		return 1;
	}

	int DrawLine_(int x, int y, int cx, int cy, RGBQUAD rgb){
		
	//	BYTE* iDst = info.pImage + y*info.dwEffWidth + x*3;
	//	*iDst++ = c.rgbBlue;
	//	*iDst++ = c.rgbGreen;
	//	*iDst   = c.rgbRed;
		int _x=x, _y=y, _cx=cx, _cy=cy;
		if(!Is()) return 0;

		int i, s, j, k; s=GetEffWidth(); int p=0; float flo=0;
		int tx=GetWidth(), ty=GetHeight(); i=y; y=ty-cy-1; cy=ty-i-1;
		if(!tx || !ty) return 1;
		BYTE* idimi = GetData()+s*ty; BYTE* imp = GetData(); 

//if(x<0) x=0; if(cx<0)cx=0; if(y<0) y=0; if(cy<0)cy=0;
//if(x>=tx) x=tx; if(cx>=tx)cx=tx; if(y>=ty) y=ty; if(cy>=ty)cy=ty;
//if(x<0) x=0; if(cx<0)cx=0; if(y<0) y=0; if(cy<0)cy=0;

//if(x>cx) {p=x; x=cx; cx=p;} if(y>cy) {p=y; y=cy; cy=p;}
		if(x>=cx){ p=x; x=cx; cx=p; p=y; y=cy; cy=p; }
		int mody=y>cy;

		if(x<0 && cx<0) return 0; if(y<0 && cy<0) return 0;
		if(x>=tx && cx>=tx) return 0; if(y>=ty && cy>=ty) return 0;

		if(cx==0) x=0; if(cy==0 && !mody) y=0;

//if(x<0 && y<0) return 0;
int _d=1;

flo=0;
if(x<0 && x<y){ flo=float(100*cx)/(cx-x);}
else if(y<0) { flo=float(100*cy)/(cy-y);}
if(flo && _d){
	x=cx-int((cx-x)*flo/100); y=cy-int((cy-y)*flo/100); }
if(x>=tx && cx>=tx) return 0; if(y>=ty && cy>=ty) return 0;

flo=0;
if(cx>tx && cx-tx>cy-ty){ flo=100*float(tx-x)/(cx-x); }
else if(cy>ty) flo=100*float(ty-y)/(cy-y);
if(flo) {
	if(x+int((cx-x)*flo/100)>tx || y+int((cy-y)*flo/100)>ty){
int dgf=35;
	}
	cx=x+int((cx-x)*flo/100); cy=y+int((cy-y)*flo/100); }
if(x<0 && cx<0) return 0; if(y<0 && cy<0) return 0;

if(x<0 || y<0 || cx>=tx || cy>=ty){
		int gd=89;
}

//int sy=cy-y; if(sy<0) sy-y-cy; int uy = (y<cy ? y : cy), dy=(y>cy ? y : cy);

//cy++; 


		if(x<0 || cx<0 || y<0 || cy<0) return 0;
		if(x>=tx || cx>=tx || y>=ty || cy>=ty) return 0;

	if(x==cx){// if(x<0) x=0; if(cx<0)cx=0;
		if(x<0 || x>=tx) return 0; if(y<-1) y=-1; if(cy<-1)cy=-1; if(y>=ty) y=ty-1; if(cy>=ty) cy=ty-1; 
		if(y>cy){ i=y; y=cy; cy=i; }
		for(i=y; i<cy; i++){
			BYTE* iDst = GetData() + i*s + x*3;
			//if(iDst<imp || iDst>idimi || x<0 || x>=tx || iDst<GetData() || iDst+3>GetData()+s*ty){
			//int err=435;
			//}else{check(iDst);
			Check(iDst);

			*iDst++ = rgb.rgbBlue;
			*iDst++ = rgb.rgbGreen;
			*iDst   = rgb.rgbRed;
		}
		return 1;
	}
			  //cx++;

	if(y==cy){ if(y<0 || y>=ty) return 0; if(x<0) x=0; if(cx<0)cx=0;
	//if(y<0) y=0; if(cy<0)cy=0;
	if(x>=tx) x=tx-1; if(cx>tx) cx=tx; if(x>cx){ i=x; x=cx; cx=i;}
		for(i=x; i<cx; i++){
			BYTE* iDst = GetData() + y*s + i*3;
			//if(iDst<GetData() || iDst+3>(GetData()+s*ty)){
			//int err=435;
			//}  //}else{
			Check(iDst);
			*iDst++ = rgb.rgbBlue;
			*iDst++ = rgb.rgbGreen;
			*iDst   = rgb.rgbRed;
		}
		return 1;
	}


		//cx++; cy++;
//	if(x<0 || cx<0 || y<0 || cy<0){
//	 int p=435;
//	}
//	if(x>cx) {i=x; x=cx; cx=i; i=y; y=cy; cy=i;}

//	if(cy-y==0){
//	int derr=435;
//	}

	//if(x>=tx) x=tx-1; if(cx>=tx)cx=tx-1; if(y>=ty) y=ty-1; if(cy>=ty) cy=ty-1;

	j=cx-x;  k=cy-y; if(j<0) j*=-1; if(j==0) return 0; 
		//if(k>2000){
		// int sfdg=546;
		//}
		if(k<0)	k*=-1;

 	//	if(k>2000){
		// int sfdg=546;
		//}

int mi=(y>cy ? 1 : -1), ry=(cy-y); if(ry<0) ry*=-1;
//if(j>k){
	if(cx-x>ry){
		for(i=x; i<(cx-(cx-x)/2*0); i++){ 	//(double(i-x)*100/j)*k/100;  //*100/100
			//image->SetPixelColor(i, cy+mi*(i-x)*float(k)/j+3, _rggb);
			BYTE* iDst = GetData() + int(cy+mi*((i-x)*float(k)/j))*s + (i)*3;
//		if(!iDst || iDst<GetData() || iDst+3>(GetData()+s*ty)){
//	int err=435;
//		}else{ check(iDst);
			Check(iDst);
			*iDst++ = rgb.rgbBlue;
			*iDst++ = rgb.rgbGreen;
			*iDst	= rgb.rgbRed;
//		}
		}
	
	//if(!(cx-x>cy-y)){ rgb->rgbBlue=240; rgb->rgbGreen=240; rgb->rgbRed=240;
	}else	 //  y++;
	if(cx-x<ry){ int my=(y<cy ? y : cy), mcy=(y>cy ? y : cy);
		for(i=my; i<mcy; i++){								 //- s
		//for(i=x; i<=cx; i++){								 //- s
			//image->SetPixelColor(x+(y-i)*j/k, i, _brgb);
 			BYTE* iDst = GetData() + (y+i-y)*s + int(cx+mi*(i-y)*float(j)/k)*3;
			//BYTE* iDst = image->GetInfo() + int(y+((i-x)*float(k)/j))*s + (cx-i+x)*3;
//			if(!iDst || iDst<GetData() || iDst+3>(GetData()+s*ty)){
//			int err=435;
//			}else{ check(iDst);
			Check(iDst);
			*iDst++ = rgb.rgbBlue;
			*iDst++ = rgb.rgbGreen;
			*iDst	= rgb.rgbRed;
		}
	}

		return 1;
	}

	int DrawBox(int x, int y, int cx, int cy, MRect box, MRGB rgb){	UGLOCK(*this);// print("DRAWBOX\r\n");
		if(x<box.left) x=box.left; if(y<box.top) y=box.top;
		if(cx>box.right) cx=box.right; if(cy>box.bottom) cy=box.bottom;
		if(cx<x || cy<y) return 0;

		int s=GetEffWidth(), i; // int i, s, j, k; 
		int tx=GetWidth(), ty=GetHeight(); i=y; y=ty-cy; cy=ty-i;
		if(!tx || !ty) return 1;
		if(x>cx){ int t=x; x=cx; x=t; t=y; y=cy; cy=t; }

		if(x<0) x=0; if(cx<0) cx=0; if(y<0) y=0; if(cy<0) cy=0;
		if(x>tx) x=tx; if(cx>tx) cx=tx; if(y>=ty) y=ty-1; if(cy>=ty) cy=ty-1;
		if(!GetData()) return 0;

		for(i=x; i<cx; i++){
			BYTE* iDst = GetData() + y*s + i*3;
/*					if(iDst<image->GetInfo() || iDst>(image->GetInfo()+s*ty)){
				int err=435; return 0;					 
			}*/ Check(iDst);
			*iDst++ = rgb.blue;
			*iDst++ = rgb.green;
			*iDst   = rgb.red;
		}
		
		BYTE* fiDst = GetData() + y*s + x*3;
		for(i=y; i<=cy; i++){
			BYTE* iDst = GetData() + i*s + x*3;
			//if(iDst<image->GetInfo() || iDst>(image->GetInfo()+s*ty)){
			//int err=435;
			//}
			Check(iDst); Check(iDst+(cx-x)*3-1);
			memcpy(iDst, fiDst, (cx-x)*3);
		}
		return 1;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////
	int DrawCircle(int x, int y, int rad, MRect box, RGBQUAD rgb, int a, int ob=0){ UGLOCK(*this);
	if(!IsValid() || box.GetH()<=0 || box.GetW()<=0) return 0;

	int rx=GetWidth(), ry=GetHeight(), rew=GetEffWidth();
	BYTE *rd=GetData();
	MRect to(x-rad, y-rad, x+rad, y+rad);

	int fx, fy, tx, ty, bx, by; // from, to, begin: x, y
	fx = to.left>=0 ? to.left : 0; tx = to.right<rx ? to.right : rx;
	fy = to.top>=1 ? to.top : 1; ty = to.bottom<=ry ? to.bottom : ry;
	if(fx<box.left) fx=box.left; if(fy<box.top) fy=box.top;
	if(tx>box.right) tx=box.right; if(ty>box.bottom) ty=box.bottom;
	bx=fx; by=fy;// int fk=fy;//, fi=fx;

#ifdef MSV_USEXPLU
	ry=ty+ty-rad*2;
#endif

	for(fy; fy<ty; fy++){
		fx=bx;
		BYTE *rdp=rd+fx*3+(ry-fy)*rew;

		for(fx; fx<tx; fx++){ 
 			int p=abs((fx-x)*(fx-x))+abs((fy-y)*(fy-y));
			p=(p<rad*rad) ? 1 : ((p==rad*rad) ? 2 : 0);
			if(p==2 || a && p){
				*rdp++=rgb.rgbBlue;
				*rdp++=rgb.rgbGreen;
				*rdp++=rgb.rgbRed;
			} else rdp+=3;
		}
	}


	//int i, s, j, k; s=GetEffWidth();
	//int tx=GetWidth(), ty=GetHeight();  i=y; y=ty-y;// cy=ty-i;
	//if(!tx || !ty || !GetData()) return 0;

	//////if(x+rad<0 || x-rad>=tx || y+rad<0 || y-rad>=ty) return 0;
	////BYTE *p=0;

	//		for(int i=0; i<=rad; i++){
	//			for(int j=0; j<rad; j++){

	//				int p=i*i+j*j; p= (p<rad*rad) ? 1 : ((p==rad*rad) ? 2 : 0);

	//				if(p==2 || a && p){

	//			BYTE* iDst = GetData() + (y+j)*s + (x+i)*3;
	//			if(iDst<GetData() || iDst>(GetData()+s*ty) || x+i>=tx){
	//				int err=435; return 0;		 
	//			}
	//			else if(!ob || ob==2){*iDst++ = rgb->rgbBlue; *iDst++ = rgb->rgbGreen; *iDst = rgb->rgbRed;}

	//			iDst = GetData() + (y-j)*s + (x+i)*3;
	//			if(iDst<GetData() || iDst>(GetData()+s*ty) || x+i>=tx){
	//				int err=435; return 0;		 
	//			}
	//			else if(!ob || ob==3){ check(iDst); *iDst++ = rgb->rgbBlue; *iDst++ = rgb->rgbGreen; *iDst   = rgb->rgbRed;}

	//			iDst = GetData() + (y+j)*s + (x-i)*3;
	//			if(iDst<GetData() || iDst>(GetData()+s*ty) || x-i>=tx){
	//				int err=435; return 0;
	//			}
	//			else if(!ob || ob==1){*iDst++ = rgb->rgbBlue; *iDst++ = rgb->rgbGreen; *iDst = rgb->rgbRed;}

	//			iDst = GetData() + (y-j)*s + (x-i)*3;
	//			if(iDst<GetData() || iDst>(GetData()+s*ty) || x-i>=tx){
	//				int err=435; return 0;		 
	//			}
	//			else if(!ob || ob==4){check(iDst); *iDst++ = rgb->rgbBlue; *iDst++ = rgb->rgbGreen; *iDst   = rgb->rgbRed;}
	//			
	//				}}
	//}

	return 1;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int DrawPoly(DINT *a, int sz, /*int x, int y, */ MWndStyle style, bool cp=0){
	if(!GetData()) return 0;

	int rx=GetWidth(), ry=GetHeight(), rew=GetEffWidth(); BYTE *rd=GetData();
	if(!rx || !ry || !sz) return 1;

	// four
	int u=0, d=0, l=0, r=0, i;
	for(i=1; i<sz; i++){
		if(a[l].x>a[i].x) l=i;
		if(a[r].x<a[i].x) r=i;
		if(a[u].y>a[i].y) u=i;
		if(a[d].y<a[i].y) d=i;	
	}

	if(cp) { a[sz]=a[0]; sz++; }

	int pu=a[u].y, pd=a[d].y, pl=a[l].x, pr=a[r].x, mx, my, dx, dy, nx, ny, mi, myx;
	//if(pr-pl<=0 || pr<0 && pl<0 || /*pl>rx || pu>ry ||*/ pr-pl>S128K) return 0;
	//ItosX<S4K> it; unsigned char *buf;
	MString it; unsigned char *buf;
	int sx=pr-pl+1; it.Reserv(rx+2); buf=it.data;

	pu--; pd++; pl--; pr++;

	if(pu<0) pu=0; if(pd>ry) pd=ry;

	for(i=pu; i<pd; i++){
//	BYTE *p=rdp;
		memset(buf, 0, rx+2);

		for(int k=0; k<sz-1; k++){// if(k>1) continue;
			mx=a[k].x<a[k+1].x ? a[k].x : a[k+1].x; my=a[k].y<a[k+1].y ? a[k].y : a[k+1].y;
			myx=a[k].y<a[k+1].y ? a[k].x : a[k+1].x;// myx-=(pl*2)/4;
			dx=a[k].x<a[k+1].x ? a[k+1].x-a[k].x : a[k].x-a[k+1].x;
			dy=a[k].y<a[k+1].y ? a[k+1].y-a[k].y : a[k].y-a[k+1].y;
 			//mi=((a[k].x>a[k+1].x) ^ (a[k].y>a[k+1].y)) ? -1 : 1;
			mi=((mx==a[k].x) ^ (my==a[k].y)) ? -1 : 1;
			//int st=(mx==a[k].x ^ my==a[k].y) ? 1 : 2;
				
			if(i>=my && i<my+dy){
				if(!dy)
					continue ;
				//if(a[k].y<a[k+1].y) {nx=99999;} //{nx=dx-(my-(i-a[k].y)*dx/dy);}
				//else {nx=mi*(i-a[k+1].y); //int((i-a[k+1].y)*float(dx)/dy);
				//	nx=nx;} //{nx=mx+(dy-(i-a[k+1].y)*dx/dy)-pl; nx=99999;}
				nx=myx+mi*(((i-((float)my-1))*dx)/dy);
				BYTE *p=rd+nx*3+(ry-i-1)*rew;

				if(nx>=0 && nx<rx){
					if(!buf[nx]) buf[nx]=1;
					else buf[nx]=0;
					//*p++=style.bgcolor.blue; *p++=style.bgcolor.green; *p=style.bgcolor.red;
					*p++=255; *p++=0; *p=0;
				} else{
					if(nx<0) buf[rx]=!buf[rx];
					else buf[rx+1]=!buf[rx+1];
				}
			}
		}

		bool d=buf[rx];
		BYTE *p=rd+0*3+(ry-i-1)*rew;
		for(int k=0; k<rx; k++){ // pr-pl
			//if(pl+k<0 || pl+k>=rx || i>=ry) continue;
			//BYTE *p=rd+(pl+k)*3+(ry-i-1)*rew;
			

			//BYTE *p=GetData()+(pl+k)*3+i*GetEffWidth();
			//print(itos(p-GetData()), " ");
			//if(p-GetData()>3000000)
			//	int rtytry=67;
			//p+=((pl+k)*3);  //print(itos(p-GetData()), " ");
			//if(p-GetData()>3000000)
			//	int rtytry8=67;
			//p+=(i*GetEffWidth());//  print(itos(p-GetData()), " ", itos(i), " ");
			//if(p-GetData()>3000000)
			//	int rtytry9=67;

			if(buf[k]){
				d=!d;
				if(/*pl/10*/+k<0 || k>=rx || i>=ry){ p+=3; continue; }
				Check(p);
				*p++=style.border.brcolor.blue; *p++=style.border.brcolor.green; *p++=style.border.brcolor.red;
			}
			else if(d){
				if(/*pl/10*/+k<0 || k>=rx || i>=ry){ p+=3; continue; }
				Check(p);
				*p++=style.bgcolor.blue; *p++=style.bgcolor.green; *p++=style.bgcolor.red;
			}
			else p+=3;
			//if(b[k]) { image->SetPixelColor(pl+k, i, *rgb); d=!d;}
			//else if(d) image->SetPixelColor(pl+k, i, *rgb);
		}

		//rdp+=rew;

	}

	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int DrawPoly_(DINT *a, int sz, /*int x, int y, */ MRGB rgb, bool cp=0){

	//	BYTE* iDst = info.pImage + y*info.dwEffWidth + x*3;
	//	*iDst++ = c.rgbBlue;
	//	*iDst++ = c.rgbGreen;
	//	*iDst   = c.rgbRed;
 // int _x=x, _y=y, _cx=cx, _cy=cy;
  if(!GetData()) return 0;
//	if(x>8000 || cx>8000 || y>8000 || cy>8000){
//		 int p=435;
//	}

//  if(sz>7) return 1;

int i, s, j, k; s=GetEffWidth(); int p=0; float flo=0;
int tx=GetWidth(), ty=GetHeight();// i=y; y=ty-cy-1; cy=ty-i-1;
if(!tx || !ty) return 1;
BYTE* idimi = GetData()+s*ty; BYTE* imp = GetData(); 

//for(int i=0; i<=sz; i++){ a[i].x+=x; a[i].y=ty-(a[i].y+y); }

// four
int u=0, d=0, l=0, r=0;
for(int i=1; i<sz; i++){if(a[l].x>a[i].x) l=i;}
for(int i=1; i<sz; i++){if(a[r].x<a[i].x) r=i;}
for(int i=1; i<sz; i++){if(a[u].y>a[i].y) u=i;}
for(int i=1; i<sz; i++){if(a[d].y<a[i].y) d=i;}

if(cp) { a[sz]=a[0]; sz++; }

int pu=a[u].y, pd=a[d].y, pl=a[l].x, pr=a[r].x, mx, my, dx, dy, nx, ny, mi, myx;
if(pr-pl<=0 || pr<0 && pl<0 || pl>tx || pu>ty || pr-pl>S128K) return 0;
MString bf; char bfc[128], *b; int bs=pr-pl+1; if(bs>128) { bf.Reserv(bs, 0); b=bf; } else {b=bfc;}

//for(int i=0; i<sz; i++){
//	for(int k=i+1; k<sz; k++){
//		TMLine(image, a[i].x, ty-a[i].y, a[k].x, ty-a[k].y, _grrgb);		
//	}
//}

//return 1;

i=pu; if(i<0) i=0; if(pd>ty) pd=ty;
for(i; i<=pd; i++){ memset(b, 0, bs);
	for(int k=0; k<sz-1; k++){// if(k>1) continue;
		mx=a[k].x<a[k+1].x ? a[k].x : a[k+1].x; my=a[k].y<a[k+1].y ? a[k].y : a[k+1].y;
		myx=a[k].y<a[k+1].y ? a[k].x : a[k+1].x; myx-=pl;
		dx=a[k].x<a[k+1].x ? a[k+1].x-a[k].x : a[k].x-a[k+1].x;
		dy=a[k].y<a[k+1].y ? a[k+1].y-a[k].y : a[k].y-a[k+1].y;
 		//mi=((a[k].x>a[k+1].x) ^ (a[k].y>a[k+1].y)) ? -1 : 1;
		mi=((mx==a[k].x) ^ (my==a[k].y)) ? -1 : 1;
		//int st=(mx==a[k].x ^ my==a[k].y) ? 1 : 2;

	//	image->SetPixelColor(a[k].x, i, _brgb);

		if(a[k].y+999!=a[k+1].y && (i>=my && i<my+dy)){// image->SetPixelColor(a[k].x, i, _blrgb);
			//if(my<=i && my+dy>=i){
			
			if(!dy) return 0;
			//if(a[k].y<a[k+1].y) {nx=99999;} //{nx=dx-(my-(i-a[k].y)*dx/dy);}
			//else {nx=mi*(i-a[k+1].y); //int((i-a[k+1].y)*float(dx)/dy);
			//	nx=nx;} //{nx=mx+(dy-(i-a[k+1].y)*dx/dy)-pl; nx=99999;}
			nx=myx+mi*(((i-(my-1))*dx)/dy);
				if(nx>=0 && nx<bs) { 
					if(!b[nx]) b[nx]=1;
					else b[nx]=0;				
				} else
					int err=1;
				//image->SetPixelColor(mx-1, i, _rrgb);// image->SetPixelColor(mx+1, i, _rrgb);
				//image->SetPixelColor(pl+nx, i, _rggb);
			//}
		}}

		bool d=0;
		for(int k=0; k<pr-pl; k++){
			if(pl+k<0 || pl+k>=tx || i>=ty) continue;

			BYTE *p=GetData(); //print(itos(p-GetData()), " ");
			if(p-GetData()>3000000)
				int rtytry=67;
			p+=((pl+k)*3);  //print(itos(p-GetData()), " ");
			if(p-GetData()>3000000)
				int rtytry8=67;
			p+=(i*GetEffWidth());//  print(itos(p-GetData()), " ", itos(i), " ");
			if(p-GetData()>3000000)
				int rtytry9=67;

			if(b[k]){
				*p++=rgb.blue; *p++=rgb.green; *p=rgb.red; d=!d;
			}
			else if(d){
				*p++=rgb.blue; *p++=rgb.green; *p=rgb.red;
			}
			//if(b[k]) { image->SetPixelColor(pl+k, i, *rgb); d=!d;}
			//else if(d) image->SetPixelColor(pl+k, i, *rgb);
		}
		
//TMLine(image, a[l].x+x, i+y, a[r].x+x, i+y, _grrgb);	
}

return 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
	int DrawImage_old(MWndIPE_C iim, MRect rc, MRect to, int cnf){ UGLOCK(*this);
        MRect trc, trct; int i, j, k, mj, mk;// MRGB rgb;
		if(!iim.IsValid() || to.GetH()<=0 || to.GetW()<=0) return 0;
		
		int mx, my, imh=iim.GetHeight(), imw=iim.GetWidth(); BYTE*ida=iim.GetData(), *ide=GetData();
		int aew=iim.GetEffWidth(); int ew=GetWidth(); int eew=GetEffWidth();// blsrx=blsefw;
		int eh=GetHeight(); mx=to.GetW(); my=to.GetH();

		j = to.left>=0 ? to.left : 0; mj = to.right<ew ? to.right : ew;
		k = to.top>=0 ? to.top : 0; mk = to.bottom<eh ? to.bottom : eh;
		int fk=k, fi=j;

		if(rc.right>iim.GetWidth()) rc.right=iim.GetWidth();
		if(rc.bottom>iim.GetHeight()) rc.bottom=iim.GetHeight();

#ifdef MSV_USEXPLU
		eh=mk+mk-iim.GetHeight();
#endif

		// Draw
		BYTE *pa=ida+rc.left*3+(rc.top)*aew, *pe=ide+j*3+(eh-mk)*eew;
		int sa=aew, se=eew, ss=(mj-j)*3, ssa=(rc.right-rc.left)*3;
		if(j>mj) return 0;

#ifdef MSV_USEXPLU
			pa+=sa*(mk-k-1); sa*=-1;
#endif

		if(cnf&MWND_DRAW_RESIZE){
			int step=mj-j;

			for(k; k<mk; k++){ Check(pe);
				for(int i=0; i<step; i++){
					BYTE *e=pe+i*3, *a=pa+(i*100/step)*(rc.right-rc.left)/100*3;
					if(!Check(e))
						return 0;
						//print("!check ", itos(i), ", ", itos(k) ,"\r\n");
					*e++=*a++;
					*e++=*a++;
					*e  =*a  ;
					//if(!Check(e))
						//print("!check 2\r\n");
				}
				//memcpy(pe, pa, ss);
				pa+=sa; pe+=se;
			//	memcpy(p-pm, pt-pmr, mx*3);
			}

	}else{
		for(k; k<mk; k++){
			Check(pe);
			memcpy(pe, pa, ss); pa+=sa; pe+=se;
		}
	}

	//DrawImage_(iim, rc, to, cnf);
return 1;
}

	static SIZE GetTextSize(VString text, MFont &ft, MyFont2 &mf){
		SIZE rsz={0,0};

		const MyFontString *str=mf.GetString(ft.lfHeight);
		if(!str) return rsz;

		rsz=GetTextSize(text, text.endu(), str);

		return rsz;
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////

int Load(VString file){
#ifndef NOUSEMSV_WINAPI
		MString d=LoadFile(file);
		if(!d) return 0;
		return Decode(d, d, RetType(file.str(-3)));
#endif // #ifndef NOUSEMSV_WINAPI
		return 0;
	}


int Decode(BYTE*data, int sz, int type){
	CxImage img;
	if(!img.Decode(data, sz, type) || !img.IsValid()) return 0;
	New(img.GetWidth(), img.GetHeight());
	DrawImage(img, MRect(0, 0, img.GetWidth(), img.GetHeight()),  MRect(0, 0, img.GetWidth(), img.GetHeight()), 0);
	return 1;
}

int Save(VString file){
//#ifdef USEMSV_CXIMAGE_EMULATE
	if(file.str(-3) == "bmp")
		return SaveBmp(file);
//#endif

#ifndef NOUSEMSV_WINAPI
	CxImage img;
	img.CreateFromArray(GetData(), GetWidth(), GetHeight(), 24, GetEffWidth(), 0);
	return img.Save(file, RetType(file.str(-3)));
#endif // #ifndef NOUSEMSV_WINAPI
	return 0;
	}

// Save to memory buffer as bmp
int SaveToMem(TString &ret){
	// Reserv memory
	if(!ret.Reserv(14 + sizeof(BITMAPINFOHEADER) + dsize))
		return 0;

	// Bmp header
	BITMAPINFOHEADER head;

	memset(&head, 0, sizeof(BITMAPINFOHEADER));
	head.biSize = sizeof(BITMAPINFOHEADER);
	head.biWidth = width;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biBitCount = 24;
	head.biSizeImage = dsize;

	// Bmp file header
	BITMAPFILEHEADER hdr;
	hdr.bfType = 0x4d42;
	hdr.bfSize = dsize + 14;
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = 14 + head.biSize;

	memcpy(ret.data + 0, &hdr, 14);
	memcpy(ret.data + 14, &head, sizeof(BITMAPINFOHEADER));
	memcpy(ret.data + 14 + sizeof(BITMAPINFOHEADER), GetData(), dsize);

	return 1;
}

int SaveBmp(VString file){
	// Open file
	HFILE fl = CreateFile(file, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
	if(!ishandle(fl))
		return 0;

	// Bmp header
	BITMAPINFOHEADER head;

	memset(&head, 0, sizeof(BITMAPINFOHEADER));
	head.biSize = sizeof(BITMAPINFOHEADER);
	head.biWidth = width;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biBitCount = 24;
	head.biSizeImage = dsize;

	// Bmp file header
	BITMAPFILEHEADER hdr;
	hdr.bfType = 0x4d42;
	hdr.bfSize = dsize + 14;
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = 14 + head.biSize;

	if(WriteFile(fl, &hdr, 14) != 14)
		return 0;

	if(WriteFile(fl, &head, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
		return 0;

	if(WriteFile(fl, GetData(), dsize) != dsize)
		return 0;

	CloseHandle(fl);

	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Load Section
int LoadMim(VString path){
	UGLOCK(*this);
	minimimg mimo;
#ifndef NOUSEMSV_WINAPI
	print("Load\r\n");
	// Test File
	if(!IsFile(path)) return 0;
	
	// Open
	HFILE hfile=CreateFile(path, O_RDONLY, S_IREAD|S_IWRITE);
	if(!ishandle(hfile)) return 0;

	// Read && Test
	if(ReadFile(hfile, (char*)&mimo, sizeof(mimo))!=sizeof(mimo)) return 0;
	if(mimo.x<=0 || mimo.y<=0) return 0;
	
	// Read Full
	if(!New(mimo.x, mimo.y)) return 0;
	if(ReadFile(hfile, (char*)GetData(), GetEffWidth()*GetHeight())!=GetEffWidth()*GetHeight()){ Del(); return 0; }	
	
	print("Loaded\r\n");

	// Close
	CloseHandle(hfile);
#endif // NOUSEMSV_WINAPI
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Save Section
int SaveMim(VString path){
	UGLOCK(*this);
	minimimg mimo;
	mimo.x=GetWidth(); mimo.y=GetHeight();
	
#ifndef NOUSEMSV_WINAPI
	// Create
	HFILE hfile=CreateFile(path, O_RDWR|O_CREAT, S_IREAD|S_IWRITE);
	if(!ishandle(hfile)) return 0;

	// Write
	WriteFile(hfile, (char*)&mimo, sizeof(mimo));
	WriteFile(hfile, (char*)GetData(), GetEffWidth()*GetHeight());
	
	// Close
	CloseHandle(hfile);
#endif // NOUSEMSV_WINAPI
	return 1;
}


};

#define MWndIPE_CD(IsValid) MWndIPE_C::IsValid(){ return ipe ? ipe->IsValid() : img ? img->IsValid() : vim->IsValid(); }

int MWndIPE_CD(IsValid);
//int MWndIPE_CD(GetBpp);
int MWndIPE_CD(GetWidth);
int MWndIPE_CD(GetHeight);
int MWndIPE_CD(GetEffWidth);
BYTE* MWndIPE_CD(GetData);
int MWndIPE_C::GetAllSize(){ return GetEffWidth()*GetHeight(); }

//MWndIPE_C::operator TLock*(){ if(ipe) return ipe->lock; return 0; }

//BYTE* MWndIPE_C::GetData(){ return ipe ? ipe->GetData() : img ? img->GetInfo() : vim->GetData(); }

//int MWndIPE_C::GetBpp(){  return img ? img->GetBpp() : ipe->GetBpp();}
//int MWndIPE_C::GetWidth(){  return img ? img->GetWidth() : ipe->GetWidth(); }
//int MWndIPE_C::GetHeight(){  return img ? img->GetHeight() : ipe->GetHeight(); }
//int MWndIPE_C::GetEffWidth(){  return img ? img->GetEffWidth() : ipe->GetEffWidth(); }
//BYTE* MWndIPE_C::GetInfo(){  return img ? img->GetInfo() : ipe->GetData(); }
//BYTE* MWndIPE_C::GetData(){  return img ? img->GetInfo() : ipe->GetData(); }



int MWndIPE::DrawImage(MWndIPE_C iim, MRect rc, MRect to, int cnf){ UGLOCK(*this); if(iim.ipe) UGLOCK2(iim.ipe);
		if(!iim.IsValid() || !IsValid() || to.GetH()<=0 || to.GetW()<=0) return 0;

		// iim & res: x, y, eff width. data  //		not used: ix, ih
		int ix=iim.GetWidth(), iy=iim.GetHeight(), iew=iim.GetEffWidth();
		int rx=GetWidth(), ry=GetHeight(), rew=GetEffWidth();
		BYTE *id=iim.GetData(), *rd=GetData();

//		print(itos(to.GetH()), "-", itos(rc.GetH()), " _ ");

		if(rc.GetW()>iim.GetWidth()) rc.right-=iim.GetWidth()-rc.GetW();
		if(rc.GetH()>iim.GetHeight()) rc.bottom-=iim.GetHeight()-rc.GetH();

		if(!(cnf&MWND_DRAW_RESIZE)){
			if(to.GetW()>rc.GetW())
				to.right-=to.GetW()-rc.GetW();
			if(to.GetH()>rc.GetH())
				to.bottom-=to.GetH()-rc.GetH();
			if(to.GetH()<rc.GetH())
				rc.bottom-=rc.GetH()-to.GetH();
		}

//		print(itos(to.GetH()), "-", itos(rc.GetH()), "\r\n");

		int fx, fy, tx, ty, bx, by; // from, to, begin: x, y
		fx = to.left>=0 ? to.left : 0; tx = to.right<rx ? to.right : rx;
		//fy = to.top>=0 ? to.top : 0; ty = to.bottom<ry ? to.bottom : ry;
		fy = to.top>=1 ? to.top : 1; ty = to.bottom<=ry ? to.bottom : ry;
		bx=fx; by=fy;// int fk=fy;//, fi=fx;

		if(cnf&MWND_DRAW_RESIZE && rc.GetW()==to.GetW() && rc.GetH()==to.GetH()){ cnf&=~MWND_DRAW_RESIZE; }

		if(!(cnf&MWND_DRAW_RESIZE)){ // normal size
			if(fx!=to.left) rc.left+=fx-to.left;
			if(fy!=to.top) rc.top+=fy-to.top;
			if(tx!=to.right) rc.right-=tx-rx;
			if(ty!=to.bottom) rc.bottom-=ty-ry;
			//if(to.GetW()>rc.GetW())
			//	to.right-=to.GetW()-rc.GetW();
			//if(to.GetH()>rc.GetH())
			//	to.bottom-=to.GetH()-rc.GetH();
		}


#ifdef MSV_USEXPLU
		ry=ty+ty-iy;
		//ry=ty;//-iy;
		//ry=ty-ry;
		//ry+=fy+1;
		//ry+=ty-fy-1 -(ty-fy);
		//ry=ty+ry-ty+fy;
#endif

		// Draw
		BYTE *idp=id+rc.left*3+(rc.top*0+iy-rc.bottom)*iew, *rdp=rd+fx*3+(ry-ty)*rew;
		int /*sa=iew, se=rew,*/ rs=(tx-fx)*3; //, is=(rc.right-rc.left)*3;
		if(fx>tx) return 0;

#ifdef MSV_USEXPLU
		//idp+=iew*(ty-fy-1); iew*=-1;
		//idp+=iew*(ty-fy-1); iew*=-1;
		idp+=iew*((ty-fy-1)*(rc.bottom-rc.top)/(ty-fy)); iew*=-1;
		//idp+=iew*(ty-fy-1+(ty-fy)/10); iew*=-1;
		//rew*=-1;
#endif

		if(cnf&MWND_DRAW_RESIZE){
			int step=tx-fx, stepy=ty-fy, tk=0, lk=0; // step, this-last k
			// for slow
			float stepp=1.*(rc.right-rc.left)/step; if(stepp>2) stepp=2;
			float stepa=(stepp-1)*0.40/1;
			if(stepa<0.2) stepa=0.2;
			float stepb=1-stepa*2;

			for(fy; fy<ty; fy++){ Check(rdp);
				for(int i=0; i<step; i++){
					//BYTE *e=rdp+i*3, *a=idp+(i*100/step)*(rc.right-rc.left)/100*3;
					BYTE *e=rdp+i*3, *a=idp+i*(rc.right-rc.left)/step*3; //(i*100/step)*(rc.right-rc.left)/100*3;
					if(!Check(e)){
						//Itos it; it.Format("!E fy:%d ty:%d i:%d st:%d r:%s%s \r\n", fy, ty, i, step, e<data ? "<" : ">", e>(data+dsize) ? ">" : "<" );
						//print(it);
					}

					if(a<id || a>id+iim.GetAllSize()){
						int errewr=234; return 0;
					}

					*e++=*a++; *e++=*a++; *e  =*a;

					if(cnf&MWND_DRAW_SLOW && rc.right-rc.left>step && stepp>1){
						e-=2;
						if(i && i<step-1){ *e++=*e*stepb; *e++=*e*stepb; *e=*e*stepb; }
						else if(i || i<step-1){ *e++=*e*stepb/stepa; *e++=*e*stepb/stepa; *e=*e*stepb/stepa; }
						
						if(i){ e-=2; a-=2-3; *e++=*e+*a++*stepa; *e++=*e+*a++*stepa; *e=*e+*a*stepa; }
						if(i<step-1){ e-=2; a++; *e++=*e+*a++*stepa; *e++=*e+*a++*stepa; *e=*e+*a*stepa; }
						//*e++=*a++;
						//*e  =*a  ;
					}
				}
				//tk=(fy-by+1)*(rc.bottom-rc.top)/stepy;
				tk=(fy-by+1)*(rc.bottom-rc.top)/stepy;
				idp+=iew*(tk-lk); lk=tk; rdp+=rew;
			}
	}else{
		for(fy; fy<ty; fy++){
			Check(rdp);
			memcpy(rdp, idp, rs); idp+=iew; rdp+=rew;
		}
	}
	return 1;
}

int MWndIPE::DrawImageTransp(MWndIPE_C iim, MRect rc, MRect to, MRGB rgb){ UGLOCK(*this);
		if(!iim.IsValid() || !IsValid() || to.GetH()<=0 || to.GetW()<=0) return 0;

#ifndef MSV_USEXPLU
		int _z=rc.top; rc.top=iim.GetHeight()-rc.bottom; rc.bottom=iim.GetHeight()-_z;
#endif

		// iim & res: x, y, eff width. data  //		not used: ix, ih
		int ix=iim.GetWidth(), iy=iim.GetHeight(), iew=iim.GetEffWidth();
		int rx=GetWidth(), ry=GetHeight(), rew=GetEffWidth();
		BYTE *id=iim.GetData(), *rd=GetData();

		if(to.GetW()>rc.GetW()) to.right=to.left+rc.GetW(); // to=rc
		if(to.GetH()>rc.GetH()) to.bottom=to.top+rc.GetH();

		int fx, fy, tx, ty, bx, by; // from, to, begin: x, y
		fx = to.left>=0 ? to.left : 0; tx = to.right<rx ? to.right : rx;
		fy = to.top>=0 ? to.top : 0; ty = to.bottom<ry ? to.bottom : ry;
		bx=fx; by=fy;// int fk=fy;//, fi=fx;

		if(rc.right>iim.GetWidth()) rc.right=iim.GetWidth();
		if(rc.bottom>iim.GetHeight()) rc.bottom=iim.GetHeight();

#ifdef MSV_USEXPLU
		ry=ty+ty-iy;
#endif

		// Draw
		BYTE *idp=id+rc.left*3+(rc.top)*iew, *rdp=rd+fx*3+(ry-ty)*rew;
		int /*sa=iew, se=rew,*/ rs=(tx-fx)*3; //, is=(rc.right-rc.left)*3;
		if(fx>tx) return 0;

#ifdef MSV_USEXPLU
		//idp+=iew*(ty-fy-1); rew*=-1;
		idp+=iew*((ty-fy-1)*(rc.bottom-rc.top)/(ty-fy)); iew*=-1;
#endif

		for(fy; fy<ty; fy++){
			Check(rdp);
			BYTE *i=idp, *r=rdp, *ti=idp+rs;
			while(i<ti){
				if(*i){ *r++=rgb.blue; *r++=rgb.green; *r++=rgb.red; }
				else r+=3;
				i+=3;
			}

			//memcpy(rdp, idp, rs);
			idp+=iew; rdp+=rew;
		}

	return 1;
}


int MWndIPE::DrawImageTextM(VString line, MyFontCur &cur, MRect mr, MRGB rgb){
//	bool DrawTextM(CxImage &img, char*line, unsigned int size, MRect mr, MyFontCur &cur){
SIZE sz; if(!cur.pmf) return 0; if(!useloadfont) return 0;
unsigned char*ln=(unsigned char*)line, *to=line.endu();
for(ln; ln<to; ln++){
	sz.cx=cur.pmf->font[cur.pos].szx[*ln];
	sz.cy=cur.pmf->font[cur.pos].szy[*ln];

	mbool bl; int i=0;
	bl=(bool*)cur.pmf->font[cur.pos]._data.pos(
		(2+4)*(cur.pmf->font[cur.pos].tsz-cur.pmf->font[cur.pos].sz)+cur.pmf->font[cur.pos].data[*ln]);

	for(int x=0; x<sz.cx; x++){
#ifndef MSV_USEXPLU
		BYTE *pp=GetData()+(mr.left+x)*3+(height-mr.top+1)*ewidth, *p;
#else
		BYTE *pp=GetData()+(mr.left+x)*3+(mr.top)*ewidth, *p;
#endif
		for(int ii=0; ii<sz.cy; ii++){
#ifndef MSV_USEXPLU
			p=pp-(sz.cy-ii)*ewidth;
#else
			p=pp+(sz.cy-ii)*ewidth;
#endif
			if(bl.get(i++)){
				if(!Check(p)) continue;
				*p++=rgb.blue; *p++=rgb.green; *p=rgb.red;
				//img.SetPixelColor(mr.left+x, mr.top+ii, 0xffffff);
			} 
		}
	} mr.left+=sz.cx;// mr.top+=sz.cy;

}
	return 1;
//}


}


// MWndIPE Cursor
class MWndIPECur{
	MWndIPE *ipe;
public:

	int y, ty;
	unsigned char *fp, *p, *tp;
	int sx, sy; 

	MWndIPECur(MWndIPE &i){
		ipe=&i; sx=ipe->GetWidth(); sy=ipe->GetHeight();
		First();
	}
	
	MWndIPECur(MWndIPE *i){
		ipe=i; sx=ipe->GetWidth(); sy=ipe->GetHeight();
		First();
	}

	void First(){
		if(!ipe) return ;
		y=0; ty=ipe->GetHeight();
		fp=ipe->GetData(); p=fp; tp=p+sx*3;
	}

	void End(){
		if(!ipe) return ;
		ty=ipe->GetHeight(); y=ty-1;
		fp=ipe->GetData()+y*ipe->GetEffWidth(); p=fp; tp=p+sx*3;
	}

	void Y(int i){
		if(y<0 || y>=sy){ tp=fp; }
		y=i; fp=ipe->GetData()+y*ipe->GetEffWidth(); p=fp; tp=p+sx*3;
	}

	void X(int i){
		p=fp+i*3;
	}

	void XYS(int _x, int _y, int _sx, int _sy){
		if(y<0 || y>=sy || y+_sy>sy){ tp=fp; y=ty; }
		y=_y; ty=y+_sy;
		fp=ipe->GetData()+y*ipe->GetEffWidth(); p=fp; tp=p+sx*3;
		p+=_x*3; tp= (tp>=p+_sx*3) ? p+_sx*3 : tp;
	}

	void XYSR(int _x, int _y, int _sx, int _sy){
		if(y<0 || y>=sy || y+_sy>sy){ tp=fp; y=ty; }
		y=sy-(_y+_sy); ty=y+_sy;
		fp=ipe->GetData()+y*ipe->GetEffWidth(); p=fp; tp=p+sx*3;
		p+=_x*3; tp= (tp>=p+_sx*3) ? p+_sx*3 : tp;
		return ;
	}

	int GetX(){ return (p-fp)/3; }
	int GetRX(){ return (tp-p)/3; }

	void Prev(){
		y--; fp=ipe->GetData()+y*ipe->GetEffWidth(); p=fp; tp=p+sx*3;
	}

	void Next(){
		y++; fp=ipe->GetData()+y*ipe->GetEffWidth(); p=fp; tp=p+sx*3;
	}

	int IFC(int x, int y, int c){
		if(x<0 || x>=sx || y<0 || y>=ty) return 0;
		unsigned char *p=ipe->GetData()+y*ipe->GetEffWidth()+x*3;
		//if(*p==(c&255) && *(p+1)==((c>>8)&255) && *(p+2)==((c>>16)&255))
		return ((*(int*)p)&16777215)==c;
		//return 0;
		//return (*(int*)p)&16777215==c;
	}

	void Stop(){ y=ty; }

	void Clran(){ y=0; ty=0; }
};

SIZE MWndIPE::GetTextSize(unsigned char *ln, unsigned char *to, const MyFontString *str, int rls){
	//const MyFontString *str=mf.GetString(fontsize);
	SIZE rsz={0,0}, lsz={0,0}, sz;

	if(!str) return rsz;
	
	for(ln; ln<to; ln++){
		sz.cx=str->GetW(*ln); sz.cy=str->GetH(*ln);

		if(*ln=='\n'){
			rsz.cy+=lsz.cy;
			if(rsz.cx<lsz.cx) rsz.cx=lsz.cx;
			lsz.cx=0; lsz.cy=0;
			if(rls) return rsz;
			continue;
		}

		if(lsz.cy<sz.cy) lsz.cy=sz.cy;
		lsz.cx+=sz.cx;
	}

	rsz.cy+=lsz.cy;
	if(rsz.cx<lsz.cx) rsz.cx=lsz.cx;

	// return: width, height
	return rsz;
}

void MWndIPE::TestRect(MRect &rc){
	if(rc.left<0) rc.left=0;
	if(rc.top<0) rc.top=0;
	if(rc.right>=width) rc.right=width;
	if(rc.bottom>=height) rc.bottom=height;
	return ;
}

int MWndIPE::DrawTextChar(const MyFontChar *chr, const MRect rc, MRect to, const MRGB rgb){
	if(!chr) return 0;
	TestRect(to);

	to.left=rc.left>=to.left ? rc.left : to.left; 
	to.right=rc.right<=to.right ? rc.right : to.right;
	to.top=rc.top>=to.top ? rc.top : to.top; 
	to.bottom=rc.bottom<=to.bottom ? rc.bottom : to.bottom;

	MWndIPECur cur(this);

	int y=to.top-rc.top;
	//if()

	for(y; y<to.bottom-to.top; y++){
#ifndef MSV_USEXPLU
		cur.Y(height-((to.bottom-to.top)-y-1+rc.top)-1);

		//BYTE *pp=GetData()+(mr.left+x)*3+(height-mr.top+1)*ewidth, *p;
#else
		//BYTE *pp=GetData()+(mr.left+x)*3+(mr.top)*ewidth, *p;
		cur.Y(y+rc.top);
#endif
		 cur.X(to.left);
		for(int x=to.left-rc.left; x<to.right-to.left; x++){

			if(!Check(cur.p)){
				globalerror("MWNDIPE!!!");
				continue;
			}

			if(chr->data[(x+y*chr->sx)/8]&(1<<(x+y*chr->sx)%8)){
			//if(1){
				*cur.p++=rgb.blue; *cur.p++=rgb.green; *cur.p++=rgb.red;
			} else
				//{*cur.p++=rgb.blue; *cur.p++=rgb.green; *cur.p++=rgb.red; }
				cur.p+=3;
		}

	}
	return 1;
}

int MWndIPE::DrawText(const VString text, const MyFont2 &mf, const MRect rc, const MRGB rgb, int fontsize, const int style){
	if(!IsValid() || rc.GetH()<=0 || rc.GetW()<=0) return 0;

	if(!fontsize) fontsize=18;

	const MyFontString *str=mf.GetString(fontsize);
	if(!str) return 0;

	unsigned char*ln=(unsigned char*)text, *to=text.endu();
	SIZE tsz, lsz, sz; MRect mr=rc;

	tsz=GetTextSize(ln, to, str);
	lsz=GetTextSize(ln, to, str, 1);

	if(style&VCENTER){ mr.top=mr.top+(rc.GetH()-tsz.cy)/2; }
	else if(style&BOTTOM){ mr.top=rc.bottom-tsz.cy; }

	if(style&CENTER){ mr.left=mr.left+(rc.GetW()-lsz.cx)/2; }
	else if(style&RIGHT){ mr.left=mr.left+rc.right-lsz.cx; }

	for(ln; ln<to; ln++){
		const MyFontChar *chr=str->GetChar(*ln);
		if(!chr) continue;

		mr.right=mr.left+chr->sx; mr.bottom=mr.top+chr->sy;
		DrawTextChar(chr, MRect(mr.left, mr.top, mr.left+chr->sx, mr.top+chr->sy), rc, rgb);
		mr.left+=chr->sx;
	}

	return 1;
}


int MWndIPE::DrawBox(SRect rc, MRGB rgb){ UGLOCK(*this);
	rc.And(GetRect());
	if(!rc.sx || !rc.sy) return 0;

	MWndIPECur cur(this);
	unsigned char *p, *t; int ps;

#ifndef MSV_USEXPLU
	cur.XYSR(rc.x, rc.y, rc.sx, rc.sy);
#else
	cur.XYS(rc.x, rc.y, rc.sx, rc.sy);
#endif
	
	p=cur.p; t=p; ps=cur.tp-cur.p;

	if(cur.y<cur.ty){
		memset2(cur.p, rgb, 3, ps);
	} cur.y++; t+=ewidth;

	while(cur.y<cur.ty){
		memcpy(t, p, ps);
		cur.y++; t+=ewidth;
	}

	return 1;
}

int MWndIPE::DrawImage2(MWndIPE_C iim, SRect rc, SRect to, int cnf){ UGLOCK(*this);
	if(!iim.IsValid() || !IsValid() || to.sx<=0 || to.sy<=0) return 0;

//	rc.And(GetRect());
//	if(!rc.sx || !rc.sy) return 0;

	/*

	MWndIPECur cur(this);
	unsigned char *p, *t; int ps;

#ifndef MSV_USEXPLU
	cur.XYSR(rc.x, rc.y, rc.sx, rc.sy);
#else
	cur.XYS(rc.x, rc.y, rc.sx, rc.sy);
#endif
	
	p=cur.p; t=p; ps=cur.tp-cur.p;

	if(cur.y<cur.ty){
		memset2(cur.p, rgb, 3, ps);
	} cur.y++; t+=ewidth;

	while(cur.y<cur.ty){
		memcpy(t, p, ps);
		cur.y++; t+=ewidth;
	}*/

	return 1;
}