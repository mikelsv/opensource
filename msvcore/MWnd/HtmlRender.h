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
#define HTMLRENT_P		3
#define HTMLRENT_A		4
#define HTMLRENT_FONT	5
#define HTMLRENT_LEFT	6
#define HTMLRENT_CENTER	7
#define HTMLRENT_RIGHT	8


// Render element. 
class HtmlRenderEl : public OMatrixT<HtmlRenderEl>{
public:
	// OMatrixT<HtmlRenderEl> add *_a, *_e; Pointer to next level.
	HtmlRenderEl *_p, *_n; // prev, next

	// Data
	int type;
	VString text;
	unsigned int font_size;
	MRGB font_color;

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

// Render tag info
struct HtmlRenderTagInfo{
	HtmlRenderTagInfo *ti;
	HtmlRenderEl *el;

	//int is_tag_a;

	HtmlRenderTagInfo(){
		ti = 0;
		el = 0;

//			is_tag_a = 0;
	}

	//bool IsTagA(){
	//	HtmlRenderTagInfo *t = this;

	//	while(t){
	//		if(t->el && t->el->type == HTMLRENT_A)
	//			return 1;

	//		t = t->ti;
	//	}

	//	return 0;
	//}

	HtmlRenderTagInfo* IsTag(int type){
		HtmlRenderTagInfo *t = ti;

		while(t){
			if(t->el && t->el->type == type)
				return t;

			t = t->ti;
		}

		return 0;
	}

	int IsTagAlign(){
		HtmlRenderTagInfo *t = ti;

		while(t){
			if(t->el && (t->el->type == HTMLRENT_LEFT || t->el->type == HTMLRENT_CENTER || t->el->type == HTMLRENT_RIGHT))
				return t->el->type;

			t = t->ti;
		}

		return 0;
	}

	//bool IsTagLeft(){
	//	return IsTag(HTMLRENT_LEFT);
	//}

	//bool IsTagRight(){
	//	return IsTag(HTMLRENT_RIGHT);
	//}

	//bool IsTagCenter(){
	//	return IsTag(HTMLRENT_CENTER);
	//}

	MRGB GetTagFontColor(){
		HtmlRenderTagInfo *t = ti;

		while(t){
			if(t->el && t->el->type == HTMLRENT_A)
				return "0000FF";
			else if(t->el && t->el->type == HTMLRENT_FONT){
				return t->el->font_color;
			}

			t = t->ti;
		}

		return MRGB(0, 0, 0);
	}

	int GetTagFontSize(){
		HtmlRenderTagInfo *t = ti;

		while(t){
			if(t->el && t->el->type == HTMLRENT_FONT){
				return t->el->font_size;
			}

			t = t->ti;
		}

		return 0;
	}

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
		xml.ReadHtml(data);

		// Html to render struct
		HtmlToRender(xml.GetData()->a(), &rel);

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
				if(el->key.compareu("br")){
					r->type = HTMLRENT_BR;
				}
				else if(el->key.compareu("p")){
					r->type = HTMLRENT_P;
				}
				else if(el->key.compareu("a")){
					r->type = HTMLRENT_A;
				}
				else if(el->key.compareu("font")){
					r->type = HTMLRENT_FONT;
					HtmlToRenderTagFont(el, r);					
				}
				else if(el->key.compareu("center")){
					r->type = HTMLRENT_CENTER;
				}
				else if(el->key.compareu("left")){
					r->type = HTMLRENT_LEFT;
				}
				else if(el->key.compareu("right")){
					r->type = HTMLRENT_RIGHT;
				}
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

	// Html to Render. Tag font.
	void HtmlToRenderTagFont(XDataEl *el, HtmlRenderEl *rel){
		el = el->_l;

		while(el){
			if(el->key == "color" && el->val[0] == '#'){
				rel->font_color = el->val.str(1);
			}
			else if(el->key == "size"){
				rel->font_size = el->val.toi();
			}

			el = el->_l;
		}

		return ;
	}

	SIZE RenderAnalysRect(MWndIPE &ipe, HtmlRenderEl *el, HtmlRenderTagInfo *ti = 0){
		// Add in HtmlRenderEl sx, sy and count text rect
		
		// Tag info
		HtmlRenderTagInfo taginfo;
		if(ti){
			taginfo.ti = ti; // Prevous tag
		}

		// Box size
		SIZE bsz;
		bsz.cx = 0;
		bsz.cy = 0;

		while(el){

			if(!el->type && ti){
				el->font_color = taginfo.GetTagFontColor();
				el->font_size = taginfo.GetTagFontSize();
			}

			switch(el->type){
				//case HTMLRENT_BR:
				////	pos_x = 0;
				////	pos_y = box_y;
				//break;
				//case HTMLRENT_P:
				////	pos_x = 0;
				////	pos_y += 2 * (box_y - pos_y);
				//break;
				case HTMLRENT_A:
					taginfo.el = el;
				break;
				case HTMLRENT_FONT:
					taginfo.el = el;
				break;
				default:
					taginfo.el = 0;
				break;
			}

			if(el->_a){
				SIZE s = RenderAnalysRect(ipe, el->_a, &taginfo);
				el->sx = s.cx;
				el->sy = s.cy;

				bsz.cx += s.cx;
				if(bsz.cy < s.cy)
					bsz.cy = s.cy;
			}
			
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

		return bsz;
	}

	void RenderAnalysPos(unsigned int &box_x, unsigned int &box_y, unsigned int &pos_x, unsigned int &pos_y,
		HtmlRenderEl *el, HtmlRenderTagInfo *ti = 0){
		// Count text position

		// Tag info
		HtmlRenderTagInfo taginfo;
		if(ti){
			taginfo.ti = ti; // Prevous tag
		}

		while(el){
			if(el->sx && el->sy){
				el->px = pos_x;
				el->py = pos_y;

				if(box_x < pos_x + el->sx)
					box_x = pos_x + el->sx;
				if(box_y < pos_y + el->sy)
					box_y = pos_y + el->sy;

				if(ti){
					int align = taginfo.IsTagAlign();
					if(align == HTMLRENT_LEFT)
						el->px = 0;
					else if(align == HTMLRENT_CENTER)
						el->px = (box_x - el->sx) / 2;
					else if(align == HTMLRENT_RIGHT)
						el->px = box_x - el->sx;

					if(align)
						pos_y += el->sy;	
					
					//if(taginfo.IsTagFont()){
						//el->font_color = taginfo.GetTagFontColor();
					//}

					//if(taginfo.IsTagFont()){
						//el->font_size = taginfo.GetTagFontSize();
					//}

				}

				// Add test on newline

				if(el->type == HTMLRENT_BR){
					pos_x = 0;
					pos_y += el->sy;
				} else{
					//pos_x = 0;
					pos_x += el->sx;
					//pos_y += el->sy;
				}
			}

			switch(el->type){
				case HTMLRENT_BR:
					pos_x = 0;
					pos_y = box_y;
				break;
				case HTMLRENT_P:
					pos_x = 0;
					pos_y += 2 * (box_y - pos_y);
				break;
				case HTMLRENT_LEFT:
				case HTMLRENT_CENTER:
				case HTMLRENT_RIGHT:
					taginfo.el = el;
				break;
				//case HTMLRENT_A:
				//	taginfo.el = el;
				//break;
				//case HTMLRENT_FONT:
				//	taginfo.el = el;
				//break;
				default:
				break;
			}

			if(el->_a)
				RenderAnalysPos(box_x, box_y, pos_x, pos_y, el->_a, &taginfo);

			el = el->_n;
		}

		return ;
	}

	void RenderDraw(MWndIPE &ipe, HtmlRenderEl *el){
		while(el){
			if(el->_a)
				RenderDraw(ipe, el->_a);

			if(el->text){
				ipe.DrawText(el->text, mydefaultfont,
					MRect(el->px, el->py, el->px + el->sx, el->py + el->sy), el->font_color, el->font_size ? el->font_size : opt_font_size);
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