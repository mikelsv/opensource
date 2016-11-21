// 1.0 - 16.10.14 16:06 
// 2.0

// Header structure
// ver:8, flags:8.
// crc:32.

// Data structure
// type:2 - Int, Float, String
// size:6 - 0...58 - size, 59 (-58 = next bytes), 62 - up, 63 - down.
// ext butes.
// value.
// key: size(1b), key data.

#define CJX_TYPE_ERR	0
#define CJX_TYPE_UP		1
#define CJX_TYPE_DATA	2
#define CJX_TYPE_DOWN	3

#define USEMSV_CJX

#ifdef IAM_YOU_CODE___NOOOOOOO

#define CJX_CRC				1 << 0
#define CJX_KEY_INT2		1 << 1
#define CJX_KEY_INT3		1 << 2 

#define CJX_VER_ONE			1 << 8

//#define CJX_FLAG_GZIP	1

struct CJXParseH{
	// version, flags, elements, size, gzipsize
	//unsigned int ver, flags, els, size, gsize, _nouse;
	//unsigned int ver:8;
	unsigned int flags:16;
	//unsigned int hsize:8; // nouse
	//unsigned int _nouse:8; // nouse
	unsigned int size;

	// unsigned int crc; // CJX_CRC
};

struct CJXParseL{
	//unsigned short up:1;
	unsigned short valsz:6; // 64: 0..58 - size, 59 -58 = ext bytes, 62 - up, 63 - down; 
	unsigned short u:1;		// free
	unsigned short keyis:1; // is next byte
	unsigned short keysz:5; // 31
	unsigned short unkno:3; // free 
//	unsigned char key[keysz];
//	unsigned char val[valsz];
};
#endif


class CjxProtoHead{
public:
	unsigned int hsize;
	unsigned int size;
	unsigned int flags:16;

	void Init(unsigned int sz){
		hsize = sizeof(CjxProtoHead);
		size = sz;
		flags = 0;
	}
};

class CjxProtoData{
public:
	unsigned short valsz:6; // 64: 0..58 - size, 59 -58 = ext bytes, 62 - up, 63 - down;
	unsigned short u:1;		// free
	unsigned short keyis:1; // is next byte
	unsigned short keysz:5; // 31
	unsigned short unkno:3; // free

	// Count
	unsigned int CountUp(unsigned int ksz){
		return 1 + (ksz <= 0 ? 0 : 1) + (ksz < 32 ? ksz : 31);
	}

	unsigned int Count(unsigned int ksz, unsigned int vsz){
		return  1 + (ksz <= 0 ? 0 : 1) + (ksz < 32 ? ksz : 31) + ( vsz < 59 ? 0 : (vsz < 256 ? 1 : (vsz < 65535 ? 2 : (vsz < S16M ? 3 : 4)))) + vsz;
	}

	unsigned int CountDown(){
		return 1;
	}

	// SetLine
	unsigned int SetLineUp(VString key){
		valsz = 62;

		if(key){
			keyis = 1;
			keysz = key.sz < 31 ? key.sz : 31;
			memcpy(((unsigned char*)this) + 2, key, keysz);

			return keysz + 1;
		} else
			return 1;
	}

	unsigned int SetLine(VString key, VString val){
		unsigned char *ln = (unsigned char*)this, *to = ln + 2;
		unsigned int vsz = val;
		unsigned int evsz = GetValExtSz(vsz);

		if(key){
			keyis = 1;
			keysz = key < 31 ? key : 31;
			memcpy(to, key, keysz);
			to += keysz;
		}
		else
			keyis = 0;
		
		if(!evsz)
			valsz = vsz;
		else{
			valsz = 58 + evsz;
			memcpy(to, &vsz, evsz);
			to += evsz;
		}

		memcpy(to, val, vsz);
		to += vsz;

		return to - ln;
	}

	unsigned int SetLineDown(){
		valsz = 63;
		keyis = 0;

		return 1;
	}

	unsigned int GetValExtSz(unsigned int vsz){
		return (vsz < 59 ? 0 : (vsz < 256 ? 1 : (vsz < 65535 ? 2 : (vsz < S16M ? 3 : 4))));
	}

	unsigned int GetTypeKeyVal(unsigned char *to, unsigned int &type, VString &key, VString &val){
		unsigned char *ln = (unsigned char*)this;

		if(keyis){
			ln += 2;
			if(ln + keysz >= to){
				type = CJX_TYPE_ERR;
				return 0;
			}

			key.setu(ln, keysz);
			ln += keysz;
		}
		else{
			key.Clean();
			ln ++;
		}

		if(valsz >= 62){
			type = valsz == 62 ? CJX_TYPE_UP : CJX_TYPE_DOWN;
			return ln - (unsigned char*)this;
		}

		if(valsz > 58){
			unsigned int vsz = 0;

			if(ln + (valsz - 58) > to){
				type = CJX_TYPE_ERR;
				return 0;
			}

			memcpy(&vsz, ln, valsz - 58);
			ln += valsz - 58;

			if(ln + vsz > to){
				type = CJX_TYPE_ERR;
				return 0;
			}

			val.setu(ln, vsz);
			ln += vsz;
		} else{
			if(ln + valsz > to){
				type = CJX_TYPE_ERR;
				return 0;
			}

			val.setu(ln, valsz);
			ln += valsz;
		}

		type = CJX_TYPE_DATA;
		return ln - (unsigned char*)this;
	}

};


class CjxCont{
	UList<XDataEl, 0, 1> data;
	int sz;

public:
	CjxCont(){
		sz = 0;
	}

	CjxCont(VString line){
		sz = 0;
		Read(line);
	}

	// Get Data
	operator XDataEl*(){ if(data.Size()) return data.Get(0); return 0; }
	operator XDataPEl(){ if(data.Size()) return data.Get(0); return 0; }
	XDataEl* GetData(){ if(data.Size()) return data.Get(0); return 0; }

	// GetValue
	VString GetValue(VString line){ return GetData()->GetVal(line); }
	XDataEl* Get(VString line){ return GetData()->Get(line); }

	// Operators
	VString operator[](VString line){ return GetData()->GetVal(line); }
	XDataEl* operator ()(const VString key){ return Get(key); }

	void Read(VString line){
		Release();

		CjxProtoHead &head = *(CjxProtoHead*)line.data;
		
		if(line < 4 || head.hsize > line)
			return ;

		unsigned char *ln = line.data + head.hsize, *to = line.endu();

		return Read(ln, to, data.Add());
	}

	void Read(unsigned char *&ln, unsigned char *to, XDataEl *parent){
		CjxProtoData *el;
		XDataEl *item;
		unsigned int type;
		VString key, val;

		while(ln < to){
			el = (CjxProtoData*)ln;

			ln += el->GetTypeKeyVal(to, type, key, val);

			if(type == CJX_TYPE_ERR || type == CJX_TYPE_DOWN)
				return ;

			item = data.Add();
			item->key = key;
			parent->AddToList(item);

			if(type == CJX_TYPE_UP)
				Read(ln, to, item);
			else
				item->val = val;
		}

		return ;
	}


	// Release & Clean
	void Release(){ Clear(); return ; }
	void Clear(){ data.Clear(); sz=0; }

};


#ifdef IAM_YOU_CODE___NOOOOOOO

class CJXParseD{
public:
	int _p, _n;
	VString key, val;
};

class CJXParse{
	MString _data;
	CJXParseD *data;
	int sz, wsz;

public:
	CJXParse(){
		Clean();
	}

	int Read(VString line){
		Clean();

		if(line.sz<sizeof(CJXParseH))
			return 0;

		CJXParseH &head=*(CJXParseH*)line.data;

		if(head.size+sizeof(CJXParseH)!=line.sz)
			return 0;

//		if(head.els>line.sz)
//			return 0;

		wsz=0;
//		sz=head.els;
		_data.Reserv(sz*sizeof(CJXParseD));
		data=(CJXParseD*)_data.data;
		
		int ret=ReadLine(0, line.upos(sizeof(CJXParseH)), line.endu());

		if(!ret || wsz!=sz)
			return 0;
		return 1;
	}

	int ReadLine(int el, unsigned char *ln, unsigned char *to){
		if(ln>=to)
			return 0;

		// read count
		int l=*ln, count=0;
		ln++;

		if(l&128){
			l&=127;
			if(l>4 || ln+l<to)
				return 0;
			memcpy(&count, ln, l);
			ln+=l;
		}else
			count=l;
		
		// read data
		while(ln<to){
			if(count<=0)
				return 1;
			count--;

			if(ln+2>to)
				return 0;

			CJXParseL ll=*(CJXParseL*)ln;
			ln+=sizeof(ll);
			
			int valsz=ll.valsz;
			if(ll.valsz&512){
				l=ll.valsz&511;
				if(l>4 || ln+l>to)
					return 0;
				memcpy(&valsz, ln, l);
				ln+=l;
			}

			if(ln+ll.keysz+ll.valsz>to)
				return 0;

			if(wsz>=sz)
				return 0;

			data[wsz].key.setu(ln, ll.keysz);
			data[wsz].val.setu(ln+ll.keysz, valsz);
			ln+=ll.keysz+valsz;
			wsz++;

			if(ll.up){
				int ret=ReadLine(0, ln, to);
				if(!ret)
					return 0;
			}
		}

		return 1;
	}

	VString operator[](VString key){
		return "";
	}

	void Clean(){
		data=0;
		sz=0;
	}

};


#endif