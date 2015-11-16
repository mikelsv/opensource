#define PROJECTNAME "htmlrender"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

#define USEMSV_ITOS
#define USEMSV_XDATACONT
#define USEMSV_MWND
#define USEMSV_HTMLRENDER
#define USEMSV_MLIST
#define USEMSV_ALIST

//#define USEMSV_GAMEBOX
#define MWNDUSELOADFONT_MEM times_new_romain
#include "../../opensource/msvcore/font/times_new_romain.h"

//typedef struct tagSIZE
//{
//    unsigned int        cx;
//    unsigned int        cy;
//} SIZE;

#include "../../opensource/msvcore/msvcore.cpp"

Versions PROJECTVER[]={
"0.0.0.1", "15.11.2015 18:27"
};


int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link); LoadLang("lang");
	print(PROJECTNAME, " ", PROJECTVER[0].ver," (", PROJECTVER[0].date, ")\r\n\r\n");
	
	// analys command line
	VString l = "GO", t;
	int lto = 2, ltop = 1;

	VString in, out;
	int ishelp = 0, optsz = 0;

	while(l){
		if(!ltop && !lto)
			lto = 1;
		ltop = lto;

		if(lto == 1){
			l = t;
			t = link.GetArg();
			lto = 0;
		} else
			if(lto == 2){
				l = link.GetArg();
				t = link.GetArg();
				lto = 0;
			}
		if(!l)
			break;

		if(!in){
			lto = 1;
			in = l;
		}		
		else if(!out){
			lto = 1;
			out = l;
		} else{
			print("Invalid options: unknown key", " '", l, "'\r\n");
			return 1;
		}
	}

	if(ishelp || !in || !out){
		print("Usage: htmlrender input.html output.jpg" "\r\n" \
			""
			);

		return 0;
	}


	HtmlRender hren;

	if(hren.RenderFile(in, out)){
		print("Render ok.\r\n");
		return 0;
	} else{
		print("Render FAIL.\r\n");
		return -1;
	}

	//TString mem = hren.RenderToMem(LoadFile("html.htm"));
	//SaveFile("image.bmp", mem);

	return 0;

	/*
	// 1. Get simple html
	TString html = LoadFile("html.htm");

	// 2. Parse this html
	XDataCont ct;
	ct.ReadXML(html);
	XDataEl *el = ct;

	// 3. Create new structure and dynamic array
	// Created HtmlRenderEl.

	// 4. Convert html to HtmlRenderEl.
	HtmlRenderEl rel; // parent element
	HtmlToRender(ct, &rel);

	// 5. Need images library
	// Ok. I'am used CxImage library.

	// 6. MWndIPE - my image memory buffer.
	MWndIPE ipe;

	// Test it
	ipe.New(100, 100);
	ipe.Clear(MRGB(255, 0, 0));
	//ipe.Save("image.jpg");
	// Ok

	// 7. Fonts. Used old fonts project;
	ipe.DrawText("Hello World!", mydefaultfont, MRect(0, 0, 100, 100), "000000");
	//ipe.Save("image.jpg");
	// Ok

	// 8. Count rect of text blocks
	RenderAnalysRect(ipe, &rel);

	// 9. Problem. Text: <tag>Hello World<br>Hi friends!</tag>  Parse result: <tag><br>Hi friends!</tag>
	// Ok. <tag> -> "Hello World" , <br> -> "Hi friends".
	// Problems of <br>. Need <br />.

	// 10. Count text position
	unsigned int pos_x = 0, pos_y = 0;
	SIZE box; // Image size
	box.cx = 0;
	box.cy = 0;
	RenderAnalysPos(box, pos_x, pos_y, &rel);

	// 11. Render it!
	ipe.New(box.cx, box.cy);
	ipe.Clear("FFFFFF");
	RenderDraw(ipe, &rel);

	// 12. And save it.
	ipe.Save("image.jpg");

	// Ok. Simple render ready.
	*/

	// In the next series:
	// Create render class
	// Set max width.
	// Add normal tags <br>, <a>, <font>
	// Optimize code

	// The end. Thanks.

	// New stream go...

	return 0;
}

