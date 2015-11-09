//int rts(VString& line, char fr, int &pos){ //read to symbol
//unsigned char *to=line.data+line.sz; line.data+=pos; if(!line) return 0;
//for (line.data; line.data<to; line.data++){
//	if (*line.data==fr) {pos=line.sz-(to-line.data); return 1;}
//} pos=line.sz-(to-line.data); return 0;
//}

// New rts class
int rts(unsigned char *to, char fr, unsigned char *&line){ //read to symbol
	if(!line) return 0;
	for(line; line<to; line++){
		if(*line==fr) return 1;
	}
	return 0;
}


int rtns(unsigned char *to, char fr, unsigned char *&line){ //read to no symbol
	if(!line) return 0;
	for(line; line<to; line++){
		if(*line!=fr) return 1;
	}
	return 0;
}

int rtms(unsigned char *to, unsigned char *fr, unsigned int frs, unsigned char *&line){ //read to symbol
	if(!line || !fr || !frs) return 0;
	unsigned char *lfr, *tfr, *ln;
	to-=frs-1;

	for(line; line<to; line++){
		if(*line==*fr){
			lfr=fr+1; tfr=fr+frs; ln=line+1;
			for(lfr; lfr<tfr; lfr++){ if(*ln++!=*lfr){ break; } }
			if(lfr==tfr){ return 1; }
		}
	} line=to+(frs-1); return 0;
}

int rtmsu(unsigned char *to, unsigned char *fr, unsigned int frs, unsigned char *&line){ //read to symbol up
	if(!line || !fr || !frs) return 0;
	unsigned char *lfr, *tfr, *ln;
	to-=frs-1;

	for(line; line<to; line++){
		if(upd(*line)==upd(*fr)){
			lfr=fr+1; tfr=fr+frs; ln=line+1;
			for(lfr; lfr<tfr; lfr++){ if(upd(*ln++)!=upd(*lfr)){ break; } }
			if(lfr==tfr){ return 1; }
		}
	} line=to+(frs-1); return 0;
}



unsigned char* rtms(unsigned char *line, unsigned char *to, unsigned char *fr, unsigned int frs){ //read to symbol
	if(!line || !fr || !frs) return 0;
	unsigned char *lfr, *tfr, *ln;
	to-=frs-1;

	for(line; line<to; line++){
		if(*line==*fr){
			lfr=fr+1; tfr=fr+frs; ln=line+1;
			for(lfr; lfr<tfr; lfr++){ if(*ln++!=*lfr){ break; } }
			if(lfr==tfr){ return line; }
		}
	}
	//line=to+(frs-1);
	return 0;
}

// short
int rts(unsigned short *to, char fr, unsigned short *&line){ //read to symbol
	if(!line) return 0;
	for(line; line<to; line++){
		if(*line==fr) return 1;
	}
	return 0;
}

int rtms(unsigned short *to, unsigned short *fr, unsigned int frs, unsigned short *&line){ //read to symbol
	if(!line || !fr || !frs) return 0;
	unsigned short *lfr, *tfr, *ln;
	to-=frs-1;

	for(line; line<to; line++){
		if(*line==*fr){
			lfr=fr+1; tfr=fr+frs; ln=line+1;
			for(lfr; lfr<tfr; lfr++){ if(*ln++!=*lfr){ break; } }
			if(lfr==tfr){ return 1; }
		}
	} return 0;
}