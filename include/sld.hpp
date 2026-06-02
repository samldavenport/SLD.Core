#ifndef SLD_HPP
#define SLD_HPP

#include <Windows.h>
#include <cstdint>
#include <assert.h>

#ifdef SLD_STATIC
#   define SLD_API
#elif defined(SLD_DLL_EXPORT)
#   define SLD_API __declspec(dllexport)
#else
#   define SLD_API __declspec(dllimport)
#endif

#define SLD_API_INLINE  inline
#define SLD_API_UTILITY static constexpr

#if _MSC_VER
#   if !defined(__clang__)
#       define INSTRUCTION_REORDER_BARRIER _ReadWriteBarrier()
#   endif
#   include <intrin.h>
#else
#   include <x86intrin.h>
#endif

namespace sld {

    //--------------------------------------------------------------------
    // PRIMITIVE TYPES
    //--------------------------------------------------------------------

    // signed integers
    typedef int8_t    s8;
    typedef int16_t   s16;
    typedef int32_t   s32;
    typedef int64_t   s64;

    // unsigned integers
    typedef uint8_t   u8;
    typedef uint16_t  u16;
    typedef uint32_t  u32;
    typedef uint64_t  u64;

    // floats
    typedef float    f32;
    typedef double   f64;

    // simd
    typedef __m128i  simd_4u32;
    typedef __m128d  simd_2f64;
    typedef __m128   simd_4f32;

    // booleans
    typedef u8       b8;
    typedef u16      b16;
    typedef u32      b32;
    typedef u64      b64;

    // characters
    typedef char     cchar8;
    typedef wchar_t  wchar16;

    // memory
    typedef u8       byte;
    typedef void*    vptr;
    typedef intptr_t addr;
    typedef u32      index;
    typedef void     element;
    typedef void     key;
    typedef void     value;

    // handles
    typedef u8       h8;
    typedef u16      h16;
    typedef u32      h32;
    typedef u64      h64;

    //--------------------------------------------------------------------
    // MEMORY
    //--------------------------------------------------------------------

    struct memory {
        u64 size;
        union {
            void* ptr;
            byte* bytes;
            addr  address;
        };
    };

    //--------------------------------------------------------------------
    // LARGE VALUES
    //--------------------------------------------------------------------

    struct alignas(16) u128 {
        union {
            struct {
                u32 u32_0;
                u32 u32_1;
                u32 u32_2;
                u32 u32_3;
            };
            struct {
                u64 u64_lo;
                u64 u64_hi;
            };
            u64        val64[2];
            u32        val32[4];
            simd_4u32 simd_reg;
        };
        
        SLD_API_INLINE u128() = default;
        SLD_API_INLINE u128(simd_4u32 reg) : simd_reg(reg) { }

        SLD_API_INLINE u128&
        operator=(simd_4u32 reg) {
            this->simd_reg = reg;
            return(*this);
        }
    };

    struct alignas(16) f128 {
        union {
            struct {
                f32 f32_0;
                f32 f32_1;
                f32 f32_2;
                f32 f32_3;
            };
            struct {
                f64 u64_lo;
                f64 u64_hi;
            };
            f64       val64[2];
            f32       val32[4];
            simd_4f32 simd_reg;
        };

        SLD_API_INLINE f128() = default;
        SLD_API_INLINE f128(simd_4f32 reg) : simd_reg(reg) { }

        SLD_API_INLINE f128&
        operator=(simd_4f32 reg) {
            this->simd_reg = reg;
            return(*this);
        }
    };

    //--------------------------------------------------------------------
    // GRAPHICS
    //--------------------------------------------------------------------

    struct color_rgba_u32 {
        union {
            struct {
                byte a;
                byte b;
                byte g;
                byte r;
            };
            u32 hex;
        };

        SLD_API_INLINE color_rgba_u32() = default;
        SLD_API_INLINE color_rgba_u32(u32 hex_val) : hex(hex_val)  { }

        SLD_API_INLINE color_rgba_u32&
        operator=(u32 hex_val) {
            this->hex = hex_val;
            return(*this);
        }
    };

    struct color_rgba_f32 {
        f32 r;
        f32 g;
        f32 b;
        f32 a;

        SLD_API_INLINE color_rgba_f32() = default;

        SLD_API_INLINE
        color_rgba_f32(
            const u32 hex_Val) {

            const f32  normal_factor = (1.0f / 255.0f);
            const byte r             = (hex_Val >> 24) & 0xFF;
            const byte g             = (hex_Val >> 16) & 0xFF;
            const byte b             = (hex_Val >> 8)  & 0xFF;
            const byte a             =  hex_Val        & 0xFF;

            this->r = normal_factor * (f32)r;
            this->g = normal_factor * (f32)g;
            this->b = normal_factor * (f32)b;
            this->a = normal_factor * (f32)a;            
        }

    };

    struct size {
        u32 width;
        u32 height;
    };

    typedef struct {
        u32 x;
        u32 y; 
    } point, position;

    struct dimensions {
        size     size;
        position pos;
    };

    //--------------------------------------------------------------------
    // HELPERS
    //--------------------------------------------------------------------

    SLD_API_UTILITY void
    swap_u16(u16& a, u16& b) {
        u16 tmp = a;
        a = b;
        b = tmp;
    }
    
    SLD_API_UTILITY void
    swap_u32(u32& a, u32& b) {
        u32 tmp = a;
        a = b;
        b = tmp;
    }
    
    SLD_API_UTILITY void
    swap_u64(u64& a, u64& b) {
        u64 tmp = a;
        a = b;
        b = tmp;
    }

    SLD_API_INLINE void
    zero_memory(
        void* mem,
        u64   size) {
        
        memset(mem, 0, size);
    }    

    //--------------------------------------------------------------------
    // SIZING
    //--------------------------------------------------------------------

    SLD_API_UTILITY u32 size_kilobytes(const u32 num_kb) { return(num_kb * 1024);               }
    SLD_API_UTILITY u32 size_megabytes(const u32 num_mb) { return(num_mb * 1024 * 1024);        }
    SLD_API_UTILITY u32 size_gigabytes(const u32 num_gb) { return(num_gb * 1024 * 1024 * 1024); }

    //--------------------------------------------------------------------
    // BITWISE LOGIC
    //--------------------------------------------------------------------

    SLD_API_UTILITY u32  bit_value     (const u32 bit)                       { return (1 << bit);           }
    SLD_API_UTILITY bool bit_test      (const u32 value, const u32 bit)      { return ((value >> bit) & 1); }
    SLD_API_UTILITY bool bit_mask_all  (const u32 value, const u32 mask)     { return ((value & mask) > 0); }
    SLD_API_UTILITY bool bit_mask_any  (const u32 value, const u32 mask)     { return ((value | mask) > 0); }
    SLD_API_UTILITY void bit_on        (u32&      value, const u32 bit)      { value |=  (1 << bit);        }
    SLD_API_UTILITY void bit_off       (u32&      value, const u32 bit)      { value &= ~(1 << bit);        }
    SLD_API_UTILITY void bit_toggle    (u32&      value, const u32 bit)      { value ^=  (1 << bit);        }
    SLD_API_UTILITY bool bit_mask_test (const u32 value, const u32 mask)     { return((value & mask) > 0);  }
    SLD_API_UTILITY void bit_mask_or   (u32&      value, const u32 mask)     { (value |=  mask);            }
    SLD_API_UTILITY void bit_mask_and  (u32&      value, const u32 mask)     { (value &=  mask);            }
    SLD_API_UTILITY void bit_mask_off  (u32&      value, const u32 mask)     { (value &= ~mask);            }

    //--------------------------------------------------------------------
    // SIMD
    //--------------------------------------------------------------------

    SLD_API_INLINE u128 simd_u128_a_add_b  (u128& reg_a, const u128& reg_b) { return(_mm_add_epi32(reg_a.simd_reg, reg_b.simd_reg)); }
    SLD_API_INLINE u128 simd_u128_a_sub_b  (u128& reg_a, const u128& reg_b) { return(_mm_sub_epi32(reg_a.simd_reg, reg_b.simd_reg)); }
    SLD_API_INLINE u128 simd_u128_a_mul_b  (u128& reg_a, const u128& reg_b) { return(_mm_mul_epi32(reg_a.simd_reg, reg_b.simd_reg)); }
    SLD_API_INLINE u128 simd_u128_a_div_b  (u128& reg_a, const u128& reg_b) { return(_mm_div_epi32(reg_a.simd_reg, reg_b.simd_reg)); }
    
    SLD_API_INLINE f128 simd_f128_a_add_b  (f128& reg_a, const f128& reg_b) { return(_mm_add_ps(reg_a.simd_reg, reg_b.simd_reg));    }
    SLD_API_INLINE f128 simd_f128_a_sub_b  (f128& reg_a, const f128& reg_b) { return(_mm_sub_ps(reg_a.simd_reg, reg_b.simd_reg));    }
    SLD_API_INLINE f128 simd_f128_a_mul_b  (f128& reg_a, const f128& reg_b) { return(_mm_mul_ps(reg_a.simd_reg, reg_b.simd_reg));    }
    SLD_API_INLINE f128 simd_f128_a_div_b  (f128& reg_a, const f128& reg_b) { return(_mm_div_ps(reg_a.simd_reg, reg_b.simd_reg));    }

    //--------------------------------------------------------------------
    // HASHING
    //--------------------------------------------------------------------

    SLD_API u32  hash_u32      (void* data,   const u32   size);
    SLD_API u64  hash_u64      (void* data,   const u32   size);
    SLD_API u128 hash_u128     (void* data,   const u32   size);
    SLD_API bool hash_is_equal (void* data_a, void* data_b, const u32 size);

};

#endif //SLD_HPP