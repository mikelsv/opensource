// New Unlimited List. Prevous version - Unlimited Matrix.
// 0.0.0.1 - 18.06.15 18:26

// Methods: Add, Get, Set
// Use Stack: Yes! / More stack for god of stack!
// Delete: No
// Locked: No (Not Threads Safe)

// Options: Constructor, Destructor
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
	UListItem data[1];

	void Clear(int sz);
};

template<class UListItem, const int options = ULIST_OPT_CON | ULIST_OPT_DES, const int tstacksize = USEMSV_ULIST_BLOCK_SZ, const int tblocksize = USEMSV_ULIST_BLOCK_SZ>
class UList{
	// stack
	unsigned char stackdata[tstacksize];
	
	// data
	UListData<UListItem> * data, *data_end;
	
	// counts
	int size_all;
	int size_used;

public:
	UList();
	~UList();

	UListItem* Add(UListItem &item);
	UListItem* Add(UListItem *item = 0);
	UListItem* Add(UListItem *item, int count, int nofragment = 0);
	UListItem* Add(UList<UListItem>);

	UListItem* Get(int pos);
	UListItem* operator[](int pos);

	int Size();
	int Blocks();

	UListItem* GetBlock(int pos, int &sz);
	UListItem* OneLine();

protected:
	void Copy(UListItem *to, UListItem *from, int size);
	void AddBlock(int size);
	
public:
	void Clear();
};
