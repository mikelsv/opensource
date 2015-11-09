// Versions
class Versions{ public: VString ver, date; };

// Global Error
int globalerror();
int globalerror(char*line);

void MsvAllocConsole();

int rtms(unsigned char *line, unsigned int sz, unsigned char *fr, unsigned int frsz, int &pos, bool res);
inline int rtms(const MString &line, const MString fr, int &pos, bool res=0);

#include "list/UList.h"

template<int stacksize>
class HLStringX : public UList<unsigned char, 0, stacksize>{
public:

// emulate LString, pleasse DON't use it
	void add(const char data);
	void add(char *data, unsigned int sz);
	int addnf(unsigned char *data, unsigned int sz);
	VString addnfr(VString line);
	unsigned char* addnfv(unsigned int sz);

	int size();
	char& el(int p);

	operator bool();
	operator const VString();

	VString String();

//#ifndef WIN32
	operator const MString();
//#endif

	char* oneline();

	int Send(SOCKET sock);
	void String(MString &line);

// end emulate LString, pleasse DON't use it

	HLStringX& operator+(const short i);
	HLStringX& operator+(const unsigned short i);
	HLStringX& operator+(const int i);
	HLStringX& operator+(const unsigned int i);
	HLStringX& operator+(const int64 i);
	HLStringX& operator+(const uint64 i);

	HLStringX& operator+(const MString& string);
	HLStringX& operator+(const VString& string);
	HLStringX& operator+(const char* lpsz);
	HLStringX& operator+(HLStringX& lst);

	HLStringX& operator+=(const VString& string);

};