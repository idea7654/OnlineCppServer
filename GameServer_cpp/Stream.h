#include <string>
#include <windows.h>
#include <stdio.h>

//using namespace std;
class CStream
{
public:
	CStream(void);
	~CStream(void);
private:
	std::string *mBufferPointer;
	unsigned long mLength;
public:
	bool SetBuffer(std::string *data);

	bool ReadInt(int *data);
	bool ReadWORD(unsigned long *data);
	bool ReadByte(std::string *data);
	bool ReadBytes(std::string *data, unsigned long length);
	bool ReadFloat(float *data);
	bool ReadDouble(double *data);
	bool ReadBOOL(bool *data);

	bool WriteInt(int *data);
	bool WriteWORD(unsigned long *data);
	bool WriteByte(std::string *data);
	bool WriteBytes(std::string *data, unsigned long length);
	bool WriteFloat(float *data);
	bool WriteDouble(double *data);
	bool WriteBOOL(bool *data);

	unsigned long GetLength(void);
};