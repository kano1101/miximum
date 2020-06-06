#pragma once

#include <map>
#include <unordered_set>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <loki/Singleton.h>
#include <loki/TypeManip.h>
#include <mix/Creator.h>
#include <mix/Utility.h>
#include <mix/mini/MultiMapArray.h>

namespace Mix {

  ///////////////////////////////////////////////////////////////
  // Orderingポリシー
  
  // ListedOrderポリシークラス：TList順のとおりにに比較してくれる
  template<class TList>
  class ListedOrder {
  public:
    using Priority = Less<unsigned int>;
    template<class T>
    ListedOrder(Loki::Type2Type<T> t)
      : priority_(Loki::TL::IndexOf<TList, T>::value) {
      (void) t;
    }
    bool operator < (const ListedOrder& rhs) const {
      return priority_ < rhs.priority_;
    }
  private:
    Priority priority_;
  };

  
  ///////////////////////////////////////////////////////////////
  // Poolポリシー
  
  // PoolMallocポリシークラス：メモリ効率が良いがPoolStaticより少し低速
  template<class TList>
  class PoolMalloc {
  public:
    template<class T>
    PoolMalloc(Loki::Type2Type<T> t)
      : size_(sizeof(T)), pool_(static_cast<void*>(std::malloc(size_))) {
      (void) t;
    }
    PoolMalloc(const PoolMalloc& obj)
      : size_(obj.size_), pool_(static_cast<void*>(std::malloc(size_))) {
    }
    ~PoolMalloc() {
      if (pool_ != nullptr) std::free(pool_);
      pool_ = nullptr;
    }
    bool operator < (const PoolMalloc& rhs) const {
      return this < &rhs;
    }
    void* Address() {
      return pool_;
    }
  private:
    std::size_t size_;
    void* pool_;
  };
  
  // PoolStaticポリシークラス：高速だがメモリ断片が発生する
  template<class TList>
  class PoolStatic {
  public:
    template<class T>
    PoolStatic(Loki::Type2Type<T> t) {
      (void) t;
    }
    bool operator < (const PoolStatic& rhs) const {
      return this < &rhs;
    }
    enum { ByteSize = Mix::MaxSizeOf<TList>::value };
    void* Address() {
      return pool_;
    }
  private:
    int pool_[(ByteSize + 3) / 4];
  };

  
  //////////////////////////////////////////////////////////////
  // Iterationポリシー
  
  // ループ中の自身イテレータを指すオブジェクト削除時にも次に安全にシフトさせる
  template<class Iterator>
  class SafeShiftIteration {
    static std::unordered_multiset<Iterator*> iterators_;
  public:
    static void ShiftIf(const Iterator& target) {
      std::for_each(
        iterators_.begin(), iterators_.end(),
        [&] (auto& e) { if ( *e == target ) ++(*e); });
    }
    static void Add(Iterator& it) {
      iterators_.insert(&it);
    }
    static void Remove(Iterator& it) {
      iterators_.erase(&it);
    }
  };
  template<class I>
  std::unordered_multiset<I*> SafeShiftIteration<I>::iterators_;

  template<class Iterator>
  class NoShift {
  public:
    static void ShiftIf(const Iterator& t) { (void)t; }
    static void Add(Iterator& t) { (void)t; }
    static void Remove(Iterator& t) { (void)t; }
  };
  
  //////////////////////////////////////////////////////////////
  // コンテナ
  
  // 自作固定長配列を採用した両面性コンテナ（STLのものより生成が20倍低速）
  template<class OrderingPolicy, class PoolPolicy, std::size_t N = 10480>
  class DualityMultiMap
    : private Mix::MultiMapArray<OrderingPolicy, PoolPolicy, N> {
  public:
    using base = Mix::MultiMapArray<OrderingPolicy, PoolPolicy, N>;
    using iterator = typename base::iterator;
    static iterator GetIterator(iterator first, iterator last, void* target) {
      auto it = std::find_if( // 全検索開始
        first, last,
        [&] (auto& h) { return h.second.Address() == target; }); // 消したいポインタに一致か
      if (it != last) return it;

      return last;
      // auto it = base::GetIterator(target); // ヒープでは使えない（静的記憶域で使おう）
      // return it;
    }
    DualityMultiMap() : base(0, std::make_pair(0, Loki::NullType())) {
    }
    using base::begin;
    using base::end;
    using base::insert;
    using base::empty;
    using base::erase;
  };

  // 一番実用性が高いコンテナラッパークラス
  template<class OrderingPolicy, class PoolPolicy>
  class MultiMap
    : private std::multimap<OrderingPolicy, PoolPolicy> {
  public:
    using base = std::multimap<OrderingPolicy, PoolPolicy>;
    using iterator = typename base::iterator;
    static iterator GetIterator(iterator first, iterator last, void* target) {
      auto it = std::find_if( // 全検索開始
        first, last,
        [&] (auto& h) { return h.second.Address() == target; }); // 消したいポインタに一致か
      if (it != last) return it;

      return last;
    }
    using base::begin;
    using base::end;
    using base::insert;
    using base::empty;
    using base::erase;
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // MemoryManagerホストクラス

  template<class TList, class O, class P, class C, class I>
  class MemoryManager;
  
  template<
    class TList,
    class OrderingPolicy  = ListedOrder<TList>,
    class PoolPolicy      = PoolMalloc <TList>,
    class Container       = MultiMap<OrderingPolicy, PoolPolicy>,
    class IterationPolicy = NoShift<typename Container::iterator>
    >
  class MemoryManager
    : public Container {
  public:
    using Base      = Container;
    using Iterating = IterationPolicy;
    
    template<class T>
    T* Get() {
      auto ins_key = OrderingPolicy(Loki::Type2Type<T>());
      auto ins_val = PoolPolicy    (Loki::Type2Type<T>());
      auto it = Base::insert(std::make_pair(ins_key, ins_val));
      return static_cast<T*>(it->second.Address());
    }
    void Release(void* p) {
      auto it = Base::GetIterator(Base::begin(), Base::end(), p);
      if (it == Base::end()) assert(!"削除対象が存在しませんでした。"); // ヒットしていなければエラー終了
      Iterating::ShiftIf(it);
      Base::erase(it); // ヒットしたのでそれを消去
    }
    ~MemoryManager() {
      while(!Base::empty()) {
        auto it = Base::begin();
        Base::erase(it);
      }
    }
  
  private:
    MemoryManager() {}
    MemoryManager(const MemoryManager&) {}
    MemoryManager& operator=(const MemoryManager&) {}
    MemoryManager(MemoryManager&&) {}
    MemoryManager& operator=(MemoryManager&&) {}
    friend struct Loki::CreateUsingNew<MemoryManager>;
  };

  template<class Manager>
  class KeepInWith {
  public:
    template<class T>
    class Allocator {
    public:
      using value_type = T;

    public:
      static T* allocate(std::size_t n) {
        if (n != 1) return nullptr; // 必ずひとつずつ取得するルールとする
      
        T* p = TheManager().template Get<T>(); // メモリ領域を確保するだけ
        return static_cast<T*>(p);
      }
      static void deallocate(T* p, std::size_t n) {
        if (n != 1) return; // ひとつずつしか受け付けないルール
      
        TheManager().Release(p);
      }
    };
  public:
    template<class I>
    class Executor {
    public:
      template<class Function>
      static Function each(Function f) {
        auto first = TheManager().begin();
        auto last = TheManager().end();
        Manager::Iterating::Add(first);
        for (;first != last;) {
          auto prev = first;
          f(*(reinterpret_cast<I*>(first->second.Address())));
          if ( prev == first ) ++first;
        }
        Manager::Iterating::Remove(first);
        return f;
      }
    };
  private:
    static Manager& TheManager() {
      return SingletonManager::Instance();
    }
  private:
    using SingletonManager =  Loki::SingletonHolder<Manager, Loki::CreateUsingNew, Loki::PhoenixSingleton>;
  };

  template<class TList>
  struct Director {
    template<class T>
    using Allocator = typename KeepInWith<MemoryManager<TList>>::template Allocator<T>;
    template<class I>
    using Executor = typename KeepInWith<MemoryManager<TList>>::template Executor<I>;
  };
  
} // namespace Mix

