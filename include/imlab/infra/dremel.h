// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_INFRA_DREMEL_H_
#define INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------

template<typename T>
class TableBase<T> {
 public:
    virtual uint64_t insert(T tuple) = 0;
    uint64_t get_size() { return size; }
 protected:
    uint64_t size = 0;
};

template<typename T>
class DremelColumn<T> {
 public:
    explicit DremelColumn(std::string identifier) : identifier(identifier) {}
    std::string get_identifier() { return identifier; }

 protected:
    std::string identifier;
    std::vector<T> value;
    std::vector<unsigned> repetition_level;
    std::vector<unsigned> definition_level;
};
//---------------------------------------------------------------------------
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
