// Assembler parser

// http://ref.x86asm.net/coder32.html

unsigned char asmcode[256]={
	// 00
0,
0, 
0,
6, // 03 05 70 D9 4D 00 add         eax,dword ptr [_msvps_new_size (4DD970h)] 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// 10
0,
0, 
0,
99, // ext
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// 20
0,
0, 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// 30
0,
0, 
0,
2, // 33 C5            xor         eax,ebp 
0,
0,
0,
0,
0,
2, // 39 0a            cmp     DWORD PTR [edx], ecx
0,
2, // 3B F4            cmp         esi,esp
0,
0,
0,
0,
	// 40
0,
0, 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// 50
1, // 50               push        eax
1, // 51               push        ecx
1, // 52               push    edx
1, // 53               push        ebx  
0,
1, // 55               push        ebp
1, // 56               push        esi  
1, // 57               push        edi
1, // 58               pop     eax
1, // 59               pop         ecx  
1, // 5A               pop     edx
1, // 5B               pop         ebx
0,
1, // 5D               pop         ebp  
1, // 5E               pop         esi
1, // 5F               pop         edi
	// 60
0,
0, 
0,
0,
99, // ext
0,
0,
0,
5, // 68 6B 6F 4C 00   push        offset __ehhandler$?my_malloc@@YAPAXI@Z (4C6F6Bh) 
0,
2, // 6A 01            push        1
0,
0,
0,
0,
0,
	// 70
0,
0, 
0,
0,
2, // 74 2B            je          my_malloc+87h (493B37h) 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// 80
0,
99, // ext
0,
99, // ext
0,
0,
0,
0,
0,
99, // ext
0,
99, // ext
0,
99, // ext
0,
0,
	// 90
0,
0, 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// A0
0,
5, // A1 E8 61 31 10   mov         eax,dword ptr [__newmode (103161E8h)]
0,
5, // A3 B0 FD 4D 00   mov         dword ptr [_msvps_new_size (4DFDB0h)],eax 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// B0
0,
0, 
0,
0,
0,
0,
0,
0,
5, // B8 CC CC CC CC   mov         eax,0CCCCCCCCh
5, // B9 37 00 00 00   mov         ecx,37h 
0,
0,
0,
0,
0,
0,
	// C0
0,
0, 
3, // c2 08 00         ret     8
1, // C3               ret
0,
0,
5, // C6 05 4D 00 00
7, // C7 45 FC 00 00 00 00 mov         dword ptr [ebp-4],0 
0,
0,
0,
0,
1, // CC               int         3
0,
0,
0,
	// D0
0,
0, 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
	// E0
0,
0, 
0,
0,
0,
0,
0,
0,
5, // E8 55 48 FF FF   call        _nh_malloc_dbg (102CFAB0h)
5, // jmp rel16/32
0,
0,
0,
0,
0,
0,
	// F0
0,
0, 
0,
2, // F3 AB            rep stos    dword ptr es:[edi] 
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
6  // FF 15 40 D9 4D 00 call        dword ptr [f_my_malloc (4DD940h)] 
};

unsigned char asmcode13[256], asmcode64[256], asmcode81[256], asmcode83[256], asmcode89[256], asmcode8B[256], asmcode8D[256];

class initasmcode{

public:
	initasmcode(){
		memset(asmcode13, 0, 256); memset(asmcode64, 0, 256); memset(asmcode81, 0, 256); 
		memset(asmcode83, 0, 256); memset(asmcode89, 0, 256); memset(asmcode8B, 0, 256); memset(asmcode8D, 0, 256);

		asmcode13[0xD1]=2;	// 13 D1            adc         edx,ecx 
		asmcode13[0x0D]=6;	// 13 0D 3C 01 31 01    adc         ecx,dword ptr [_msvps_new_size+4 (131013Ch)]  

		asmcode64[0xA1]=6;	// 64 A1 00 00 00 00 mov         eax,dword ptr fs:[00000000h] 
		asmcode64[0xA3]=6;	// 64 A3 00 00 00 00 mov         dword ptr fs:[00000000h],eax 
		asmcode64[0x89]=7;  // 64 89 0D 00 00 00 00 mov         dword ptr fs:[0],ecx 

		asmcode81[0xEC]=6;  // 81 EC DC 00 00 00 sub         esp,0DCh 
		asmcode81[0xC4]=5;	// 81 c4 c0 00 00


		asmcode83[0xC0]=3;	// 83 C0 01         add         eax,1 
		asmcode83[0xC4]=3;  // 83 C4 14         add         esp,14h
		asmcode83[0x7D]=4;	// 83 7D EC 00      cmp         dword ptr [ebp-14h],0
		
		asmcode89[0x0D]=6;	// 89 0D 3C 01 31 01    mov         dword ptr [_msvps_new_size+4 (131013Ch)],ecx  
		asmcode89[0x48]=3;	// 89 48 0c         mov     DWORD PTR [eax+12], ecx
		asmcode89[0x4D]=3;	// 89 4D F8         mov     DWORD PTR _this$[ebp], ecx
		asmcode89[0x15]=6;	// 89 15 B4 FD 4D 00 mov         dword ptr [_msvps_new_size+4 (4DFDB4h)],edx 
		asmcode89[0x45]=3;  // 89 45 FC         mov         dword ptr [res],eax
		
		
		asmcode8B[0x08]=2;	// 8B 08            mov     ecx, DWORD PTR [eax]
		asmcode8B[0x15]=6;	// 8B 15 B4 FD 4D 00 mov         edx,dword ptr [_msvps_new_size+4 (4DFDB4h)] 
		asmcode8B[0x45]=3; 	// 8B 45 FC         mov         eax,dword ptr [res]
		asmcode8B[0x4D]=3; 	// 8B 4D 08         mov         ecx,dword ptr [nSize] 
		asmcode8B[0x8A]=5; 	// 8B 8A 30 FF FF
		asmcode8B[0xE5]=2; 	// 8B E5            mov         esp,ebp 
		asmcode8B[0xEC]=2; 	// 8B EC            mov         ebp,esp 
		asmcode8B[0xF4]=2; 	// 8B F4            mov         esi,esp
		asmcode8B[0xFF]=2; 	// 8B FF            mov         edi,edi

		

		asmcode8D[0x45]=3;	// 8D 45 F4         lea         eax,[ebp-0Ch] 
		asmcode8D[0x85]=6;	// 8D 85 1C FF FF FF lea         eax,[ebp-0E4h] 
		asmcode8D[0x8D]=6;	// 8D 8D 1C FF FF FF lea         ecx,[ebp-0E4h] 
		asmcode8D[0xBD]=6;	// 8D BD 18 FF FF FF lea         edi,[ebp-0E8h] 
	}

} initasmcode;

unsigned char* getasmcode(unsigned char *ln, int &l){
	if(!ln) return 0;

	unsigned char c=asmcode[*ln];
	if(c!=99){ l=1; return &asmcode[*ln]; }

	unsigned char *b=0;
	if(*ln==0x13) b=asmcode13; if(*ln==0x64) b=asmcode64; if(*ln==0x81) b=asmcode81;
	if(*ln==0x83) b=asmcode83; if(*ln==0x89) b=asmcode89;
	if(*ln==0x8B) b=asmcode8B; if(*ln==0x8D) b=asmcode64; if(*ln==0x8D) b=asmcode8D;

	if(b[*(ln+1)]!=99){ l=2; return &b[*(ln+1)]; }

	return 0;
}

class asmcodesize{
	MString _data;

	unsigned char *data;
	unsigned int sz, upd;

public:

	asmcodesize(){ data=0; sz=0; upd=1; }

	void Upd(int v=1){ upd=v; }

// Get & Set
	unsigned char Get(unsigned char *line, unsigned int &l, unsigned char *val=0){ // return last value, may set new val.
		if(!data) return 0;

		unsigned char *d=data; unsigned int x=0; l=0;
		unsigned char *ln=line;

		while(1){
			unsigned char c= *ln%2 ? d[*ln/2]>>4 : d[*ln/2]&0xF;
			// add box
			if(val && c && c!=0xF && c!=*val){
				if(!upd || !AddBox((x<<8)+*ln)) return 0;
				c=0xF;
				if(*ln%2) d[*ln/2]=(d[*ln/2]&0x0F)|((c&0xF)<<4); else d[*ln/2]=(d[*ln/2]&0xF0)|(c&0xF);
				return Get(line, l, val);
			}

			if(c!=0xF){
				if(val) if(*ln%2) d[*ln/2]=(d[*ln/2]&0x0F)|((*val&0xF)<<4); else d[*ln/2]=(d[*ln/2]&0xF0)|(*val&0xF);
				return c;
			}

			x<<=8; x+=*ln; l++; ln++;
			d=GetBox(x);
			if(!d && upd){ AddBox(x); d=GetBox(x); }

			//for(i=0; i<sz; i++){
			//	if(*(unsigned int*)&data[128*sz+i*4]==x){ d=&data[128*i]; break; }
			//}
			if(!d || l>3) return 0;
		}
		return 0;
	}

	unsigned char* GetBox(unsigned int x){
		for(unsigned int i=0; i<sz; i++){
				if(*(unsigned int*)&data[128*sz+i*4]==x){ return &data[128*i]; }
			}
		return 0;
	}

	unsigned int GetUsedSz(){
		for(unsigned int i=1; i<sz; i++){
				if(*(unsigned int*)&data[128*sz+i*4]==0){ return i; }
			}
		return 0;
	}

	unsigned int GetFreeSz(){
		for(unsigned int i=1; i<sz; i++){
				if(*(unsigned int*)&data[128*sz+i*4]==0){ return sz-i; }
			}
		return 0;
	}

	unsigned int AddBox(unsigned int x){
		unsigned int s=GetFreeSz(); if(!s) return 0;
		*(unsigned int*)&data[128*sz+(sz-s)*4]=x;
		return 1;
	}


//	Generate
	void Use(unsigned char *ln, unsigned int s){
		data=ln; sz=s/132;
	}

	int Create(int s){
		_data.Reserv(s*128+s*4);
		data=(unsigned char*)_data.data; sz=s;
		return data!=0;
	}

#ifdef GENERALCPP
	int Load(VString file){
		_data=LoadFile(file);
		data=(unsigned char*)_data.data; sz=_data.sz/132;
		return data!=0;
	}

	int Save(VString file){
		return SaveFile(file, _data);
	}

	int SaveHex(VString file){
		HLString ls; Itos it;
		for(int i=0; i<sz*132; i+=4){
			if(i%128==0 && i!=0) ls+"\r\n";
			ls+"0x"+it.itos(*(unsigned int*)(data+i), 16); if(sz+4<sz*132) ls+",";
		}		

		ls.oneline();
		return SaveFile(file, ls);
	}
#endif

};

asmcodesize asmcodesz;


class asmlexp{
public:

	int next(unsigned char *ln, unsigned char *to){
		if(!ln || ln>=to) return 0;

		//unsigned char *c=getasmcode(ln, l);
		unsigned int l;
		unsigned char c=asmcodesz.Get(ln, l);

		if(c) return c;
#ifdef GENERALCPP
		// err
		if(!c){ print("ASM! Code box not found: 0x", itos(*(ln), 16), "\r\n"); return 0; }

		if(l==1){ print("ASM! Code size not found: 0x", itos(*(ln), 16), "\r\n"); return 0; }
		if(l==2){ print("ASM! Code size not found: 0x", itos(*(ln), 16), " 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		if(l==3){ print("ASM! Code size not found: 0x", itos(*(ln), 16), " 0x", itos(*(ln+1), 16), " 0x", itos(*(ln+2), 16), "\r\n"); return 0; }
#endif
		return 0;
		//unsigned int a=0; a=*ln;
		//unsigned char l=*ln;
		//unsigned int c=asmcode[*ln];
		//unsigned int d=asmcode[*ln];

		//if(c && c!=99) return c;
		//if(!c){
		//	print("ASM! Code size not found: 0x", itos(*(ln), 16), "\r\n"); return 0;
		//}

		//unsigned char *b=0;
		//if(c==0x13) b=asmcode13; if(c==0x64) b=asmcode64; if(c==0x81) b=asmcode81;
		//if(c==0x83) b=asmcode83; if(c==0x89) b=asmcode89;
		//if(c==0x8B) b=asmcode8B; if(c==0x8D) b=asmcode64; if(c==0x8D) b=asmcode8D;

		//c=b[*(ln+1)]; if(c && c!=99) return c;
		//if(!c){ print("ASM! Code size not found: 0x", itos(*(ln), 16), " 0x", itos(*(ln+1), 16), "\r\n"); return 0; }


		//if(*ln==0x13){
		//	c=asmcode13[*(ln+1)]; if(c && c!=99) return c;
		//	if(!c){ print("ASM! Code size not found: 0x13 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		//}
		//
		//if(*ln==0x64){
		//	c=asmcode64[*(ln+1)]; if(c && c!=99) return c;
		//	if(!c){ print("ASM! Code size not found: 0x64 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		//}

		//if(*ln==0x81){
		//	c=asmcode81[*(ln+1)]; if(c && c!=99) return c;
		//	if(!c){ print("ASM! Code size not found: 0x81 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		//}
	
		//if(*ln==0x83){
		//	c=asmcode83[*(ln+1)]; if(c && c!=99) return c;
		//	if(!c){ print("ASM! Code size not found: 0x83 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		//}

		//if(*ln==0x89){
		//	c=asmcode89[*(ln+1)]; if(c && c!=99) return c;
		//	if(!c){ print("ASM! Code size not found: 0x89 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		//}

		//if(*ln==0x8B){
		//	c=asmcode8B[*(ln+1)]; if(c && c!=99) return c;
		//	if(!c){ print("ASM! Code size not found: 0x8B 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		//}

		//if(*ln==0x8D){
		//	c=asmcode8D[*(ln+1)]; if(c && c!=99) return c;
		//	if(!c){ print("ASM! Code size not found: 0x8D 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		//}

		return 0;
	}

#ifdef NOUSEIT
	int next(unsigned char *ln, unsigned char *to){
		if(ln>=to) return 0;

		int s=0;

		switch(*ln){
			case 0x03: s=6; break; // 03 05 70 D9 4D 00 add         eax,dword ptr [_msvps_new_size (4DD970h)] 


			case 0x33: s=2; break; // 33 C5            xor         eax,ebp 
			case 0x3B: s=2; break; // 3B F4            cmp         esi,esp


			case 0x50: s=1; break; // 50               push        eax
			case 0x51: s=1; break; // 51               push        ecx
			case 0x53: s=1; break; // 53               push        ebx  
			case 0x55: s=1; break; // 55               push        ebp
			case 0x56: s=1; break; // 56               push        esi  
			case 0x57: s=1; break; // 57               push        edi
			case 0x5D: s=1; break; // 5D               pop         ebp  

			case 0x64: s=5; break; // 64 A1 00 00 00 00 mov         eax,dword ptr fs:[00000000h] 
			case 0x68: s=5; break; // 68 6B 6F 4C 00   push        offset __ehhandler$?my_malloc@@YAPAXI@Z (4C6F6Bh) 
			case 0x6A: s=2; break; // 6A 01            push        1    

			case 0x74: s=2; break; // 74 2B            je          my_malloc+87h (493B37h) 


			case 0x83:
				switch(*(ln+1)){
					case 0xC4: s=3; break; // 83 C4 14         add         esp,14h
					case 0x7D: s=4; break; // 83 7D EC 00      cmp         dword ptr [ebp-14h],0
				default:
						print("ASM! Code size not found: 0x83 0x", itos(*(ln+1), 16), "\r\n");
						s=2;
				break;

				}
				break;
			case 0x81: s=6; break; // 81 EC DC 00 00 00 sub         esp,0DCh 
			case 0x89: s=3; break; // 89 45 FC         mov         dword ptr [res],eax
			case 0x8B: // mov
				switch(*(ln+1)){
					//case 0x45: s=
					case 0x45: s=3; break; // 8B 45 FC         mov         eax,dword ptr [res]
					case 0x4D: s=3; break; // 8B 4D 08         mov         ecx,dword ptr [nSize] 
					case 0xE5: s=2; break; // 8B E5            mov         esp,ebp 
					case 0xEC: s=2; break; // 8B EC            mov         ebp,esp 
					case 0xF4: s=2; break; // 8B F4            mov         esi,esp
					case 0xFF: s=2; break; // 8B FF            mov         edi,edi
				default:
					print("ASM! Code size not found: 0x8B 0x", itos(*(ln+1), 16), "\r\n");
					s=2;
				break;
				}
			break;
			case 0x8D:
				switch(*(ln+1)){
					case 0x8D: s=6; break; // 8D BD 18 FF FF FF lea         edi,[ebp-0E8h] 
					case 0x45: s=3; break; // 8D 45 F4         lea         eax,[ebp-0Ch] 
				default:
						print("ASM! Code size not found: 0x8D 0x", itos(*(ln+1), 16), "\r\n");
						s=2;
				break;
				}
			break;

			case 0xA1: s=5; break; // A1 E8 61 31 10   mov         eax,dword ptr [__newmode (103161E8h)]

			case 0xB8: s=5; break; // B8 CC CC CC CC   mov         eax,0CCCCCCCCh
			case 0xB9: s=5; break; // B9 37 00 00 00   mov         ecx,37h 


			case 0xC3: s=1; break; // C3               ret
			case 0xCC: s=1; break; // CC               int         3

			case 0xE8: s=5; break; // E8 55 48 FF FF   call        _nh_malloc_dbg (102CFAB0h)
			case 0xE9: s=5; break; // jmp rel16/32

			case 0xF3: c=2; break; // F3 AB            rep stos    dword ptr es:[edi] 
			case 0xFF: c=6; break; // FF 15 40 D9 4D 00 call        dword ptr [f_my_malloc (4DD940h)] 

			default:
				print("ASM! Code size not found: 0x", itos(*ln, 16), "\r\n");
				s=1;
			break;
		}

		ln+=s; return s;
		return 1;
	}
#endif
};


#ifdef GENERALCPP
void dissasm(VString file){ // Visual Studio .cod file "  00000	13 d1		adc         edx,ecx"
	MString fl=LoadFile(file);
	VString o, t=fl; int z=0, es=0;
	int code=0;

	unsigned char buf[11];

	while(t){
		o=PartLine(t, t, "\r\n");

		if(o.incompare("_TEXT	SEGMENT")) code=1; if(o.incompare("_TEXT	ENDS")) code=0;

		if(o[0]==' ' && o[1]==' ' && o[7]=='\t'){ if(!code) continue;
			z=1;

			for(int i=0; i<5; i++){
				if(o[i+2]>='0' && o[i+2]<='9' || o[i+2]>='a' && o[i+2]<='f') continue;
				z=0;
			} if(!z) continue;
			z=0;
			
			//unsigned int c=stoi(o.str(8, 2), 2, 16);
			unsigned char *l=o.data+8+z*3;		

			while(l<o.endu()){
				buf[z]= *l>='0' && *l<='9' ? *l-'0' : *l-'a'+10; buf[z]<<=4;
				if(!(*l>='0' && *l<='9' || *l>='a' && *l<='f')) break; l++;

				buf[z]+= *l>='0' && *l<='9' ? *l-'0' : *l-'a'+10;
				if(!(*l>='0' && *l<='9' || *l>='a' && *l<='f')) break; l++;

				if(*l!=' ' && *l!='\t' && l!=o.endu()) break; l++;	
				
				// multiline
				if(l-1==o.endu()){
					o=PartLine(t, t, "\r\n");
					if(o[0]!=9) break;
					l=o.data+1;
				}

				z++;

				if(*l=='\t') break;
			}

			while(l<o.endu()){
				if(*l!=' ' && *l!='\t') break;
				l++;
			}

			if(l>=o.endu() || *l=='D') continue;

			unsigned int lev;
			unsigned char c=asmcodesz.Get(buf, lev, (unsigned char*)&z);
			//if(c!=z) es++;

			//if(!c){ print("ASM! Code box not found: ", itos(*(buf), 16), "\r\n"); continue; }
			
			if(c!=z){
				print("Asm code table error: 0x", itos(*buf, 16), " size is ", itos(z), ", no ", itos(c), "\r\n");
				print(o, "\r\n");
				es++;
			}



			continue;

			//int lev;
			//unsigned char *c=getasmcode(buf, lev);
			//if(c && *c==z) continue;
			//
			//if(!c){ print("ASM! Code box not found: ", itos(*(buf), 16), "\r\n"); continue; }

			//print("Asm code table error: 0x", itos(*buf, 16), " size is ", itos(z), ", no ", itos(*c), "\r\n");
			//print(o, "\r\n"); es++;
			//if(!*c) *c=z;
			//continue;
			
	/*	

		if(l==1){ print("ASM! Code size not found: 0x", itos(*(ln), 16), "\r\n"); return 0; }
		if(l==2){ print("ASM! Code size not found: 0x", itos(*(ln), 16), " 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		if(l==3){ print("ASM! Code size not found: 0x", itos(*(ln), 16), " 0x", itos(*(ln+1), 16), " 0x", itos(*(ln+2), 16), "\r\n"); return 0; }
		*/

			//if(asmcode[c]!=99 && asmcode[c]!=z){
			//	if(!asmcode[c]) asmcode[c]=z;
			//	print("Asm code table error: 0x", itos(c, 16), " size is ", itos(z), ", no ", itos(asmcode[c]), "\r\n");
			//	print(o, "\r\n"); es++;
			//}

			//if(asmcode[c]==99){
			//	unsigned char *b=0;
			//	if(c==0x13) b=asmcode13; if(c==0x64) b=asmcode64; if(c==0x81) b=asmcode81;
			//	if(c==0x83) b=asmcode83; if(c==0x89) b=asmcode89;
			//	if(c==0x8B) b=asmcode8B; if(c==0x8D) b=asmcode64; if(c==0x8D) b=asmcode8D;

			//	unsigned int d=stoi(o.str(8+3, 2), 2, 16);

			//	if(b[d]!=99 && b[d]!=z){
			//		if(!b[d]) b[d]=z;

			//		print("Asm code table error: 0x", itos(c, 16), " 0x", itos(d, 16), " size is ", itos(z), ", no ", itos(b[d]), "\r\n");
			//		print(o, "\r\n"); es++;
			//	}

			//}

		}
	}

	print("Asm Errors: ", itos(es), "\r\n"); // 37192
	// VString 2270 16xx 950

	return ;
}

void dissasm_vs(VString file){ // Visual Studio memory dissasemble "71A9615A 8B FF            mov         edi,edi "
	MString fl=LoadFile(file);
	VString o, t=fl; int z=0, es=0;

	unsigned char buf[11];

	while(t){
		o=PartLine(t, t, "\r\n");

		if(o[8]==' '){
			z=1;

			for(int i=0; i<8; i++){
				if(o[i]>='0' && o[i]<='9' || o[i]>='A' && o[i]<='F') continue;
				z=0;
			} if(!z) continue;
			z=0;
			
			//unsigned int c=stoi(o.str(8, 2), 2, 16);
			unsigned char *l=o.data+9;		

			while(l<o.endu()){
				buf[z]= *l>='0' && *l<='9' ? *l-'0' : *l-'A'+10; buf[z]<<=4;
				if(!(*l>='0' && *l<='9' || *l>='A' && *l<='F')) break; l++;

				buf[z]+= *l>='0' && *l<='9' ? *l-'0' : *l-'A'+10;
				if(!(*l>='0' && *l<='9' || *l>='A' && *l<='F')) break; l++;

				if(*l!=' ' && *l!='\t' && l!=o.endu()) break; l++;	
				
				z++;
				if(*l=='\t') break;
			}

			while(l<o.endu()){
				if(*l!=' ' && *l!='\t') break;
				l++;
			}

			if(l>=o.endu() || *l=='D') continue;

			unsigned int lev;
			unsigned char c=asmcodesz.Get(buf, lev, (unsigned char*)&z);
			//if(c!=z) es++;

			//if(!c){ print("ASM! Code box not found: ", itos(*(buf), 16), "\r\n"); continue; }
			
			if(c!=z){
				print("Asm code table error: 0x", itos(*buf, 16), " size is ", itos(z), ", no ", itos(c), "\r\n");
				print(o, "\r\n");
				es++;
			}



			continue;

			//int lev;
			//unsigned char *c=getasmcode(buf, lev);
			//if(c && *c==z) continue;
			//
			//if(!c){ print("ASM! Code box not found: ", itos(*(buf), 16), "\r\n"); continue; }

			//print("Asm code table error: 0x", itos(*buf, 16), " size is ", itos(z), ", no ", itos(*c), "\r\n");
			//print(o, "\r\n"); es++;
			//if(!*c) *c=z;
			//continue;
			
	/*	

		if(l==1){ print("ASM! Code size not found: 0x", itos(*(ln), 16), "\r\n"); return 0; }
		if(l==2){ print("ASM! Code size not found: 0x", itos(*(ln), 16), " 0x", itos(*(ln+1), 16), "\r\n"); return 0; }
		if(l==3){ print("ASM! Code size not found: 0x", itos(*(ln), 16), " 0x", itos(*(ln+1), 16), " 0x", itos(*(ln+2), 16), "\r\n"); return 0; }
		*/

			//if(asmcode[c]!=99 && asmcode[c]!=z){
			//	if(!asmcode[c]) asmcode[c]=z;
			//	print("Asm code table error: 0x", itos(c, 16), " size is ", itos(z), ", no ", itos(asmcode[c]), "\r\n");
			//	print(o, "\r\n"); es++;
			//}

			//if(asmcode[c]==99){
			//	unsigned char *b=0;
			//	if(c==0x13) b=asmcode13; if(c==0x64) b=asmcode64; if(c==0x81) b=asmcode81;
			//	if(c==0x83) b=asmcode83; if(c==0x89) b=asmcode89;
			//	if(c==0x8B) b=asmcode8B; if(c==0x8D) b=asmcode64; if(c==0x8D) b=asmcode8D;

			//	unsigned int d=stoi(o.str(8+3, 2), 2, 16);

			//	if(b[d]!=99 && b[d]!=z){
			//		if(!b[d]) b[d]=z;

			//		print("Asm code table error: 0x", itos(c, 16), " 0x", itos(d, 16), " size is ", itos(z), ", no ", itos(b[d]), "\r\n");
			//		print(o, "\r\n"); es++;
			//	}

			//}

		}
	}

	print("Asm Errors: ", itos(es), "\r\n"); // 37192
	// VString 2270 16xx 950

	return ;
}

#endif





/* 
http://read.pudn.com/downloads168/sourcecode/windows/773086/LDasm.h__.htm
  ___________________________________________________ 
  Opcode Length Disassembler. 
  Coded By Ms-Rem ( Ms-Rem@yandex.ru ) ICQ 286370715 
  --------------------------------------------------- 
  12.08.2005 - fixed many bugs... 
  09.08.2005 - fixed bug with 0F BA opcode. 
  07.08.2005 - added SSE, SSE2, SSE3 and 3Dnow instruction support. 
  06.08.2005 - fixed bug with F6 and F7 opcodes. 
  29.07.2005 - fixed bug with OP_WORD opcodes. 

  03.10.2011 - modify mikelsv. not good function size, ignore jmp.
*/ 
 
//#ifndef _LDASM_ 
//#define _LDASM_ 
// 
//#ifdef __cplusplus 
//extern "C" { 
//#endif 
// 
//unsigned long __fastcall SizeOfCode(void *Code, unsigned char **pOpcode); 
// 
//unsigned long __fastcall SizeOfProc(void *Proc); 
// 
//char __fastcall IsRelativeCmd(unsigned char *pOpcode); 
// 
//#ifdef __cplusplus 
//} 
//#endif 

//#define OP_NONE				0x00 
#define OP_MODRM			0x01 
#define OP_DATA_I8			0x02 
#define OP_DATA_I16			0x04 
#define OP_DATA_I32			0x08 
#define OP_DATA_PRE66_67	0x10 
#define OP_WORD				0x20 
#define OP_REL32			0x40 

#define OP_OF_PREFIX		0x80 
#define OP_SEG_PREFIX		0x100 

#define X86DIS_0F_PREFIX	0x80 
#define X86DIS_SEG_PREFIX	0x100 

// -
#define X86DIS_O16_PREFIX	0x200
#define X86DIS_A16_PREFIX	0x400
#define X86DIS_LOCK_PREFIX	0x2000
#define X86DIS_REP_PREFIX	0x4000

#define X86DIS_JMP_REL1		0x800
#define X86DIS_JMP_ADDR6	0x1000

#define OP_INVALID			0x10000

 
struct X86_INST{
	unsigned char *line;
	unsigned char *op;
	unsigned char op_size, op_flag;
	unsigned char offset_size; // bytes
	unsigned long offset_pos; // real position
			 long offset_val; // value

	//int rOffs_offset;
	//int rOffs_size;
	//int rOffs_rel;
	//int rOffs_seg;
	//int op_length;
	//int is_rjc:1;
	//int is_rcall:1;
	//int is_rjmp:1;
	unsigned int /*is_rj:1,*/ is_rcall:1, is_rjmp:1;

	void upd(){
		if(!op) return ;
		is_rjmp=( *op==0x75 || ((*op==0x84 || *op==0x85) && op_flag>1) || *op==0xe9 || *op==0xea || *op==0xeb
			/*|| *op==0xff && ((*(op+1)& 0x38) >> 3)==4*/ );
		is_rcall=(*op==0xe8); // (*cPtr & 0x38) >> 3;
		
		if(is_rjmp || is_rcall){
			unsigned char *ln= op_flag&OP_MODRM ? op+2 : op+1;

			offset_val= offset_size==1 ? *(char*)(ln) : offset_size==2 ? *(short*)(ln) : *(int*)(ln);
			offset_pos=(unsigned long)line+offset_val+op_size;
		}

		//is_rj=(*op>=0x70 && *op<0x80);
	}

	int isjmp(){
		if(!op) return 0;
		if(*op==0xe9 || *op==0xea || *op==0xeb) return 1;
		return 0;
	}

	int isj(){
		if(!op) return 0;
		if(*op>=0x70 && *op<0x80) return 1;
		return 0;
	}

	int iscall(){
		if(!op) return 0;
		if(*op==0xe8) return 1;
		return 0;	
	}

};
 
#define UCHAR unsigned char 
#define ULONG unsigned long 
#define PVOID void* 
#define PUCHAR unsigned char* 
#define BOOLEAN char 

// 1 = OP_DATA_I8
// 2 = OP_DATA_I16
// 4 = OP_DATA_PRE66_67

// X86DIS_JMP_REL1 && OP_DATA_I8

  
unsigned int X86_FLAGS[256]={
	// 00
	/*ADD*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67,
	/*PUSH/POP ES*/ 0, 0,

	// 08
	/*OR*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67, 
	/*PUSH CS*/ 0, 
	X86DIS_0F_PREFIX,

    // 10
    /*ADC*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67,
	/*PUSH/POP SS*/ 0, 0,

	// 18
	/*SBB*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67,
	/*PUSH/POP DS*/ 0, 0,

	// 20
	/*AND*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67,
	X86DIS_SEG_PREFIX /*ES*/,
    /*DAA*/ 0,

	// 28
	/*SUB*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67,
	X86DIS_SEG_PREFIX /*CS*/,
    /*DAS*/ 0,

	// 30
	/*XOR*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67,
	X86DIS_SEG_PREFIX /*SS*/,
    /*AAA*/ 0,

	// 38
	/*CMP*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_DATA_I8, OP_DATA_PRE66_67,
	X86DIS_SEG_PREFIX /*DS*/,
    /*AAS*/ 0,

	// 40
    /*INC*/  0,0,0,0,0,0,0,0,
    /*DEC*/  0,0,0,0,0,0,0,0,
    /*PUSH*/ 0,0,0,0,0,0,0,0,
    /*POP*/  0,0,0,0,0,0,0,0,

	// 60
	/*PUSHA/POPA*/ 0, 0,
    /*BOUND*/ OP_MODRM, // |8 ??
	/*ARPL*/ OP_MODRM,
	X86DIS_SEG_PREFIX /*FS*/,
    X86DIS_SEG_PREFIX /*GS*/,
    X86DIS_O16_PREFIX, /*x16 oprerand*/
    X86DIS_A16_PREFIX, /*x16 address*/

	// 68
    /*PUSH*/ OP_DATA_PRE66_67,
    /*IMUL*/ OP_MODRM | OP_DATA_PRE66_67,
    /*PUSH*/ OP_DATA_I8,
    /*IMUL*/ OP_MODRM | OP_DATA_I8,
    /*INSB*/ 0,
    /*INS*/  0,
    /*OUTSB*/0,
    /*OUTS*/ 0,

	// 70
    /*Jx*/ OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1,
	/*Jx*/ OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1,
    /*Jx*/ OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1,
	/*Jx*/ OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1,

	// 80
    /*ADD,OR,ADC,SBB,AND,SUB,XOR,CMP*/ OP_MODRM | OP_DATA_I8,
    /*ADD,OR,ADC,SBB,AND,SUB,XOR,CMP*/ OP_MODRM | OP_DATA_PRE66_67,
    /*ADD,OR,ADC,SBB,AND,SUB,XOR,CMP*/ OP_MODRM | OP_DATA_I8,
    /*ADD,OR,ADC,SBB,AND,SUB,XOR,CMP*/ OP_MODRM | OP_DATA_I8,
    /*TEST*/ OP_MODRM, OP_MODRM,
    /*XCHG*/ OP_MODRM, OP_MODRM,

	// 88
    /*MOV*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
    /*LEA*/ OP_MODRM,
    /*MOV*/ OP_MODRM,
    /*POP*/ OP_MODRM,

	// 90
    0,
    /*XCHG*/ 0,0,0,0,0,0,0,

	// 98
    /*CBW,CWD*/ 0,0,
    /*CALL*/ OP_DATA_I16 | OP_DATA_PRE66_67,
    /*WAIT,PUSHF,POPF,SAHF,LAHF*/ 0,0,0,0,0,

	// A0
    /*MOV*/   OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67,
    /*MOVS(B/D)*/ 0,0,
    /*CMPS(B/D)*/ 0,0,

	// A8
    /*TEST*/  OP_DATA_I8, OP_DATA_PRE66_67,
    /*STOSB*/ 0, 0,
    /*LODSB*/ 0, 0,
    /*SCASB*/ 0, 0,

	// B0
    /*MOV*/ OP_DATA_I8, OP_DATA_I8, OP_DATA_I8, OP_DATA_I8, OP_DATA_I8, OP_DATA_I8, OP_DATA_I8, OP_DATA_I8,
    // B8
    /*MOV*/ OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67, OP_DATA_PRE66_67,

	// C0
    /*ROL,ROR,RCL,SHL,SHR,SAR*/ OP_DATA_I8|OP_MODRM, OP_DATA_I8|OP_MODRM,
    /*RET*/ OP_DATA_I16, 0,
    /*LDS,LES*/ OP_MODRM, OP_MODRM,
    /*MOV*/ OP_DATA_I8|OP_MODRM, OP_DATA_PRE66_67|OP_MODRM,

	// C8
    /*ENTER/LEAVE*/ OP_DATA_I8 | OP_DATA_I16, 0,
    /*RETFAR*/ OP_DATA_I16, 0,
    /*INT3/INT/INT0/IRET*/ 0,OP_DATA_I8,0,0,

	// D0
    /*ROL,ROR,RCL,SHL,SHR,SAR*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
    /*AAM,AAD*/ OP_DATA_I8, OP_DATA_I8,
    OP_INVALID,
    /*XLAT*/ 0,
 
    // D8
     OP_WORD, OP_WORD, OP_WORD, OP_WORD, OP_WORD, OP_WORD, OP_WORD, OP_WORD, /*FLOAT*/

	// E0
	/*LOOP/N/E*/ OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1, OP_DATA_I8|X86DIS_JMP_REL1,
    /*JCXZ*/ OP_DATA_I8 | X86DIS_JMP_REL1,
    /*IN,OUT*/ OP_DATA_I8, OP_DATA_I8, OP_DATA_I8, OP_DATA_I8, // 1, 4, 1, 4 = OP_DATA_PRE66_67

//    OP_DATA_I8,                    // E4 
//    OP_DATA_I8,                    // E5 
//    OP_DATA_I8,                    // E6 
//    OP_DATA_I8,                    // E7

	// E8
    /*CALL*/ OP_DATA_PRE66_67 | OP_REL32,
    /*JMP*/  OP_DATA_PRE66_67 | OP_REL32, OP_DATA_I16 | OP_DATA_PRE66_67 | X86DIS_JMP_ADDR6, OP_DATA_I8|X86DIS_JMP_REL1,
    /*IN,OUT*/ 0,0,0,0,
 
    // F0
    X86DIS_LOCK_PREFIX,
    OP_INVALID,
    X86DIS_REP_PREFIX, X86DIS_REP_PREFIX,
    0,0,
    OP_MODRM, OP_MODRM, // |OP_DATA_I8, | OP_DATA_PRE66_67
 
    // F8
    0,0,0,0,0,0,
    OP_MODRM, OP_MODRM | OP_REL32,
}; 
 
 
UCHAR X86_FLAGS_EXT[256]={ 
	// 0F 00
	/*SLDT,STR,LLDT,LTR,VERR,VERW*/ OP_MODRM,
    /*SGDT, SIDT, LGDT, LIDT, SMSW,LMSW*/ OP_MODRM,
    /*LAR,LSL*/ OP_MODRM, OP_MODRM,
    OP_INVALID, OP_INVALID,
    /*CLTS*/ 0,
    OP_INVALID,

	// 0F 08
    OP_INVALID, 0, OP_INVALID, 0, // OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID,
	OP_INVALID, OP_MODRM, OP_INVALID,  OP_MODRM | OP_DATA_I8,
 
    // 0F 10
    OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F 18
    OP_MODRM, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID,
 
    // 0F 20
    /*MOV*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
    OP_MODRM, OP_INVALID, OP_MODRM, OP_INVALID,

    // 0F 28
    OP_MODRM, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID,
 
    // 0F 30
    /*WRMSR,RDTSC,RDMSR,RDPMC*/ 0, 0, 0, 0,
    OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID,
 
    // 0F 38
    OP_MODRM, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID,

	// 0F 40
    /*CMOVxx*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F 48
    /*CMOVxx*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F 50
    OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F 58
    OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,

	// 0F 60
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F 68
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
    OP_MODRM, OP_MODRM,
    /*MOVD/Q*/ OP_MODRM, OP_MODRM,
 
    // 0F 70
    OP_MODRM | OP_DATA_I8, OP_MODRM | OP_DATA_I8, OP_MODRM | OP_DATA_I8, OP_MODRM | OP_DATA_I8, OP_MODRM, OP_MODRM, OP_MODRM,
    /*_EMMS*/ 0,
 
    // 0F 78
    OP_INVALID, OP_INVALID, OP_INVALID, OP_INVALID, OP_MODRM, OP_MODRM,
    /*MOVD/Q*/ OP_MODRM, OP_MODRM,
 
    // 0F 80
    /*Jxx*/ OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32,
	/*Jxx*/ OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32, OP_DATA_PRE66_67 | OP_REL32,
 
    // 0F 90
    /*SETxx*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F 98
    /*SETxx*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 	
	// 0F A0
    /*PUSH/POP/CPUID*/ 0, 0, 0,
    /*BT*/ OP_MODRM,
    /*SHLD*/ OP_MODRM | OP_DATA_I8,
    /*SHLD*/ OP_MODRM,
    OP_INVALID, OP_INVALID,
 
    // 0F A8
    /*PUSH/POP/RSM*/ 0, 0, 0,
    /*BTS*/ OP_MODRM,
    /*SHRD*/ OP_MODRM | OP_DATA_I8,
    /*SHRD*/ OP_MODRM,
    OP_MODRM,
    /*IMUL*/ OP_MODRM,

    // 0F B0
    /*CMPXCHG*/ OP_MODRM, OP_MODRM,
    /*LSS/BTR/LFS/LGS/MOVZX/MOVZX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F B8
    /*Bxx*/ OP_INVALID, OP_INVALID, OP_MODRM | OP_DATA_I8, OP_MODRM, OP_MODRM, OP_MODRM,
    /*MOVSX*/ OP_MODRM, OP_MODRM,
 
    // 0F C0
    /*XADD*/ OP_MODRM, OP_MODRM,
    OP_MODRM | OP_DATA_I8, OP_MODRM, OP_MODRM | OP_DATA_I8, OP_MODRM | OP_DATA_I8, OP_MODRM | OP_DATA_I8, OP_MODRM,
 
    // 0F C8
    /*BSWAP*/ 0,0,0,0,0,0,0,0,
 
    // 0F D0
    OP_MODRM,
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM,
    -1,
    /*PMULLW*/ OP_MODRM,
    OP_MODRM, OP_MODRM,
 
    // 0F D8
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F E0
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F E8
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
	// 0F F0
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
 
    // 0F F8
    /*xMMX*/ OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM, OP_MODRM,
}; 

 
unsigned long SizeOfCode2(unsigned char *Code, unsigned char **pOpcode){ 
	PUCHAR cPtr; 
	UCHAR Flags; 
	BOOLEAN PFX66, PFX67; 
	BOOLEAN SibPresent; 
	UCHAR iMod, iRM, iReg; 
	UCHAR OffsetSize, Add; 
	UCHAR Opcode; 
 
	OffsetSize = 0; 
	PFX66 = FALSE; 
	PFX67 = FALSE; 
	cPtr = (PUCHAR)Code; 
 
	while( (*cPtr == 0x2E) || (*cPtr == 0x3E) || (*cPtr == 0x36) || 
		    (*cPtr == 0x26) || (*cPtr == 0x64) || (*cPtr == 0x65) ||  
			(*cPtr == 0xF0) || (*cPtr == 0xF2) || (*cPtr == 0xF3) || 
			(*cPtr == 0x66) || (*cPtr == 0x67) ){ 
		if (*cPtr == 0x66) PFX66 = TRUE; 
		if (*cPtr == 0x67) PFX67 = TRUE; 
		cPtr++; 
		if (cPtr > (PUCHAR)Code + 16) return 0;  
	} 
	Opcode = *cPtr; 
	if(pOpcode) *pOpcode = cPtr;  
 
	if(*cPtr == 0x0F){ 
		cPtr++; 
		Flags = X86_FLAGS_EXT[*cPtr]; 
	}else { 
		Flags = X86_FLAGS[Opcode]; 
 
		if(Opcode >= 0xA0 && Opcode <= 0xA3) PFX66 = PFX67; 
	} 
	cPtr++; 
	if(Flags & OP_WORD) cPtr++;	 
 
	if(Flags & OP_MODRM){ 
		iMod = *cPtr >> 6; 
		iReg = (*cPtr & 0x38) >> 3;   
		iRM  = *cPtr &  7; 
		cPtr++; 
 
		if((Opcode == 0xF6) && !iReg) Flags |= OP_DATA_I8;     
		if((Opcode == 0xF7) && !iReg) Flags |= OP_DATA_PRE66_67;  
  
		SibPresent = !PFX67 & (iRM == 4); 
		switch(iMod){ 
			case 0:  
			  if( PFX67 && (iRM == 6)) OffsetSize = 2; 
			  if(!PFX67 && (iRM == 5)) OffsetSize = 4;  
			 break; 
			case 1: OffsetSize = 1; 
			 break;  
			case 2: if (PFX67) OffsetSize = 2; else OffsetSize = 4; 
			 break; 
			case 3: SibPresent = FALSE; 
		} 
		if(SibPresent){ 
			if (((*cPtr & 7) == 5) && ( (!iMod) || (iMod == 2) )) OffsetSize = 4; 
			cPtr++; 
		} 
		cPtr = (PUCHAR)(ULONG)cPtr + OffsetSize; 
	} 
 
	if (Flags & OP_DATA_I8)  cPtr++; 
	if (Flags & OP_DATA_I16) cPtr += 2; 
	if (Flags & OP_DATA_I32) cPtr += 4; 
	if (PFX66) Add = 2; else Add = 4; 
	if (Flags & OP_DATA_PRE66_67) cPtr += Add; 
	return (ULONG)cPtr - (ULONG)Code; 
}

unsigned int SizeOfCode(unsigned char *line, X86_INST &ret){
	unsigned char *ln=line; ret.line=line;
	unsigned char offset=0, flags;
	unsigned char opcode;

	/*unsigned char *Code;*/ 
	//unsigned char **pOpcode;



	//PUCHAR cPtr; 
	//UCHAR Flags; 
	BOOLEAN PFX66, PFX67; 
	BOOLEAN SibPresent; 
	UCHAR iMod, iRM, iReg; 
//	UCHAR /*OffsetSize,*/ Add; 
	//UCHAR Opcode; 
 
	offset = 0; 
	PFX66 = FALSE; 
	PFX67 = FALSE; 
//	ln = (PUCHAR)Code; 
 
	while((*ln == 0x26) || (*ln == 0x2E) || (*ln == 0x36) || (*ln == 0x3E) || (*ln == 0x64) || (*ln == 0x65) ||  
			(*ln == 0x66) || (*ln == 0x67) || (*ln == 0xF0) || (*ln == 0xF2) || (*ln == 0xF3)){ 
		if(*ln == 0x66) PFX66 = 1; 
		if(*ln == 0x67) PFX67 = 1; 
		ln++; 
		if (ln > line + 16) return 0;
	}

	opcode = *ln; 
 
	if(*ln == 0x0F){
		ln++;
		flags = X86_FLAGS_EXT[*ln];
	}else {
		flags = X86_FLAGS[opcode];
 
		if(opcode >= 0xA0 && opcode <= 0xA3) PFX66 = PFX67;
	} ret.op = ln; ret.op_flag=flags;

	ln++;
	if(flags & OP_WORD) ln++;
 
	if(flags & OP_MODRM){
		iMod = *ln >> 6;
		iReg = (*ln & 0x38) >> 3;
		iRM  = *ln &  7;
		ln++;
 
		if((opcode == 0xF6) && !iReg) flags |= OP_DATA_I8;
		if((opcode == 0xF7) && !iReg) flags |= OP_DATA_PRE66_67;
  
		SibPresent = !PFX67 & (iRM == 4);
		switch(iMod){
			case 0:
			  if( PFX67 && (iRM == 6)) offset = 2;
			  if(!PFX67 && (iRM == 5)) offset = 4;
			 break;
			case 1: offset = 1;
			 break;
			case 2: if (PFX67) offset = 2; else offset = 4;
			 break;
			case 3: SibPresent = FALSE;
		}
		if(SibPresent){
			if (((*ln & 7) == 5) && ( (!iMod) || (iMod == 2) )) offset = 4;
			ln++;
		}
		ln+=offset;
	}

	ret.offset_size=offset;
 
	if(flags & OP_DATA_I8){ ln++; ret.offset_size=1; }
	if(flags & OP_DATA_I16) ln += 2;
	if(flags & OP_DATA_I32) ln += 4;
	//if(PFX66) Add = 2; else Add = 4; 
	if(flags & OP_DATA_PRE66_67){ ret.offset_size= PFX66 ? 2 : 4; ln += PFX66 ? 2 : 4; }

	ret.op_size=(unsigned long)ln - (unsigned long)line; ret.upd(); 

	unsigned char **pOpcode=0;
	int s=SizeOfCode2(line, pOpcode);
	if(s!=ret.op_size)
		int retryr=56;

	return ret.op_size;
}

// not good:
//unsigned long __fastcall SizeOfProc(void *Proc){ 
//	ULONG  Length; 
//	PUCHAR pOpcode; 
//	ULONG  Result = 0;
// 
//	do{ 
//		Length = SizeOfCode(Proc, &pOpcode); 
//		Result += Length; 
//		if((Length == 1) && (*pOpcode == 0xC3))
//			break; 
//		if((Length == 3) && (*pOpcode == 0xC2))
//			break; 
//		Proc = (PVOID)((ULONG)Proc + Length); 
//	}while (Length); 
//	return Result; 
//} 
// 
//char __fastcall IsRelativeCmd(unsigned char *pOpcode){ 
//	UCHAR Flags; 
//	if (*pOpcode == 0x0F) Flags = OpcodeFlagsExt[*(PUCHAR)((ULONG)pOpcode + 1)];  
//	    else Flags = OpcodeFlags[*pOpcode]; 
//	return (Flags & OP_REL32); 
//} 

// thanks. next my.


 
//#endif