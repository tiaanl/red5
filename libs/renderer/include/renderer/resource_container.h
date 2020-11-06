#pragma once

#include <deque>
#include <vector>

namespace renderer {

template <typename DataType>
class ResourceContainer {
public:
  struct Identifier {
    constexpr static Identifier invalidValue() {
      return Identifier{std::numeric_limits<U32>::max()};
    }

    U32 id = invalidValue().id;

    Identifier() : id{invalidValue().id} {}

    Identifier(U32 id) : id{id} {}

    bool isValid() const {
      return id != invalidValue();
    }

    friend bool operator==(const Identifier& left, const Identifier& right) {
      return left.id == right.id;
    }

    friend bool operator!=(const Identifier& left, const Identifier& right) {
      return left.id != right.id;
    }

    bool operator!() const {
      return id == invalidValue().id;
    }
  };

  MemSize size() const {
    return m_data.size();
  }

  DataType* getData(Identifier identifier) {
    // TODO: This should return const?
    if (!identifier.isValid()) {
      return nullptr;
    }

    return &m_data[identifier.id];
  }

protected:
  template <typename... Args>
  Identifier emplaceData(Args&&... args) {
    if (m_openings.empty()) {
      m_data.emplace_back(DataType{std::forward<Args>(args)...});
      return Identifier{static_cast<U32>(m_data.size() - 1)};
    }

    auto openIndex = m_openings.front();
    m_openings.pop_front();

    m_data[openIndex] = DataType{std::forward<Args>(args)...};
    return Identifier{static_cast<U32>(openIndex)};
  }

  void removeData(Identifier identifier) {
    m_openings.push_back(identifier.id);
  }

  std::vector<DataType> m_data;
  std::deque<MemSize> m_openings;
};

}  // namespace renderer
