#pragma once

namespace Mix
{

  namespace Private {}
  namespace
  {
  
    namespace Private
    {
      
      template<class Node>
      class NodeBase
      {
      private:
        using node_type          = Node;
        using node_pointer       = node_type*;
//        using node_const_pointer = const node_type*;

      public:
        NodeBase() : prev_(nullptr) , next_(nullptr) {}
        
      protected:
        node_pointer prev_;
        node_pointer next_;
        
      }; // class NodeBase<Node>
      
    } // namespace Private
    
    template<typename T>
    class Node
    : public Private::NodeBase<Node<T>>
    {
    public:
      using value_type         = T;
      using reference          = value_type&;
      using const_reference    = const value_type&;
      using pointer            = value_type*;
      using const_pointer      = const value_type*;
      
    private:
      value_type   data_;
      
      template<typename, std::size_t>
      friend class TwinArray;
      
    }; // class Node<T>
    
    //////////////////////////////////////////////////////////////////
    // 最大確保個数固定の二面双方向リンク配列
    //////////////////////////////////////////////////////////////////
    template<typename T, std::size_t MaxSize>
    class TwinArray
    {
    private:
      using node_type       = Node<T>;
      using node_pointer    = node_type*;
      
    public:
      using value_type      = typename node_type::value_type;
      using reference       = typename node_type::reference;
      using const_reference = typename node_type::const_reference;
      using pointer         = typename node_type::pointer;
      using const_pointer   = typename node_type::const_pointer;

    public:
      struct iterator
      {
      public:
        friend TwinArray<T, MaxSize>;
        
      public:
        using value_type        = TwinArray::value_type;
        using reference         = TwinArray::reference;
        using const_reference   = TwinArray::const_reference;
        using pointer           = TwinArray::pointer;
        using const_pointer     = TwinArray::const_pointer;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        
      public:
        iterator() : p_(nullptr) { // construct with null node pointer
        }
//        iterator(node_pointer p) : p_(p) {
//        }
        iterator(void* p) : p_(static_cast<node_pointer>(p)) {
        }
        
        reference operator*() const { // return designated value
          return this->p_->data_;
        }
        pointer operator->() const { // return pointer to class object
          return &(*(*this));
        }
        
        iterator& operator++() { // preincrement
          this->p_ = this->p_->next_;
          return *this;
        }
        iterator operator++(int) { // postincrement
          iterator _Tmp = *this;
          ++* this;
          return _Tmp;
        }
        iterator& operator--() { // predecrement
          const auto _New_ = this->p_->prev_;
          this->p_ = _New_;
          return *this;
        }
        iterator operator--(int) { // postdecrement
          iterator _Tmp = *this;
          --* this;
          return _Tmp;
        }
        
        bool operator==(const iterator& _Right) const { // test for iterator equality
          return this->p_ == _Right.p_;
        }
        bool operator!=(const iterator& _Right) const { // test for iterator inequality
          return !(*this == _Right);
        }
        
        bool operator!() const {
          return nullptr == this->p_;
        }
        
      public:
        std::ptrdiff_t Diff() {
          return reinterpret_cast<int64_t>(&(**this)) - reinterpret_cast<int64_t>(p_);
        }
        
      private:
        node_pointer p_;
        
      };
      
    private:
      node_type nodes_[MaxSize]; // 固定メモリ領域の最先端を常に指し示す

      node_pointer used_; // 使用中メモリ領域の最後尾ノードのひとつあとのダミーノード。循環リンクにする
      node_pointer free_; // 未使用メモリ領域の最後尾ノードのひとつあとのダミーノード。循環リンクにする

    private:
      std::size_t num_; // 実行時取り扱っている要素数（常時変動）
      
    public:
      // 本クラスを範囲forなど、コンテナの形で扱うことができるようにする
      iterator begin() const noexcept { return iterator(used_->next_); }
      iterator end() const noexcept { return iterator(used_); }
      iterator free_begin() const noexcept { return iterator(free_->next_); }
      iterator free_end() const noexcept { return iterator(free_); }

    public:
      bool empty() const noexcept { return begin() == end(); } // ダミーの次もダミー
      bool full() const noexcept { return free_begin() == free_end(); } // ダミーの次もダミー
      std::size_t max_size() const noexcept { return MaxSize; }
      std::size_t size() const noexcept { return num_; }

    public:
      iterator insert(iterator position, const T& val) noexcept
      {
        if (full()) return iterator(nullptr);
        iterator input = free_begin(); // 空列より未初期化対象を獲得
        iterator result = LinkTo(position, input); // resultはinputを指す
        if (!result) return iterator(nullptr);
        
        *result = val;
        num_++;
        return result;
      }
      void push_back(const T& val) { insert(end(), val); }
      void push_front(const T& val) { insert(begin(), val); }

      iterator erase(iterator target) noexcept
      {
        if (empty()) return iterator(nullptr);
        iterator position = free_end(); // positionはtargetを挿入する先の空列側を指す
        
        iterator result = target;
        ++result;
        
        LinkTo(position, target); // free_end()による空列最後尾にtargetを連結しerase完了
        // このときLinkToの戻り値は削除対象だったtargetを指しているため空列側。
        
        num_--;
        return result; // 予め退避しておいたresultを返すのが正解
      }
      void pop_back() noexcept { iterator it = end(); erase(--it); }
      void pop_front() noexcept { erase(begin()); }

      value_type back() const noexcept { iterator it = end(); return *(--it); }
      value_type front() const noexcept { return *(begin()); }

    public:
      // 上限要素個数をnumに指定して初期化する
      template<typename... TInit>
      explicit TwinArray(const int sz = 0, TInit... args) noexcept
        : num_(sz)
        , used_(new node_type) // ダミー。work_領域が不要でplacement newしない
        , free_(new node_type) // 同上
      {
        assert(0 <= num_);
        assert(num_ <= MaxSize);

        // ワークメモリ初期化(work_確保)
        for (int i = 0; i < MaxSize; i++)
        {
          //new(nodes_ + i) Node();

          // used_ と free_はワーク領域不要のためplacement newで確保しなくてよい
        } // ワーキングメモリ確保完了

        { // リンクリストの連結

          // 最後尾のインデックス
          const int c = MaxSize - 1;

          // 各ノードの前後連結リンクを結ぶ
          for (int i = 0; i < c; i++)
          {
            nodes_[i].next_ = &nodes_[i + 1];
            nodes_[i + 1].prev_ = &nodes_[i];
          }

          // ダミーノードを結ぶ
          used_->next_ = used_;      // 開始時はダミーを指し示す
          used_->prev_ = used_;      // 循環させる

          free_->next_ = &nodes_[0]; // 自由領域はノード列の最初
          free_->prev_ = &nodes_[c]; // 自由領域の末尾から循環

          nodes_[0].prev_ = free_;
          nodes_[c].next_ = free_;

        } // 連結完了

        { // 要素初期化
          for (unsigned int i = 0; i < num_; i++)
          {
            this->push_back(T(args...));
          }
        }

      }
      ~TwinArray() noexcept
      {
        for (int i = 0; i < MaxSize; i++)
        {
          nodes_[i].data_.~T();
        }
        //if (nodes_) free(nodes_);
        delete used_;
        delete free_;
      }

    public:
      // positionの位置にinputが入り、position含めそれ以降はリンクがひとつずつ後ろへずれる
      // 注意点としては、リンクリストをLinkToでつなぎかえた後、
      // イテレータがfreeリストをさしている可能性があるということ。
      iterator LinkTo(iterator position, iterator input) noexcept
      {
        // 移動元リンク列が空列であればエラーのためnullptrを返す
        if (input.p_->next_ == input.p_)
          return nullptr;

        // inputを移動元リンク列から離脱
        input.p_->next_->prev_ = input.p_->prev_;
        input.p_->prev_->next_ = input.p_->next_;

        // 内リンクをはる
        input.p_->next_ = position.p_;
        input.p_->prev_ = position.p_->prev_;

        // 外リンクをはる
        position.p_->prev_->next_ = input.p_;
        position.p_->prev_ = input.p_;

        return input;
      }

    }; // class TwinArray

  } // namespace API

} // namespace Mix
