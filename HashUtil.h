/** \class HashUtil
 * HashUtil (MD5 hashing)
 *
 * Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 * rights reserved.
 *
 * License to copy and use this software is granted provided that it
 * is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 * Algorithm" in all material mentioning or referencing this software
 * or this function.
 * 
 * License is also granted to make and use derivative works provided
 * that such works are identified as "derived from the RSA Data
 * Security, Inc. MD5 Message-Digest Algorithm" in all material
 * mentioning or referencing the derived work.
 * 
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 * 
 * These notices must be retained in any copies of any part of this
 * documentation and/or software. 
 *
 * @author  RSA Data Security, Inc. (http://www.rsasecurity.com)
 * @date  21/08/04
 * @see RFC1321
 */

#ifndef _HASHUTIL_H_
#define _HASHUTIL_H_

class HashUtil{
private:

    static const unsigned int S11=7;
    static const unsigned int S12=12;
    static const unsigned int S13=17;
    static const unsigned int S14=22;
    static const unsigned int S21=5;
    static const unsigned int S22=9;
    static const unsigned int S23=14;
    static const unsigned int S24=20;
    static const unsigned int S31=4;
    static const unsigned int S32=11;
    static const unsigned int S33=16;
    static const unsigned int S34=23;
    static const unsigned int S41=6;
    static const unsigned int S42=10;
    static const unsigned int S43=15;
    static const unsigned int S44=21;

    // F, G, H and I are basic MD5 functions.
    static inline unsigned int F(unsigned int x, unsigned int y, unsigned int z){
        return (((x) & (y)) | ((~x) & (z)));
    }
    static inline unsigned int G(unsigned int x, unsigned int y, unsigned int z){
        return (((x) & (z)) | ((y) & (~z)));
    }
    static inline unsigned int H(unsigned int x, unsigned int y, unsigned int z){
        return ((x) ^ (y) ^ (z));
    }
    static inline unsigned int I(unsigned int x, unsigned int y, unsigned int z){
        return ((y) ^ ((x) | (~z)));
    }
    static inline unsigned int ROTATE_LEFT(unsigned int x, unsigned int n){
        return (((x) << (n)) | ((x) >> (32-(n))));
    }

    // FF,GG,HH, and II transformations for rounds 1,2,3, and 4. Rotation is separate from addition to prevent recomputation.
    static inline void FF(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac){
        a += F(b, c, d) + x + ac;
        a = ROTATE_LEFT(a, s);
        a+=b;
    }
    static inline void GG(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac){
        a += G (b, c, d) + x + ac;
        a = ROTATE_LEFT(a, s);
        a+=b;
    }
    static inline void HH(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac){
        a += H (b, c, d) + x + ac;
        a = ROTATE_LEFT(a, s);
        a+=b;
    }
    static inline void II(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac){
        a += I (b, c, d) + x + ac;
        a = ROTATE_LEFT(a, s);
        a+=b;
    }

    typedef struct {
        unsigned int state[4];
        unsigned int count[2];
        unsigned char buffer[64];
    } MD5_CTX;

    static unsigned char m_PADDING[64];

    static void MD5Init(MD5_CTX *context);
    static void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputLen);
    static void MD5Final(unsigned char digest[16], MD5_CTX *context);

    static void MD5Transform(unsigned int state[4], unsigned char block[64]);
    static void MD5_Encode(unsigned char *, unsigned int *, unsigned int);
    static void MD5_Decode(unsigned int *, unsigned char *, unsigned int);
    static void MD5_memcpy(unsigned char*, unsigned char*, unsigned int);
    static void MD5_memset(unsigned char*, int, unsigned int);

public:
	static unsigned char* getMD5Sum(unsigned char* pData, int size);
};

#endif
