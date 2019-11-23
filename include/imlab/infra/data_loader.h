// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_INFRA_DATA_LOADER_H_
#define INCLUDE_IMLAB_INFRA_DATA_LOADER_H_
//---------------------------------------------------------------------------
#include <stdexcept>
#include <string>
#include <immintrin.h>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------

// Many thanks to Prof. Neumann
template<char c>
static inline const char *findChar(const char *iter, const char *limit) {
    assert(iter < limit);
    auto limit32 = limit - 32;
    auto char_mask = _mm256_set1_epi8(c);
    while (iter < limit32) {
        auto block = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(iter));
        uint32_t matches = _mm256_movemask_epi8(_mm256_cmpeq_epi8(block, char_mask));
        if (matches) {
            return iter + __builtin_ctzll(matches);
        } else {
            iter += 32;
        }
    }
    while ((iter < limit) && ((*iter) != c)) ++iter;
    return iter;
}

// Case for line ending
template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp) - 1, void>::type
parseFields(std::tuple<Tp...>& t, std::istream &in)
{
    std::string line = "";
    std::getline(in, line, '\n');

    using type = typename std::tuple_element<I, std::tuple<Tp...>>::type;
    std::get<I>(t) = type::castString(line.c_str(), line.size());
}
// Case for regular fields
template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp) - 1, void>::type
parseFields(std::tuple<Tp...>& t, std::istream &in)
{
    std::string line = "";
    std::getline(in, line, '|');

    using type = typename std::tuple_element<I, std::tuple<Tp...>>::type;
    std::get<I>(t) = type::castString(line.c_str(), line.size());

    parseFields<I + 1, Tp...>(t, in); // compile time loop
}

/**
 * Yeah, this is some really cool stuff.
 * What's behind this: a cool compile time loop that assigns the proper type conversions to one line of the input file.
 * Gosh, no idea why I am doing this but this is cool.
 */
template<typename... Types>
std::tuple<Types...> parseLine(std::istream &in) {
    std::tuple<Types...> line;
    parseFields(line, in);
    return line;
}
//---------------------------------------------------------------------------
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_INFRA_DATA_LOADER_H_
//---------------------------------------------------------------------------
