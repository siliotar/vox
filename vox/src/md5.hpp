#pragma once

#include <string>
#include <vector>

class MD5
{
private:
	void init();
	enum { blocksize = 64 };

	typedef unsigned char uint1;
	typedef unsigned int uint4;

	void transform(const unsigned char block[blocksize]);

	void update(const unsigned char* buf, size_t length);
	MD5& finalize();

	static void decode(uint4 output[], const uint1 input[], unsigned int len);
	static void encode(uint1 output[], const uint4 input[], unsigned int len);

	bool finalized;
	unsigned char buffer[blocksize];
	unsigned int count[2];
	unsigned int state[4];

	static inline uint4 F(uint4 x, uint4 y, uint4 z);
	static inline uint4 G(uint4 x, uint4 y, uint4 z);
	static inline uint4 H(uint4 x, uint4 y, uint4 z);
	static inline uint4 I(uint4 x, uint4 y, uint4 z);
	static inline uint4 rotate_left(uint4 x, int n);
	static inline void FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);

public:
	unsigned char digest[16];
	MD5(const std::string& text);
	~MD5();
	std::string hexdigest() const;
};

std::string md5(const std::string str);
std::vector<unsigned char> md5AsBytes(const std::string str);
