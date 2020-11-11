#pragma once

#include <deque>
#include <vector>

namespace engine {

template <typename DataType>
class ResourceContainer {
public:
  struct Identifier {
    constexpr static Identifier invalidValue() {
      return Identifier{kInvalidValue};
    }

    U32 id = kInvalidValue;

    Identifier() : id{kInvalidValue} {}

    Identifier(U32 id) : id{id} {}

    bool isValid() const {
      return id != kInvalidValue;
    }

    void reset() {
      id = kInvalidValue;
    }

    friend bool operator==(const Identifier& left, const Identifier& right) {
      return left.id == right.id;
    }

    friend bool operator!=(const Identifier& left, const Identifier& right) {
      return left.id != right.id;
    }

    bool operator!() const {
      return !isValid();
    }

  private:
    constexpr static U32 kInvalidValue = std::numeric_limits<U32>::max();
  };

  MemSize size() const {
    return m_data.size();
  }

  DataType* getData(Identifier identifier) {
    // TODO: This should return const?
    if (!identifier.isValid()) {
      return nullptr;
    }

    auto it = m_data.find(identifier.id);
    if (it == std::end(m_data)) {
      return nullptr;
    }

    return &it->second;
  }

protected:
  template <typename... Args>
  Identifier emplaceData(Args&&... args) {
    U32 newId = m_lastId++;

    m_data.emplace(newId, DataType{std::forward<Args>(args)...});

    return {newId};
  }

  void removeData(Identifier identifier) {
    m_data.erase(identifier.id);
  }

  U32 m_lastId = 1;
  std::unordered_map<U32, DataType> m_data;
};

}  // namespace engine
