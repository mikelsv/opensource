// MikelSV (MSV) Template Constructor v.0.1.0.1
// Do Not Write To This File, All Data Well Be Lost!
// ##MD5:010fbd99afd6fd1d455ff2f8670cf245 : 2010.06.13 14:23:35


// NEW IDEAL MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class VSiLine{ // NEW Ideal Matrix
#define VSiLine_minel 16
unsigned int allsize;
public:
VSi *data;
unsigned int size;

VSiLine(){ data=0; size=0; allsize=0; }
~VSiLine(){ Clear(); }

void* AddVData(void *v, int sz, int nsz){
void *nw=_newn(sz+nsz);
if(sz) memcpy((char*)nw, (char*)v, sz);
memset((char*)nw+sz, 0, nsz);
_del(v); return nw;
}

bool Add(unsigned int add=VSiLine_minel){ if(!add) return 1;
data=(VSi*)AddVData(data, allsize*sizeof(VSi), add*sizeof(VSi));
allsize+=add;
return 1;
}
inline bool A(){ if(size==allsize) return Add(); return 0; }
inline bool A(int sz){ if(size==allsize) return Add(sz); return 0; }

inline VSi& operator [](unsigned int i){ return data[i]; }
inline unsigned int asize(){ return allsize; }
/// Input Data

///
inline VSi& n(){ return data[size]; }
void Clear(){ 
_deln(data); size=0; allsize=0; }
};// END TEMPLATE 
// Do Not Write To This File, All Data Well Be Lost!
// ##EMD5:010fbd99afd6fd1d455ff2f8670cf245

