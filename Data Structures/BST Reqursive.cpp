/**
 * @file bst.hpp
 * @author yahya mohammed (goldroger.1993@outlook.com)
 * @date 2021-07-7 (date started)
 */

#ifndef BST_HPP
#define BST_HPP

// includes

#include <initializer_list>
#include <iostream>
#include <memory>
#include <stack>

template <class Ty> class bst_ {
  //
  struct node {
    using sh_ptr = std::shared_ptr<node>;
    //
    Ty m_data{};
    sh_ptr m_left{};
    sh_ptr m_right{};
    node *m_parent{}; // to hold parent of each node
    //
    explicit constexpr node(const Ty &val, const sh_ptr &left,
                            const sh_ptr &right, node *parent) noexcept
        : m_data(val), m_left(left), m_right(right), m_parent(parent) {}
    //
    explicit constexpr node(Ty &&val, sh_ptr &&left, sh_ptr &&right,
                            node *parent) noexcept
        : m_data(val), m_left(left), m_right(right), m_parent(parent) {}
  }; // end of struct node

private:
  using sh_ptr = std::shared_ptr<node>;
  // allocators
  constexpr auto allocate(const Ty &val, const sh_ptr &left,
                          const sh_ptr &right, node *par) -> sh_ptr {
    return std::make_shared<node>(val, left, right, par);
  }
  constexpr auto allocate(Ty &&val, sh_ptr &&left, sh_ptr &&right, node *par)
      -> sh_ptr {
    return std::make_shared<node>(val, left, right, par);
  }
  //
private:
  sh_ptr m_root{};
  std::size_t m_size{};
  Ty _failed_{};
  //
public:
  class bst_itr {
  private:
    friend class bst_<Ty>;
    node *root_itr{};
    bst_<Ty> *tree{};
    bst_itr(const node *&ptr, const bst_<Ty> *tree_ptr)
        : root_itr(ptr), tree(tree_ptr) {}
    //
  public:
    constexpr bst_itr() : root_itr(nullptr) {}
    constexpr bst_itr(const node *&copy) : root_itr(copy) {}
    constexpr bst_itr(node *&&move) : root_itr(move) {}
    constexpr bst_itr(const sh_ptr &copy) : root_itr(copy.get()) {}
    constexpr bst_itr(sh_ptr &&move) : root_itr(move.get()) {}
    constexpr bst_itr(std::nullptr_t &&move) : root_itr(move) {}
    constexpr bst_itr(const std::nullptr_t &copy) : root_itr(copy) {}

    // foo++ in-order
    constexpr bst_itr operator++(int) {
      node *temp{};
      if (root_itr == nullptr) {
        // ++ from end(). get the root of the tree
        root_itr = tree->m_root.get();
        // error! ++ requested for an empty tree
        // move to the smallest value in the tree,
        // which is the first node in order
        while (root_itr->m_left != nullptr) {
          root_itr = root_itr->m_left.get();
        }
      } else if (root_itr->m_right != nullptr) {
        // successor is the farthest left node of right subtree
        root_itr = root_itr->m_right.get();
        while (root_itr->m_left != nullptr) {
          root_itr = root_itr->m_left.get();
        }
      } else {
        // have already processed the left subtree, and
        // there is no right subtree. move up the tree,
        // looking for a parent for which nodePtr is a left child,
        // stopping if the parent becomes NULL. a non-NULL parent
        // is the successor. if parent is NULL, the original node
        // was the last node in order, and its successor
        // is the end of the list
        temp = root_itr->m_parent;
        while (temp != nullptr && root_itr == temp->m_right.get()) {
          root_itr = temp;
          temp = temp->m_parent;
        }
        // if we were previously at the right-most node in
        // the tree, nodePtr = nullptr, and the iterator specifies
        // the end of the list
        root_itr = temp;
      }
      return *this;
    }
    // ++foo // in-order
    // many thanks to:
    // https://www.cs.odu.edu/~zeil/cs361/latest/Public/treetraversal
    constexpr bst_itr operator++() {
      node *temp{};
      if (root_itr == nullptr) {
        // ++ from end(). get the root of the tree
        root_itr = tree->m_root.get();
        // error! ++ requested for an empty tree
        // move to the smallest value in the tree,
        // which is the first node in order
        while (root_itr->m_left != nullptr) {
          root_itr = root_itr->m_left.get();
        }
      } else if (root_itr->m_right != nullptr) {
        // successor is the farthest left node of right subtree
        root_itr = root_itr->m_right.get();
        while (root_itr->m_left != nullptr) {
          root_itr = root_itr->m_left.get();
        }
      } else {
        // have already processed the left subtree, and
        // there is no right subtree. move up the tree,
        // looking for a parent for which nodePtr is a left child,
        // stopping if the parent becomes NULL. a non-NULL parent
        // is the successor. if parent is NULL, the original node
        // was the last node in order, and its successor
        // is the end of the list
        temp = root_itr->m_parent;
        while (temp != nullptr && root_itr == temp->m_right.get()) {
          root_itr = temp;
          temp = temp->m_parent;
        }
        // if we were previously at the right-most node in
        // the tree, nodePtr = nullptr, and the iterator specifies
        // the end of the list
        root_itr = temp;
      }
      return *this;
    }
    // get data
    constexpr Ty &operator*() const { return root_itr->m_data; }
    // get and modify data
    constexpr Ty &operator*() { return root_itr->m_data; }
    constexpr bool operator!=(const bst_itr &rhs) const {
      return root_itr != rhs.root_itr;
    }
    constexpr bool operator==(const bst_itr &rhs) const {
      return root_itr == rhs.root_itr;
    }
    //
  }; // end of class iterator
  //
public:
  //
  using const_iterator = bst_itr;
  using iterator = const_iterator;

  constexpr bst_() noexcept : m_root{nullptr} {};

  constexpr bst_(const bst_<Ty> &rhs) noexcept {
    m_root = rhs.m_root;
    m_size = rhs.m_size;
  }

  constexpr bst_(bst_<Ty> &&rhs) noexcept
      : m_root(std::move(rhs.m_root)), m_size(std::move(rhs.m_size)) {
    rhs.m_root.reset();
    rhs.m_size = 0;
  }

  /**
   * @brief returns an iterator to the node if found, end() if not
   *
   * @param node you are looking for
   * @return const const_iterator
   */
  [[nodiscard]] constexpr auto find(Ty &&node) const -> const const_iterator {
    if (empty()) {
      return end();
    }
    auto find_hidden = [this](Ty &&node, const sh_ptr &root,
                              auto &&lambda) -> const const_iterator {
      if (!root)
        return end(); // not found
      if (node > root->m_data) {
        return lambda(std::move(node), root->m_right, std::move(lambda));
      } else if (node < root->m_data) {
        return lambda(std::move(node), root->m_left, lambda);
      } else
        return root;
      return end();
    };
    return find_hidden(std::move(node), m_root, std::move(find_hidden));
  }

  /**
   * @brief returns an iterator to the node if found, end() if not
   *
   * @param node you are looking for
   * @return iterator
   */
  [[nodiscard]] constexpr auto find(Ty &&node) -> iterator {
    if (empty()) {
      return end();
    }
    auto find_hidden = [this](Ty &&node, const sh_ptr &root,
                              auto &&lambda) -> iterator {
      if (!root)
        return end(); // not found
      if (node > root->m_data) {
        return lambda(std::move(node), root->m_right, std::move(lambda));
      } else if (node < root->m_data) {
        return lambda(std::move(node), root->m_left, lambda);
      } else
        return root;
    };
    return find_hidden(std::move(node), m_root, std::move(find_hidden));
  }

  /**
   * @brief returns an iterator to the node if found, end() if not
   *
   * @param node you are looking for
   * @return const const_iterator
   */
  [[nodiscard]] constexpr auto find(const Ty &node) const
      -> const const_iterator {
    if (empty()) {
      return end();
    }
    auto find_hidden = [this](const Ty &node, const sh_ptr &root,
                              auto &&lambda) -> const const_iterator {
      if (!root)
        return end(); // not found
      if (node > root->m_data) {
        return lambda(node, root->m_right, std::move(lambda));
      } else if (node < root->m_data) {
        return lambda(node, root->m_left, lambda);
      } else
        return root;
    };
    return find_hidden(node, m_root, std::move(find_hidden));
  }

  /**
   * @brief returns an iterator to the node if found, end() if not
   *
   * @param node you are looking for
   * @return iterator
   */
  [[nodiscard]] constexpr auto find(const Ty &node) -> iterator {
    if (empty()) {
      return end();
    }
    auto find_hidden = [this](const Ty &node, const sh_ptr &root,
                              auto &&lambda) -> iterator {
      if (!root)
        return end(); // not found
      if (node > root->m_data) {
        return lambda(node, root->m_right, std::move(lambda));
      } else if (node < root->m_data) {
        return lambda(node, root->m_left, lambda);
      } else
        return root;
    };
    return find_hidden(node, m_root, std::move(find_hidden));
  }

  /**
   * @brief returns an iterator to the left-most node if tree is constructed,
   * end() if not
   *
   * @return const_iterator
   */
  [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator {
    if (empty()) {
      return end();
    }
    return const_iterator(min(m_root));
  }

  /**
   * @brief returns an iterator to the left-most node if tree is constructed,
   * end() if not
   *
   * @return iterator
   */
  [[nodiscard]] constexpr auto begin() noexcept -> iterator {
    if (empty()) {
      return end();
    }
    return iterator(min(m_root));
  }

  /**
   * @brief returns `nullptr`
   *
   * @return const_iterator
   */
  [[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
    return const_iterator(nullptr);
  }
  //

  /// variadic inserting
  template <class... arg> explicit constexpr bst_(arg &&...values) noexcept {
    (insert(std::move(values)), ...);
  }
  template <class... arg>
  explicit constexpr bst_(const arg &...values) noexcept {
    (insert(values), ...);
  }
  // init-list inserting
  explicit constexpr bst_(std::initializer_list<Ty> &&values) noexcept {
    for (auto &&i : values) {
      insert(i);
    }
  }
  explicit constexpr bst_(const std::initializer_list<Ty> &values) noexcept {
    for (const auto &i : values) {
      insert(i);
    }
  }
  //
  /**
   * @brief checks if the tree is empty
   * @return true
   * @return false
   */
  [[nodiscard]] constexpr auto empty() const -> bool {
    return m_root == nullptr;
  }

  /**
   * @brief exactly!!
   * @return std::size_t
   */
  [[nodiscard]] constexpr auto size() const -> std::size_t { return m_size; }

  /**
   * @brief insert an element to BSTree also supply `parent`
   * @param val
   */
  constexpr auto insert(Ty &&val) -> void {
    ++m_size;
    auto insert_hidden = [this](Ty &&val, sh_ptr &root, const node *par,
                                auto &&lambda) -> void {
      if (!root) {
        root = allocate(std::move(val), std::move(nullptr), std::move(nullptr),
                        par);
      } else if (val > root->m_data) {
        lambda(std::move(val), root->m_right, root.get(), std::move(lambda));
      } else if (val < root->m_data) {
        lambda(std::move(val), root->m_left, root.get(), lambda);
      } else
        ;
    };
    insert_hidden(std::move(val), m_root, nullptr, std::move(insert_hidden));
  }

  /**
   * @brief insert an element to BSTree also supply `parent`
   * @param val
   */
  constexpr auto insert(const Ty &val) -> void {
    ++m_size;
    auto insert_hidden = [this](const Ty &val, sh_ptr &root, node *par,
                                auto &&lambda) -> void {
      if (!root) { // root == nullptr
        root = allocate(val, nullptr, nullptr, par);
      } else if (val > root->m_data) {
        lambda(val, root->m_right, root.get(), std::move(lambda));
      } else if (val < root->m_data) {
        lambda(val, root->m_left, root.get(), lambda);
      } else
        ;
    };
    insert_hidden(val, m_root, nullptr, std::move(insert_hidden));
  }

  /**
   * @brief you what is does
   * @param order `1` for pre-order
   * @param order `2` for in-order
   * @param order `3` for post-order
   */
  constexpr auto print(const int order) const -> void {
    if (empty()) {
      return;
    }
    //
    constexpr auto print_hidden = [](const sh_ptr &root, const int order,
                                     auto &&lambda) -> void {
      if (order == 1 && root != nullptr) {
        std::cout << root->m_data << ' ';
        lambda(root->m_left, order, std::move(lambda));
        lambda(root->m_right, order, lambda);
      } else if (order == 2 && root != nullptr) {
        lambda(root->m_left, order, std::move(lambda));
        std::cout << root->m_data << ' ';
        lambda(root->m_right, order, lambda);
      } else if (order == 3 && root != nullptr) {
        lambda(root->m_left, order, std::move(lambda));
        lambda(root->m_right, order, lambda);
        std::cout << root->m_data << ' ';
      }
    };
    print_hidden(m_root, order, std::move(print_hidden));
  }

  /**
   * @brief find if certain element exits int BSTree
   * @param val
   * @return true
   * @return false
   */
  [[nodiscard]] constexpr auto contains(Ty &&val) const -> bool {
    if (empty()) {
      return false;
    }
    //
    constexpr auto contains_hidden = [](Ty &&val, const sh_ptr &root,
                                        auto &&lambda) -> bool {
      if (!root) {
        return false;
      } else if (val > root->m_data) {
        return lambda(std::move(val), root->m_right, std::move(lambda));
      } else if (val < root->m_data) {
        return lambda(std::move(val), root->m_left, lambda);
      } else
        return true; // found
      return false;
    };
    return contains_hidden(std::move(val), m_root, std::move(contains_hidden));
  }

  /**
   * @brief find if certain element exits int BSTree
   * @param val
   * @return true
   * @return false
   */
  [[nodiscard]] constexpr auto contains(const Ty &val) const -> bool {
    if (empty()) {
      return false;
    }
    //
    constexpr auto contains_hidden = [](const Ty &val, const sh_ptr &root,
                                        auto &&lambda) -> bool {
      if (!root) {
        return false;
      } else if (val > root->m_data) {
        return lambda(val, root->m_right, std::move(lambda));
      } else if (val < root->m_data) {
        return lambda(val, root->m_left, lambda);
      } else
        return true; // found
      return false;
    };
    return contains_hidden(val, m_root, std::move(contains_hidden));
  }

  /**
   * @brief get maximum element in BSTree
   * @return Ty
   */
  [[nodiscard]] constexpr auto max() const -> Ty {
    if (empty()) {
      return _failed_;
    }
    //
    constexpr auto max_hidden = [](const sh_ptr &root,
                                   auto &&lambda) -> sh_ptr {
      if (!root) {
        return nullptr;
      }
      if (!root->m_right) {
        return root;
      }
      return lambda(root->m_right, std::move(lambda));
    };
    return max_hidden(m_root, std::move(max_hidden))->m_data;
  }
  /**
   * @brief get minimum element in BSTree
   * @return const Ty
   */
  [[nodiscard]] constexpr auto min() -> Ty {
    if (empty()) {
      return _failed_;
    }
    //
    return min(m_root)->m_data;
  }

  /**
   * @brief remove a node from the BSTree
   * @param val the node to be removed
   */
  constexpr auto remove(Ty &&val) -> void {
    if (empty()) {
      return;
    }
    //
    --m_size;
    auto remove_hidden = [this](Ty &&val, sh_ptr &root, auto &&lambda) -> void {
      if (!root) {
        return;
      } else if (val > root->m_data) {
        lambda(std::move(val), root->m_right, std::move(lambda));
      } else if (val < root->m_data) {
        lambda(std::move(val), root->m_left, lambda);
      } else if (root->m_right != nullptr &&
                 root->m_left != nullptr) { // Has two children
        // get the min of right sub-tree
        root->m_data = min(root->m_right)->m_data;
        lambda(std::move(root->m_data), root->m_right, lambda);
      } else { // Node has one leaf
        sh_ptr old_node = root;
        root = (root->m_left != nullptr) ? root->m_left : root->m_right;
        old_node.reset();
      }
    };
    remove_hidden(std::move(val), m_root, std::move(remove_hidden));
  }

  /**
   * @brief remove a node from the BSTree
   * @param val the node to be removed
   */
  constexpr auto remove(const Ty &val) -> void {
    if (empty()) {
      return;
    }
    //
    --m_size;
    auto remove_hidden = [this](const Ty &val, sh_ptr &root,
                                auto &&lambda) -> void {
      if (!root) {
        return;
      } else if (val > root->m_data) {
        lambda(val, root->m_right, std::move(lambda));
      } else if (val < root->m_data) {
        lambda(val, root->m_left, lambda);
      } else if (root->m_right != nullptr &&
                 root->m_left != nullptr) { // Has two children
        root->m_data = min(root->m_right)->m_data;
        lambda(root->m_data, root->m_right, std::move(lambda));
      } else {
        sh_ptr old_node = root;
        root = (root->m_left != nullptr) ? root->m_left : root->m_right;
        old_node.reset();
      }
    };
    remove_hidden(val, m_root, std::move(remove_hidden));
  }

  /**
   * @brief get the height/depth of given node number in the BSTree
   * @param node node number
   * @return std::size_t
   */
  constexpr auto depth(Ty &&node) const -> std::size_t {
    if (empty()) {
      return 0ull;
    }
    //
    constexpr auto depth_hidden = [](const sh_ptr &root, Ty &&node,
                                     std::size_t &&height,
                                     auto &&lambda) -> std::size_t {
      if (!root) {
        return 0ull;
      } else if (root->m_data == node) {
        return height;
      }
      return static_cast<std::size_t>(std::max(
          lambda(root->m_left, std::move(node), height + 1ull,
                 std::move(lambda)),
          lambda(root->m_right, std::move(node), height + 1ull, lambda)));
    };
    //
    return depth_hidden(m_root, std::move(node), 0ull, std::move(depth_hidden));
  }

  /**
   * @brief get the height/depth of given node number in the BSTree
   * @param node node number
   * @return std::size_t
   */
  constexpr auto depth(const Ty &node) const -> std::size_t {
    if (empty()) {
      return 0ull;
    }
    //
    constexpr auto depth_hidden = [](const sh_ptr &root, const Ty &node,
                                     std::size_t &&height,
                                     auto &&lambda) -> std::size_t {
      if (!root) {
        return 0ull;
      } else if (root->m_data == node) {
        return height;
      }
      return static_cast<std::size_t>(
          std::max(lambda(root->m_left, node, height + 1ull, std::move(lambda)),
                   lambda(root->m_right, node, height + 1ull, lambda)));
    };
    //
    return depth_hidden(m_root, node, 0ull, std::move(depth_hidden));
  }

  /**
   * @brief Get an iterator to the parent of node if exits, end() if not
   * @param val the node you want it's parent
   * @return const_iterator
   */
  [[nodiscard]] constexpr auto get_parent(Ty &&val) const -> const_iterator {
    if (empty()) {
      return end();
    }
    if (find(val) == end() || val == m_root->m_data) {
      return end();
    }
    // return iterator to val
    const auto val_itr = find(val);
    // return iterator to val's parent
    return const_iterator(std::move(val_itr.root_itr->m_parent));
  }

  /**
   * @brief Get an iterator to the parent of node if exits, end() if not
   * @param val the node you want it's parent
   * @return const_iterator
   */
  [[nodiscard]] constexpr auto get_parent(const Ty &val) const
      -> const_iterator {
    if (empty()) {
      return end();
    }
    if (find(val) == end() || val == m_root->m_data) {
      return end();
    }
    // return iterator to val
    const auto val_itr = find(val);
    // return iterator to val's parent
    return const_iterator(std::move(val_itr.root_itr->m_parent));
  }

  /**
   * @brief makes the tree empty
   * @brief make sure to not use it twice without some safety chekcs
   * @brief make sure to not use any other stuff related to the burned tree
   * after calling this
   */
  constexpr auto burn_tree() -> void {
    if (empty()) {
      return;
    }
    m_root.reset();
  }

  //
private:
  // hidden function
  constexpr auto min(const sh_ptr &root) -> sh_ptr {
    if (!root) {
      return nullptr;
    }
    if (!root->m_left) {
      return root;
    }
    return min(root->m_left);
  }

  // operators on tree
public:
  constexpr bst_<Ty> &operator=(const bst_<Ty> &rhs) noexcept {
    if (this != &rhs) {
      m_root = rhs.m_root;
      m_size = rhs.m_size;
    }
    return *this;
  }

  constexpr bst_<Ty> &operator=(bst_<Ty> &&rhs) noexcept {
    if (this != &rhs) {
      m_root = std::move(rhs.m_root);
      m_size = std::move(rhs.m_size);
      //
      rhs.m_root.reset();
      rhs.m_size = 0;
    }
    return *this;
  }

  // prints elements in-order
  constexpr friend std::ostream &operator<<(std::ostream &os,
                                            const bst_ &tree) {
    tree.print(2);
    return os;
  }
};

#endif // BST_HPP