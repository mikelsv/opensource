// New Line List. Prevous version - Unlimited Matrix.

// Methods: Add, Set
// Use Stack: Yes! / More stack for god of stack!
// Delete: No
// Locked: No (Not Threads Safe)

// Options: Constructor, Destructor, List
#define ULIST_OPT_CON	1
#define ULIST_OPT_DES	2

#ifndef USEMSV_ULIST_BLOCK_SZ
	#define USEMSV_ULIST_BLOCK_SZ S4K
#endif

template<class UListItem>
class UListData{
public:
	UListData *next;
	int size_all;
	int size_used;
	UListItem data[0];

	void Clear(int sz){
		next = 0;
		size_all = sz;
		size_used = 0;
	}
};

template<class UListItem, const int options = 0, int tstacksize = USEMSV_ULIST_BLOCK_SZ, const int tblocksize = USEMSV_ULIST_BLOCK_SZ>
class UList{
	// stack
	unsigned char stackdata[tstacksize];
	
	// data
	UListData<UListItem> * data, *data_end;
	
	// counts
	int size_all;
	int size_used;

public:
	UList(){
		data = 0;
		data_end = 0;
		size_all = 0;
		size_used = 0;
	}

	~UList(){
		Clear();
	}

	UListItem* Add(UListItem &item){
		return Add(&item, 1);
	}

	UListItem* Add(UListItem *item = 0){
		return Add(item, 1);
	}

	UListItem* Add(UListItem *item, int count, int nofragment = 0){
		int size_free = size_all - size_used;
		UListItem * ritem = 0;

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
		
			size_used += size_free;
			data_end->size_used += size_free;
		}

		if(size_free - count){
			AddBlock(size_free - count);

			if(item)
				item += size_free;
			count -= size_free;

			Copy(data_end->data, item, count);

			size_used += count;
			data_end->size_used += count;

			if(!ritem)
				ritem = data_end->data;
		}

		return ritem;
	}

	VString OneLine(){
		if(!size_used)
			return VString();

		if(data == data_end)
			return VString(data->data, size_used * sizeof(UListItem));

		AddBlock(size_used);

		UListData<UListItem> *p = data, *d;
		int pos = 0;

		while(p != data_end){
			memcpy(data_end->data + pos, p, p->size_used * sizeof(UListItem));
			pos += p->size_used;
			size_all -= p->size_all;
			
			d = p;
			p = p->next;
			free(d);
		}

		data = data_end;
		data->size_used = pos;

		return VString(data->data, size_used * sizeof(UListItem));
	}

protected:
	void Copy(UListItem *to, UListItem *from, int size){
		if(!(options & ULIST_OPT_CON))
			if(from)
				memcpy(to, from, size * sizeof(UListItem));
			else
				memset(to, 0, size * sizeof(UListItem));

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

	void AddBlock(int size){
		UListData<UListItem> *block = 0;
		int blocksize = 0;
		
		if(!data && (unsigned char*)data != stackdata && size <= sizeof(stackdata) - sizeof(UListData<UListItem>)){
			block = (UListData<UListItem>*)stackdata;
			blocksize = sizeof(stackdata);
		}
		else{
			blocksize = size + sizeof(UListData<UListItem>);

			if(blocksize < tblocksize)
				blocksize = tblocksize;

			if(blocksize % 1024)
				blocksize += 1024 - (blocksize % 1024);

			block = (UListData<UListItem>*) malloc(blocksize);
		}

		// clear
		int els = (blocksize - sizeof(UListData<UListItem>)) / sizeof(UListItem);
		block->Clear(els);

		// set
		if(!data)
			data = block;
		else
			data_end = data_end->next;

		size_all += els;
		data_end = block;

		return ;
	}
	
public:

	void Clear(){
		UListData<UListItem> *p = data, *d;
		int pos = 0;

		while(p){
			if(options & ULIST_OPT_DES){
				for(int i = 0; i < p->size_used; i ++){
					p->data[i].~UListItem();
				}
			}

			d = p;
			p = p->next;
			free(d);
		}

		size_used = 0;
		size_all = 0;

		data = 0;
		data_end = 0;
		

		return ;
	}
};