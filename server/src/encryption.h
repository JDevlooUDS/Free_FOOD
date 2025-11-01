#include <wolfssl/wolfcrypt/rsa.h>
#include <wolfssl/wolfcrypt/error-crypt.h>

bool verifySignature(RsaKey *key, byte *sig, byte *hash) {

    byte *out = (byte*)malloc(32);
    word32 outLen = sizeof(out);
    if (wc_RsaSSL_Verify(sig, sizeof(sig), out, outLen, key) != 0 ) {
        free(out);
        return false;
    }

    if (hash == out) {
        free(out);
        return true;
    }
    free(out);
    return false;
}

int hashFile(byte *data, byte *hash) {
    return wc_Sha256Hash(data, sizeof(data), hash);
}



