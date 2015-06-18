// Versions
class Versions{ public: VString ver, date; };

// Global Error
int globalerror();
int globalerror(char*line);

int rtms(unsigned char *line, unsigned int sz, unsigned char *fr, unsigned int frsz, int &pos, bool res);
inline int rtms(const MString &line, const MString fr, int &pos, bool res=0);