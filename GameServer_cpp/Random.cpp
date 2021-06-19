#include "Random.h"

CRandom::CRandom(void){}
CRandom::~CRandom(void){}

bool CRandom::Init(int seed)
{
	mSeed = seed;
	return true;
}

int CRandom::Rand(void)
{
	return (((mSeed = mSeed * 214013L + 2531011L) >> 16) & 0x7FFF);
}

int CRandom::SimpleRandom(void)
{
	return (((mSeed = mSeed + 2531011L) >> 3) & 0x7FFF);
}

int CRandom::ComplexRandom(void)
{
	int next = mSeed;
	int result;

	next *= 1103515245;
	next += 12345;
	result = (next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (next / 65536) % 1024;

	mSeed = next;
	return result;
}