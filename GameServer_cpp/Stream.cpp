#include "Stream.h"
CStream::CStream(void)
{
	mBufferPointer = NULL;
	mLength = 0;
}

CStream::~CStream(void) {};

bool CStream::SetBuffer(std::string *buffer)
{
	if (!buffer)
	{
		return false;
	}

	mBufferPointer = buffer;
	mLength = 0;

	return true;
}

bool CStream::ReadInt(int *data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(int));
	//copymemory를 통해 지정된 버퍼에 값을 복사
	mLength += sizeof(int);
	//int(4바이트)만큼 데이터 길이 더해줌
	return true;
}

bool CStream::ReadWORD(unsigned long *data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(unsigned long));
	mLength += sizeof(unsigned long);
	return true;
}

bool CStream::ReadByte(std::string *data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(std::string));
	mLength += sizeof(std::string);
	return true;
}

bool CStream::ReadBytes(std::string *data, unsigned long length)
{
	CopyMemory(data, mBufferPointer + mLength, length);
	mLength += length;
	return true;
}

bool CStream::ReadFloat(float *data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(float));
	mLength += sizeof(float);
	return true;
}

bool CStream::ReadDouble(double *data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(double));
	mLength += sizeof(double);
	return true;
}

bool CStream::ReadBOOL(bool *data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(bool));
	mLength += sizeof(bool);
	return true;
}

bool CStream::WriteInt(int *data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(int));
	mLength += sizeof(int);
	return true;
}

bool CStream::WriteWORD(unsigned long *data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(unsigned long));
	mLength += sizeof(unsigned long);
	return true;
}

bool CStream::WriteByte(std::string *data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(std::string));
	mLength += sizeof(std::string);
	return true;
}

bool CStream::WriteBytes(std::string *data, unsigned long length)
{
	CopyMemory(mBufferPointer + mLength, data, length);
	mLength += length;
	return true;
}

bool CStream::WriteFloat(float *data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(float));
	mLength += sizeof(float);
	return true;
}

bool CStream::WriteDouble(double *data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(double));
	mLength += sizeof(double);
	return true;
}

bool CStream::WriteBOOL(bool *data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(bool));
	mLength += sizeof(bool);
	return true;
}

unsigned long CStream::GetLength(void)
{
	return mLength;
}