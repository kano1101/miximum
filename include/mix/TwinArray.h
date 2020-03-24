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
    // �ő�m�ی��Œ�̓�ʑo���������N�z��
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
      node_type nodes_[MaxSize]; // �Œ胁�����̈�̍Ő�[����Ɏw������

      node_pointer used_; // �g�p���������̈�̍Ō���m�[�h�̂ЂƂ��Ƃ̃_�~�[�m�[�h�B�z�����N�ɂ���
      node_pointer free_; // ���g�p�������̈�̍Ō���m�[�h�̂ЂƂ��Ƃ̃_�~�[�m�[�h�B�z�����N�ɂ���

    private:
      std::size_t num_; // ���s����舵���Ă���v�f���i�펞�ϓ��j
      
    public:
      // �{�N���X��͈�for�ȂǁA�R���e�i�̌`�ň������Ƃ��ł���悤�ɂ���
      iterator begin() const noexcept { return iterator(used_->next_); }
      iterator end() const noexcept { return iterator(used_); }
      iterator free_begin() const noexcept { return iterator(free_->next_); }
      iterator free_end() const noexcept { return iterator(free_); }

    public:
      bool empty() const noexcept { return begin() == end(); } // �_�~�[�̎����_�~�[
      bool full() const noexcept { return free_begin() == free_end(); } // �_�~�[�̎����_�~�[
      std::size_t max_size() const noexcept { return MaxSize; }
      std::size_t size() const noexcept { return num_; }

    public:
      iterator insert(iterator position, const T& val) noexcept
      {
        if (full()) return iterator(nullptr);
        iterator input = free_begin(); // ����薢�������Ώۂ��l��
        iterator result = LinkTo(position, input); // result��input���w��
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
        iterator position = free_end(); // position��target��}�������̋�񑤂��w��
        
        iterator result = target;
        ++result;
        
        LinkTo(position, target); // free_end()�ɂ����Ō����target��A����erase����
        // ���̂Ƃ�LinkTo�̖߂�l�͍폜�Ώۂ�����target���w���Ă��邽�ߋ�񑤁B
        
        num_--;
        return result; // �\�ߑޔ����Ă�����result��Ԃ��̂�����
      }
      void pop_back() noexcept { iterator it = end(); erase(--it); }
      void pop_front() noexcept { erase(begin()); }

      value_type back() const noexcept { iterator it = end(); return *(--it); }
      value_type front() const noexcept { return *(begin()); }

    public:
      // ����v�f����num�Ɏw�肵�ď���������
      template<typename... TInit>
      explicit TwinArray(const int sz = 0, TInit... args) noexcept
        : num_(sz)
        , used_(new node_type) // �_�~�[�Bwork_�̈悪�s�v��placement new���Ȃ�
        , free_(new node_type) // ����
      {
        assert(0 <= num_);
        assert(num_ <= MaxSize);

        // ���[�N������������(work_�m��)
        for (int i = 0; i < MaxSize; i++)
        {
          //new(nodes_ + i) Node();

          // used_ �� free_�̓��[�N�̈�s�v�̂���placement new�Ŋm�ۂ��Ȃ��Ă悢
        } // ���[�L���O�������m�ۊ���

        { // �����N���X�g�̘A��

          // �Ō���̃C���f�b�N�X
          const int c = MaxSize - 1;

          // �e�m�[�h�̑O��A�������N������
          for (int i = 0; i < c; i++)
          {
            nodes_[i].next_ = &nodes_[i + 1];
            nodes_[i + 1].prev_ = &nodes_[i];
          }

          // �_�~�[�m�[�h������
          used_->next_ = used_;      // �J�n���̓_�~�[���w������
          used_->prev_ = used_;      // �z������

          free_->next_ = &nodes_[0]; // ���R�̈�̓m�[�h��̍ŏ�
          free_->prev_ = &nodes_[c]; // ���R�̈�̖�������z��

          nodes_[0].prev_ = free_;
          nodes_[c].next_ = free_;

        } // �A������

        { // �v�f������
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
      // position�̈ʒu��input������Aposition�܂߂���ȍ~�̓����N���ЂƂ����ւ����
      // ���ӓ_�Ƃ��ẮA�����N���X�g��LinkTo�łȂ���������A
      // �C�e���[�^��free���X�g�������Ă���\��������Ƃ������ƁB
      iterator LinkTo(iterator position, iterator input) noexcept
      {
        // �ړ��������N�񂪋��ł���΃G���[�̂���nullptr��Ԃ�
        if (input.p_->next_ == input.p_)
          return nullptr;

        // input���ړ��������N�񂩂痣�E
        input.p_->next_->prev_ = input.p_->prev_;
        input.p_->prev_->next_ = input.p_->next_;

        // �������N���͂�
        input.p_->next_ = position.p_;
        input.p_->prev_ = position.p_->prev_;

        // �O�����N���͂�
        position.p_->prev_->next_ = input.p_;
        position.p_->prev_ = input.p_;

        return input;
      }

    }; // class TwinArray

  } // namespace API

} // namespace Mix
