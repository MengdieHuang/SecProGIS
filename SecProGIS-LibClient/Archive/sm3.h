
#ifndef SM3_H
#define SM3_H
#define u8 unsigned char
#define u32 unsigned long

typedef struct
{
    u32 total[2];     /*!< number of bytes processed  */
    u32 state[8];     /*!< intermediate digest state  */
    u8 buffer[64];   /*!< data block being processed */
    u8 ipad[64];     /*!< HMAC: inner padding        */
    u8 opad[64];     /*!< HMAC: outer padding        */

}
sm3_context;

#ifdef __cplusplus
extern "C" {
#endif

void sm3_starts( sm3_context *ctx );
void sm3_update( sm3_context *ctx, u8 *input, int ilen );
void sm3_finish( sm3_context *ctx, u8 output[32] );
void sm3( u8 *input, int ilen,u8 output[32]);
int sm3_file( char *path, u8 output[32] );
void sm3_hmac_starts( sm3_context *ctx, u8 *key, int keylen);
void sm3_hmac_update( sm3_context *ctx, u8 *input, int ilen );
void sm3_hmac_finish( sm3_context *ctx, u8 output[32] );
void sm3_hmac( u8 *key, int keylen,
                u8 *input, int ilen,
                u8 output[32] );

#ifdef __cplusplus
}
#endif

#endif 
