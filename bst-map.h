#ifndef BSTMAP_H
#define BSTMAP_H

/**
 * @brief 32 Bit Floating Point Number
 */
#include <cstdint>
using f32 = float;

/**
 * @brief 64 Bit Floating Point Number
 */
using f64 = long double;

/**
 * @brief Fix Sized Unsigned 8 Bit Integer (cannot be negative)
 */
using u8 = std::uint8_t;

/**
 * @brief Fix Sized Unsigned 16 Bit Integer (cannot be negative)
 */
using u16 = std::uint16_t;

/**
 * @brief Fix Sized Unsigned 32 Bit Integer (cannot be negative)
 */
using u32 = std::uint32_t;

/**
 * @brief Fix Sized Unsigned 64 Bit Integer (cannot be negative)
 */
using u64 = unsigned long int;

/**
 * @brief Biggest Unsigned Integer type that the current platform can use
 * (cannot be negative)
 */
using umax = std::uintmax_t;

/**
 * @brief Unsigned Integer for when referring to any form of memory size or
 * offset (eg. an array length or index)
 */
using usize = std::size_t;
/**
 * @brief Unsigned Integer Pointer typically used for pointer arithmetic
 */
using uptr = std::uintptr_t;

/**
 * @brief Signed 8 bit Integer
 */
using i8 = std::int8_t;

/**
 * @brief Signed 16 bit Integer
 */
using i16 = std::int16_t;

/**
 * @brief Signed 32 bit Integer
 */
using i32 = std::int32_t;

/**
 * @brief Signed 64 bit Integer
 */
using i64 = std::int64_t;

/**
 * @brief Integer pointer typically used for pointer arithmetic
 */
using iptr = std::intptr_t;

#include <cstddef>
#include <ostream>

namespace CS280 {

  /**
   * @brief Binary Search Tree
   *
   * @tparam K Key
   * @tparam V Value
   */
  template<typename K, typename V>
  class BSTmap {

  public:

    class iterator;
    class const_iterator;

    /**
     * @class Node
     * @brief BST Node
     */
    class Node {
    public:

      /**
       * @brief Normal constructor
       */
      Node(K k, V val, Node* p, usize h, i32 b, Node* l, Node* r);

      /**
       * @brief Copy constructor
       */
      Node(const Node&) = delete;

      /**
       * @brief Destructor
       */
      ~Node();

      /**
       * @brief Copy assignment
       */
      auto operator=(const Node&) -> Node& = delete;

      /**
       * @brief Gets the key stored
       */
      auto Key() const -> const K&; // return a const reference

      /**
       * @brief Gets the value stored
       */
      auto Value() -> V&; // return a reference

      /**
       * @brief Gets the leftmost node
       */
      [[nodiscard]] auto first()
        -> Node*; // minimum - follow left as far as possible

      /**
       * @brief Gets the rightmost node
       */
      auto last() -> Node*; // maximum - follow right as far as possible

      /**
       * @brief Gets the successor node
       */
      auto successor() -> Node*; // successor

      /**
       * @brief Gets the predecessor Node
       */
      auto decrement() -> Node*; // predecessor

      /**
       * @brief Prints to stream
       */
      auto print(std::ostream& os) const -> void;

    private:

      /**
       * @brief Clones a new node on the heap
       */
      auto clone() const -> Node*;

      /**
       * @brief Creates & adds a child with the given key and value
       */
      auto add_child(K key, V value) -> Node&;

      /**
       * @brief Recomputes the height (recurses up)
       */
      auto recalc_height() -> void;

      /**
       * @brief Key data
       */
      K key;

      /**
       * @brief Value data
       */
      V value;

      /**
       * @brief Pointer to the parent
       */
      Node* parent{nullptr};

      /**
       * @brief Height of this node
       */
      usize height;

      /**
       * @brief Balance of the node
       */
      i32 balance;

      /**
       * @brief Left child
       */
      Node* left{nullptr};

      /**
       * @brief Right child
       */
      Node* right{nullptr};

      friend class BSTmap;
    };

    /**
     * @class iterator
     * @brief Iterator for a non-const BST
     *
     */
    class iterator {
    public:

      /**
       * @brief Default / normal constructor
       */
      iterator(Node* p = nullptr);

      /**
       * @brief Pre-increment, move to the next
       */
      auto operator++() -> iterator&;

      /**
       * @brief Post-increment, returns the current and after move to the next
       */
      auto operator++(int) -> iterator;

      /**
       * @brief Gets the inner node
       */
      [[nodiscard]] auto operator*() const -> Node&;

      /**
       * @brief Gets the inner node
       */
      auto operator->() const -> Node*;

      /**
       * @brief Checks if this and another iterator are not equal
       */
      [[nodiscard]] auto operator!=(const iterator& rhs) const -> bool;

      /**
       * @brief Checks if this and another iterator are equal
       */
      [[nodiscard]] auto operator==(const iterator& rhs) const -> bool;

      friend class BSTmap;

    private:

      Node* node;
    };

    /**
     * @class const_iterator
     * @brief Iterator for a const BST
     */
    class const_iterator {
    public:

      /**
       * @brief Default/Normal Constructor
       */
      const_iterator(Node* p = nullptr);

      /**
       * @brief Pre-increment
       */
      auto operator++() -> const_iterator&;

      /**
       * @brief Post-increment
       */
      auto operator++(int) -> const_iterator;

      /**
       * @brief Gets a refereence to the inner node
       */
      auto operator*() const -> const Node&;

      /**
       * @brief Gets the inner node
       */
      auto operator->() const -> const Node*;

      /**
       * @brief Checks if this and another iter is not equal
       */
      auto operator!=(const const_iterator& rhs) const -> bool;

      /**
       * @brief Checks if this and another iter is equal
       */
      auto operator==(const const_iterator& rhs) const -> bool;

      friend class BSTmap;

    private:

      /**
       * @brief Pointer to the given node
       */
      Node* node;
    };

    /**
     * @brief Iterator at the end of every BST
     */
    static const iterator end_it;

    /**
     * @brief Const Iterator at the end of every BST
     */
    static const const_iterator const_end_it;

    /**
     * @brief Default constructor
     */
    BSTmap();

    /**
     * @brief Copy constructor
     *
     * @param rhs
     */
    BSTmap(const BSTmap& rhs);

    /**
     * @brief Move constructor
     *
     * @param from
     */
    BSTmap(BSTmap&& from);

    /**
     * @brief Copy assignment
     */
    auto operator=(const BSTmap& rhs) -> BSTmap&;

    /**
     * @brief Move assignment
     *
     * @param rhs
     */
    auto operator=(BSTmap&& rhs) -> BSTmap&;

    /**
     * @brief Destructor
     */
    virtual ~BSTmap();

    /**
     * @brief How many elements are in the tree
     */
    auto size() -> usize;

    /**
     * @brief Is the size 0
     */
    auto empty() -> bool;

    /**
     * @brief Value getter and setter, creates key if it does not exist
     */
    auto operator[](const K& key) -> V&;

    /**
     * @brief Beginning iterator (mutable)
     */
    auto begin() -> iterator;

    /**
     * @brief End iterator (mutable)
     */
    auto end() -> iterator;

    /**
     * @brief Attempts to find an iterator pointing to a node in this BST that
     * has the given key
     */
    auto find(const K& key) -> iterator;

    /**
     * @brief Attempts to erase the node represented by the given iterator
     */
    auto erase(iterator it) -> void;

    /**
     * @brief Beginning iterator (const)
     */
    auto begin() const -> const_iterator;

    /**
     * @brief End iterator (const)
     */
    auto end() const -> const_iterator;

    /**
     * @brief Attempts to find an iterator pointing to a node in this BST that
     * has the given key
     */
    auto find(const K& key) const -> const_iterator;

    // do not need this one (why)
    // const_iterator erase(iterator& it) const;

    /**
     * @brief Prints this to an output stream
     */
    auto print(std::ostream& os, bool print_value = false) const -> void;

    /**
     * @brief Gets the edge symbol character
     */
    auto getedgesymbol(const Node* node) const -> char;

    auto sanityCheck() -> bool;

    friend class iterator;
    friend class const_iterator;

  private:

    /**
     * @brief Gets how deep the given node is
     */
    [[nodiscard]] auto getdepth(const Node& node) const -> usize;

    /**
     * @brief Gets the node with the given key, or what its parent should be
     */
    auto index(Node* node, const K& key) const -> Node*;

    /**
     * @brief Root of the tree
     */
    Node* root = nullptr;

    /**
     * @brief Size of the tree
     */
    usize count = 0;
  };

  /**
   * @brief Prints out the bst map to the stream
   */
  template<typename K, typename V>
  auto operator<<(std::ostream& os, const BSTmap<K, V>& map) -> std::ostream&;
} // namespace CS280

#ifndef BSTMAP_CPP
#include "bst-map.cpp"
#endif
#endif
