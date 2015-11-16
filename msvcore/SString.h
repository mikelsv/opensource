// SString by MikelSV.
// New vatiant of Itos
// 0.0.0.1  12.11.2015 12:42

template<int stacksize>
class SStringX : public VString{
	// --------------------------------------------------- Data	 Данные
	unsigned char sdata[stacksize];
	MString mdata;
//	unsigned char *data;
//	unsigned int sz;

	// --------------------------------------------------- Constructor  Конструктор
public:	
	SStringX();
	~SStringX();
	void Init();

protected:
	void StringNew(const void *line);
	void StringDel(unsigned char *line);

	// --------------------------------------------------- Create object  Создание обьекта
public:
	SStringX(const char string);
	SStringX(const char *line);
	SStringX(const unsigned char *line);
	SStringX(const char *line, const unsigned int s);
	SStringX(const unsigned char *line, const unsigned int s);

	SStringX(const short string);
	SStringX(const short *line, int size);
	SStringX(const int code);
	SStringX(const unsigned int code);
	SStringX(const unsigned int code, const unsigned int sz);
	
	SStringX(const VString &line);
	//SString(const MString &line);
	SStringX(const SStringX &line);

	// --------------------------------------------------- Set	Установка

	const SStringX& operator=(char string);
	const SStringX& operator=(const char* line);
	const SStringX& operator=(const unsigned char* line);
	const SStringX& operator=(const VString& line);
	//const SString& operator=(const MString& line);

	SStringX& set(const VString line);
	SStringX& set(const char* line, const unsigned int size);
	SStringX& set(const unsigned char* line, const unsigned int size);
	SStringX& setu(const unsigned char* line, const unsigned int size);

	// --------------------------------------------------- Add  Добавление	

	//const MString& operator+=(const MString& string);
	const SStringX& operator+=(const VString& line);
	const SStringX& operator+=(const char line);
	SStringX& Add(VString o, VString t, VString th=VString(), VString f=VString(), VString fi=VString());
	SStringX& Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7=VString(), VString l8=VString(), VString l9=VString());
	SStringX& Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9,
		VString l10, VString l11=VString(), VString l12=VString(), VString l13=VString(), VString l14=VString(), VString l15=VString(), VString l16=VString(), VString l17=VString(), VString l18=VString(), VString l19=VString());
	SStringX& AddR(VString &l1, VString &l2=_vstringnull_, VString &l3=_vstringnull_, VString &l4=_vstringnull_, VString &l5=_vstringnull_, VString &l6=_vstringnull_, VString &l7=_vstringnull_, VString &l8=_vstringnull_, VString &l9=_vstringnull_);

	SStringX& Format(VString line, MTEVA v1=MTEVA(), MTEVA v2=MTEVA(), MTEVA v3=MTEVA(), MTEVA v4=MTEVA(), MTEVA v5=MTEVA(),
		MTEVA v6=MTEVA(), MTEVA v7=MTEVA(), MTEVA v8=MTEVA(), MTEVA v9=MTEVA(), MTEVA v10=MTEVA());


	// Itos
	VString itos(int64 val, int radix = 10, int null = 0);
	VString dtos(double val, int ml = 0, int radix = 10, int null = 1);
	VString ipitos(unsigned int val);
	VString ip6itos(IP6_ADDRESS ip);
	VString macitos(MACADDR_S mac);
	//---------------------------------------------------- Move   Перемещение

	SStringX& operator-=(SStringX& line);
	void move(SStringX& line);
		
	//---------------------------------------------------- Association Обьединение

	//friend SString operator+(const SString& ln1, const SString& ln2);
	//friend SString operator+(const SString& ln1, const VString& ln2);
 	//friend SString operator+(const SString& ln1, const char* ln2);
	//friend SString operator+(const char* ln1, const SString& ln2);

	//---------------------------------------------------- Operations  Операции

	SStringX& Reserv(int size, int p=0);
	//MString& ReservM(int size, int p=0); // Reserv More
	void CleanData(int p);
	void RClean(int sz, int p);
	//void RSize(int size, int p=0);

	void Clean();
};


typedef SStringX<1024> SString;