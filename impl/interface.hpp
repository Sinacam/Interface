// DO NOT modify, this is a machine generated file.
// DO NOT include directly, this is a implementation file.
// See impl/README for details.

#include<memory>
#include<type_traits>
#include<cstddef>

// Implementaion namespace.
namespace interface_detail
{
    struct interface_tag {}; // As extra parameter for certain implementation functions to avoid namespace pollution.

    template<typename T>
    struct is_interface : std::is_base_of<interface_tag, T> {};

    template<typename T>
    inline static constexpr bool is_interface_v = is_interface<T>::value;

    // Base case factory for type erased method call.
    // Shouldn't be called. Working factories within the defined interface.
    struct nothing
    {
        template<typename... Args>
        void call(void*, Args&&...) {}
    };

    // erasure_fn is a traits class that handles void return types gracefully.
    template<typename Signature, typename Factory = nothing>
    struct erasure_fn;

    template<typename Ret, typename... Args, typename Factory>
    struct erasure_fn<Ret(Args...), Factory> : Factory
    {
        using type = Ret(void*, Args...);
        using return_type = Ret;
        static constexpr Ret value(void* p, Args... args)
        {
            if constexpr(std::is_void_v<Ret>)
                Factory::call(p, std::forward<Args>(args)...);
            else
                return Factory::call(p, std::forward<Args>(args)...);
        };
    };

    // Unified interface to access stored object.
    // Stored pointer signifies reference semantics.
    template<typename T>
    decltype(auto) as_object(void* p)
    {
        if constexpr(std::is_pointer_v<T>)
            return **static_cast<T*>(p);
        else
            return *static_cast<T*>(p);
    }

    // Type erased special member functions.
    struct thunk
    {
        void (*copy)(void* dst, const void* src) = nullptr;
        void (*move)(void* dst, void* src) = nullptr;
        void (*destroy)(void* p) noexcept = nullptr;
        std::size_t size = 0;
    };

    // Address of t acts as RTTI.
    template<typename T, bool = std::is_constructible_v<T, const T&>>
    struct thunk_storage
    {
        inline static constexpr thunk t = {
            [](void* dst, const void* src) {
                new (dst) T{*static_cast<const T*>(src)};
            },
            [](void* dst, void* src) {
                new (dst) T{std::move(*static_cast<T*>(src))};
            },
            [](void* p) noexcept {
                static_cast<T*>(p)->~T();
            },
            sizeof(T)
        };
    };
    template<typename T>
    struct thunk_storage<T, false>
    {
        inline static constexpr thunk t = {
            nullptr,
            nullptr,
            [](void* p) noexcept {
                static_cast<T*>(p)->~T();
            },
            sizeof(T)
        };
    };

    template<typename T>
    constexpr const thunk* get_thunk()
    {
        // Returns same thunk for all pointer types, used to determine whether
        // interface has reference semantics.
        if constexpr (std::is_pointer_v<T>)
            return &thunk_storage<void*>::t;
        else
            return &thunk_storage<T>::t;
    }

    // All pointer thunks are void* thunks.
    constexpr bool is_pointer_thunk(const thunk* t)
    {
        return t == get_thunk<void*>();
    }
}

// For ADL purposes.
template<typename T, typename I>
void target(I&&, ::interface_detail::interface_tag);

// For creating anonymous variables.
#define INTERFACE_CONCAT_DIRECT(x, y) x##y
#define INTERFACE_CONCAT(x, y) INTERFACE_CONCAT_DIRECT(x, y)
#define INTERFACE_APPEND_LINE(x) INTERFACE_CONCAT(x, __LINE__)

#ifdef INTERFACE_FOR_EXPOSITION_ONLY
// The following is used only as documentation to the implementation of interface.
// SIGNATURE and METHOD_NAME are the parameters passed in by the user.
// The multitudes of versions each have a different arity.

// Inherits from interface_tag for type traits is_interface.
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag
{
    // Alias for both readability and for recursively defined functions:
    // user may provide a function signature including interface.
    using interface = INTERFACE_APPEND_LINE(interface__);

    // Used in dispatching function call.
    // Used in converting from one interface to another to bypass access level.
    // interface_tag used to avoid namespace pollution, however improbable.
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)
    {
        using std::get;
        return get<0>(i._vtable);
    }

    // Factory for type erased method call
    // Suffix used to avoid name collisions.
    template <typename T>
    struct METHOD_NAME0##_0_factory
    {
        template <typename... Args>
        static decltype(auto) call(void* p, Args&&... args)
        {
            return ::interface_detail::as_object<T>(p).METHOD_NAME0(::std::forward<Args>(args)...);
        }
    };

    // Used in target.
    // Used in converting from one interface to another to bypass access level.
    // interface_tag used to avoid namespace pollution, however improbable.
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag) { return i._ptr; }

    // Used in target.
    // Used in converting from one interface to another to bypass access level.
    // interface_tag used to avoid namespace pollution, however improbable.
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)
    {
        return i._t;
    }

    template<typename I>
    void construct(I&& i)
    {
        if(!i)
            return;

        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});

        // Exception safe buffer allocation.
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);

        // Other constructor guarantees the two following calls are both valid.
        if constexpr(::std::is_lvalue_reference_v<I> || ::std::is_const_v<I>)
            t->copy(buf.get(), p);
        else
            t->move(buf.get(), p);

        // Avoid [basic.life]/8 where original pointer cannot be used to refer to the newly
        // constructed object.
        _ptr = ::std::launder(buf.get());

        buf.release();
        _t = t;

        // Magic here. Constructs _vtable by name at compile time.
        // This is the reason why we can't use polymorphic classes as in std::function.
        _vtable = {
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),
        };
    }

  public:
    INTERFACE_APPEND_LINE(interface__)() = default;
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }

    // SFINAE on whether argument is an interface.
    // This is the converting constructor from other superset interfaces.
    template <typename I,
              ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I>>, bool> = false>
    INTERFACE_APPEND_LINE(interface__)
    (I&& i)
    {
        construct(::std::forward<I>(i));
    }

    // SFINAE on whether argument is an interface.
    // This is the conversion from any type to an interface.
    template <typename T,
              ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T>>, bool> = false>
    INTERFACE_APPEND_LINE(interface__)
    (T&& t)
    {
        using U = ::std::decay_t<T>;
        static_assert(alignof(U) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");
        static_assert(::std::is_constructible_v<U, const U&>, "Value semantics require the type be copy constructible.");

        // Exception safe buffer allocation.
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U)]);
        _ptr = new (buf.get()) U{::std::forward<T>(t)};
        buf.release();
        _t = ::interface_detail::get_thunk<U>();

        // Constructs _vtable by name at compile time.
        // erasure_fn is a unified interface to the method.
        _vtable = {
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U>>::value,
        };
    }

    ~INTERFACE_APPEND_LINE(interface__)()
    {
        if(_ptr)
            _t->destroy(_ptr);
        delete[] reinterpret_cast<::std::byte*>(_ptr);
    }

    interface& operator=(const interface& other)
    {
        auto tmp = other;
        swap(*this, tmp);
        return *this;
    }
    interface& operator=(interface&& other) noexcept
    {
        auto tmp = ::std::move(other);
        swap(*this, tmp);
        return *this;
    }

    template <typename... Args>
    decltype(auto) METHOD_NAME0(Args&&... args)
    {
        // Dispatches to type erased method call.
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(
            _ptr, ::std::forward<Args>(args)...);
    }

    // Fetches underlying type if thunk* matches, which serves as RTTI.
    template<typename T>
    friend T* target(interface&& i) noexcept
    {
        if(i._t == ::interface_detail::get_thunk<T>())
            return reinterpret_cast<T*>(i._ptr);
        else
            return nullptr;
    }
    template<typename T>
    friend T* target(interface& i) noexcept
    {
        if(i._t == ::interface_detail::get_thunk<T>())
            return reinterpret_cast<T*>(i._ptr);
        else
            return nullptr;
    }
    template<typename T>
    friend const T* target(const interface& i) noexcept
    {
        if(i._t == ::interface_detail::get_thunk<T>())
            return reinterpret_cast<T*>(i._ptr);
        else
            return nullptr;
    }

    // Returns true if there is an underlying object.
    explicit operator bool() const noexcept { return _ptr; }

    // Returns true iff both interfaces are empty or both references the same object.
    template<typename I, std::enable_if_t<std::is_same_v<interface, std::decay_t<I>>, bool> = false>
    bool operator==(I&& rhs) const noexcept
    {
        if(!_ptr)
            return !rhs._ptr;
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);
        return false;
    }
    template<typename I, std::enable_if_t<std::is_same_v<interface, std::decay_t<I>>, bool> = false>
    bool operator!=(I&& rhs) const noexcept { return !(*this == rhs); }

    friend void swap(interface& x, interface& y) noexcept
    {
        using ::std::swap;
        swap(x._ptr, y._ptr);
        swap(x._t, y._t);
        swap(x._vtable, y._vtable);
    }

  private:
    template <typename T>
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T>::type;
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE>*>;

    void* _ptr = nullptr;
    const ::interface_detail::thunk* _t = nullptr;
    vtable_t _vtable = {};
}

#endif // INTERFACE_FOR_EXPOSITION_ONLY

// The following is the actual implementaion for interface.


#define INTERFACE_1(SIGNATURE0, METHOD_NAME0)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}

#define INTERFACE_2(SIGNATURE0, METHOD_NAME0, SIGNATURE1, METHOD_NAME1)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME1(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<1>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME1##_1_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME1(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME1(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE1, METHOD_NAME1##_1_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME1(Args__&&... as)\
    {\
        return get_##METHOD_NAME1(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*, erasure_fn_t<SIGNATURE1>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}

#define INTERFACE_3(SIGNATURE0, METHOD_NAME0, SIGNATURE1, METHOD_NAME1, SIGNATURE2, METHOD_NAME2)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME1(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<1>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME1##_1_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME1(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME2(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<2>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME2##_2_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME2(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME1(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME2(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE1, METHOD_NAME1##_1_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE2, METHOD_NAME2##_2_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME1(Args__&&... as)\
    {\
        return get_##METHOD_NAME1(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME2(Args__&&... as)\
    {\
        return get_##METHOD_NAME2(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*, erasure_fn_t<SIGNATURE1>*, erasure_fn_t<SIGNATURE2>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}

#define INTERFACE_4(SIGNATURE0, METHOD_NAME0, SIGNATURE1, METHOD_NAME1, SIGNATURE2, METHOD_NAME2, SIGNATURE3, METHOD_NAME3)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME1(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<1>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME1##_1_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME1(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME2(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<2>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME2##_2_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME2(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME3(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<3>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME3##_3_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME3(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME1(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME2(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME3(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE1, METHOD_NAME1##_1_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE2, METHOD_NAME2##_2_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE3, METHOD_NAME3##_3_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME1(Args__&&... as)\
    {\
        return get_##METHOD_NAME1(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME2(Args__&&... as)\
    {\
        return get_##METHOD_NAME2(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME3(Args__&&... as)\
    {\
        return get_##METHOD_NAME3(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*, erasure_fn_t<SIGNATURE1>*, erasure_fn_t<SIGNATURE2>*, erasure_fn_t<SIGNATURE3>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}

#define INTERFACE_5(SIGNATURE0, METHOD_NAME0, SIGNATURE1, METHOD_NAME1, SIGNATURE2, METHOD_NAME2, SIGNATURE3, METHOD_NAME3, SIGNATURE4, METHOD_NAME4)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME1(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<1>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME1##_1_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME1(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME2(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<2>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME2##_2_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME2(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME3(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<3>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME3##_3_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME3(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME4(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<4>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME4##_4_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME4(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME1(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME2(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME3(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME4(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE1, METHOD_NAME1##_1_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE2, METHOD_NAME2##_2_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE3, METHOD_NAME3##_3_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE4, METHOD_NAME4##_4_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME1(Args__&&... as)\
    {\
        return get_##METHOD_NAME1(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME2(Args__&&... as)\
    {\
        return get_##METHOD_NAME2(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME3(Args__&&... as)\
    {\
        return get_##METHOD_NAME3(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME4(Args__&&... as)\
    {\
        return get_##METHOD_NAME4(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*, erasure_fn_t<SIGNATURE1>*, erasure_fn_t<SIGNATURE2>*, erasure_fn_t<SIGNATURE3>*, erasure_fn_t<SIGNATURE4>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}

#define INTERFACE_6(SIGNATURE0, METHOD_NAME0, SIGNATURE1, METHOD_NAME1, SIGNATURE2, METHOD_NAME2, SIGNATURE3, METHOD_NAME3, SIGNATURE4, METHOD_NAME4, SIGNATURE5, METHOD_NAME5)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME1(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<1>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME1##_1_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME1(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME2(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<2>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME2##_2_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME2(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME3(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<3>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME3##_3_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME3(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME4(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<4>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME4##_4_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME4(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME5(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<5>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME5##_5_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME5(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME1(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME2(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME3(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME4(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME5(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE1, METHOD_NAME1##_1_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE2, METHOD_NAME2##_2_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE3, METHOD_NAME3##_3_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE4, METHOD_NAME4##_4_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE5, METHOD_NAME5##_5_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME1(Args__&&... as)\
    {\
        return get_##METHOD_NAME1(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME2(Args__&&... as)\
    {\
        return get_##METHOD_NAME2(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME3(Args__&&... as)\
    {\
        return get_##METHOD_NAME3(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME4(Args__&&... as)\
    {\
        return get_##METHOD_NAME4(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME5(Args__&&... as)\
    {\
        return get_##METHOD_NAME5(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*, erasure_fn_t<SIGNATURE1>*, erasure_fn_t<SIGNATURE2>*, erasure_fn_t<SIGNATURE3>*, erasure_fn_t<SIGNATURE4>*, erasure_fn_t<SIGNATURE5>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}

#define INTERFACE_7(SIGNATURE0, METHOD_NAME0, SIGNATURE1, METHOD_NAME1, SIGNATURE2, METHOD_NAME2, SIGNATURE3, METHOD_NAME3, SIGNATURE4, METHOD_NAME4, SIGNATURE5, METHOD_NAME5, SIGNATURE6, METHOD_NAME6)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME1(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<1>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME1##_1_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME1(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME2(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<2>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME2##_2_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME2(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME3(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<3>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME3##_3_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME3(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME4(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<4>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME4##_4_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME4(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME5(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<5>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME5##_5_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME5(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME6(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<6>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME6##_6_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME6(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME1(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME2(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME3(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME4(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME5(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME6(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE1, METHOD_NAME1##_1_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE2, METHOD_NAME2##_2_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE3, METHOD_NAME3##_3_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE4, METHOD_NAME4##_4_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE5, METHOD_NAME5##_5_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE6, METHOD_NAME6##_6_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME1(Args__&&... as)\
    {\
        return get_##METHOD_NAME1(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME2(Args__&&... as)\
    {\
        return get_##METHOD_NAME2(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME3(Args__&&... as)\
    {\
        return get_##METHOD_NAME3(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME4(Args__&&... as)\
    {\
        return get_##METHOD_NAME4(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME5(Args__&&... as)\
    {\
        return get_##METHOD_NAME5(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME6(Args__&&... as)\
    {\
        return get_##METHOD_NAME6(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*, erasure_fn_t<SIGNATURE1>*, erasure_fn_t<SIGNATURE2>*, erasure_fn_t<SIGNATURE3>*, erasure_fn_t<SIGNATURE4>*, erasure_fn_t<SIGNATURE5>*, erasure_fn_t<SIGNATURE6>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}

#define INTERFACE_8(SIGNATURE0, METHOD_NAME0, SIGNATURE1, METHOD_NAME1, SIGNATURE2, METHOD_NAME2, SIGNATURE3, METHOD_NAME3, SIGNATURE4, METHOD_NAME4, SIGNATURE5, METHOD_NAME5, SIGNATURE6, METHOD_NAME6, SIGNATURE7, METHOD_NAME7)\
class INTERFACE_APPEND_LINE(interface__) : ::interface_detail::interface_tag\
{\
    using interface = INTERFACE_APPEND_LINE(interface__);\
\
    friend auto get_##METHOD_NAME0(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<0>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME0##_0_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME0(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME1(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<1>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME1##_1_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME1(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME2(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<2>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME2##_2_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME2(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME3(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<3>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME3##_3_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME3(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME4(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<4>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME4##_4_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME4(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME5(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<5>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME5##_5_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME5(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME6(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<6>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME6##_6_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME6(::std::forward<Args__>(as)...);\
        }\
    };\
    friend auto get_##METHOD_NAME7(const interface& i, ::interface_detail::interface_tag)\
    {\
        using std::get;\
        return get<7>(i._vtable);\
    }\
\
    template<typename T__>\
    struct METHOD_NAME7##_7_factory\
    {\
        template<typename... Args__>\
        static decltype(auto) call(void* p, Args__&&... as)\
        {\
            return ::interface_detail::as_object<T__>(p).METHOD_NAME7(::std::forward<Args__>(as)...);\
        }\
    };\
\
    friend auto fetch_ptr(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._ptr;\
    }\
\
    friend auto&& fetch_thunk(const interface& i, ::interface_detail::interface_tag)\
    {\
        return i._t;\
    }\
\
    template<typename I__>\
    void construct(I__&& i)\
    {\
        if(!i)\
            return;\
\
        auto p = fetch_ptr(i, ::interface_detail::interface_tag{});\
        auto t = fetch_thunk(i, ::interface_detail::interface_tag{});\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[t->size]);\
        if constexpr(::std::is_lvalue_reference_v<I__> || ::std::is_const_v<I__>)\
            t->copy(buf.get(), p);\
        else\
            t->move(buf.get(), p);\
        _ptr = ::std::launder(buf.get());\
        buf.release();\
        _t = t;\
        _vtable = {\
            get_##METHOD_NAME0(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME1(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME2(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME3(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME4(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME5(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME6(i, ::interface_detail::interface_tag{}),\
            get_##METHOD_NAME7(i, ::interface_detail::interface_tag{}),\
        };\
    }\
\
public:\
    INTERFACE_APPEND_LINE(interface__)() = default;\
    INTERFACE_APPEND_LINE(interface__)(interface&& other) noexcept { swap(*this, other); }\
    INTERFACE_APPEND_LINE(interface__)(const interface& other) { construct(other); }\
    template<typename I__, ::std::enable_if_t<::interface_detail::is_interface_v<::std::decay_t<I__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(I__&& i)\
    {\
        construct(::std::forward<I__>(i));\
    }\
\
\
    template <typename T__, ::std::enable_if_t<!::interface_detail::is_interface_v<::std::decay_t<T__>>, bool> = false>\
    INTERFACE_APPEND_LINE(interface__)(T__&& t)\
    {\
        using U__ = ::std::decay_t<T__>;\
        static_assert(alignof(U__) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__, "Doesn't support overaligned type.");\
        static_assert(::std::is_constructible_v<U__, const U__&>, "Value semantics require the type be copy constructible.");\
        auto buf = ::std::unique_ptr<::std::byte[]>(new ::std::byte[sizeof(U__)]);\
        _ptr = new (buf.get()) U__{::std::forward<T__>(t)};\
        buf.release();\
        _t = ::interface_detail::get_thunk<U__>();\
\
        _vtable = {\
            ::interface_detail::erasure_fn<SIGNATURE0, METHOD_NAME0##_0_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE1, METHOD_NAME1##_1_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE2, METHOD_NAME2##_2_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE3, METHOD_NAME3##_3_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE4, METHOD_NAME4##_4_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE5, METHOD_NAME5##_5_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE6, METHOD_NAME6##_6_factory<U__>>::value,\
            ::interface_detail::erasure_fn<SIGNATURE7, METHOD_NAME7##_7_factory<U__>>::value,\
        };\
    }\
\
    ~INTERFACE_APPEND_LINE(interface__)()\
    {\
        if(_ptr)\
            _t->destroy(_ptr);\
        delete[] reinterpret_cast<::std::byte*>(_ptr);\
    }\
\
    interface& operator=(const interface& other)\
    {\
        auto tmp = other;\
        swap(*this, tmp);\
        return *this;\
    }\
    interface& operator=(interface&& other) noexcept\
    {\
        auto tmp = ::std::move(other);\
        swap(*this, tmp);\
        return *this;\
    }\
\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME0(Args__&&... as)\
    {\
        return get_##METHOD_NAME0(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME1(Args__&&... as)\
    {\
        return get_##METHOD_NAME1(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME2(Args__&&... as)\
    {\
        return get_##METHOD_NAME2(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME3(Args__&&... as)\
    {\
        return get_##METHOD_NAME3(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME4(Args__&&... as)\
    {\
        return get_##METHOD_NAME4(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME5(Args__&&... as)\
    {\
        return get_##METHOD_NAME5(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME6(Args__&&... as)\
    {\
        return get_##METHOD_NAME6(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
    template<typename... Args__>\
    decltype(auto) METHOD_NAME7(Args__&&... as)\
    {\
        return get_##METHOD_NAME7(*this, ::interface_detail::interface_tag{})(_ptr, ::std::forward<Args__>(as)...);\
    }\
\
    template<typename T__>\
    friend T__* target(interface&& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend T__* target(interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
    template<typename T__>\
    friend const T__* target(const interface& i) noexcept\
    {\
        if(i._t == ::interface_detail::get_thunk<T__>())\
            return reinterpret_cast<T__*>(i._ptr);\
        else\
            return nullptr;\
    }\
\
    explicit operator bool() const noexcept { return _ptr; }\
\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator==(I__&& rhs) const noexcept\
    {\
        if(!_ptr)\
            return !rhs._ptr;\
        if(::interface_detail::is_pointer_thunk(_t) && ::interface_detail::is_pointer_thunk(rhs._t))\
            return *reinterpret_cast<void**>(_ptr) == *reinterpret_cast<void**>(rhs._ptr);\
        return false;\
    }\
    template<typename I__, std::enable_if_t<std::is_same_v<interface, std::decay_t<I__>>, bool> = false>\
    bool operator!=(I__&& rhs) const noexcept { return !(*this == rhs); }\
\
    friend void swap(interface& x, interface& y) noexcept\
    {\
        using ::std::swap;\
        swap(x._ptr, y._ptr);\
        swap(x._t, y._t);\
        swap(x._vtable, y._vtable);\
    }\
\
private:\
    template<typename T__>\
    using erasure_fn_t = typename ::interface_detail::erasure_fn<T__>::type;\
    using vtable_t = ::std::tuple<erasure_fn_t<SIGNATURE0>*, erasure_fn_t<SIGNATURE1>*, erasure_fn_t<SIGNATURE2>*, erasure_fn_t<SIGNATURE3>*, erasure_fn_t<SIGNATURE4>*, erasure_fn_t<SIGNATURE5>*, erasure_fn_t<SIGNATURE6>*, erasure_fn_t<SIGNATURE7>*>;\
\
    void* _ptr = nullptr;\
    const ::interface_detail::thunk* _t = nullptr;\
    vtable_t _vtable = {};\
}


// Overloaded macros through __VA_ARGS__ hacking.
// Selects implementation by argument count.
#define GET_INTERFACE_FROM(_8a, _8b, _7a, _7b, _6a, _6b, _5a, _5b, _4a, _4b, _3a, _3b, _2a, _2b, _1a, _1b, x, ...) x
#define INTERFACE(...)\
GET_INTERFACE_FROM(__VA_ARGS__, INTERFACE_8, _8, INTERFACE_7, _7, INTERFACE_6, _6, INTERFACE_5, _5, INTERFACE_4, _4, INTERFACE_3, _3, INTERFACE_2, _2, INTERFACE_1, _1)(__VA_ARGS__)

