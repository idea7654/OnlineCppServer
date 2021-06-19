#pragma once
class CRandom
{
public:
	CRandom(void);
	virtual ~CRandom(void);

private:
	int mSeed;
public:
	bool Init(int seed);

	int Rand(void);
	int SimpleRandom(void);
	int ComplexRandom(void);
};