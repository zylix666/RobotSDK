#ifndef _SHA256_H_
#define _SHA256_H_

typedef struct {
   unsigned char data[64];
   unsigned long datalen;
   unsigned long bitlen[2];
   unsigned long state[8];
} SHA256_CTX;

void sha256_transform(SHA256_CTX *ctx, unsigned char data[]);
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, unsigned char data[], unsigned long len);
void sha256_final(SHA256_CTX *ctx, unsigned char hash[]);


#endif
