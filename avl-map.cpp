#pragma once

#include <algorithm>
#include <utility>

#ifndef AVLMAP_H
#include "avl-map.h"
#endif

#ifndef AVLMAP_CPP
#define AVLMAP_CPP

#include <iostream>

namespace CS280 {

  // static data members
  template<typename K, typename V>
  const typename AVLmap<K, V>::iterator AVLmap<K, V>::end_it{
    nullptr,
  };

  template<typename K, typename V>
  const typename AVLmap<K, V>::const_iterator AVLmap<K, V>::const_end_it{
    nullptr,
  };

  template<typename K, typename V>
  AVLmap<K, V>::Node::Node(
    K key,
    V value,
    Node* parent,
    usize height,
    i32 balance,
    Node* left,
    Node* right
  ):
      key{key}, //
      value{value},
      parent{parent},
      height{height},
      balance{balance},
      left{left},
      right{right} {}

  template<typename K, typename V>
  AVLmap<K, V>::Node::~Node() {
    delete left;
    delete right;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::clone() const -> Node* {
    return new Node{
      key,
      value,
      parent,
      height,
      balance,
      left ? left->clone() : left,
      right ? right->clone() : right
    };
  }

  template<typename K, typename V>
  const K& AVLmap<K, V>::Node::Key() const {
    return key;
  }

  template<typename K, typename V>
  V& AVLmap<K, V>::Node::Value() {
    return value;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::first() -> Node* {
    Node* node = this;

    while (node->left) {
      node = node->left;
    }

    return node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::last() -> Node* {
    Node* node = this;

    while (node->right) {
      node = node->right;
    }

    return node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::successor() -> Node* {
    if (right) {
      return right->first();
    }

    Node* current = this;
    Node* prev = current->parent;

    while (prev and current == prev->right) {
      current = prev;
      prev = prev->parent;
    }

    return prev;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::decrement() -> Node* {
    if (left) {
      return left->last();
    }

    Node* current = this;
    while (current->parent and current == current->parent->left) {
      current = current->parent;
    }

    Node* predecessor = current->parent;

    return (predecessor and predecessor->key == key) ? nullptr : predecessor;
  }

  template<typename K, typename V>
  AVLmap<K, V>::Node::Node(Node&& from):
      key{std::move(from.key)},
      value{std::move(from.value)},
      height{std::exchange(from.height, 0)},
      balance{std::exchange(from.balance, 0)},
      left{std::exchange(from.left, nullptr)},
      right{std::exchange(from.right, nullptr)} {}

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::operator=(Node&& from) -> Node& {
    key = std::move(from.key);
    value = std::move(from.value);
    height = std::exchange(from.height, 0);
    balance = std::exchange(from.balance, 0);
    left = std::exchange(from.left, nullptr);
    right = std::exchange(from.right, nullptr);

    return *this;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::refresh_balance_and_height() -> void {
    height = 0;
    balance = 0;

    if (left) {
      height = left->height + 1;
      balance = left->balance;
    }

    if (right) {
      height = std::max(height, right->height + 1);
      balance -= right->balance;
    }

    if (parent) {
      parent->refresh_balance_and_height();
    }
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::add_child(K key, V value) -> Node& {
    Node* node = new Node{
      std::move(key),
      std::move(value),
      this, // parent
      0,
      0,
      nullptr, // left
      nullptr, // right
    };

    if (key < this->key) {
      left = node;
    } else {
      right = node;
    }

    node->refresh_balance_and_height();

    return *node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::Node::print(std::ostream& os) const -> void {
    os << value;
  }

  template<typename K, typename V>
  AVLmap<K, V>::iterator::iterator(Node* node): node{node} {}

  template<typename K, typename V>
  auto AVLmap<K, V>::iterator::operator++() -> iterator& {
    if (node == nullptr) {
      return *this;
    }

    node = node->successor();

    return *this;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::iterator::operator++(int) -> iterator {
    iterator iter{*this};
    operator++();
    return iter;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::iterator::operator*() const -> Node& {
    return *node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::iterator::operator->() const -> Node* {
    return node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::iterator::operator!=(const iterator& rhs) const -> bool {
    return node != rhs.node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::iterator::operator==(const iterator& rhs) const -> bool {
    return node == rhs.node;
  }

  template<typename K, typename V>
  AVLmap<K, V>::const_iterator::const_iterator(Node* p): node{p} {}

  template<typename K, typename V>
  auto AVLmap<K, V>::const_iterator::operator++() -> const_iterator& {
    if (node == nullptr) {
      return *this;
    }

    node = node->successor();

    return *this;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::const_iterator::operator++(int) -> const_iterator {
    const_iterator iter{*this};
    operator++();
    return iter;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::const_iterator::operator*() const -> const Node& {
    return *node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::const_iterator::operator->() const -> const Node* {
    return node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::const_iterator::operator!=( //
    const const_iterator& rhs
  ) const -> bool {
    return node != rhs.node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::const_iterator::operator==( //
    const const_iterator& rhs
  ) const -> bool {
    return node == rhs.node;
  }

  template<typename K, typename V>
  AVLmap<K, V>::AVLmap(): root{nullptr}, count{0} {}

  template<typename K, typename V>
  AVLmap<K, V>& AVLmap<K, V>::operator=(const AVLmap& rhs) {
    if (&rhs == this) {
      return *this;
    }

    delete root;

    count = rhs.count;
    root = rhs.root ? rhs.root->clone() : rhs.root;

    return *this;
  }

  template<typename K, typename V>
  AVLmap<K, V>& AVLmap<K, V>::operator=(AVLmap&& from) {
    delete root;

    count = std::exchange(from.count, 0);
    root = std::exchange(from.root, nullptr);

    return *this;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::size() -> usize {
    return count;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::empty() -> bool {
    return count == 0;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::operator[](const K& key) -> V& {
    if (empty()) {
      root = new Node{
        key,     // key
        V{},     // default value
        nullptr, // parent
        0,       // height
        0,       // balance
        nullptr, // left
        nullptr, // right
      };
      count++;
      return root->value;
    }

    Node* node = index(root, key);

    // proper node found
    if (node->key == key) {
      return node->value;
    }

    count++;
    return node->add_child(std::move(key), V{}).value;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::index(Node* node, const K& key) const -> Node* {
    if (node == nullptr) {
      return nullptr;
    }

    if (node->key == key) {
      return node;
    }

    // if on left
    if (key < node->key) {

      // if no left, return parent
      if (node->left == nullptr) {
        return node;
      }

      return index(node->left, key);
    }

    // if on right

    // if right is none, reutnr parent
    if (node->right == nullptr) {
      return node;
    }

    return index(node->right, key);
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::balanced_index(Node* node, const K& key) const -> Node* {
    if (node == nullptr) {
      return nullptr;
    }

    if (node->key == key) {
      return node;
    }

    // if on left
    if (key < node->key) {

      // if no left, return parent
      if (node->left) {
        return index(node->left, key);
      }

      return node;
    }

    // if on right

    // if right is none, reutnr parent
    if (node->right) {
      return index(node->right, key);
    }

    return node;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::end() -> iterator {
    return end_it;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::find(const K& key) -> iterator {
    Node* node = index(root, key);

    return (node and node->key == key) ? iterator{node} : end();
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::erase(iterator it) -> void {
    if (it == end()) {
      return;
    }

    count--;

    Node* const to_erase = it.node;

    Node* const parent = to_erase->parent;

    Node* left = std::exchange(to_erase->left, nullptr);
    Node* right = std::exchange(to_erase->right, nullptr);

    if (not parent and not left and not right) {
      root = nullptr;
      delete to_erase;
      return;
    }

    if (parent == nullptr) {
      delete to_erase;
      Node* other = nullptr;

      if (left) {
        root = left;
        other = right;
      } else {
        root = right;
        other = left;
      }
      root->parent = nullptr;

      if (other == nullptr) {
        return;
      }

      Node* other_parent = index(root, other->key);

      other->parent = other_parent;
      if (other_parent->key > other->key) {
        other_parent->left = other;
      } else {
        other_parent->right = other;
      }
      other->refresh_balance_and_height();

      return;
    }

    // erase node from parent's
    if (parent->left == to_erase) {
      parent->left = nullptr;
    } else {
      parent->right = nullptr;
    }

    delete to_erase;

    if (left == nullptr and right == nullptr) {
      parent->refresh_balance_and_height();
      return;
    }

    if (left) {
      Node* left_parent = index(parent, left->key);
      left->parent = left_parent;
      if (left_parent->key > left->key) {
        left_parent->left = left;
      } else {
        left_parent->right = left;
      }
      left->refresh_balance_and_height();
    }

    if (right) {
      Node* right_parent = index(parent, right->key);
      right->parent = right_parent;
      if (right_parent->key > right->key) {
        right_parent->left = right;
      } else {
        right_parent->right = right;
      }

      right->refresh_balance_and_height();
    }
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::begin() const -> const_iterator {
    return root ? const_iterator{root->first()} : end();
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::end() const -> const_iterator {
    return end_it;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::find(const K& key) const -> const_iterator {
    Node* node = index(root, key);
    return (node and node->key == key) ? iterator{node} : end();
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::sanityCheck() -> bool {
    return true;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::rotate_left(Node* node) -> Node* {
    Node*& tree = node_ref(*node);
    Node* const temp = tree;
    tree = tree->right;
    temp->right = tree->left;
    tree->left = temp;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::rotate_right(Node* node) -> Node* {
    Node*& tree = node_ref(*node);
    Node* const temp = tree;
    tree = tree->left;
    temp->left = tree->right;
    tree->right = temp;
  }

  template<typename K, typename V>
  [[nodiscard]] auto AVLmap<K, V>::node_ref(Node& node) -> Node*& {
    Node* parent = node.parent;

    if (parent == nullptr) {
      return root;
    }

    return (parent->left == &node) ? parent->left : parent->right;
  }

  template<typename K, typename V>
  AVLmap<K, V>::AVLmap(const AVLmap& rhs):
      root{
        rhs.root ? rhs.root->clone() : nullptr,
      },
      count{rhs.count} {}

  template<typename K, typename V>
  AVLmap<K, V>::AVLmap(AVLmap&& from):
      root{std::exchange(from.root, nullptr)},
      count{std::exchange(from.count, 0)} {}

  template<typename K, typename V>
  AVLmap<K, V>::~AVLmap() {
    delete root;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::begin() -> iterator {
    return root ? iterator{root->first()} : end();
  }

  ////////////////////////////////////////////////////////////
  // do not change this code from here to the end of the file
  /* figure out whether node is left or right child or root
   * used in print_backwards_padded
   */
  template<typename K, typename V>
  auto AVLmap<K, V>::getedgesymbol(const Node* node) const -> char {
    const Node* parent = node->parent;

    if (parent == nullptr) {
      return '-';
    }

    return parent->left == node ? '\\' : '/';
  }

  /* this is another "ASCII-graphical" print, but using
   * iterative function.
   * Left branch of the tree is at the bottom
   */
  template<typename K, typename V>
  auto operator<<(std::ostream& os, const AVLmap<K, V>& map) -> std::ostream& {
    map.print(os);
    return os;
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::print(std::ostream& os, bool print_value) const -> void {
    if (root) {
      AVLmap<K, V>::Node* b = root->last();
      while (b) {
        int depth = getdepth(*b);
        int i;
        /* printf(b); */

        char edge = getedgesymbol(b);
        switch (edge) {
          case '-':
            for (i = 0; i < depth; ++i) {
              std::printf("       ");
            }
            os << b->key;
            if (print_value) {
              os << " -> " << b->value;
            }
            os << std::endl;
            break;
          case '\\':
            for (i = 0; i < depth; ++i) {
              std::printf("       ");
            }
            os << edge << std::endl;
            for (i = 0; i < depth; ++i) {
              std::printf("       ");
            }
            os << b->key;
            if (print_value) {
              os << " -> " << b->value;
            }
            os << std::endl;
            break;
          case '/':
            for (i = 0; i < depth; ++i) {
              std::printf("       ");
            }
            os << b->key;
            if (print_value) {
              os << " -> " << b->value;
            }
            os << std::endl;
            for (i = 0; i < depth; ++i) {
              std::printf("       ");
            }
            os << edge << std::endl;
            break;
        }
        b = b->decrement();
      }
    }
    std::printf("\n");
  }

  template<typename K, typename V>
  auto AVLmap<K, V>::getdepth(const Node& node) const -> usize {
    return root->height - node.height;
  }
} // namespace CS280

#endif
