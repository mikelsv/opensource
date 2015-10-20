//#include "UList.h"

template<class UListItem>
void UListData<UListItem>::Clear(int sz){
	next = 0;
	size_all = sz;
	size_used = 0;
}


template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UList<UListItem, options, tstacksize, tblocksize>::UList(){
	data = 0;
	data_end = 0;
	size_all = 0;
	size_used = 0;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UList<UListItem, options, tstacksize, tblocksize>::~UList(){
	Clear();
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UListItem* UList<UListItem, options, tstacksize, tblocksize>::Add(UListItem &item){
	return Add(&item, 1);
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UListItem* UList<UListItem, options, tstacksize, tblocksize>::Add(UListItem *item){
	return Add(item, 1);
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UListItem* UList<UListItem, options, tstacksize, tblocksize>::Add(UListItem *item, int count, int nofragment){
	int size_free = size_all - size_used;
	UListItem * ritem = 0;

	if(!count)
		return 0;

	if(size_free && nofragment && size_free < count){
		size_all -= data_end->size_all - data_end->size_used;
		data_end->size_all = data_end->size_used;
		size_free = 0;
	}

	if(size_free){
		ritem = data_end->data + data_end->size_used;
			
		if(size_free > count)
			size_free = count;

		Copy(ritem, item, size_free);
		
		count -= size_free;
		size_used += size_free;
		data_end->size_used += size_free;
	}

	if(count){
		AddBlock(count);

		if(item)
			item += size_free;
		//count -= size_free;

		Copy(data_end->data, item, count);

		size_used += count;
		data_end->size_used += count;

		if(!ritem)
			ritem = data_end->data;
	}

	return ritem;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UListItem* UList<UListItem, options, tstacksize, tblocksize>::Get(int pos){
	UListData<UListItem> *p = data;

	if(pos < 0)
		return 0;

	while(p){
		if(pos < p->size_used)
			return p->data + pos;
		
		pos -= p->size_used;
		p = p->next;	
	}

	return 0;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UListItem* UList<UListItem, options, tstacksize, tblocksize>::operator[](int pos){
	return Get(pos);
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
int UList<UListItem, options, tstacksize, tblocksize>::Size(){
	return size_used;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
int UList<UListItem, options, tstacksize, tblocksize>::Blocks(){
	UListData<UListItem> *p = data;
	int s = 0;

	while(p){
		s ++;
		p = p->next;
	}

	return s;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UListItem* UList<UListItem, options, tstacksize, tblocksize>::GetBlock(int pos, int &sz){
	UListData<UListItem> *p = data;

	while(p){
		if(!pos){
			sz = p->size_used;
			return p->data;
		}

		pos --;
		p = p->next;
	}

	sz = 0;
	return 0;
}



template<class UListItem, const int options, const int tstacksize, const int tblocksize>
UListItem* UList<UListItem, options, tstacksize, tblocksize>::OneLine(){
	if(!size_used)
		return VString();

	if(data == data_end)
		return VString(data->data, size_used * sizeof(UListItem));

	AddBlock(size_used);

	UListData<UListItem> *p = data, *d;
	int pos = 0;

	while(p != data_end){
		memcpy(data_end->data + pos, p->data, p->size_used * sizeof(UListItem));
		pos += p->size_used;
		size_all -= p->size_all;
			
		d = p;
		p = p->next;

		if(d != (void*)stackdata)
			free(d);
	}

	data = data_end;
	data->size_used = pos;

	return data->data;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
void UList<UListItem, options, tstacksize, tblocksize>::Copy(UListItem *to, UListItem *from, int size){
	if((options & ULIST_OPT_CON) == 0){
		if(from)
			memcpy(to, from, size * sizeof(UListItem));
		else
			memset(to, 0, size * sizeof(UListItem));
		return ;
	}

	if(from)
		for(int i = 0; i < size; i++){
			new(to)UListItem;
			*to++ = *from++;
		}
	else
		for(int i = 0; i < size; i++){
			new(to)UListItem;
		}
	return ;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
void UList<UListItem, options, tstacksize, tblocksize>::AddBlock(int size){
	UListData<UListItem> *block = 0;
	int blocksize = 0;
		
	if(!data && (unsigned char*)data != stackdata && size * sizeof(UListItem) + sizeof(UListData<UListItem>) - sizeof(UListItem) <= sizeof(stackdata)){
		block = (UListData<UListItem>*)stackdata;
		blocksize = sizeof(stackdata);
	}
	else{
		blocksize = size * sizeof(UListItem) + sizeof(UListData<UListItem>) - sizeof(UListItem);

		if(blocksize < tblocksize)
			blocksize = tblocksize;

		if(blocksize % 1024)
			blocksize += 1024 - (blocksize % 1024);

		block = (UListData<UListItem>*) malloc(blocksize);
	}

	// clear
	int els = (blocksize - sizeof(UListData<UListItem>) + sizeof(UListItem)) / sizeof(UListItem);
	block->Clear(els);

	// set
	if(!data)
		data = block;
	else
		data_end->next = block;

	size_all += els;
	data_end = block;

	return ;
}

template<class UListItem, const int options, const int tstacksize, const int tblocksize>
void UList<UListItem, options, tstacksize, tblocksize>::Clear(){
	UListData<UListItem> *p = data, *d;
	int pos = 0;

	while(p){
		if((options & ULIST_OPT_DES) != 0){
			for(int i = 0; i < p->size_used; i ++){
				p->data[i].~UListItem();
			}
		}

		d = p;
		p = p->next;
		
		if(d != (void*)stackdata)
			free(d);
	}

	size_used = 0;
	size_all = 0;

	data = 0;
	data_end = 0;		

	return ;
}