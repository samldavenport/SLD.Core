#include "sld.hpp"

#include <meow-hash/meow_hash_x64_aesni.h>

namespace sld {

    SLD_API u32
    hash_u32(
        void*     data,
        const u32 size) {

        assert(data != NULL && size != 0);

        u128 hash;
        hash.simd_reg = MeowHash(MeowDefaultSeed, size, data);

        return(hash.u32_0);
    }

    SLD_API u64
    hash_u64(
        void* data,
        const u32   size) {

        assert(data != NULL && size != 0);

        u128 hash;
        hash.simd_reg = MeowHash(MeowDefaultSeed, size, data);

        return(hash.u64_lo);
    }

    SLD_API u128
    hash_u128(
        void* data,
        const u32   size) {

        assert(data != NULL && size != 0);

        u128 hash;
        hash.simd_reg = MeowHash(MeowDefaultSeed, size, data);

        return(hash);
    }

    SLD_API bool
    hash_is_equal(
        void* data_a,
        void* data_b,
        const u32   size) {

        assert(
            data_a != NULL &&
            data_b != NULL &&
            size   != 0
        );

        u128 hash_a, hash_b;

        hash_a.simd_reg = MeowHash(MeowDefaultSeed, size, data_a);
        hash_b.simd_reg = MeowHash(MeowDefaultSeed, size, data_b);

        const bool is_equal = (
            hash_a.u64_lo == hash_b.u64_lo &&
            hash_a.u64_hi == hash_b.u64_hi
        );

        return(is_equal);
    }

};