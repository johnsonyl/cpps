/*
 * Copyright (c) 2022 Vladimir Kozelkov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <type_traits>
#include <initializer_list>
#include <cstddef>
#include <sstream>

#ifndef __has_include
#define INTEGER_HPP_HAS_OSTREAM 0
#else
#if __has_include(<ostream>)
#define INTEGER_HPP_HAS_OSTREAM 1
#else
#define INTEGER_HPP_HAS_OSTREAM 0
#endif
#endif

#if INTEGER_HPP_HAS_OSTREAM
#include <ostream>
#endif

#ifdef WIN32
#ifdef __cplusplus
#pragma warning(disable: 4146) 
#endif
#endif
 //TODO: shift operators

 // -std=c++14
namespace JIO {

    namespace ct {

        template<typename R, bool v>
        using if_t = typename std::enable_if<v, R>::type;
    }

    template<typename T>
    constexpr inline T lowestOneBit(const T i) noexcept {
        // HD, Section 2-1
        return i & static_cast<T>(-i);
    }

    template<typename T>
    constexpr inline bool isOneBit(const T i) noexcept {
        return lowestOneBit(i) == i;
    }

    namespace p_i_seq {

        using signed_size_t = std::make_signed<size_t>::type;

        template <typename T, T... values>
        struct array_t {
            using type = T;
            constexpr static size_t length = sizeof...(values);
            using A = array_t<T, values...>;

            constexpr inline static const T get(size_t index) noexcept {
                constexpr T data[length == 0 ? 1 : length] = { values... };
                return  data[index];
            }

            constexpr inline const T operator[](size_t index) const noexcept {
                return get(index);
            }

            constexpr inline signed_size_t index_of(T value) const noexcept {
                for (size_t i = 0; i < length; i++) {
                    if (get(i) == value) {
                        return i;
                    }
                }
                return -1;
            }

            template<T value>
            constexpr inline static signed_size_t index_of() noexcept {
                return A().index_of(value);
            }
        };

        template <typename T, size_t size>
        struct v_array_t {
            using type = T;
            constexpr static size_t length = size;
            T data[length == 0 ? 1 : length];

            constexpr inline signed_size_t index_of(T value) const noexcept {
                for (size_t i = 0; i < length; i++) {
                    if (data[i] == value) {
                        return i;
                    }
                }
                return -1;
            }

            constexpr inline const T& operator[](size_t index) const noexcept {
                return data[index];
            }

            //constexpr inline T& operator[](size_t index) noexcept {
            //    return data[index];
            //}
        };

        struct unused {
            constexpr inline unused() noexcept = default;

            template<typename T>
            constexpr inline unused(T) noexcept { }
        };

        constexpr inline int unused_array(std::initializer_list<unused>) noexcept { return 0; }

        template<typename T, T... v1, T... v2>
        array_t<T, v1..., v2...>
            append(array_t<T, v1...>, array_t<T, v2...>);

        template <typename T, T f, T l,
            int = (f > l) ? -1 : (l == f ? 0 : ((l - f == 1) ? 1 : 2))>
            struct seq_h;

        template <typename T, T f, T l>
        struct seq_h <T, f, l, 0> {
            typedef array_t<T> type;
        };

        template <typename T, T f, T l>
        struct seq_h <T, f, l, 1> {
            typedef array_t<T, f> type;
        };

        template <typename T, T f, T l>
        struct seq_h <T, f, l, 2> {
            constexpr static T d = l - f;
            typedef decltype(append<T>(
                typename seq_h<T, f, f + d / 2 > ::type(),
                typename seq_h<T, f + d / 2, f + d> ::type()
                )) type;
        };

        template<typename T, T f, T l>
        using make_array = typename seq_h<T, f, l>::type;

        template<size_t... i1>
        struct p_wrapper {
            template<size_t>
            using any_h = unused;

            template<size_t... i2, typename T>
            constexpr inline static T
                get_value(any_h<i1>..., T value, any_h<i2>...) noexcept {
                return value;
            }
        };

        template<size_t... i1, size_t... i2, typename... Tp>
        constexpr inline auto p_element_h(array_t<size_t, i1...>,
            array_t<size_t, i2...>, Tp... arr) -> decltype(p_wrapper<i1...>::template get_value<i2...>(arr...))  {
            return p_wrapper<i1...>::template get_value<i2...>(arr...);
        }

        template<size_t index, typename... Tp>
        constexpr inline auto element(Tp... arr) -> decltype(p_element_h(make_array<size_t, 0, index>(),
            make_array<size_t, index + 1, sizeof...(arr)>(), arr...)) {
            return p_element_h(make_array<size_t, 0, index>(),
                make_array<size_t, index + 1, sizeof...(arr)>(), arr...);
        }

        template<size_t index, typename... Tp>
        using element_t = decltype(element<index>(std::declval<Tp>()...));

        template<typename... Tp>
        constexpr inline auto last_element(Tp... arr) -> decltype(element<sizeof...(arr) - 1 >(arr...)) {
            return element<sizeof...(arr) - 1 >(arr...);
        }

        template <typename... Tp>
        struct t_array_t {
            constexpr static size_t length = sizeof...(Tp);

            template<size_t index>
            using get = element_t<index, Tp...>;
        };

        template<typename T, size_t length, size_t... index>
        constexpr inline bool
            any(const v_array_t<T, length>& v, array_t<size_t, index...>) noexcept {
            bool out = false;
            p_i_seq::unused_array({
                (out |= !!v[index])...
                });
            return out;
        }

        template<typename T, size_t length>
        constexpr inline bool
            any(const v_array_t<T, length>& v) noexcept {
            return any(v, make_array<size_t, 0, length>());
        }

        template<typename... Tp>
        constexpr inline bool any(Tp... v) noexcept {
            return any(v_array_t<bool, sizeof...(v)>{(!!v)...});
        }

        template<typename T, size_t length, size_t... index>
        constexpr inline bool
            all(const v_array_t<T, length>& v, array_t<size_t, index...>) noexcept {
            bool out = true;
            p_i_seq::unused_array({
                (out &= !!v[index])...
                });
            return out;
        }

        template<typename T, size_t length>
        constexpr inline bool
            all(const v_array_t<T, length>& v) noexcept {
            return all(v, make_array<size_t, 0, length>());
        }

        template<typename... Tp>
        constexpr inline bool all(Tp... v) noexcept {
            return all(v_array_t<bool, sizeof...(v)>{(!!v)...});
        }
    }

    namespace p_i_native {

        using ints_t = p_i_seq::t_array_t<char, short, int, long, long long>;

        template<typename Arr, size_t index>
        constexpr inline bool is_unique_h1() {
            bool u = true;
            for (size_t i = 0; i < index; i++) {
                if (Arr::get(i) == Arr::get(index)) {
                    u = false;
                    break;
                }
            }
            return u;
        }

        template<typename Arr, size_t... index>
        p_i_seq::array_t<bool, is_unique_h1<Arr, index>()...>
            is_unique_h2(p_i_seq::array_t<size_t, index...>);

        template<typename Arr>
        using is_unique = decltype(is_unique_h2<Arr>(p_i_seq::make_array<size_t, 0, Arr::length>()));

        template<typename... Tp>
        is_unique<p_i_seq::array_t<size_t, sizeof(Tp)...>>
            is_unique_size_h(p_i_seq::t_array_t<Tp...>);

        template<typename Arr>
        using is_unique_size = decltype(is_unique_size_h(Arr()));

        template<bool, typename Arr, typename AT>
        struct add_if_true {
            using type = Arr;
        };

        template<typename AT, typename... Tp>
        p_i_seq::t_array_t<Tp..., AT>
            addType(p_i_seq::t_array_t<Tp...>);

        template<typename Arr, typename AT>
        struct add_if_true<true, Arr, AT> {
            using type = decltype(addType<AT>(Arr()));
        };

        template<typename, typename, typename, size_t...>
        struct fill_if_true_h;

        template<typename Out, typename ArrT, typename ArrB>
        struct fill_if_true_h<Out, ArrT, ArrB> {
            using type = Out;
        };

        template<typename Out, typename ArrT, typename ArrB, size_t i, size_t... index>
        struct fill_if_true_h<Out, ArrT, ArrB, i, index...> {
            using type = typename fill_if_true_h<
                typename add_if_true<ArrB::get(i), Out, typename ArrT::template get<i>>::type,
                ArrT, ArrB, index...>::type;
        };

        template<typename ArrT, typename ArrB, size_t... index>
        typename fill_if_true_h<p_i_seq::t_array_t<>, ArrT, ArrB, index...>::type
            fill_if_true_fh(p_i_seq::array_t<size_t, index...>);

        template<typename ArrT, typename ArrB>
        using fill_if_true = decltype(fill_if_true_fh<ArrT, ArrB>(p_i_seq::make_array<size_t, 0, ArrT::length>()));

        template<typename Arr>
        using fill_if_unique_size = fill_if_true<Arr, is_unique_size<Arr>>;

        template<typename... Tp>
        p_i_seq::array_t<size_t, sizeof(Tp)...>
            sizes_array_h(p_i_seq::t_array_t<Tp...>);

        template<typename Arr>
        using sizes_array = decltype(sizes_array_h(Arr()));

        template<typename... Tp>
        p_i_seq::t_array_t<typename std::make_unsigned<Tp>::type...>
            unsigned_t_array_h(p_i_seq::t_array_t<Tp...>);

        template<typename Arr>
        using unsigned_t_array = decltype(unsigned_t_array_h(Arr()));

        using filtred_ints_t = unsigned_t_array<fill_if_unique_size<ints_t>>;
        using int_sizes_t = sizes_array<filtred_ints_t>;

        template<size_t size, bool sig, p_i_seq::signed_size_t = int_sizes_t::index_of<size>()>
        struct native_int_type_h;

        template<size_t size, bool sig>
        struct native_int_type_h<size, sig, -1 > {
        };

        template<size_t size, p_i_seq::signed_size_t index>
        struct native_int_type_h<size, false, index> {
            using type = typename std::make_unsigned<filtred_ints_t::get<index>>::type;
        };

        template<size_t size, p_i_seq::signed_size_t index>
        struct native_int_type_h<size, true, index> {
            using type = typename std::make_signed<filtred_ints_t::get<index>>::type;
        };

        template<size_t size, bool sig>
        using native_int_type = typename native_int_type_h<size, sig>::type;

        template<typename Type>
        constexpr inline auto max(Type a, Type b) {
            return a > b ? a : b;
        }

        template<typename Arr>
        constexpr inline auto max() {
            auto out = Arr::get(0);
            for (size_t i = 1; i < Arr::length; i++) {
                out = max(out, Arr::get(i));
            }
            return out;
        }

        template<typename T>
        constexpr inline auto min(T a, T b) {
            return a < b ? a : b;
        }

        template<typename Arr>
        constexpr inline auto min() {
            auto out = Arr::get(0);
            for (size_t i = 1; i < Arr::length; i++) {
                out = min(out, Arr::get(i));
            }
            return out;
        }

        template<typename T, typename UT = typename std::make_unsigned<T>::type>
        constexpr inline size_t get_bits() {
            UT test = ~UT(0);
            size_t out = 0;
            while (test) {
                test >>= 1;
                out++;
            }
            return out;
        }

        constexpr static size_t max_native_size = max<int_sizes_t>();
        constexpr static size_t min_native_size = min<int_sizes_t>();

        using max_native_t = filtred_ints_t::get<int_sizes_t::index_of<max_native_size>()>;
        using min_native_t = filtred_ints_t::get<int_sizes_t::index_of<min_native_size>()>;

        constexpr static size_t max_native_bits = get_bits<max_native_t>();
        constexpr static size_t min_native_bits = get_bits<min_native_t>();

        static_assert(min_native_size == 1, "min_native_bits != 1");
        static_assert(min_native_bits == get_bits<char>(), "min_native_bits != char_bits");

        template<size_t... index>
        constexpr inline bool check_bits(p_i_seq::array_t<size_t, index...>) {
            return p_i_seq::all((get_bits<filtred_ints_t::get < index >>() ==
                sizeof(filtred_ints_t::get < index >) * min_native_bits)...);
        }

        static_assert(check_bits(p_i_seq::make_array<size_t, 0, filtred_ints_t::length>()), "check_bits failed");
    }

    namespace p_i_utils {

        constexpr static p_i_seq::v_array_t<char, 62> digits = {
            //10
            '0', '1', '2', '3', '4',
            '5', '6', '7', '8', '9',
            //26
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'g', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            //26
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'G', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
        };

        template<char c>
        constexpr inline size_t indexOfDigit() noexcept {
            auto index = digits.index_of(c);
            return index < 36 ? index : index - 26;
        }

        constexpr inline size_t lpo2_s(size_t value) noexcept {
            size_t i = 0;
            size_t p = 1;
            for (; p <= value && i < p_i_native::get_bits<size_t>(); i++) {
                p <<= 1;
            }
            return i - 1;
        }

        template<size_t... sh, size_t... add>
        p_i_seq::array_t<size_t, (1 << sh)..., add...>
            sh_array_h(p_i_seq::array_t<size_t, sh...>,
                p_i_seq::array_t<size_t, add...>);

        template<typename Arr, size_t bits, bool = isOneBit(bits)>
        struct sh_array_h2 {
            using type = decltype(sh_array_h(Arr(), p_i_seq::array_t<size_t>()));
        };

        template<typename Arr, size_t bits>
        struct sh_array_h2<Arr, bits, false> {
            using type = decltype(sh_array_h(Arr(), p_i_seq::array_t<size_t, bits - (1 << Arr::get(Arr::length - 1)) * 2 >()));
        };

        template<typename T, size_t bits = p_i_native::get_bits<T>()>
        using sh_array = typename sh_array_h2<p_i_seq::make_array <size_t, 0, lpo2_s(bits)>, bits>::type;

        template<typename T, size_t... sh>
        constexpr inline T npo2m1_n_h(T value, p_i_seq::array_t<size_t, sh...>) noexcept {
            p_i_seq::unused_array({ (value |= (value >> sh))... });
            return value;
        }

        template<typename T>
        constexpr inline T npo2m1_n(T value) noexcept {
            return npo2m1_n_h<typename std::make_unsigned < T >::type >(value, sh_array<T>());
        }

        //TODO!

        constexpr inline int bitCount_h(p_i_native::native_int_type<8, false > value) noexcept {
            value -= ((value >> 1) & 0x5555555555555555);
            value = ((value >> 2) & 0x3333333333333333) + (value & 0x3333333333333333);
            return ((((value >> 4) + value) & 0xf0f0f0f0f0f0f0f) * 0x101010101010101) >> 56;
        }

        constexpr inline int bitCount_h(p_i_native::native_int_type<4, false > value) noexcept {
            value -= ((value >> 1) & 0x55555555);
            value = ((value >> 2) & 0x33333333) + (value & 0x33333333);
            return ((((value >> 4) + value) & 0xf0f0f0f) * 0x1010101) >> 24;
        }

        constexpr inline int bitCount_h(p_i_native::native_int_type<2, false > value) noexcept {
            return bitCount_h(p_i_native::native_int_type<4, false >(value));
        }

        constexpr inline int bitCount_h(p_i_native::native_int_type<1, false > value) noexcept {
            return bitCount_h(p_i_native::native_int_type<4, false >(value));
        }

        template<typename T>
        constexpr inline int bitCount_n(T value) noexcept {
            return bitCount_h(p_i_native::native_int_type<sizeof(T), false >(value));
        }

        template<typename T>
        constexpr inline int logb2_n(T value) noexcept {
            return bitCount_n(npo2m1_n(value));
        }

        template<typename T>
        constexpr inline int numberOfLeadingZeros_n(T value) noexcept {
            return sizeof(T) * p_i_native::min_native_bits - logb2_n(value);
        }

        template<typename T>
        constexpr inline int numberOfTrailingZeros_n(T value) noexcept {
            return bitCount_n((value & -value) - 1);
        }

        constexpr inline size_t make_pow2(size_t i) noexcept {
            if (i == 0) {
                return 0;
            }
            size_t log = logb2_n(i);
            if (i == (size_t(1) << (log - 1))) {
                return i;
            }
            return static_cast<size_t>(static_cast<size_t>(1)) << log;
        }

        template<typename T, bool sig>
        constexpr inline T min_int_value() noexcept {
            return sig ? T(1) << (sizeof(T) * p_i_native::min_native_bits - 1) : T(0);
        }

        template<typename T, bool sig>
        constexpr inline T max_int_value() noexcept {
            return ~min_int_value<T, sig>();
        }

        template<typename T, size_t length>
        using array_ref = T(&)[length];
    }

    enum p_IType {
        illegal = 0,
        native,
        pow2,
        array
    };

    constexpr inline p_IType p_intType(size_t size) noexcept {
        if (!size) {
            return illegal;
        }
        if (p_i_native::int_sizes_t().index_of(size) != -1) {
            return native;
        }
        return (size % p_i_native::max_native_size == 0) &&
            isOneBit(size / p_i_native::max_native_size) ? pow2 : array;
    }

    template<size_t size, bool = (size > sizeof(unsigned int))>
                struct p_SHValue {
                    typedef unsigned int type;
                };

                template<size_t size>
                struct p_SHValue<size, true> {
                    typedef size_t type;
                };

                template<size_t size>
                using p_SHType = typename p_SHValue<size>::type;

                template<size_t size, bool sig>
                class Integer;

                template<size_t size, bool sig, p_IType = p_intType(size)>
                struct p_Integer_Impl;

                template<size_t size, bool sig>
                struct p_native_Integer_Impl;

                template<size_t size, bool sig>
                struct p_Integer_Impl <size, sig, native> {
                    typedef p_native_Integer_Impl<size, sig> type;
                };

                template<size_t size, bool sig>
                struct p_native_Integer_Base;

                template<size_t size>
                struct p_native_Integer_Base<size, false> {
                    typedef p_i_native::native_int_type<size, true> S;
                    typedef p_i_native::native_int_type<size, false> U;
                    typedef p_native_Integer_Base I;
                    typedef p_SHType<sizeof(U) > M;
                    constexpr static M shmask = sizeof(U) * 8 - 1;
                    U value;

                    constexpr inline p_native_Integer_Base() noexcept = default;

                    constexpr explicit inline
                        p_native_Integer_Base(const U n) noexcept : value(n) { }

                    constexpr inline bool isNegative() const noexcept {
                        return false;
                    }

                    constexpr inline static bool increment_overflow(I& value) noexcept {
                        return !(++value.value);
                    }

                    constexpr inline static bool decrement_overflow(I& value) noexcept {
                        return !(value.value--);
                    }

                    constexpr inline static bool
                        increment_overflow_carry(I& value, bool cf) noexcept {
                        return (value.value += 1 + cf) <= cf;
                    }

                    constexpr inline static bool
                        decrement_overflow_carry(I& value, bool cf) noexcept {
                        bool tmp = value.value <= cf;
                        value.value -= 1 + cf;
                        return tmp;
                    }

                    constexpr inline static bool
                        add_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return (out.value = v1.value + v2.value) < v1.value;
                    }

                    constexpr inline static bool
                        sub_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return (out.value = v1.value - v2.value) > v1.value;
                    }

                    constexpr inline static bool
                        add_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        bool tmp = add_overflow(v1, v2, out);
                        return add_overflow(I(out), I(cf), out) | tmp;
                    }

                    constexpr inline static bool
                        sub_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        bool tmp = sub_overflow(v1, v2, out);
                        return sub_overflow(I(out), I(cf), out) | tmp;
                    }

                    constexpr inline static bool
                        add_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return add_overflow(v1, I(cf), out);
                    }

                    constexpr inline static bool
                        sub_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return sub_overflow(v1, I(cf), out);
                    }

                    constexpr inline I operator/(const I& other) const noexcept {
                        return I(value / other.value);
                    }

                    constexpr inline I operator%(const I& other) const noexcept {
                        return I(value % other.value);
                    }

                    constexpr inline I operator>>(const M other) const noexcept {
                        return I(value >> (other & shmask));
                    }

                    constexpr inline bool operator>(const I& other) const noexcept {
                        return value > other.value;
                    }

                    constexpr inline bool operator<(const I& other) const noexcept {
                        return value < other.value;
                    }

                    constexpr inline bool operator>=(const I& other) const noexcept {
                        return value >= other.value;
                    }

                    constexpr inline bool operator<=(const I& other) const noexcept {
                        return value <= other.value;
                    }
                };

                template<size_t size>
                struct p_native_Integer_Base<size, true> {
                    typedef p_i_native::native_int_type<size, true> S;
                    typedef p_i_native::native_int_type<size, false> U;
                    typedef p_native_Integer_Base I;
                    typedef p_SHType<sizeof(U) > M;
                    constexpr static M shmask = sizeof(U) * 8 - 1;
                    U value;

                    constexpr inline p_native_Integer_Base() noexcept = default;

                    constexpr explicit inline
                        p_native_Integer_Base(const S n) noexcept : value(n) { }

                    constexpr inline bool isNegative() const noexcept {
                        return S(value) < 0;
                    }

                    template<size_t size2, size_t = size2>
                    struct divrem_h {

                        constexpr inline static I div(const I& a, const I& b) noexcept {
                            return I(S(a.value) / S(b.value));
                        }

                        constexpr inline static I rem(const I& a, const I& b) noexcept {
                            return I(S(a.value) % S(b.value));
                        }
                    };

                    template<size_t size2>
                    struct divrem_h<size2, 1> {

                        constexpr inline static I div(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x80)) && (b.value == U(-1))) {
                                return I(0x80);
                            }
                            return divrem_h<size2, 0>::div(a, b);
                        }

                        constexpr inline static I rem(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x80)) && (b.value == U(-1))) {
                                return I(0);
                            }
                            return divrem_h<size2, 0>::rem(a, b);
                        }
                    };

                    template<size_t size2>
                    struct divrem_h<size2, 2> {

                        constexpr inline static I div(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x8000)) && (b.value == U(-1))) {
                                return I(0x8000);
                            }
                            return divrem_h<size2, 0>::div(a, b);
                        }

                        constexpr inline static I rem(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x8000)) && (b.value == U(-1))) {
                                return I(0);
                            }
                            return divrem_h<size2, 0>::rem(a, b);
                        }
                    };

                    template<size_t size2>
                    struct divrem_h<size2, 4> {

                        constexpr inline static I div(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x80000000L)) && (b.value == U(-1L))) {
                                return I(0x80000000L);
                            }
                            return divrem_h<size2, 0>::div(a, b);
                        }

                        constexpr inline static I rem(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x80000000L)) && (b.value == U(-1L))) {
                                return I(0);
                            }
                            return divrem_h<size2, 0>::rem(a, b);
                        }
                    };

                    template<size_t size2>
                    struct divrem_h<size2, 8> {

                        constexpr inline static I div(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x8000000000000000LL)) && (b.value == U(-1LL))) {
                                return I(0x8000000000000000LL);
                            }
                            return divrem_h<size2, 0>::div(a, b);
                        }

                        constexpr inline static I rem(const I& a, const I& b) noexcept {
                            if ((a.value == U(0x8000000000000000LL)) && (b.value == U(-1LL))) {
                                return I(0);
                            }
                            return divrem_h<size2, 0>::rem(a, b);
                        }
                    };

                    constexpr inline I operator/(const I& other) const noexcept {
                        return divrem_h<size>::div(*this, other);
                    }

                    constexpr inline I operator%(const I& other) const noexcept {
                        return divrem_h<size>::rem(*this, other);
                    }

                    constexpr inline I operator>>(const M other) const noexcept {
                        return I(S(value) >> (other & shmask));
                    }

                    constexpr inline bool operator>(const I& other) const noexcept {
                        return S(value) > S(other.value);
                    }

                    constexpr inline bool operator<(const I& other) const noexcept {
                        return S(value) < S(other.value);
                    }

                    constexpr inline bool operator>=(const I& other) const noexcept {
                        return S(value) >= S(other.value);
                    }

                    constexpr inline bool operator<=(const I& other) const noexcept {
                        return S(value) <= S(other.value);
                    }
                };

                template<size_t size, bool sig>
                struct p_native_Integer_Impl : public p_native_Integer_Base<size, sig> {
                    typedef p_native_Integer_Base<size, sig> T;
                    typedef p_native_Integer_Impl<size, sig> I;
                    typedef p_native_Integer_Impl<size, false> UI;
                    typedef p_native_Integer_Impl<size, true> SI;
                    typedef typename T::U U;
                    typedef typename T::S S;
                    typedef typename T::M M;

                    using T::T;

                    constexpr inline p_native_Integer_Impl() noexcept = default;

                    constexpr inline p_native_Integer_Impl(const T& obj) noexcept : T(obj) { }

                    constexpr inline static I ZERO() noexcept {
                        return T(0);
                    }

                    constexpr inline bool isZero() const noexcept {
                        return T::value == 0;
                    }

                    constexpr inline bool upperBit() const noexcept {
                        return S(T::value) < 0;
                    }

                    constexpr inline I addOne() const noexcept {
                        return I(T::value + 1);
                    }

                    constexpr inline I subOne() const noexcept {
                        return I(T::value - 1);
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<p_i_native::min_native_t, (index < size) >
                        getByte() const noexcept {
                        return T::value >> (index * p_i_native::min_native_bits);
                    }

                    template<size_t index, U mask = U(~(U(p_i_native::min_native_t(~0)) <<
                        (index * p_i_native::min_native_bits)))>
                        constexpr inline ct::if_t<void, (index < size) >
                        setByte(p_i_native::min_native_t v) noexcept {
                        T::value = (T::value & mask) | (U(v) << index * p_i_native::min_native_bits);
                    }

                    template<size_t index, U mask = U(U(1) << index)>
                    constexpr inline ct::if_t<bool, (index < size* p_i_native::min_native_bits) >
                        getBit() const noexcept {
                        return T::value & mask;
                    }

                    template<size_t index, U mask1 = U(U(1) << index), U mask2 = U(~mask1)>
                    constexpr inline ct::if_t<void, (index < size* p_i_native::min_native_bits) >
                        setBit(bool v) noexcept {
                        T::value = v ? (T::value | mask1) : (T::value & mask2);
                    }

                    constexpr inline static bool increment_overflow(I& value) noexcept {
                        return T::increment_overflow(value);
                    }

                    constexpr inline static bool decrement_overflow(I& value) noexcept {
                        return T::decrement_overflow(value);
                    }

                    constexpr inline static bool
                        increment_overflow_carry(I& value, bool cf) noexcept {
                        return T::increment_overflow_carry(value, cf);
                    }

                    constexpr inline static bool
                        decrement_overflow_carry(I& value, bool cf) noexcept {
                        return T::decrement_overflow_carry(value, cf);
                    }

                    constexpr inline static void
                        increment_carry(I& value, bool cf) noexcept {
                        value.value += 1 + cf;
                    }

                    constexpr inline static void
                        decrement_carry(I& value, bool cf) noexcept {
                        value.value -= 1 + cf;
                    }

                    constexpr inline static bool
                        add_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return T::add_overflow(v1, v2, out);
                    }

                    constexpr inline static bool
                        sub_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return T::sub_overflow(v1, v2, out);
                    }

                    constexpr inline static bool
                        add_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        return T::add_overflow_carry(v1, v2, cf, out);
                    }

                    constexpr inline static bool
                        sub_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        return T::sub_overflow_carry(v1, v2, cf, out);
                    }

                    constexpr inline static bool
                        add_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return T::add_zero_overflow_carry(v1, cf, out);
                    }

                    constexpr inline static bool
                        sub_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return T::sub_zero_overflow_carry(v1, cf, out);
                    }

                    constexpr inline static void
                        add_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        out.value = v1.value + v2.value + cf;
                    }

                    constexpr inline static void
                        sub_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        out.value = v1.value - v2.value - cf;
                    }

                    constexpr inline static void
                        add_zero_carry(const I& v1, bool cf, I& out) noexcept {
                        out.value = v1.value + cf;
                    }

                    constexpr inline static void
                        sub_zero_carry(const I& v1, bool cf, I& out) noexcept {
                        out.value = v1.value - cf;
                    }

                    constexpr inline static bool
                        leftShiftOneBit(I& value, bool bit) noexcept {
                        bool tmp = value.upperBit();
                        value.value = (value.value << 1) | U(bit);
                        return tmp;
                    }

                    constexpr inline size_t numberOfLeadingZeros() const noexcept {
                        return p_i_utils::numberOfLeadingZeros_n(T::value);
                    }

                    constexpr inline size_t numberOfTrailingZeros() const noexcept {
                        return p_i_utils::numberOfTrailingZeros_n(T::value);
                    }

                    constexpr inline I operator+() const noexcept {
                        return *this;
                    }

                    constexpr inline I operator-() const noexcept {
                        return I(-T::value);
                    }

                    constexpr inline I operator+(const I& other) const noexcept {
                        return I(T::value + other.value);
                    }

                    constexpr inline I operator-(const I& other) const noexcept {
                        return I(T::value - other.value);
                    }

                    constexpr inline I operator*(const I& other) const noexcept {
                        return I(T::value * other.value);
                    }

                    constexpr inline I operator|(const I& other) const noexcept {
                        return I(T::value | other.value);
                    }

                    constexpr inline I operator&(const I& other) const noexcept {
                        return I(T::value & other.value);
                    }

                    constexpr inline I operator^(const I& other) const noexcept {
                        return I(T::value ^ other.value);
                    }

                    constexpr inline I operator<<(const M other) const noexcept {
                        return I(T::value << (other & T::shmask));
                    }

                    constexpr inline bool operator==(const I& other) const noexcept {
                        return T::value == other.value;
                    }

                    constexpr inline bool operator!=(const I& other) const noexcept {
                        return T::value != other.value;
                    }

                    constexpr inline I operator~() const noexcept {
                        return I(~T::value);
                    }
                };

                template<size_t half, bool sig>
                struct p_pow2_Integer_Impl;

                template<size_t size, bool sig>
                struct p_Integer_Impl <size, sig, pow2> {
                    typedef p_pow2_Integer_Impl<size / 2, sig> type;
                };

                template<size_t half, bool sig>
                struct p_pow2_Integer_Base;

                template<size_t half>
                struct p_pow2_Integer_Base<half, false> {
                    typedef Integer<half, true> S;
                    typedef Integer<half, false> U;
                    typedef p_pow2_Integer_Base<half, false> I;
                    typedef p_SHType<half * 2> M;
                    constexpr static M shmask = half * 2 * 8 - 1;
                    U low, high;

                    constexpr inline static I
                        rightShift(const I& value, const M shiftDistance) noexcept {
                        if (shiftDistance == 0) {
                            return value;
                        }
                        if (shiftDistance < (half * 8)) {
                            return I((value.low >> shiftDistance) |
                                (value.high << (half * 8 - shiftDistance)),
                                value.high >> shiftDistance);
                        }
                        return I(value.high >> (shiftDistance - half * 8), U::ZERO());
                    }

                    constexpr inline p_pow2_Integer_Base() noexcept = default;

                    constexpr explicit inline p_pow2_Integer_Base(const U& low) noexcept :
                        low(low), high(U::ZERO()) { }

                    constexpr explicit inline p_pow2_Integer_Base(const S& low) noexcept :
                        low(low), high(low.upperBit() ? ~U::ZERO() : U::ZERO()) { }

                    constexpr inline p_pow2_Integer_Base(const U& low, const U& high)
                        noexcept : low(low), high(high) { }

                    constexpr inline bool isNegative() const noexcept {
                        return false;
                    }

                    constexpr inline static bool increment_overflow(I& value) noexcept {
                        return U::add_zero_overflow_carry(U(value.high),
                            U::increment_overflow(value.low), value.high);
                    }

                    constexpr inline static bool decrement_overflow(I& value) noexcept {
                        return U::sub_zero_overflow_carry(U(value.high),
                            U::decrement_overflow(value.low), value.high);
                    }

                    constexpr inline static bool
                        increment_overflow_carry(I& value, bool cf) noexcept {
                        return U::add_overflow_carry(U(value.high), U::ZERO(),
                            U::increment_overflow_carry(value.low, cf), value.high);
                    }

                    constexpr inline static bool
                        decrement_overflow_carry(I& value, bool cf) noexcept {
                        return U::sub_overflow_carry(U(value.high), U::ZERO(),
                            U::decrement_overflow_carry(value.low, cf), value.high);
                    }

                    constexpr inline static bool
                        add_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return U::add_overflow_carry(v1.high, v2.high,
                            U::add_overflow(v1.low, v2.low, out.low), out.high);
                    }

                    constexpr inline static bool
                        sub_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return U::sub_overflow_carry(v1.high, v2.high,
                            U::sub_overflow(v1.low, v2.low, out.low), out.high);
                    }

                    constexpr inline static bool
                        add_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        return U::add_overflow_carry(v1.high, v2.high,
                            U::add_overflow_carry(v1.low, v2.low, cf, out.low),
                            out.high);
                    }

                    constexpr inline static bool
                        sub_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        return U::sub_overflow_carry(v1.high, v2.high,
                            U::sub_overflow_carry(v1.low, v2.low, cf, out.low),
                            out.high);
                    }

                    constexpr inline static bool
                        add_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return U::add_zero_overflow_carry(v1.high,
                            U::add_zero_overflow_carry(v1.low, cf, out.low), out.high);
                    }

                    constexpr inline static bool
                        sub_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return U::sub_zero_overflow_carry(v1.high,
                            U::sub_zero_overflow_carry(v1.low, cf, out.low), out.high);
                    }

                    constexpr inline bool operator>(const I& other) const noexcept {
                        if (high == other.high) {
                            return low > other.low;
                        }
                        return high > other.high;
                    }

                    constexpr inline bool operator<(const I& other) const noexcept {
                        if (high == other.high) {
                            return low < other.low;
                        }
                        return high < other.high;
                    }

                    constexpr inline bool operator>=(const I& other) const noexcept {
                        if (high == other.high) {
                            return low >= other.low;
                        }
                        return high > other.high;
                    }

                    constexpr inline bool operator<=(const I& other) const noexcept {
                        if (high == other.high) {
                            return low <= other.low;
                        }
                        return high < other.high;
                    }

                    constexpr inline I operator>>(const M other) const noexcept {
                        return rightShift(*this, other & shmask);
                    }
                };

                template<size_t half>
                struct p_pow2_Integer_Base<half, true> {
                    typedef Integer<half, true> S;
                    typedef Integer<half, false> U;
                    typedef p_pow2_Integer_Base<half, true> I;
                    typedef p_SHType<half * 2> M;
                    constexpr static M shmask = half * 2 * 8 - 1;
                    U low, high;

                    constexpr inline static I
                        rightShift(const I& value, const M shiftDistance) noexcept {
                        if (shiftDistance == 0) {
                            return value;
                        }
                        if (shiftDistance < (half * 8)) {
                            return I((value.low >> shiftDistance) |
                                (value.high << (half * 8 - shiftDistance)),
                                S(value.high) >> shiftDistance);
                        }
                        return I(S(value.high) >> (shiftDistance - half * 8),
                            value.high.upperBit() ? ~U::ZERO() : U::ZERO());
                    }

                    constexpr inline p_pow2_Integer_Base() noexcept = default;

                    constexpr explicit inline p_pow2_Integer_Base(const U& low) noexcept :
                        low(low), high(U::ZERO()) { }

                    constexpr explicit inline p_pow2_Integer_Base(const S& low) noexcept :
                        low(low), high(low.isNegative() ? ~U::ZERO() : U::ZERO()) { }

                    constexpr inline p_pow2_Integer_Base(const U& low, const U& high)
                        noexcept : low(low), high(high) { }

                    constexpr inline bool isNegative() const noexcept {
                        return high.upperBit();
                    }

                    constexpr inline bool operator>(const I& other) const noexcept {
                        if (high == other.high) {
                            return low > other.low;
                        }
                        return S(high) > S(other.high);
                    }

                    constexpr inline bool operator<(const I& other) const noexcept {
                        if (high == other.high) {
                            return low < other.low;
                        }
                        return S(high) < S(other.high);
                    }

                    constexpr inline bool operator>=(const I& other) const noexcept {
                        if (high == other.high) {
                            return low >= other.low;
                        }
                        return S(high) > S(other.high);
                    }

                    constexpr inline bool operator<=(const I& other) const noexcept {
                        if (high == other.high) {
                            return low <= other.low;
                        }
                        return S(high) < S(other.high);
                    }

                    constexpr inline I operator>>(const M other) const noexcept {
                        return rightShift(*this, other & shmask);
                    }
                };

                template<size_t half, bool sig>
                struct p_pow2_Integer_Impl : public p_pow2_Integer_Base<half, sig> {
                    typedef p_pow2_Integer_Base<half, sig> T;
                    typedef p_pow2_Integer_Impl<half, sig> I;
                    typedef p_pow2_Integer_Impl<half, false> UI;
                    typedef p_pow2_Integer_Impl<half, true> SI;
                    typedef typename T::S S;
                    typedef typename T::U U;
                    typedef typename T::M M;

                    constexpr inline static I
                        leftShift(const I& value, const typename T::M shiftDistance) noexcept {
                        if (shiftDistance == 0) {
                            return value;
                        }
                        if (shiftDistance < (half * 8)) {
                            return I(value.low << shiftDistance,
                                (value.high << shiftDistance) |
                                (value.low >> (half * 8 - shiftDistance)));
                        }
                        return I(U::ZERO(), value.low << (shiftDistance - half * 8));
                    }

                    using T::T;

                    constexpr inline p_pow2_Integer_Impl() noexcept = default;

                    template<bool sig2>
                    constexpr inline
                        p_pow2_Integer_Impl(const p_pow2_Integer_Impl<half, sig2>& other)
                        noexcept : T(other.low, other.high) { }

                    constexpr inline p_pow2_Integer_Impl(const T& obj) noexcept : T(obj) { }

                    constexpr inline SI s() const noexcept {
                        return SI(T::low, T::high);
                    }

                    constexpr inline UI u() const noexcept {
                        return UI(T::low, T::high);
                    }

                    constexpr inline static I ZERO() noexcept {
                        return I(U::ZERO());
                    }

                    constexpr inline bool isZero() const noexcept {
                        return T::low.isZero() && T::high.isZero();
                    }

                    constexpr inline bool upperBit() const noexcept {
                        return T::high.upperBit();
                    }

                    constexpr inline I addOne() const noexcept {
                        I tmp = *this;
                        U::add_zero_carry(U(tmp.high),
                            U::increment_overflow(tmp.low), tmp.high);
                        return tmp;
                    }

                    constexpr inline I subOne() const noexcept {
                        I tmp = *this;
                        U::sub_zero_carry(U(tmp.high),
                            U::decrement_overflow(tmp.low), tmp.high);
                        return tmp;
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<p_i_native::min_native_t, (index < half) >
                        getByte() const noexcept {
                        return T::low.template getByte<index>();
                    }

                    template<size_t index>
                    constexpr inline
                        ct::if_t<p_i_native::min_native_t, (index >= half) && (index < half * 2) >
                        getByte() const noexcept {
                        return T::high.template getByte < index - half >();
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<void, (index < half) >
                        setByte(p_i_native::min_native_t v) noexcept {
                        T::low.template setByte<index>(v);
                    }

                    template<size_t index>
                    constexpr inline
                        ct::if_t<void, (index >= half) && (index < half * 2) >
                        setByte(p_i_native::min_native_t v) noexcept {
                        T::high.template setByte < index - half >(v);
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<bool, (index < half* p_i_native::min_native_bits) >
                        getBit() const noexcept {
                        return T::low.template getBit<index>();
                    }

                    template<size_t index>
                    constexpr inline
                        ct::if_t<bool, (index >= half * p_i_native::min_native_bits) &&
                        (index < half * 2 * p_i_native::min_native_bits) >
                        getBit() const noexcept {
                        return T::high.template getBit < index - half * p_i_native::min_native_bits >();
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<void, (index < half* p_i_native::min_native_bits) >
                        setBit(bool v) noexcept {
                        T::low.template setBit<index>(v);
                    }

                    template<size_t index>
                    constexpr inline
                        ct::if_t<void, (index >= half * p_i_native::min_native_bits) &&
                        (index < half * 2 * p_i_native::min_native_bits) >
                        setBit(bool v) noexcept {
                        T::high.template setBit < index - half * p_i_native::min_native_bits >(v);
                    }

                    constexpr inline static bool increment_overflow(I& value) noexcept {
                        return T::increment_overflow(value);
                    }

                    constexpr inline static bool decrement_overflow(I& value) noexcept {
                        return T::decrement_overflow(value);
                    }

                    constexpr inline static bool
                        increment_overflow_carry(I& value, bool cf) noexcept {
                        return T::increment_overflow_carry(value, cf);
                    }

                    constexpr inline static bool
                        decrement_overflow_carry(I& value, bool cf) noexcept {
                        return T::decrement_overflow_carry(value, cf);
                    }

                    constexpr inline static void
                        increment_carry(I& value, bool cf) noexcept {
                        U::add_zero_carry(U(value.high),
                            U::increment_overflow_carry(value.low, cf), value.high);
                    }

                    constexpr inline static void
                        decrement_carry(I& value, bool cf) noexcept {
                        U::sub_zero_carry(U(value.high),
                            U::decrement_overflow_carry(value.low, cf), value.high);
                    }

                    constexpr inline static bool
                        add_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return T::add_overflow(v1, v2, out);
                    }

                    constexpr inline static bool
                        sub_overflow(const I& v1, const I& v2, I& out) noexcept {
                        return T::sub_overflow(v1, v2, out);
                    }

                    constexpr inline static bool
                        add_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        return T::add_overflow_carry(v1, v2, cf, out);
                    }

                    constexpr inline static bool
                        sub_overflow_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        return T::sub_overflow_carry(v1, v2, cf, out);
                    }

                    constexpr inline static bool
                        add_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return T::add_zero_overflow_carry(v1, cf, out);
                    }

                    constexpr inline static bool
                        sub_zero_overflow_carry(const I& v1, bool cf, I& out) noexcept {
                        return T::sub_zero_overflow_carry(v1, cf, out);
                    }

                    constexpr inline static void
                        add_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        U::add_carry(v1.high, v2.high,
                            U::add_overflow_carry(v1.low, v2.low, cf, out.low),
                            out.high);
                    }

                    constexpr inline static void
                        sub_carry(const I& v1, const I& v2, bool cf, I& out) noexcept {
                        U::sub_carry(v1.high, v2.high,
                            U::sub_overflow_carry(v1.low, v2.low, cf, out.low),
                            out.high);
                    }

                    constexpr inline static void
                        add_zero_carry(const I& v1, bool cf, I& out) noexcept {
                        U::add_zero_carry(v1.high,
                            U::add_zero_overflow_carry(v1.low, cf, out.low), out.high);
                    }

                    constexpr inline static void
                        sub_zero_carry(const I& v1, bool cf, I& out) noexcept {
                        U::sub_zero_carry(v1.high,
                            U::sub_zero_overflow_carry(v1.low, cf, out.low), out.high);
                    }

                    constexpr inline static bool
                        leftShiftOneBit(I& value, bool bit) noexcept {
                        return U::leftShiftOneBit(value.high,
                            U::leftShiftOneBit(value.low, bit));
                    }

                    constexpr inline size_t numberOfLeadingZeros() const noexcept {
                        size_t hz = T::high.isZero();
                        U tmp = hz ? T::low : T::high;
                        return tmp.numberOfLeadingZeros() + half * 8 * hz;
                    }

                    constexpr inline size_t numberOfTrailingZeros() const noexcept {
                        size_t lz = T::low.isZero();
                        U tmp = lz ? T::high : T::low;
                        return tmp.numberOfTrailingZeros() + half * 8 * lz;
                    }

                    constexpr inline I operator+() const noexcept {
                        return *this;
                    }

                    constexpr inline I operator-() const noexcept {
                        return (~(*this)).addOne();
                    }

                    template<typename II = Integer<half * 2, sig>>
                    constexpr inline I operator/(const I& other) const noexcept {
                        return divrem(II(*this), II(other)).value;
                    }

                    template<typename II = Integer<half * 2, sig>>
                    constexpr inline I operator%(const I& other) const noexcept {
                        II rem;
                        divrem(II(*this), II(other), &rem);
                        return rem.value;
                    }

                    constexpr inline I operator+(const I& other) const noexcept {
                        I tmp = I::ZERO();
                        U::add_carry(T::high, other.high,
                            U::add_overflow(T::low, other.low, tmp.low), tmp.high);
                        return tmp;
                    }

                    constexpr inline I operator-(const I& other) const noexcept {
                        I tmp = I::ZERO();
                        U::sub_carry(T::high, other.high,
                            U::sub_overflow(T::low, other.low, tmp.low), tmp.high);
                        return tmp;
                    }

                    constexpr inline I operator*(const I& other) const noexcept {
                        auto tmp = wmultiply(T::low, other.low).value;
                        return I(tmp.low, tmp.high + T::low * other.high + T::high * other.low);
                    }

                    constexpr inline I operator<<(const M other) const noexcept {
                        return leftShift(*this, other & T::shmask);
                    }

                    constexpr inline I operator|(const I& other) const noexcept {
                        return I(T::low | other.low, T::high | other.high);
                    }

                    constexpr inline I operator&(const I& other) const noexcept {
                        return I(T::low & other.low, T::high & other.high);
                    }

                    constexpr inline I operator^(const I& other) const noexcept {
                        return I(T::low ^ other.low, T::high ^ other.high);
                    }

                    constexpr inline I operator~() const noexcept {
                        return I(~T::low, ~T::high);
                    }

                    constexpr inline bool operator==(const I& other) const noexcept {
                        return (T::low == other.low) && (T::high == other.high);
                    }

                    constexpr inline bool operator!=(const I& other) const noexcept {
                        return (T::low != other.low) || (T::high != other.high);
                    }
                };

                namespace ct {

                    template<typename T1, typename T2>
                    constexpr inline bool is_same_cv() noexcept {
                        return std::is_same<typename std::remove_cv<T1>::type,
                            typename std::remove_cv<T2>::type>::value;
                    }

                    template<typename T>
                    constexpr inline bool is_integral_no_bool() noexcept {
                        return std::is_integral<T>::value && (!is_same_cv<T, bool>());
                    }

                    template<typename R, typename T, bool V = true >
                    using if_int_t = if_t<R, is_integral_no_bool<T>() && V>;

                    template<typename R, typename T, bool V = true >
                    using if_bool_t = if_t<R, is_same_cv<T, bool>() && V>;

                    template<typename T>
                    constexpr inline bool is_signed() noexcept {
                        return T(-1) < T(0);
                    }
                }

                template<typename U, typename S, bool sig>
                struct p_US {
                    typedef U type;
                };

                template<typename U, typename S>
                struct p_US <U, S, true> {
                    typedef S type;
                };

                template<typename U, typename S, bool sig>
                using p_US_t = typename p_US<U, S, sig>::type;

                template<size_t size, typename UI = Integer<size, false>,
                    typename Uh = Integer<size / 2, false >>
                    constexpr inline ct::if_t<UI, p_intType(size) == pow2>
                    p_udivrem(const Integer<size, false>& a, const Integer<size, false>& b,
                        Integer<size, false>* rem) noexcept {
                    constexpr size_t n_word_bits = size * p_i_native::min_native_bits;
                    constexpr size_t n_half_bits = size * p_i_native::min_native_bits / 2;
                    UI q = UI::ZERO();
                    UI r = UI::ZERO();
                    size_t sr = 0;
                    // special cases, X is unknown, K != 0
                    if (a.uhigh().isZero()) {
                        if (b.uhigh().isZero()) {
                            // 0 X
                            // ---
                            // 0 X
                            if (rem) {
                                rem->uhigh() = Uh::ZERO();
                                return divrem(a.ulow(), b.ulow(), &(rem->ulow()));
                            }
                            return a.ulow() / b.ulow();
                        }
                        // 0 X
                        // ---
                        // K X
                        if (rem)
                            *rem = a.ulow();
                        return UI::ZERO();
                    }
                    // a.uhigh() != 0
                    if (b.ulow().isZero()) {
                        if (b.uhigh().isZero()) {
                            // K X
                            // ---
                            // 0 0
                            if (rem) {
                                *rem = a.uhigh() % b.ulow();
                            }
                            return a.uhigh() / b.ulow();
                        }
                        // b.uhigh() != 0
                        if (a.ulow().isZero()) {
                            // K 0
                            // ---
                            // K 0
                            if (rem) {
                                rem->ulow() = Uh::ZERO();
                                return divrem(a.uhigh(), b.uhigh(), &(rem->uhigh()));
                            }
                            return a.uhigh() / b.uhigh();
                        }
                        // K K
                        // ---
                        // K 0
                        if (isOneBit(b.uhigh())) /* if b is a power of 2 */ {
                            if (rem) {
                                r.ulow() = a.ulow();
                                r.uhigh() = a.uhigh() & (b.uhigh() - 1);
                                *rem = r;
                            }
                            return a.uhigh() >> b.uhigh().numberOfTrailingZeros();
                        }
                        // K K
                        // ---
                        // K 0
                        sr = b.uhigh().numberOfLeadingZeros() - a.uhigh().numberOfLeadingZeros();
                        // 0 <= sr <= n_half_bits - 2 or sr large
                        if (sr > n_half_bits - 2) {
                            if (rem) {
                                *rem = a;
                            }
                            return UI::ZERO();
                        }
                        ++sr;
                        // 1 <= sr <= n_half_bits - 1
                        // q = a << (n_word_bits - sr);
                        q.ulow() = Uh::ZERO();
                        q.uhigh() = a.ulow() << (n_half_bits - sr);

                        r = a >> sr;
                    }
                    else /* b.ulow() != 0 */ {
                        if (b.uhigh().isZero()) {
                            // K X
                            // ---
                            // 0 K
                            if (isOneBit(b.ulow())) /* if b is a power of 2 */ {
                                if (rem) {
                                    *rem = a.ulow() & (b.ulow() - 1);
                                }
                                //TODO
                                if (b.ulow() == 1) {
                                    return a;
                                }
                                sr = b.ulow().numberOfTrailingZeros();
                                return a >> sr;
                            }
                            // K X
                            // ---
                            // 0 K
                            sr = 1 + n_half_bits + b.ulow().numberOfLeadingZeros() -
                                a.uhigh().numberOfLeadingZeros();
                            // 2 <= sr <= n_word_bits - 1
                            // q = a << (n_word_bits - sr);
                            // r = a >> sr;
                            if (sr == n_half_bits) {
                                q.ulow() = Uh::ZERO();
                                q.uhigh() = a.ulow();
                                r.uhigh() = Uh::ZERO();
                                r.ulow() = a.uhigh();
                            }
                            else if (sr < n_half_bits) /* 2 <= sr <= n_half_bits - 1 */ {
                                q.ulow() = Uh::ZERO();
                                q.uhigh() = a.ulow() << (n_half_bits - sr);
                                r = a >> sr;
                            }
                            else /* n_half_bits + 1 <= sr <= n_word_bits - 1 */ {
                                q = a << (n_word_bits - sr);
                                r.uhigh() = Uh::ZERO();
                                r.ulow() = a.uhigh() >> (sr - n_half_bits);
                            }
                        }
                        else {
                            // K X
                            // ---
                            // K K
                            sr = b.uhigh().numberOfLeadingZeros() - a.uhigh().numberOfLeadingZeros();
                            // 0 <= sr <= n_half_bits - 1 or sr large
                            if (sr > n_half_bits - 1) {
                                if (rem)
                                    *rem = a;
                                return UI::ZERO();
                            }
                            ++sr;
                            // 1 <= sr <= n_half_bits
                            // q = a << (n_word_bits - sr);
                            // r = a >> sr;
                            q.ulow() = Uh::ZERO();
                            if (sr == n_half_bits) {
                                q.uhigh() = a.ulow();
                                r.uhigh() = Uh::ZERO();
                                r.ulow() = a.uhigh();
                            }
                            else {
                                r = a >> sr;
                                q.uhigh() = a.ulow() << (n_half_bits - sr);
                            }
                        }
                    }
                    // Not a special case
                    // q and r are initialized with:
                    // q = a << (n_word_bits - sr);
                    // r = a >> sr;
                    // 1 <= sr <= n_word_bits - 1
                    bool carry = false;
                    for (; sr > 0; --sr) {
                        // r:q = ((r:q) << 1) | carry
                        UI::leftShiftOneBit(r, UI::leftShiftOneBit(q, carry));

                        carry = false;
                        if (r >= b) {
                            r -= b;
                            carry = true;
                        }
                    }
                    UI::leftShiftOneBit(q, carry);
                    if (rem) {
                        *rem = r;
                    }
                    return q;
                }

                template<size_t size, typename UI = Integer<size, false>,
                    typename SI = Integer<size, true> >
                    constexpr inline ct::if_t<SI, p_intType(size) == pow2>
                    p_sdivrem(const Integer<size, true>& x,
                        const Integer<size, true>& y, Integer<size, true>* rem) noexcept {
                    bool xNeg = x.isNegative();
                    bool yNeg = y.isNegative();
                    UI q;
                    if (rem) {
                        UI r;
                        q = p_udivrem<size>(xNeg ? -x : x, yNeg ? -y : y, &r);
                        *rem = xNeg ? -r : r;
                    }
                    else {
                        q = p_udivrem<size>(xNeg ? -x : x, yNeg ? -y : y, nullptr);
                    }
                    return xNeg ^ yNeg ? -q : q;
                }

                template<size_t size, bool sig, p_IType = p_intType(size)>
                struct p_divrem_h;

                template<size_t size, bool sig>
                struct p_divrem_h<size, sig, native> {

                    template<typename I = Integer<size, sig>>
                    constexpr inline static I divrem(
                        const I& x, const I& y, I* rem) noexcept {
                        if (rem) {
                            *rem = x % y;
                        }
                        return x / y;
                    }
                };

                template<size_t size>
                struct p_divrem_h<size, false, pow2> {

                    template<typename UI = Integer<size, false >>
                    constexpr inline static UI divrem(
                        const UI& x, const UI& y, UI* rem) noexcept {
                        return p_udivrem(x, y, rem);
                    }
                };

                template<size_t size>
                struct p_divrem_h<size, true, pow2> {

                    template<typename SI = Integer<size, true >>
                    constexpr inline static SI divrem(
                        const SI& x, const SI& y, SI* rem) noexcept {
                        return p_sdivrem(x, y, rem);
                    }
                };

                template<size_t size, bool sig>
                constexpr inline Integer<size, sig> divrem(const Integer<size, sig>& x,
                    const Integer<size, sig>& y) noexcept {
                    return p_divrem_h<size, sig>::divrem(x, y, (Integer<size, sig>*)nullptr);
                }

                template<size_t size, bool sig>
                constexpr inline Integer<size, sig> divrem(const Integer<size, sig>& x,
                    const Integer<size, sig>& y, Integer<size, sig>* rem) noexcept {
                    return p_divrem_h<size, sig>::divrem(x, y, rem);
                }

                //TODO

                template<size_t size, typename R = Integer<size * 2, false> >
                constexpr inline ct::if_t<R, (p_intType(size) == native) &&
                    (size != p_i_native::max_native_size) >
                    wmultiply(const Integer<size, false>& v1,
                        const Integer<size, false>& v2) noexcept {
                    return R(v1) * R(v2);
                }

                template<size_t size, typename U1 = Integer<size / 2, false >,
                    typename U2 = Integer<size, false>, typename U4 = Integer<size * 2, false> >
                    constexpr inline ct::if_t<U4, size == p_i_native::max_native_size>
                    wmultiply(const Integer<size, false>& v1,
                        const Integer<size, false>& v2) noexcept {
                    U1 a = U1(v1 >> (size * 4)), b = U1(v1);
                    U1 c = U1(v2 >> (size * 4)), d = U1(v2);
                    U2 ac = wmultiply(a, c);
                    U2 bd = wmultiply(b, d);
                    U2 ad = wmultiply(a, d);
                    U2 bc = wmultiply(b, c);
                    U4 abcd = U4(ad) + U4(bc);
                    return U4(bd, ac) + (abcd << (size * 4));
                }

                template<size_t size, typename U1 = Integer<size / 2, false >,
                    typename U2 = Integer<size, false>, typename U4 = Integer<size * 2, false> >
                    constexpr inline ct::if_t<U4, p_intType(size) == pow2>
                    wmultiply(const Integer<size, false>& v1,
                        const Integer<size, false>& v2) noexcept {
                    U1 a = v1.uhigh(), b = v1.ulow();
                    U1 c = v2.uhigh(), d = v2.ulow();
                    U2 ac = wmultiply(a, c);
                    U2 bd = wmultiply(b, d);
#if 1 //The Karatsuba algorithm
                    U1 ab = U1::ZERO(), cd = U1::ZERO();
                    bool abo = U1::add_overflow(a, b, ab);
                    bool cdo = U1::add_overflow(c, d, cd);
                    U2 abcd_low = wmultiply(ab, cd);
                    unsigned int abcd_high = abo && cdo;
                    abcd_high += abo && U2::add_overflow(U2(abcd_low), U2(cd) << (size * 4), abcd_low);
                    abcd_high += cdo && U2::add_overflow(U2(abcd_low), U2(ab) << (size * 4), abcd_low);
                    abcd_high -= U2::sub_overflow(U2(abcd_low), ac, abcd_low);
                    abcd_high -= U2::sub_overflow(U2(abcd_low), bd, abcd_low);
                    U4 abcd = U4(abcd_low, U2(abcd_high));
#else
                    U2 ad = wmultiply(a, d);
                    U2 bc = wmultiply(b, c);
                    U4 abcd = U4(ad) + U4(bc);
#endif
                    return U4(bd, ac) + (abcd << (size * 4));
                }

                template<size_t size1, size_t size2, bool sig1, bool sig2>
                using result_t = Integer<(size1 < size2 ? size2 : size1),
                    (size1 == size2) ? sig1 && sig2 :
                    (size1 > size2 ? sig1 : sig2)>;

                template<size_t size, bool sig, p_IType = p_intType(size)>
                class Integer_Base;

                template<size_t size, bool sig>
                class Integer_Base<size, sig, native> {
                private:
                    using V = typename p_Integer_Impl<size, sig>::type;
                    V value;

                    constexpr inline Integer_Base() noexcept = default;

                    constexpr inline Integer_Base(const V n) noexcept : value(n) { }

                public:

                    constexpr inline const typename V::U& uvalue() const noexcept {
                        return value.value;
                    }

                    constexpr inline typename V::U& uvalue() noexcept {
                        return value.value;
                    }

                    template<size_t, bool>
                    friend class Integer;
                };

                template<size_t size, bool sig>
                class Integer_Base<size, sig, pow2> {
                private:
                    using V = typename p_Integer_Impl<size, sig>::type;
                    V value;

                    constexpr inline Integer_Base() noexcept = default;

                    constexpr inline Integer_Base(const V n) noexcept : value(n) { }

                public:

                    constexpr inline const typename V::U& ulow() const noexcept {
                        return value.low;
                    }

                    constexpr inline typename V::U& ulow() noexcept {
                        return value.low;
                    }

                    constexpr inline const typename V::U& uhigh() const noexcept {
                        return value.high;
                    }

                    constexpr inline typename V::U& uhigh() noexcept {
                        return value.high;
                    }

                    template<size_t, bool>
                    friend class Integer;
                };
                namespace p_literal {
                    template<bool sig, size_t bytes>
                    constexpr inline auto parseDec_h(const std::string& nums) noexcept;
                    template<bool sig, size_t bytes >
                    constexpr inline auto parseHex(const std::string& nums) noexcept;
                    
                }
                namespace p_i_print {
                    template<size_t size, bool sig, typename CharT, typename Traits>
                    inline void print(const Integer<size, sig>& v,
                        std::basic_ostream<CharT, Traits>& out);
                }
                template<size_t size, bool sig>
                class Integer : public Integer_Base<size, sig> {
                private:
                    using B = Integer_Base<size, sig>;
                    using V = typename B::V;
                    using B::value;

                    template<size_t size2, bool sig2,
                        bool = (p_intType(size) == native) &&
                        (p_intType(size2) == native),
                        bool = (size == size2) && (p_intType(size) == pow2),
                        bool = (size != size2) &&
                        ((p_intType(size) == native) ||
                            (p_intType(size) == pow2)) &&
                        (p_intType(size2) == pow2)>
                        struct upcast_h {
                    };

                    template<size_t size2, bool sig2>
                    struct upcast_h<size2, sig2, true, false, false> {

                        constexpr inline static Integer<size2, sig2>
                            upcast(const Integer& v) noexcept {
                            using I = Integer<size2, sig2>;
                            return typename I::V(p_US_t<typename V::U,
                                typename V::S, sig >(v.value.value));
                        }
                    };

                    template<size_t size2, bool sig2>
                    struct upcast_h<size2, sig2, false, true, false> {

                        constexpr inline static Integer<size2, sig2>
                            upcast(const Integer& v) noexcept {
                            using I = Integer<size2, sig2>;
                            return I(typename I::V(v.value.low, v.value.high));
                        }
                    };

                    template<size_t size2, bool sig2>
                    struct upcast_h<size2, sig2, false, false, true> {

                        constexpr inline static Integer<size2, sig2>
                            upcast(const Integer& v) noexcept {
                            using I = Integer<size2, sig2>;
                            return typename I::V(p_US_t<typename I::V::U,
                                typename I::V::S, sig >(v));
                        }
                    };

                    template<size_t size2, bool sig2>
                    constexpr inline Integer<size2, sig2> upcast() const noexcept {
                        return upcast_h<size2, sig2>::upcast(*this);
                    }

                    template<size_t size2, bool sig2,
                        bool = (p_intType(size) == native) &&
                        (p_intType(size2) == native),
                        bool = (p_intType(size) == pow2) &&
                        ((p_intType(size2) == pow2) ||
                            (p_intType(size2) == native))>
                        struct downcast_h {
                    };

                    template<size_t size2, bool sig2>
                    struct downcast_h<size2, sig2, true, false> {

                        constexpr inline static Integer<size2, sig2>
                            downcast(const Integer& v) noexcept {
                            using I = Integer<size2, sig2>;
                            return typename I::V(typename I::V::U(v.value.value));
                        }
                    };

                    template<size_t size2, bool sig2>
                    struct downcast_h<size2, sig2, false, true> {

                        constexpr inline static Integer<size2, sig2>
                            downcast(const Integer& v) noexcept {
                            using I = Integer<size2, sig2>;
                            return I(v.value.low);
                        }
                    };

                    template<size_t size2, bool sig2>
                    constexpr inline Integer<size2, sig2> downcast() const noexcept {
                        return downcast_h<size2, sig2>::downcast(*this);
                    }

                    constexpr inline Integer(const V n) noexcept : B(n) { }
                public:
                    constexpr inline Integer(const std::string& n) noexcept  {
                        if (n[0] == '0' && (n[1] == 'x'|| n[1] == 'X'))
                            *this = p_literal::parseHex<sig,size>(n);
                        else
                            *this = p_literal::parseDec_h< sig, size>(n);
                    }

                    constexpr inline static size_t SIZE() noexcept {
                        return size;
                    }

                    constexpr inline static bool is_signed() noexcept {
                        return sig;
                    }

                    constexpr inline static Integer ZERO() noexcept {
                        return V::ZERO();
                    }

                    constexpr inline static Integer ONE() noexcept {
                        return ZERO().addOne();
                    }

                    constexpr inline static Integer MAX_VALUE() noexcept {
                        return p_i_utils::max_int_value<Integer, sig>();
                    }

                    constexpr inline static Integer MIN_VALUE() noexcept {
                        return p_i_utils::min_int_value<Integer, sig>();
                    }

                    constexpr inline Integer() noexcept {};

                    constexpr inline bool isZero() const noexcept {
                        return value.isZero();
                    }

                    constexpr inline bool isNegative() const noexcept {
                        return value.isNegative();
                    }

                    constexpr inline bool upperBit() const noexcept {
                        return value.upperBit();
                    }

                    constexpr inline Integer addOne() const noexcept {
                        return value.addOne();
                    }

                    constexpr inline Integer subOne() const noexcept {
                        return value.subOne();
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<p_i_native::min_native_t, (index < size) >
                        getByte() const noexcept {
                        return value.template getByte<index>();
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<Integer&, (index < size) >
                        setByte(p_i_native::min_native_t v) noexcept {
                        value.template setByte<index>(v);
                        return *this;
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<bool, (index < size* p_i_native::min_native_bits) >
                        getBit() const noexcept {
                        return value.template getBit<index>();
                    }

                    template<size_t index>
                    constexpr inline ct::if_t<Integer&, (index < size* p_i_native::min_native_bits) >
                        setBit(bool v) noexcept {
                        value.template setBit<index>(v);
                        return *this;
                    }

                    constexpr inline static bool increment_overflow(Integer& value) noexcept {
                        return V::increment_overflow(value.value);
                    }

                    constexpr inline static bool decrement_overflow(Integer& value) noexcept {
                        return V::decrement_overflow(value.value);
                    }

                    constexpr inline static bool
                        increment_overflow_carry(Integer& value, bool cf) noexcept {
                        return V::increment_overflow_carry(value.value, cf);
                    }

                    constexpr inline static bool
                        decrement_overflow_carry(Integer& value, bool cf) noexcept {
                        return V::decrement_overflow_carry(value.value, cf);
                    }

                    constexpr inline static void
                        increment_carry(Integer& value, bool cf) noexcept {
                        V::increment_carry(value.value, cf);
                    }

                    constexpr inline static void
                        decrement_carry(Integer& value, bool cf) noexcept {
                        V::decrement_carry(value.value, cf);
                    }

                    constexpr inline static bool add_overflow(
                        const Integer& v1, const Integer& v2, Integer& out) noexcept {
                        return V::add_overflow(v1.value, v2.value, out.value);
                    }

                    constexpr inline static bool sub_overflow(
                        const Integer& v1, const Integer& v2, Integer& out) noexcept {
                        return V::sub_overflow(v1.value, v2.value, out.value);
                    }

                    constexpr inline static bool add_overflow_carry(const Integer& v1,
                        const Integer& v2, bool cf, Integer& out) noexcept {
                        return V::add_overflow_carry(v1.value, v2.value, cf, out.value);
                    }

                    constexpr inline static bool sub_overflow_carry(const Integer& v1,
                        const Integer& v2, bool cf, Integer& out) noexcept {
                        return V::sub_overflow_carry(v1.value, v2.value, cf, out.value);
                    }

                    constexpr inline static bool add_zero_overflow_carry(const Integer& v1,
                        bool cf, Integer& out) noexcept {
                        return V::add_zero_overflow_carry(v1.value, cf, out.value);
                    }

                    constexpr inline static bool sub_zero_overflow_carry(const Integer& v1,
                        bool cf, Integer& out) noexcept {
                        return V::sub_zero_overflow_carry(v1.value, cf, out.value);
                    }

                    constexpr inline static void add_carry(const Integer& v1,
                        const Integer& v2, bool cf, Integer& out) noexcept {
                        V::add_carry(v1.value, v2.value, cf, out.value);
                    }

                    constexpr inline static void sub_carry(const Integer& v1,
                        const Integer& v2, bool cf, Integer& out) noexcept {
                        V::sub_carry(v1.value, v2.value, cf, out.value);
                    }

                    constexpr inline static void add_zero_carry(const Integer& v1,
                        bool cf, Integer& out) noexcept {
                        V::add_zero_carry(v1.value, cf, out.value);
                    }

                    constexpr inline static void sub_zero_carry(const Integer& v1,
                        bool cf, Integer& out) noexcept {
                        V::sub_zero_carry(v1.value, cf, out.value);
                    }

                    constexpr inline static bool
                        leftShiftOneBit(Integer& value, bool bit) noexcept {
                        return V::leftShiftOneBit(value.value, bit);
                    }

                    constexpr inline size_t numberOfLeadingZeros() const noexcept {
                        return value.numberOfLeadingZeros();
                    }

                    constexpr inline size_t numberOfTrailingZeros() const noexcept {
                        return value.numberOfTrailingZeros();
                    }

                    constexpr inline Integer<size, true> s() const noexcept {
                        return *this;
                    }

                    constexpr inline Integer<size, false> u() const noexcept {
                        return *this;
                    }

                    template<typename T, ct::if_bool_t<bool, T> = true >
                    constexpr inline Integer(T n) noexcept :
                        Integer(V(typename V::U(n))) { }

                    template<size_t size1, bool sig1, size_t size2, bool sig2,
                        ct::if_t<bool, (p_intType(size) == pow2) &&
                        (size1 <= size / 2) && (size2 <= size / 2)> = true >
                        constexpr inline Integer(const Integer<size1, sig1>& low,
                            const Integer<size2, sig2>& high) noexcept :
                        Integer(V(low, high)) { }

                    template<typename T1, typename T2,
                        ct::if_t<bool, (p_intType(size) == pow2) &&
                        (sizeof(T1) <= size / 2) && (sizeof(T2) <= size / 2)> = true,
                        ct::if_int_t<bool, T1> = true, ct::if_int_t<bool, T2> = true >
                        constexpr inline Integer(const T1 low,
                            const T2 high) noexcept : Integer(V(low, high)) { }

                    template<typename T, size_t size2, bool sig2,
                        ct::if_t<bool, (p_intType(size) == pow2) && (size2 <= size / 2)> = true,
                        ct::if_int_t<bool, T, (sizeof(T) <= size / 2)> = true >
                        constexpr inline Integer(const T low,
                            const Integer<size2, sig2>& high) noexcept :
                        Integer(V(low, high)) { }

                    template<size_t size2, bool sig2, typename T,
                        ct::if_t<bool, (p_intType(size) == pow2) && (size2 <= size / 2)> = true,
                        ct::if_int_t<bool, T, (sizeof(T) <= size / 2)> = true >
                        constexpr inline Integer(const Integer<size2, sig2>& low,
                            const T high) noexcept : Integer(V(low, high)) { }
                    constexpr inline std::string tostring() {
                        std::stringstream out;
                        out.setf(std::ios_base::dec);
                        p_i_print::print<size, sig, std::stringstream::char_type, std::stringstream::traits_type>(*this, out);
                        return out.str();
                    }
                    constexpr inline std::string tohex() {
                        std::stringstream out;
                        out.setf(std::ios_base::hex);
                        p_i_print::print<size, sig, std::stringstream::char_type, std::stringstream::traits_type>(*this, out);
                        return out.str();
                    }
                    constexpr inline std::string todec() {
                        return tostring();
                    }
                private:

                    template<typename T, p_IType = p_intType(size)>
                    struct tcast_h {
                    };

                    template<typename T>
                    struct tcast_h<T, native> {

                        constexpr inline static V tcast(const T n) noexcept {
                            return V(n);
                        }
                    };

                    template<typename T>
                    struct tcast_h<T, pow2> {

                        constexpr inline static V tcast(const T n) noexcept {
                            return Integer(Integer<sizeof(T), JIO::ct::is_signed<T>()>(n)).value;
                        }
                    };

                public:

                    template<typename T, ct::if_int_t<bool, T> = true >
                    constexpr inline Integer(const T n) noexcept :
                        Integer(V(tcast_h<T>::tcast(n))) { }

                    template<size_t size2, bool sig2, ct::if_t<bool, (size2 >= size)> = true >
                    constexpr inline operator Integer<size2, sig2>() const noexcept {
                        return upcast<size2, sig2>();
                    }

                    template<size_t size2, bool sig2, ct::if_t<bool, (size2 < size)> = true >
                    constexpr inline operator Integer<size2, sig2>() const noexcept {
                        return downcast<size2, sig2>();
                    }

                private:

                    template<typename T,
                        bool = (p_intType(size) == native) && (sizeof(T) >= size),
                        bool = (p_intType(size) == native) && (sizeof(T) < size),
                        bool = (p_intType(size) != native) && (sizeof(T) < size)>
                        struct pcast_h {
                    };

                    template<typename T>
                    struct pcast_h<T, true, false, false> {

                        constexpr inline static T pcast(const Integer& v) noexcept {
                            return T(p_US_t<typename V::U, typename V::S, sig >(v.value.value));
                        }
                    };

                    template<typename T>
                    struct pcast_h<T, false, true, false> {

                        constexpr inline static T pcast(const Integer& v) noexcept {
                            return T(v.value.value);
                        }
                    };

                    template<typename T>
                    struct pcast_h<T, false, false, true> {

                        constexpr inline static T pcast(const Integer& v) noexcept {
                            return T(Integer<sizeof(T), JIO::ct::is_signed<T>()>(v));
                        }
                    };

                public:

                    template<typename T, ct::if_int_t<bool, T> = true >
                    constexpr inline operator T() const noexcept {
                        return pcast_h<T>::pcast(*this);
                    }

                    template<typename T, ct::if_bool_t<bool, T> = true >
                    constexpr inline operator T() const noexcept {
                        return !(this->isZero());
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator+(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value + R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator-(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value - R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator*(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value * R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator/(const Integer<size2, sig2>& v2) const noexcept {
                        return typename R::V(R(*this).value / R(v2).value);
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator%(const Integer<size2, sig2>& v2) const noexcept {
                        return typename R::V(R(*this).value % R(v2).value);
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator|(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value | R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator&(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value & R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline R
                        operator^(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value ^ R(v2).value;
                    }

                    template<size_t size2, bool sig2>
                    constexpr inline Integer
                        operator<<(const Integer<size2, sig2>& v2) const noexcept {
                        return value << p_SHType<size>(v2);
                    }

                    template<typename T>
                    constexpr inline ct::if_int_t<Integer, T>
                        operator<<(const T v2) const noexcept {
                        return value << p_SHType<size>(v2);
                    }

                    template<size_t size2, bool sig2>
                    constexpr inline Integer
                        operator>>(const Integer<size2, sig2>& v2) const noexcept {
                        return V(value >> p_SHType<size>(v2));
                    }

                    template<typename T>
                    constexpr inline ct::if_int_t<Integer, T>
                        operator>>(const T v2) const noexcept {
                        return V(value >> p_SHType<size>(v2));
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline bool
                        operator==(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value == R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline bool
                        operator!=(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value != R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline bool
                        operator<=(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value <= R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline bool
                        operator>=(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value >= R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline bool
                        operator<(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value < R(v2).value;
                    }

                    template<size_t size2, bool sig2,
                        typename R = result_t<size, size2, sig, sig2>>
                        constexpr inline bool
                        operator>(const Integer<size2, sig2>& v2) const noexcept {
                        return R(*this).value > R(v2).value;
                    }

                    constexpr inline Integer operator~() const noexcept {
                        return ~value;
                    }

                    constexpr inline Integer operator-() const noexcept {
                        return -value;
                    }

                    constexpr inline Integer operator+() const noexcept {
                        return *this;
                    }

                    constexpr inline Integer& operator++() noexcept {
                        return *this = addOne();
                    }

                    constexpr inline Integer& operator--() noexcept {
                        return *this = subOne();
                    }

                    constexpr inline Integer operator++(int) noexcept {
                        Integer tmp = *this;
                        operator++();
                        return tmp;
                    }

                    constexpr inline Integer operator--(int) noexcept {
                        Integer tmp = *this;
                        operator--();
                        return tmp;
                    }

                    template<size_t, bool>
                    friend class Integer;

                    template<size_t, bool>
                    friend struct p_native_Integer_Impl;

                    template<size_t, bool>
                    friend struct p_pow2_Integer_Impl;
                };

#if INTEGER_HPP_HAS_OSTREAM
                namespace p_i_print {

                    template<size_t size, size_t... index,
                        typename R = Integer<size, false >>
                        constexpr inline R
                        div_ten_const_h(p_i_seq::array_t<size_t, index...>) noexcept {
                        R out = R::ZERO();
                        p_i_seq::unused_array({
                            (out.template setByte<index>(index == 0 ? 0xcd : 0xcc))...
                            });
                        return out;
                    }

                    template<size_t size>
                    constexpr inline Integer<size, false> div_ten_const() noexcept {
                        return div_ten_const_h<size>(p_i_seq::make_array<size_t, 0, size>());
                    }

                    template<size_t size>
                    constexpr inline ct::if_t<unsigned, p_intType(size) == native>
                        divremTen(Integer<size, false>& v) noexcept {
                        unsigned tmp = unsigned(v);
                        v /= Integer<size, false>(10);
                        return tmp - unsigned(v) * 10;
                    }

                    template<size_t size>
                    constexpr inline ct::if_t<unsigned, p_intType(size) == pow2>
                        divremTen(Integer<size, false>& v) noexcept {
                        constexpr Integer<size, false> dc = div_ten_const<size>();
                        unsigned tmp = unsigned(v);
                        v = wmultiply(v, dc).uhigh() >> 3;
                        return tmp - unsigned(v) * 10;
                    }

                    constexpr inline size_t p_digits10(size_t size, bool sig) noexcept {
                        //TODO
                        static_assert(sizeof(size_t) <= 8, "too big size_t");
                        return size_t(wmultiply<8>(size * 8 - sig,
                            0x4d104d427de7FbccULL).uhigh());
                    }

                    template<size_t size, bool sig, typename CharT, typename Traits>
                    inline void printDec(const Integer<size, sig>& v,
                        std::basic_ostream<CharT, Traits>& out) {
                        auto f = out.flags();
                        constexpr size_t d = p_digits10(size, sig) + 2;
                        bool neg = false;
                        Integer<size, false> tmp = v;
                        if (v.isNegative()) {
                            tmp = -tmp;
                            neg = true;
                        }
                        char str[d + 1]; // trailing \0
                        size_t i = d;
                        str[i--] = 0;
                        while (tmp >= 10) {
                            str[i--] = p_i_utils::digits[divremTen(tmp)];
                        }
                        str[i] = p_i_utils::digits[unsigned(tmp)];
                        if (neg) {
                            str[--i] = '-';
                        }
                        else if (f & std::ios_base::showpos) {
                            str[--i] = '+';
                        }
                        out << &str[i];
                    }

                    template<size_t size, bool sig, typename CharT, typename Traits>
                    inline void printHex(const Integer<size, sig>& v,
                        std::basic_ostream<CharT, Traits>& out) {
                        auto f = out.flags();
                        constexpr size_t d = size * 2 + 2;
                        Integer<size, false> tmp = v;
                        char str[d + 1]; // trailing \0
                        size_t i = d;
                        str[i--] = 0;
                        while (tmp >= 16) {
                            str[i--] = p_i_utils::digits[unsigned(tmp) & 0xf];
                            tmp >>= 4;
                        }
                        str[i] = p_i_utils::digits[unsigned(tmp)];
                        if (f & std::ios_base::showbase) {
                            str[--i] = 'x';
                            str[--i] = '0';
                        }
                        out << &str[i];
                    }

                    template<size_t size, bool sig, typename CharT, typename Traits>
                    inline void printOct(const Integer<size, sig>& v,
                        std::basic_ostream<CharT, Traits>& out) {
                        auto f = out.flags();
                        constexpr size_t d = (size * 8 + 2) / 3 + 1;
                        Integer<size, false> tmp = v;
                        char str[d + 1]; // trailing \0
                        size_t i = d;
                        str[i--] = 0;
                        while (tmp >= 8) {
                            str[i--] = p_i_utils::digits[unsigned(tmp) & 0x7];
                            tmp >>= 3;
                        }
                        str[i] = p_i_utils::digits[unsigned(tmp)];
                        if (f & std::ios_base::showbase) {
                            str[--i] = '0';
                        }
                        out << &str[i];
                    }

                    template<size_t size, bool sig, typename CharT, typename Traits>
                    inline void print(const Integer<size, sig>& v,
                        std::basic_ostream<CharT, Traits>& out) {
                        auto f = out.flags();
                        if (f & std::ios_base::hex) {
                            printHex(v, out);
                            return;
                        }
                        if (f & std::ios_base::oct) {
                            printOct(v, out);
                            return;
                        }
                        printDec(v, out);
                    }
                }

                template<size_t size, bool sig, typename CharT, typename Traits>
                inline std::basic_ostream<CharT, Traits>&
                    operator<<(std::basic_ostream<CharT, Traits>& out,
                        const Integer<size, sig>& v) {
                    Integer < p_i_utils::make_pow2(size), sig> tmp = v;
                    p_i_print::print(tmp, out);
                    return out;
                }
#endif

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator+(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) + R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator+(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) + R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator+=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 + Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator+=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 + Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator+=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer<sizeof(T), ct::is_signed<T>()>;
                    return v1 = T(R(v1) + R(v2));
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator-(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) - R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator-(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) - R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator-=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 - Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator-=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 - Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator-=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer<sizeof(T), ct::is_signed<T>()>;
                    return v1 = T(R(v1) - R(v2));
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator*(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) * R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator*(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) * R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator*=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 * Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator*=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 * Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator*=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer<sizeof(T), ct::is_signed<T>()>;
                    return v1 = T(R(v1) * R(v2));
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator/(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) / R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator/(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) / R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator/=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = Integer<size1, sig1>(v1 / v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator/=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = Integer<size1, sig1>(v1 / v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator/=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    return v1 = T(v1 / v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator%(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) % R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator%(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) % R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator%=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = Integer<size1, sig1>(v1 % v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator%=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = Integer<size1, sig1>(v1 % v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator%=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    return v1 = T(v1 % v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator|(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) | R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator|(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) | R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator|=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 | Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator|=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 | Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator|=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer<sizeof(T), ct::is_signed<T>()>;
                    return v1 = T(R(v1) | R(v2));
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator&(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) & R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator&(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) & R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator&=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 & Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator&=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 & Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator&=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer<sizeof(T), ct::is_signed<T>()>;
                    return v1 = T(R(v1) & R(v2));
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator^(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    return R(v1) ^ R(v2);
                }

                template<size_t size1, bool sig1, typename T,
                    typename R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>>
                    constexpr inline ct::if_int_t<R, T>
                    operator^(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    return R(v1) ^ R(v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator^=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 ^ Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator^=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 ^ Integer<size1, sig1>(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator^=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer<sizeof(T), ct::is_signed<T>()>;
                    return v1 = T(R(v1) ^ R(v2));
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator==(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) == R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator==(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) == R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator!=(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) != R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator!=(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) != R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator<=(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) <= R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator<=(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) <= R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator>=(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) >= R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator>=(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) >= R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator<(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) < R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator<(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) < R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator>(const Integer<size1, sig1>& v1, const T v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) > R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<bool, T>
                    operator>(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = result_t<size1, sizeof(T), sig1, ct::is_signed<T>()>;
                    return R(v1) > R(v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T, T>
                    operator<<(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer <sizeof(T), (ct::is_signed<T>())>;
                    return T(R(v1) << v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator<<=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 << v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator<<=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 << v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator<<=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    return v1 = T(Integer<sizeof(T), ct::is_signed<T>()>(v1) << v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T, T>
                    operator>>(const T v1, const Integer<size1, sig1>& v2) noexcept {
                    using R = Integer <sizeof(T), (ct::is_signed<T>())>;
                    return T(R(v1) >> v2);
                }

                template<size_t size1, bool sig1, size_t size2, bool sig2>
                constexpr inline Integer<size1, sig1>&
                    operator>>=(Integer<size1, sig1>& v1,
                        const Integer<size2, sig2>& v2) noexcept {
                    return v1 = (v1 >> v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<Integer<size1, sig1>&, T>
                    operator>>=(Integer<size1, sig1>& v1, const T v2) noexcept {
                    return v1 = (v1 >> v2);
                }

                template<size_t size1, bool sig1, typename T>
                constexpr inline ct::if_int_t<T&, T>
                    operator>>=(T& v1, const Integer<size1, sig1>& v2) noexcept {
                    return v1 = T(Integer<sizeof(T), ct::is_signed<T>()>(v1) >> v2);
                }

                namespace p_literal {
                    template <typename T, T... values>
                    using array_t = p_i_seq::array_t<T, values...>;

                    constexpr size_t bits_to_bytes(size_t bits, bool sig) noexcept {
                        size_t bytes = (bits + p_i_native::min_native_bits - 1 + sig) /
                            p_i_native::min_native_bits;
                        //TODO: for unusual arch`s
                        return p_i_utils::make_pow2(bytes);
                    }

                    template<bool sig, int bit_per_symbol, int first, int... nums>
                    constexpr inline auto get_bytes() noexcept {
                        return bits_to_bytes(sizeof...(nums) * bit_per_symbol +
                            (sizeof(first) * p_i_native::min_native_bits -
                                p_i_utils::numberOfLeadingZeros_n(first)), sig);
                    }

                    template<bool sig, int... nums,
                        size_t bytes = get_bytes<sig, 4, nums...>()>
                        constexpr inline auto parseHex(array_t<int, nums...> arr) noexcept {
                        using I = Integer<bytes, sig>;
                        I out = I::ZERO();
                        for (size_t i = 0; i < sizeof...(nums); i++) {
                            out <<= 4;
                            out |= I(arr[i]);
                        }
                        return out;
                    }

                    inline char HH(char c) {
                        if (c >= 'a' && c <= 'f')
                            return c - 'a' + 10;
                        else if (c >= '0' && c <= '9')
                            return c - '0';
                        
                        throw "HH FAILD. - File:interger.hpp line: 3075";
                        return 0;
                    }

                    template<bool sig,size_t bytes >
                        constexpr inline auto parseHex(const std::string &nums) noexcept {
                        using I = Integer<bytes, sig>;
                        I out = I::ZERO();
                        for (size_t i = 2; i < nums.size(); i++) { // skip 0x 
                            out <<= 4;
                            out |= I(HH(nums[i]));
                        }
                        return out;
                    }

                    template<bool sig>
                    constexpr inline auto parseHex(array_t<int>) noexcept {
                        return Integer<1, sig>::ZERO();
                    }

                    template<bool sig, size_t bytes, int... nums>
                    constexpr inline auto parseDec_h(array_t<int, nums...> arr) noexcept {
                        using I = Integer<bytes, sig>;
                        I out = I::ZERO();
                        for (size_t i = 0; i < sizeof...(nums); i++) {
                            out *= I(10);
                            out += I(arr[i]);
                        }
                        return out;
                    }
                    template<bool sig, size_t bytes>
                    constexpr inline auto parseDec_h(const std::string& nums) noexcept {
                        using I = Integer<bytes, sig>;
                        I out = I::ZERO();
                        for (size_t i = 0; i < nums.size(); i++) {
                            out *= I(10);
                            out += I(HH(nums[i]));
                        }
                        return out;
                    }

                    template<bool sig, int... nums,
                        size_t hex_bytes = bits_to_bytes(sizeof...(nums) * 4, false),
                        size_t bytes = bits_to_bytes(
                            hex_bytes* p_i_native::min_native_bits - parseDec_h<false, hex_bytes>(
                                array_t<int, nums...>()).numberOfLeadingZeros(), sig)>
                        constexpr inline auto parseDec(array_t<int, nums...> arr) noexcept {
                        return parseDec_h<sig, bytes>(arr);
                    }

                    template<bool sig, int... nums,
                        size_t bytes = get_bytes<sig, 3, nums...>()>
                        constexpr inline auto parseOct(array_t<int, nums...> arr) noexcept {
                        using I = Integer<bytes, sig>;
                        I out = I::ZERO();
                        for (size_t i = 0; i < sizeof...(nums); i++) {
                            out <<= 3;
                            out |= I(arr[i]);
                        }
                        return out;
                    }

                    template<bool sig>
                    constexpr inline auto parseOct(array_t<int>) noexcept {
                        return Integer<1, sig>::ZERO();
                    }

                    template<bool sig, int... nums,
                        size_t bytes = get_bytes<sig, 1, nums...>()>
                        constexpr inline auto parseBin(array_t<int, nums...> arr) noexcept {
                        using I = Integer<bytes, sig>;
                        I out = I::ZERO();
                        for (size_t i = 0; i < sizeof...(nums); i++) {
                            out <<= 1;
                            out |= I(arr[i]);
                        }
                        return out;
                    }

                    template<bool sig>
                    constexpr inline auto parseBin(array_t<int>) noexcept {
                        return Integer<1, sig>::ZERO();
                    }

                    template<int min, int max, int value>
                    constexpr inline ct::if_t<int, (value >= min) && (value <= max) >
                        contains() noexcept {
                        return value;
                    }

                    template<int min, int max, char... chars>
                    using verify = array_t<int, contains<min, max,
                        p_i_utils::indexOfDigit<chars>()>()...>;

                    template<int min, int max, char... chars>
                    struct remove_zeros_and_verify_h {
                        typedef verify<min, max, chars...> type;
                    };

                    template<int min, int max, char... chars>
                    using remove_zeros_and_verify =
                        typename remove_zeros_and_verify_h<min, max, chars...>::type;

                    template<int min, int max, char... chars>
                    struct remove_zeros_and_verify_h <min, max, '0', chars...> {
                        typedef remove_zeros_and_verify<min, max, chars...> type;
                    };

                    template<int min, int max>
                    struct remove_zeros_and_verify_h <min, max> {
                        typedef array_t<int> type;
                    };

                    template<bool sig, char a, char b, char... chars,
                        ct::if_t<bool, (a == '0') && !(b == 'b' || b == 'B' || b == 'x' || b == 'X')> = true >
                        constexpr inline auto parse() noexcept {
                        return parseOct<sig>(remove_zeros_and_verify<0, 7, b, chars...>());
                    }

                    template<bool sig, char a, char b, char... chars,
                        ct::if_t<bool, (a == '0') && (b == 'b' || b == 'B')> = true >
                        constexpr inline auto parse() noexcept {
                        return parseBin<sig>(remove_zeros_and_verify<0, 1, chars...>());
                    }

                    template<bool sig, char a, char b, char... chars,
                        ct::if_t<bool, (a == '0') && (b == 'x' || b == 'X')> = true >
                        constexpr inline auto parse() noexcept {
                        return parseHex<sig>(remove_zeros_and_verify<0, 15, chars...>());
                    }

                    template<bool sig, char a, char... chars,
                        ct::if_t<bool, a != '0' > = true >
                        constexpr inline auto parse() noexcept {
                        return parseDec<sig>(remove_zeros_and_verify<0, 10, a, chars...>());
                    }

                    template<bool sig, char a, ct::if_t<bool, a == '0' > = true >
                    constexpr inline auto parse() noexcept {
                        return Integer<1, sig>(0);
                    }
                }

                template<char... chars>
                constexpr inline auto operator""_UI() noexcept {
                    return p_literal::parse<false, chars...>();
                }

                template<char... chars>
                constexpr inline auto operator""_SI() noexcept {
                    return p_literal::parse<true, chars...>();
                }
}

#endif /* INTEGER_HPP */