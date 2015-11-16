// Very simple html render
// index.html -> index.jpg

Versions HtmlRenderVer[]={
"0.0.0.2", "16.11.2015 17:48",
"0.0.0.1", "15.11.2015 18:27"
};

#ifndef USEMSV_MWND
	#error Use: #define USEMSV_MWND
#endif

#ifndef USEMSV_ALIST
	#error Use: #define USEMSV_ALIST
#endif


// Html Render Types
#define HTMLRENT_TEXT	1
// And tags
#define HTMLRENT_BR		2


// Render element. 
class HtmlRenderEl : public OMatrixT<HtmlRenderEl>{
public:
	// OMatrixT<HtmlRenderEl> add *_a, *_e; Pointer to next level.
	HtmlRenderEl *_p, *_n; // prev, next

	// Data
	int type;
	VString text;
	unsigned int font_size;

	// Render data
	int sx, sy;
	int px, py;

	HtmlRenderEl(){
		_p = 0;
		_n = 0;
		type = 0;
		sx = 0;
		sy = 0;
		px = 0;
		py = 0;

		font_size = 0;
	}

	// New
	//HtmlRenderEl* New(){
	//	// New
	//	HtmlRenderEl *p = new HtmlRenderEl;
	//	// Add element, this - parent.
	//	OMAdd(p);
	//	return p;
	//}

};

#define HTMLRENDERT_FILE	1
#define HTMLRENDERT_MEM		2

class HtmlRender{
	// root element
	HtmlRenderEl rel; 
	// HtmlRenderEl array
	AList2d<AListAllocHLString<HtmlRenderEl>, HtmlRenderEl> data;
	// Html parser
	XDataCont xml;
	// Image buffer
	MWndIPE ipe;

	// default font size
	int opt_font_size;

public:
	HtmlRender(){
		opt_font_size = 24;
	}

	// Render to file
	int RenderFile(VString file, VString to){
		return Render(LoadFile(file), to);
	}

	int Render(VString data, VString to){
		return RenderProc(HTMLRENDERT_FILE, data, to);
	}

	// Render to mem
	TString RenderToMem(VString data){
		TString ret;

		RenderProc(HTMLRENDERT_MEM, data, VString(), &ret);

		return ret;
	}

protected:
	TString RenderProc(int type, VString data, VString to, TString *tomem = 0){
		// Html parse
		xml.ReadXML(data);

		// Html to render struct
		HtmlToRender(xml, &rel);

		// Count rects
		RenderAnalysRect(ipe, &rel);

		// Count text position
		unsigned int pos_x = 0, pos_y = 0;
		unsigned int box_x = 0, box_y = 0;

		RenderAnalysPos(box_x, box_y, pos_x, pos_y, &rel);

		// Render
		ipe.New(box_x, box_y);
		ipe.Clear("FFFFFF");
		RenderDraw(ipe, &rel);

		// Save result
		if(to)
			ipe.Save(to);
		else if(tomem)
			ipe.SaveToMem(*tomem);

		return 1;
	}

	// Html to render
	void HtmlToRender(XDataEl *el, HtmlRenderEl *rel){
		while(el){
			HtmlRenderEl *r = data.New(rel);

			if(el->key){ // tag

			}

			if(el->val){ // text
				r->text = el->val;
			}

			if(el->_a)
				HtmlToRender(el->_a, r); // ??

			el = el->_n;
		}

		return ;
	}

	void RenderAnalysRect(MWndIPE &ipe, HtmlRenderEl *el){
		// Add in HtmlRenderEl sx, sy and count text rect

		while(el){
			if(el->_a)//{
				RenderAnalysRect(ipe, el->_a);
			
			//	// Count up level rect
			//	SIZE sz = RenderAnalysRect(el);
			//	el->sx = sz.cx;
			//	el->sy = sz.cy;
			//} else{
			//	el->sx = 0;
			//	el->sy = 0;
			//}
		
			if(el->text){
				// Text rect
				SIZE sz = ipe.GetTextSize(el->text, el->text.endu(), mydefaultfont.GetString(el->font_size ? el->font_size : opt_font_size));
				el->sx = sz.cx;
				el->sy = sz.cy;
			}

			el = el->_n;
		}

		return ;
	}

	void RenderAnalysPos(unsigned int &box_x, unsigned int &box_y, unsigned int &pos_x, unsigned int &pos_y, HtmlRenderEl *el){
		// Count text position

		while(el){
			if(el->sx && el->sy){
				el->px = pos_x;
				el->py = pos_y;

				if(box_x < pos_x + el->sx)
					box_x = pos_x + el->sx;
				if(box_y < pos_y + el->sy)
					box_y = pos_y + el->sy;

				// Add test on newline

				pos_x = 0;
				pos_y += el->sy;
			}

			if(el->_a)
				RenderAnalysPos(box_x, box_y, pos_x, pos_y, el->_a);

			el = el->_n;
		}

		return ;
	}

	void RenderDraw(MWndIPE &ipe, HtmlRenderEl *el){
		while(el){
			if(el->_a)
				RenderDraw(ipe, el->_a);

			if(el->text){
				ipe.DrawText(el->text, mydefaultfont, MRect(el->px, el->py, el->px + el->sx, el->py + el->sy), "000000", el->font_size ? el->font_size : opt_font_size);
			}

			el = el->_n;
		}
		return ;
	}

};


//SIZE RenderAnalysRect(HtmlRenderEl *el){
//	int sx = 0, sy = 0;
//	
//	while(el){
//		sx += el->sx;
//		if(el->sy > sy)
//			sy = el->sy;
//
//		el = el->_n;
//	}
//
//	SIZE sz;
//	sz.cx = sx;
//	sz.cy = sy;
//	return sz;
//}