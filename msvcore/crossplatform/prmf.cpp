// PRMF : prev reserv memory function / post reserv memory function = prev / post reserv memory function
//#define prmf(func, ret, ...){ int s=func(0, 0, ##__VA_ARGS__); ret.Reserv(s); s=func(ret, ret, ##__VA_ARGS__); }
//#define log(priority, format, ...) \
//    logger (priority, format "\n", ##__VA_ARGS__);
////int s=func(0, 0, ...);
//// ret.Reserv(s); s=func(ret, ret, ...); }
//#define API_CALL_V( _funcName, ... ) \
//    API_RESOLVE( _funcName ) \
//        ((FunctionT)f)( __VA_ARGS__ );
//#define eprintf(format, args)  \
//      fprintf (stderr, format , ## args)


#define prmf_min(a,b) (((a) < (b)) ? (a) : (b))

// itos
unsigned int prmf_itos(unsigned char *ret, unsigned int rsz, int64 val, int radix, int null){
	unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; // define

	int64 v=val;
	int m=0, s=0;// tmp;
	if(val<0){ prmdadd_t("-"); val *= -1; m=1; }
	if(null < 1) null = 1;

	while(v){
		s++;
		v /= radix;
	}

	if(s<null) s=null; ret+=s;
	if(ret>rt) return (unsigned int)(ret-aret);

	ret--;
	while(val){
		int tmp = val % radix; if(tmp < 0) tmp *= -1;
        *ret--=cb16[tmp];
		val /= radix;
	}

	if(m) aret++;
	while(ret>=aret){
		*ret='0';
		ret--;
	}

	//if(null>ret-aret-m){
	//	tmp=null-(ret-aret-m);
 //       if(ret+m+tmp<=rt) memcpy(ret+m, ret+tmp+m, tmp);
	//	if(aret+tmp+m<=rt) memset(ret+m, '0', tmp);
	//	ret+=tmp;
	//}
	return s+m;
}

unsigned int prmf_dtos(unsigned char *ret, unsigned int rsz, double val, int ml){
	unsigned char *aret = ret, *rt = ret ? ret + rsz : 0;

	// Nan
	if(val != val){
		if(ret < rt)
			*ret = '0';
		ret ++;
		return 1;
	}

	if(val < 0){
		if(ret < rt)
			*ret = '-';
		ret ++;		
		val *= -1;
	}

	int64 val64 = val, ival;
	int64 pw = pow((double)10, ml);
	val -= (int64) val;
	val *= pw;
	val = floor(val);
	if(val >= pw){
		ival = val;
		ival -= pw;
		val64 ++;
	} else
		ival = val;

	while(ml && ival % 10 == 0){
		ival /= 10;
		ml --;
	}

	int s = prmf_itos(ret, rsz, val64);
	ret += s;

	if(ival){
		if(ret < rt)
			*ret = '.';
		ret ++;

		int sd = prmf_itos(ret, rt ? rt - ret : 0, ival, 10, ml);
		ret += sd;
	}

	return ret-aret;
}


// uptime
unsigned int prmf_uptime(unsigned char *ret, unsigned int rsz, unsigned int tm){
	unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; // define


	if(tm>=86400){ //days
		ret+=prmf_itos(ret, ret<rt ? (unsigned int)(rt-ret) : 0, tm/86400);
		if(tm/86400==1){ prmdadd_t(" day "); }else{ prmdadd_t(" days "); }
		tm%=86400;
	}

	if(tm>=3600 || ret!=aret){ //hours
		ret+=prmf_itos(ret, ret<rt ? (unsigned int)(rt-ret) : 0, tm/3600);
		prmdadd_t(" hours ");
		tm%=3600;
	}

	if(tm>=60 || ret!=aret){ //minutes
		ret+=prmf_itos(ret, ret<rt ? (unsigned int)(rt-ret) : 0, tm/60);
		prmdadd_t(" minutes ");
		tm%=60;
	}
	
	ret+=prmf_itos(ret, ret<rt ? (unsigned int)(rt-ret) : 0, tm);
	prmdadd_t("s");
	return ret-aret;
}


// hextostr
unsigned int prmf_htob(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz){
	unsigned char *ln=line, *to=ln+sz; unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; // define

	unsigned int s=sz/2 + sz%2;
	if(rsz<s || !ret) return s;
	if(sz%2) to--;

	while(ln<to){
		*ret=16*((*ln>47 && *ln<58) ? *ln-48 : *ln-87); ln++;
		*ret+=((*ln>47 && *ln<58) ? *ln-48 : *ln-87); ln++;
		ret++;
	}

	if(sz%2) prmdadd_c(*ln);
	return (unsigned int)(ret-aret);
}

// strtohex
unsigned int prmf_btoh(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz){
	unsigned char *ln=line, *to=ln+sz; unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; // define

    unsigned int s=sz*2;
	if(rsz<s || !ret) return s;
	
	while(ln<to){
		*ret++=((unsigned char)*ln/16 + ((unsigned char)*ln/16<10 ? 48 : 87));
		*ret++=((unsigned char)*ln%16 + ((unsigned char)*ln%16<10 ? 48 : 87));
		ln++;
	}

	return ret-aret;
}

unsigned int prmf_replace(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz, unsigned char *fr, unsigned int frs,
	unsigned char *toc, unsigned int tos, unsigned int cnt){
	unsigned char *ln=line, *to=ln+sz; unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; // define

	if(!cnt) cnt=S2M;
	if(!ret && (!fr || !frs || frs==tos)) return sz;
	if(ret && (!fr || !frs)){ memcpy(ret, line, prmf_min(rsz, sz)); return prmf_min(rsz, sz); }
	if(!line || !sz) return 0;

	unsigned char *ls=ln;
	//to-=frs-1;
	for(ln; ln<to; ln++){
		if(*ln==*fr){
			unsigned char *dln=ln+1, *dto=ln+frs, *dfr=fr+1;
			for(dln; dln<dto; dln++){
				if(*dln!=*dfr) break; dfr++;
			}
			if(dln==dto){
				if(ret+(ln-ls)<=rt){ memcpy(ret, ls, ln-ls); } ret+=ln-ls; ln+=frs-1; ls=ln+1;
				if(ret+tos<=rt){ memcpy(ret, toc, tos); } ret+=tos;
				if(!--cnt) break;
			}
		}
	}
	to+=frs-1;

	if(ret+(ln-ls)<=rt){ memcpy(ret, ls, ln-ls); } ret+=ln-ls;

	return ret-aret;
}


// short to punycode
unsigned int prmf_stopunyc(unsigned char*ret, unsigned int rsz, unsigned short*line, unsigned int sz){
	unsigned short *ln=line, *to=ln+sz; int r=0;
	while(ln<=to){
		if(*ln=='.' || ln==to){
			if(r){ r++; if(ret) *ret='.'; }
			unsigned int s=prmf_stopunyc_one(ret, rsz, line, ln-line); if(!s) return 0;
			r+=s; if(ret){ ret+=s; if(rsz<=s) rsz=0; else rsz-=s; }
			line=ln+1;
		}
		ln++;
	}

    return r;
}

// punycode to short
unsigned int prmf_punyctos(unsigned short*ret, unsigned int rsz, unsigned char*line, unsigned int sz){
	unsigned char *ln=line, *to=ln+sz; int r=0;
	while(ln<=to){
		if(*ln=='.' || ln==to){
			unsigned int s=prmf_punyctos_one(ret, rsz, line, ln-line);
			if(!s) return 0;
			r+=s; if(ret){ ret+=s; if(rsz<=s) rsz=0; else rsz-=s; }
			line=ln+1;
		}
		ln++;
	}

    return r;
}



unsigned char stopunyc_c(unsigned char c){ return c<26 ? c+'a' : c+'0'-26; }
unsigned char punyctos_c(unsigned char c){ return c>='a' && c<='z' ? c-'a' : c-'0'+26; }

static DWORD stopunyc_adapt(DWORD delta, DWORD numpoints, int firsttime){
	DWORD k;
	delta = firsttime ? delta / 700 : delta >> 1;
	delta += delta / numpoints;
	for(k = 0; delta > ((36 - 1) * 26) / 2; k += 36)
		delta /= 36 - 1;
	return k + (36 - 1 + 1) * delta / (delta + 38);
}

unsigned int prmf_stopunyc_one(unsigned char*ret, unsigned int rsz, unsigned short*line, unsigned int sz){
	// ln, to - input, aret - begin, ret - insert cursor, rt - ret to.
	unsigned short *ln=line, *to=ln+sz, *l; unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; int u=0;
	while(ln<to){
		if(*ln<128){
			if(ret<rt) *ret=(unsigned char)*ln; ret++;
		} else u=1;
		ln++;
	}

	if(!u){ return ret-aret; }

	if(u){
		if(ret+3<rt && aret){
			memcpy(aret+4, aret, ret-aret); aret[0]='x'; aret[1]='n'; aret[2]='-'; aret[3]='-';
		}
		ret+=4;
	}

	if(ret-aret>4){ if(ret<rt){ *ret='-'; } ret++; }

	ln=line;
	unsigned int n=128, m=S4GM, delta=0, base=36, bias=72, q, k, tmin=1, tmax=26, b=ln-line;
	while(ln<to){

		for(l=line, m=S4GM; l<to; l++){
			if(*l>=n && *l<m) m=*l;
		}

		if(m - n > (-1 - delta) / (ln-line + 1)) return 0;
		delta += (m - n) * (ln-line + 1); n = m;

		for(l=line; l<to; l++){
			if(*l<n) if(++delta==0) return 0;

			if(*l==n){
				//Represent delta as a generalized variable-length integer:
				for(q=delta, k=base;; k+=base){
					//if(out>=max_out)
					//	return (punycode_big_output);


					unsigned int t = k <= bias ? tmin : k >= bias + tmax ? tmax : k - bias;
					//проверка на переполнение
					if(q<t) break;

					if(ret<rt) *ret=stopunyc_c(t+(q-t)%(base-t)); ret++;
					q=(q-t)/(base-t);
				}
				if(ret<rt) *ret=stopunyc_c(q); ret++;
        //output[out++] = encode_digit (q, case_flags && case_flags[j]);
				bias=stopunyc_adapt(delta, ln-line+1, ln-line==b);
				delta = 0;
				//++h;
			}
		}
		++delta;
		++n;
		ln++;
	}

	return ret-aret;
}


unsigned int prmf_punyctos_one(unsigned short*ret, unsigned int rsz, unsigned char*line, unsigned int sz){
	// ln, to - input, aret - begin, ret - insert cursor, rt - ret to.
		// ln, to - input, aret - begin, ret - insert cursor, rt - ret to.
	unsigned char *ln=line, *to=ln+sz; unsigned short *aret=ret, *rt=ret ? ret+rsz : 0; int u=0;
	if(sz<5 || !cmp((char*)ln, (char*)"xn--", 4)){
		for(ln; ln<to; ln++){
			if(ret<rt){ *ret=*ln; } ret++;
		} return ret-aret;	
	}

	line+=4; ln+=4; sz-=4; to--;

	while(ln<=to){
		if(*to=='-') break;
		to--;
	}

	while(ln<to){
		if(*ln>=128) return 0;
		if(ret<rt) *ret=*ln; ret++;
		ln++;	
	}

	// decode
	unsigned int i=0, oldi, w, k, n=128, base=36, bias=72, digit, maxint=S4GM;
	ln=to+1; to=line+sz;

	for(ln; ln<to; ++ret){
		for(oldi=i, w=1, k=base;; k += base){
			digit = punyctos_c(*ln++);
			if(digit>=base) return 0;

			if(digit>(maxint-i)/w) return 0;

			i+=digit*w;
			unsigned int t = k <= bias ? 1 : k >= bias + 26 ? 26 : k - bias;
			if(digit<t) break;
			if(w>maxint/(base - t)) return 0;
			w*=(base-t);
		}
		bias=stopunyc_adapt(i-oldi, ret-aret+1, oldi==0);

		if(i/(ret-aret+1)>maxint-n) return 0;
		n+=i/((ret-aret)+1);
		i%=(ret-aret+1);
	
		memmove(aret+i+1, aret+i, (ret-aret-i)*2);
		aret[i++]=n;
	}

	return ret-aret;
}

// end punycode and short

// ampercant '&' replace;  &amp; -> &.
unsigned int prmf_ampreplace(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz){
	unsigned char *ln=line, *to=ln+sz; unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; // define

	while(ln<to){
		if(*ln=='&'){
			if(ln-line){
				if(ret+(ln-line)<=rt) memcpy(ret, line, ln-line);
				ret+=ln-line; line=ln;
			}
			
			ln++;
			if(incompare(ln, to-ln, (unsigned char*)"amp;", 3)){
				if(ret<rt) *ret='&'; ret++; ln+=1;
			}
			if(incompare(ln, to-ln, (unsigned char*)"apos;", 3)){
				if(ret<rt) *ret='\''; ret++; ln+=2;
			}
			if(incompare(ln, to-ln, (unsigned char*)"quot;", 3)){
				if(ret<rt) *ret='"'; ret++; ln+=2;
			}
			else if(incompare(ln, to-ln, (unsigned char*)"lt;", 3)){
				if(ret<rt) *ret='<'; ret++;
			}
			else if(incompare(ln, to-ln, (unsigned char*)"gt;", 3)){
				if(ret<rt) *ret='>'; ret++;
			}
			ln+=2; line=ln+1;
		}
		//else if(*ln==10){
		//	if(ln-line){
		//		if(ret+(ln-line)<=rt) memcpy(ret, line, ln-line);
		//		ret+=ln-line; line=ln;
		//	}
		//	if(ret<rt) *ret=13; ret++;
		//}

		else
			ln++;
	}

	if(ln-line){
		if(ret+(ln-line)<=rt) memcpy(ret, line, ln-line);
		ret+=ln-line;
	}


	return ret-aret;
}

#define prmd_add_memcpy() if(ln - line){ if(ret + (ln - line) <= rt) memcpy(ret, line, ln - line); ret += ln - line; line = ln; }
#define prmf_httptoval_hex(i) (i - ((i <= '9') ? '0' : 'A' - 10))

// replace %ff to value.
unsigned int prmf_httptoval(unsigned char *ret, unsigned int rsz, unsigned char *line, unsigned int sz, int np){
	unsigned char *ln = line, *to = ln + sz; unsigned char *aret = ret, *rt = ret ? ret + rsz : 0; // define
	
	while(ln < to){
		if(*ln == '%'){
			prmd_add_memcpy();

			ln++;
			if(to - ln > 1){
				if(ret < rt)
					*ret = (prmf_httptoval_hex(*ln) << 4) + prmf_httptoval_hex(*(ln+1));
				ret ++;
				ln ++;
				line = ln + 1;
			}
		}
		else if(!np && *ln == '+'){
			prmd_add_memcpy();

			if(ret < rt)
				*ret = ' ';
			ret ++;
			line = ln + 1;
		}

		ln++;
	}

	prmd_add_memcpy();
	return ret - aret;
}

#define PRMF_PCRE_RESULT_SQA	1

// replace $1 to value
unsigned int prmf_pcre_result(unsigned char *ret, unsigned int rsz, unsigned char *data, unsigned int sz, unsigned char *url, unsigned int urlsz, size_t *arr, int arrsz, int opt, VString sqa){
	unsigned char *line = data;
	unsigned char *ln = line, *to = ln + sz; unsigned char *aret = ret, *rt = ret ? ret + rsz : 0; // define

	while(ln < to){
		if(*ln == '$'){
			prmd_add_memcpy();

			int vid = 0;
			ln++;

			while(ln < to){
				if(*(ln+1) < '0' || *(ln+1) > '9'){
					vid = vid * 10 + *ln - '0';
				}
				else
					break;

				ln++;
			}

			if(ln > line + 1){
				if(vid >= 0 && vid < arrsz){
					int vsz = arr[vid * 2 + 1] - arr[vid * 2 + 0];
					if(ret + vsz <= rt){
						memcpy(ret, url + arr[vid * 2], vsz);
					}
					ret += vsz;
				}
			}

			line = ln + 1;
		}

		ln++;
	}

	prmd_add_memcpy();

	// sqa
	if(opt & PRMF_PCRE_RESULT_SQA && sqa){
		if(ret + 1 <= rt){
			*ret = PartLineOT(VString(data, sz), "?") ? '&' : '?';
		}
		ret ++;

		if(ret + sqa.sz <= rt){
			memcpy(ret, sqa, sqa);
		}

		ret += sqa;
	}


	return ret - aret;
}