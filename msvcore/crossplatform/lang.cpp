class MultiLanguage{ // not used
	ConfLine lang, lang2, lang3;

public:
	int Load(VString name){
		return lang.Load(name);
	}

	int Load2(VString name){
		return lang2.Load(name);
	}

	int Load3(VString name){
		return lang3.Load(name);
	}

	VString Lang(VString line){
		VString ret;
		if(lang3.Is()){ ret=lang3[line]; if(ret) return ret; }
		if(lang2.Is()){ ret=lang2[line]; if(ret) return ret; }
		if(lang.Is()){ ret=lang[line]; if(ret) return ret; }
		return line;
	}

};


// MultiLanguageT
//	def line
//	tr line
//	end

class MultiLanguageT{
	ConfLine lang;
	int ew;

public:
	MultiLanguageT(){
		ew=0;
	}

	int Load(VString file){
		return lang.Load(file);
	}

	void EnableWrite(){ ew=1; }

	VString Lang(VString line){
//		ItosX <S2K> it;
		MString it; it.Add(":def '", line, "'");
		VString o=lang.data, t;

		unsigned char *pos=lang.data, *to=lang.data.endu(), *ln=0;//, *tpos;
		int r=rtms(to, it, it, pos); pos=pos+it.sz;
		if(r && pos<to && (*pos==10 || *pos==13)){
			while(pos<to && (*pos==10 || *pos==13)){ pos++; }
			if(incompare(pos, to-pos, (unsigned char*)":else ", 6)){ pos+=6; ln=pos;
				r=rtms(to, (unsigned char*)"\n:end", 5, pos);
				if(r){
					if(*(pos-1)=='\r') pos--;
					if(ln && pos-ln>2 && *ln=='\'' && *(pos-1)=='\''){ ln++; pos--; }
					return VString(ln, pos-ln);
				}
			}
		} else pos=to;


		if(!ew) return line;

		if(pos==to){
			lang.data.Add(lang.data, "\r\n:def '", line, "'\r\n:else '", line, "'\r\n:end\r\n");
		}
		else{
			lang.data.Add(VString(lang.data.data, pos-lang.data.data), "\r\n:else ", line, "\r\n:end\r\n", VString(pos, to-pos));
		}

		lang.Save();
		return line;
	}
};


//#define MSVUSELOADLANG // WRITE IT TO CPP!!!
//#ifndef USEMSV_LOADLANG
//#error "Write #define MSVUSELOADLANG to cpp!"
//#endif

MultiLanguageT lang; MString _lang_tmp[5]; int _lang_tmp_i=0;
//VString msv_load_lang;

VString Lang(VString line){
	VString l=lang.Lang(line); if(!l) return line;
	if(_TSYSC==TSYSC_DOS){
		_lang_tmp_i++; if(_lang_tmp_i>=5) _lang_tmp_i=0;
		_lang_tmp[_lang_tmp_i]=ctodos(l);
		return _lang_tmp[_lang_tmp_i];
	}
	else if(_TSYSC==TSYSC_UTF){
		_lang_tmp_i++; if(_lang_tmp_i>=5) _lang_tmp_i=0;
		_lang_tmp[_lang_tmp_i]=ctoutf(l);
		return _lang_tmp[_lang_tmp_i];
	}
	else return l;
}

#define ___D( x ) #x
#define __D( marker, x ) ___D( marker ## x )
#define _D( x ) __D( LangMarker, x )


//#define Lang(...) LangG g;
//#define Lang(...) Lang(__VA_ARGS__) 

//#define Lang( x ) TextNode< x >::Text
/*
struct Node{
  static const Node*Root;
  const char*Text;
  const Node*Next;
  operator const char*( void )const{ return Text; }
  Node( const char*Text ) : Text( Text ), Next( Root ){
    Root = this;
  }
};

template< char*T >
struct TextNode{
public:
	const Node Text = T;
//	static VString Text(){
		
//	}
	//TextNode(char* T){}
  //static const Node Text = T;
	//char* z = 0;
};*/

void ParseLang();

void LoadLang(VString lpath){ // global path, local path
	MString p;// p.Add(_thispath, lpath);// int up=path && IsFile(p.Add(path, lpath, "/en.lang"));
	p.Add(_thispath, "/", lpath, "/");

	if(msv_load_lang){
		lang.Load(p+msv_load_lang+".lang"); msv_use_lang=msv_load_lang;
	}
	else{
		lang.Load(p+_TSYSL+".lang");  msv_use_lang=_TSYSL;
	}

/*
	if(msv_load_lang && IsDir(p)){
		lang.Load(p+msv_load_lang+".lang"); msv_use_lang=msv_load_lang;
	}
	else { // if(_TSYSL && IsDir(p))
		lang.Load(p+_TSYSL+".lang");  msv_use_lang=_TSYSL;
	}
	//else{
	//	lang.Load(p+"en.lang"); msv_use_lang="en";
	//}*/

	lang.EnableWrite();

	if(msv_parse_lang) ParseLang();

	return ;
}

void ParseLang(){
	Readdir rd; ILink il;
	rd.ReadDir("*");

	for(unsigned int i=0; i<rd.size(); i++){
		if(rd[i].isdir()) continue;

		il.Link(rd[i].key);
		if(il.ext()=="cpp" || il.ext()=="h" || il.ext()=="c" || il.ext()=="hxx"){
			MString data=LoadFile(il.file);
			VString l=data, o, t;

			while(l){
				o=PartLine(l, l, "Lang(\""); t=PartLine(l, l, "\")");
				if(t.sz && t.endo()!='\\'){
					Lang(t);
				}
			}
		}
	}

	return ;
}