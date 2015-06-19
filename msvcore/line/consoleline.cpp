void SetLinuxColor(unsigned short col){
	char d[]={27, '[', '0', ';', '3', '1', 'm'};
	//char d[]={27, '[', '2', ';', '3', 'x'};
	//Esc[2;nx
//		print("z", itos(col), " ");
	d[2]=col>9 ? '1' : '0';
	d[5]=col%10+'0';
	write(1, &d, sizeof(d));
	return ;
}
	
void SetLinuxBgColor(unsigned short col){
	char d[]={27, '[', '0', ';', '4', '2', 'm'};
	d[2]=col>9 ? '1' : '0';
	d[5]=col%10+'0';
	write(1, &d, sizeof(d));
	return ;
}


class CodePage{
	int incp, outcp;
	MString tmp;

public:
	CodePage(){ incp=0; outcp=0; }

	int InCp(){ return incp; }
	int InCp(int t){ return incp=t; }
	int OutCp(){ return outcp; }
	int OutCp(int t){ return outcp=t; }

	static int GetCode(VString line){
		unsigned char *ln=line, *to=line.endu();

		int w=0, u=0;

		while(ln<to){
			if(*ln<128){} // english
			else if((*ln&224)==192){ u++; ln++; } // twobyte
			else if((*ln&240)==224){ u++; ln+=2; } // three byte
			else if((*ln&248)==240){ u++; ln+=3; }
			else{
				w++;
			}

			ln++;
		}

		if(w>u) return TSYSC_WIN; else return TSYSC_UTF;
	}

	VString GetLine(VString line){
		int cp=GetCode(line);

		if(!outcp) return line;
		if(outcp==cp) return line;
		if(cp==TSYSC_WIN && outcp==TSYSC_UTF){  tmp=ctoutf(line); line=tmp; return line; }
		if(cp==TSYSC_UTF && outcp==TSYSC_WIN){  tmp=utftoc(line); line=tmp; return line; }

		return line;
	}
};

contype ConsoleRead(unsigned int &linuxread, unsigned int &linuxread_esc);

class ConsoleLine{
	unsigned short defc, defbg;
	unsigned short incol, inbgcol;

	// linuxread
	unsigned int linuxread, linuxread_esc;

public:
	ConsoleLine(){
		Clean();
	}

	void Clean(){
		defc=MRGB("cccccc"); defbg=MRGB("000000");
		incol=65534; inbgcol=65534;
		linuxread=0; linuxread_esc=0;
	}

	void Print(DrawDLine &dl){
	unsigned short col=defc, bg=defbg; unsigned short c;
	DrawDLineU *get;
	
	while(get=dl.Get()){
		switch(get->type){
			case DRAWDLINE_TEXT:{
				Print(VString(get->t.data, get->t.sz), col, bg);
			break;}

			case DRAWDLINE_COLOR:
				col=MRGB(get->c.rgb).sh();
			break;

			case DRAWDLINE_BGCOLOR:
				bg=MRGB(get->c.rgb).sh();
			break;

			case DRAWDLINE_FONT:
			break;

			case DRAWDLINE_END:
				Print("\r\n");
			break;
		}
	}
	return ;
}

	void Print(VString line, unsigned short col=65534, unsigned short bg=65534){
		if(col==65534) col=defc; if(bg==65534) bg=defbg;

		

#ifdef WIN32
		DWORD dw;
		SetConsoleTextAttribute(_std_output_handle, cicol(col, bg));
		WriteFile(_std_output_handle, line, line, &dw, 0);
#else
		//CHAR_INFO ci;
		col=cilcol(col); if(col!=incol){ SetLinuxColor(col); incol=col; }
		bg=cilcol(bg); if(bg!=inbgcol){ SetLinuxBgColor(bg); inbgcol=bg; } // color
		//ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
		//write(1, &ci.Char.UnicodeChar, 2);
		if(CodePage::GetCode(line)==TSYSC_UTF){
			MString utf=stoutf(line); write(1, utf, utf); return ;			
		}
		write(1, line, line);

#endif
	}

	contype Read(){
		return ConsoleRead(linuxread, linuxread_esc);
	}

	void Test(){
		Print("Test: ");
		for(int r=0; r<32; r+=4){
			for(int g=0; g<64; g+=8){
				for(int b=0; b<32; b+=4){
					Print("*", (r<<11)+(g<<5)+b);
				}
			}
		}
		Print("\r\n"); Print("Enter command.\r\n\r\n\r\n\r\n\r\n");
		//for(int i=0; i<12; i++){ Drawn("\r\n"); Sleep(100); }
	}

	static WORD cicol(unsigned short c, unsigned short b){
		WORD ret=0;// int z=0;
		int t=c&0x1f; if(t>=32/3) ret|=FOREGROUND_BLUE; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
		t=(c&0x7e0)>>5; if(t>=64/3) ret|=FOREGROUND_GREEN; if(t>=64*2/3) ret|=FOREGROUND_INTENSITY;t=(c&0x1f800)>>11;
		if(t>=32/3) ret|=FOREGROUND_RED; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
		//if(z>=64) ret|=FOREGROUND_INTENSITY; z=0;

		t=b&0x1f; if(t>=32/3) ret|=BACKGROUND_BLUE; if(t>=32*2/3) ret|=BACKGROUND_INTENSITY;
		t=(b&0x7e0)>>5; if(t>=64/3) ret|=BACKGROUND_GREEN; if(t>=64*2/3) ret|=BACKGROUND_INTENSITY;
		t=(b&0x1f800)>>11; if(t>=32/3) ret|=BACKGROUND_RED; if(t>=32*2/3) ret|=BACKGROUND_INTENSITY;//if(z>=64) ret|=BACKGROUND_INTENSITY; z=0;

		ret|=COMMON_LVB_LEADING_BYTE|COMMON_LVB_TRAILING_BYTE|COMMON_LVB_GRID_HORIZONTAL|COMMON_LVB_UNDERSCORE;
		return ret;
	}

	static WORD cilcol(unsigned short c){
		c=cicol(c, 0);

		int i=c&FOREGROUND_INTENSITY ? 10 : 0;
		c=c&(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		switch(c){
			case 0: return i+0; // black
			case FOREGROUND_RED: return i+1; // red
			case FOREGROUND_GREEN: return i+2; // green
			case FOREGROUND_BLUE: return i+4; // blue

			case FOREGROUND_BLUE+FOREGROUND_GREEN: return i+6; // cyan
			case FOREGROUND_BLUE+FOREGROUND_RED: return i+5; // magenta
			case FOREGROUND_GREEN+FOREGROUND_RED: return i+3; // yellow
			default: return i+7; break;
		}

		return 0;
	}

};