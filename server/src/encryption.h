#include <wolfssl/wolfcrypt/rsa.h>
#include <wolfssl/wolfcrypt/error-crypt.h>

bool verifySignature(RsaKey *key, byte *sig, word32 sigLen, byte *hash, word32 hashLen) {

    byte *out = (byte*)malloc(256);
    word32 outLen = 256;
    if (wc_RsaSSL_Verify(sig, sigLen, out, outLen, key) != 0 ) {
        free(out);
        return false;
    }

    if (memcmp(hash,out, hashLen) == 0) {
        free(out);
        return true;
    }
    free(out);
    return false;
}

bool decodePubKey(RsaKey *key, byte *input, word32 inSize) {
    word32 inOutIdx = 0;
    if (wc_RsaPublicKeyDecode(input, &inOutIdx, key, inSize) != 0) {
        return false;
    }
    return true;
}

int hashFile(byte *data, word32 dataLen, byte *hash) {
    return wc_Sha256Hash(data, dataLen, hash);
}



