// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_INFRA_DATA_LOADER_H_
#define INCLUDE_IMLAB_INFRA_DATA_LOADER_H_
//---------------------------------------------------------------------------
#include <stdexcept>
#include <string>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------

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
