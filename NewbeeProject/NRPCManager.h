#pragma once
#include "NewbeeProject.h"
#include "NSerializer.hpp"
#include <tuple>

using namespace std;

class Serializer;

template<typename T>
struct type_xx { typedef T type; };

template<>
struct type_xx<void> { typedef int8_t type; };

template<typename Tuple, std::size_t... Is>
void package_params_impl(Serializer& ds, const Tuple& t, std::index_sequence<Is...>)
{
	initializer_list<int>{((ds << std::get<Is>(t)), 0)...};
}

template<typename... Args>
void package_params(Serializer& ds, const std::tuple<Args...>& t)
{
	package_params_impl(ds, t, std::index_sequence_for<Args...>{});
}

template<typename Function, typename Tuple, std::size_t... Index>
decltype(auto) invoke_impl(Function&& func, Tuple&& t, std::index_sequence<Index...>)
{
	return func(std::get<Index>(std::forward<Tuple>(t))...);
}

template<typename Function, typename Tuple>
decltype(auto) invoke(Function&& func, Tuple&& t)
{
	constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
	return invoke_impl(std::forward<Function>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
}

template<typename R, typename F, typename ArgsTuple>
typename std::enable_if<std::is_same<R, void>::value, typename type_xx<R>::type >::type
call_helper(F f, ArgsTuple args) {
	invoke(f, args);
	return 0;
}

template<typename R, typename F, typename ArgsTuple>
typename std::enable_if<!std::is_same<R, void>::value, typename type_xx<R>::type >::type
call_helper(F f, ArgsTuple args) {
	return invoke(f, args);
}

class NRPCManager
{
	using FuncMap = map<std::string, std::function<void(Serializer*, const char*, size_t)>>;
private:

	static NRPCManager* instance;

private:
	// userName, funcName, functionPtr
	std::unordered_map<std::string, unordered_map<std::string, std::function<void(Serializer*, const char*, size_t)>>> functionMap;

public:
	NRPCManager(){}
	~NRPCManager(){}

	static NRPCManager* Get()
	{
		if (instance == nullptr)
		{
			instance = new NRPCManager();
		}

		return instance;
	}

	std::unordered_map<std::string, unordered_map<std::string, std::function<void(Serializer*, const char*, size_t)>>>& GetFuncMap() { return functionMap; }

	template <typename R, typename... Args>
	Serializer Serialize(const std::string& objName, const std::string& funcName, Args... args)
	{
		std::tuple<typename std::decay<Args>::type...> as = std::make_tuple(args...);
		Serializer sr;
		sr << objName;
		sr << funcName;
		package_params(sr, as);
		return sr;
	}

	Serializer* CallFunc(const std::string& objName, const std::string& funcName, const char* data, size_t len)
	{
		Serializer* ds = new Serializer();
		if (functionMap.find(objName) == functionMap.end())
		{
			cout << "obj or function binding is not exist\n";
			return ds;
		}

		auto tempMap = functionMap[objName];
		if (tempMap.find(funcName) == tempMap.end())
		{
			cout << "obj or function binding is not exist\n";
			return ds;
		}
		auto fun = functionMap[objName][funcName];
		fun(ds, data, len);
		ds->reset();
		return ds;
	}

	template <typename F>
	void regist(const std::string objName, const std::string& name, F func)
	{
		functionMap[objName][name] = std::bind(&NRPCManager::callproxy<F>, this, func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	
	template<typename F, typename S>
	void regist(const std::string objName, const std::string& name, F func, S* s)
	{
		functionMap[objName][name] = std::bind(&NRPCManager::callproxy<F, S>, this, func, s, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	template<typename F>
	void callproxy(F func, Serializer* pr, const char* data, size_t len)
	{
		callproxy_(func, pr, data, len);
	}
	template<typename F, typename S>
	void callproxy(F func, S* s, Serializer* pr, const char* data, size_t len)
	{
		callproxy_(func, s, pr, data, len);
	}

	template<typename R, typename... Args>
	void callproxy_(R(*func)(Args...), Serializer* pr, const char* data, size_t len) {
		callproxy_(std::function<R(Args...)>(func), pr, data, len);
	}

	template<typename R, typename C, typename S, typename... Args>
	void callproxy_(R(C::* func)(Args...), S* s, Serializer* pr, const char* data, size_t len)
	{
		using args_type = std::tuple<typename std::decay<Args>::type...>;

		Serializer sr(StreamBuffer(data, len));
		args_type as = sr.get_tuple<args_type>(std::index_sequence_for<Args...>{});

		auto ff = [=](Args... args)->R {
			return (s->*func)(args...);
		};
		typename type_xx<R>::type r = call_helper<R>(ff, as);
	}

	template<typename R, typename... Args>
	void callproxy_(std::function<R(Args... args)> func, Serializer* pr, const char* data, size_t len)
	{
		using args_type = std::tuple<typename std::decay<Args>::type...>;

		Serializer sr(StreamBuffer(data, len));
		args_type as = sr.get_tuple<args_type>(std::index_sequence_for<Args...>{});

		typename type_xx<R>::type r = call_helper<R>(func, as);
	}
};
