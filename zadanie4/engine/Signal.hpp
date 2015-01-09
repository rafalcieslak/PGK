#ifndef SIGNAL_H
#define SIGNAL_H

#include <list>
#include <functional>

template <typename...>
class Signal;

// 0 arg
template <>
class Signal<>
{
private:
    std::list< std::function<void()> > subscribers;
public:
    Signal<>() { subscribers.clear(); }
    void Subscribe( std::function<void()> f )
    {
        subscribers.push_back(f);
    };
    template<class C>
    void Subscribe( C* class_ptr, void (C::*member_ptr)())
    {
        subscribers.push_back( std::bind(member_ptr,class_ptr) );
    }
    void Happen() const
    {
        for(auto f : subscribers) f();
    }
    void Clear()
    {
        subscribers.clear();
    }
};

// 1 arg
template <typename T>
class Signal<T>
{
private:
    std::list< std::function<void(T)> > subscribers;
    std::list< std::function<void() > > deaf_subscribers;
public:
    Signal<T>() { subscribers.clear(); }
    void Subscribe( std::function<void(T)> f )
    {
        subscribers.push_back(f);
    };
    void Subscribe( std::function<void()> f )
    {
        deaf_subscribers.push_back(f);
    };
    template<class C>
    void Subscribe( C* class_ptr, void (C::*member_ptr)(T))
    {
        subscribers.push_back( std::bind(member_ptr,class_ptr,std::placeholders::_1) );
    }
    template<class C>
    void Subscribe( C* class_ptr, void (C::*member_ptr)())
    {
        deaf_subscribers.push_back( std::bind(member_ptr,class_ptr) );
    }
    void Happen(T t) const
    {
        for(auto f : subscribers) f(t);
        for(auto f : deaf_subscribers) f();
    }
};

// 2 arg
template <typename T, typename S>
class Signal<T,S>
{
private:
    std::list< std::function<void(T,S)> > subscribers;
    std::list< std::function<void() > > deaf_subscribers;
public:
    Signal<T,S>() { subscribers.clear(); }
    void Subscribe( std::function<void(T,S)> f )
    {
        subscribers.push_back(f);
    };
    void Subscribe( std::function<void()> f )
    {
        deaf_subscribers.push_back(f);
    };
    template<class C>
    void Subscribe( C* class_ptr, void (C::*member_ptr)(T,S))
    {
        subscribers.push_back( std::bind(member_ptr,class_ptr,std::placeholders::_1,std::placeholders::_2) );
    }
    template<class C>
    void Subscribe( C* class_ptr, void (C::*member_ptr)())
    {
        deaf_subscribers.push_back( std::bind(member_ptr,class_ptr) );
    }
    void Happen(T t, S s) const
    {
        for(auto f : subscribers) f(t,s);
        for(auto f : deaf_subscribers) f();
    }
};

#endif //SIGNAL_H
