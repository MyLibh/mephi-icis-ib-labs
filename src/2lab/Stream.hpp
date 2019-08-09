#pragma once

#ifndef __STREAM_HPP_INCLUDED__
#define __STREAM_HPP_INCLUDED__

#ifndef __cplusplus
#error
#error Must use C++ to compile.
#error
#endif /* __cplusplus */

#if (_MSVC_LANG < 201703L)
#error
#error Must use ISO C++17 Standart
#error
#endif /* __cplusplus < 201703L */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <type_traits>
#include <sstream>
#include <charconv>

template<typename T, typename Container = std::vector<T>>
class Stream
{
	using _T = T;
	using _Cont = Container;
	using _Iter = typename _Cont::iterator;
	using _Const_Iter = typename _Cont::const_iterator;
	using _map_func_t = std::function<_T(_T&)>;
	using _where_func_t = std::function<bool(_T&)>;
	using _reduce_func_t = std::function<_T(_T&, _T&)>;

private:
	void parse(std::string_view str);

public:
	Stream() noexcept = default;
	Stream(std::string const&);
	Stream(std::initializer_list<_T>);
	Stream(Stream const&) = delete;
	Stream(Stream&&) = delete;

	[[nodiscard]]
	inline _Iter begin() noexcept;

	[[nodiscard]]
	inline _Const_Iter cbegin() const noexcept;

	[[nodiscard]]
	inline _Iter end() noexcept;

	[[nodiscard]]
	inline _Const_Iter cend() const noexcept;

	inline void clear() noexcept;

	void map(_map_func_t map_func);

	void where(_where_func_t where_func);

	[[nodiscard]]
	_T reduce(_reduce_func_t reduce_func, _T init_val);

	[[nodiscard]]
	inline _Cont get_subseq(_Iter first, _Iter last) const;

	[[nodiscard]]
	std::pair<bool, _Const_Iter> find_subseq(std::string const& crPattern) const;

	void merge();

	void separate();

	void dump(std::ostream& = std::cout) const noexcept;

	Stream& operator<<(std::string const& crString);
	Stream& operator>>(_T& rVal);

	Stream& operator=(std::initializer_list<_T>);
	Stream& operator=(Stream const&) = delete;
	Stream& operator=(Stream&&)      = delete;

	template<typename T, typename Cont>
	friend std::istream& operator>>(std::istream& rIstr, Stream<T, Cont> const& crStream);

	template<typename T, typename Cont>
	friend std::ostream& operator<<(std::ostream& rOstr, Stream<T, Cont> const& crStream);

private:
	_Cont m_buf;
};

template<typename T, typename Container>
inline void Stream<T, Container>::parse(std::string_view str)
{
	std::stringstream ss(std::string(str) += '\n');
	_T tmp{};                                              
	while (ss.good())                                        
	{     
		ss >> tmp;                                           
		if (ss.good())
			m_buf.push_back(std::move(tmp));
		else if (ss.eof())
			break;
		else
			throw std::runtime_error("Wrong input.");
	}
}

template<typename T, typename Container>
inline Stream<T, Container>::Stream(std::string const& crString) :
	m_buf()
{
	parse(crString);
}

template<typename T, typename Container>
inline Stream<T, Container>::Stream(std::initializer_list<_T> list) :
	m_buf(list.begin(), list.end())
{ }

template<typename T, typename Container>
inline typename Stream<T, Container>::_Iter Stream<T, Container>::begin() noexcept
{
	return std::begin(m_buf);
}

template<typename T, typename Container>
inline typename Stream<T, Container>::_Const_Iter Stream<T, Container>::cbegin() const noexcept
{
	return std::cbegin(m_buf);
}

template<typename T, typename Container>
inline typename Stream<T, Container>::_Iter Stream<T, Container>::end() noexcept
{
	return std::end(m_buf);
}

template<typename T, typename Container>
inline void Stream<T, Container>::clear() noexcept
{
	m_buf.clear(); 
}

template<typename T, typename Container>
inline typename Stream<T, Container>::_Const_Iter Stream<T, Container>::cend() const noexcept
{
	return std::cend(m_buf);
}

template<typename T, typename Container>
void Stream<T, Container>::map(_map_func_t map_func)
{
	for (auto& val : m_buf)
		val = map_func(val);
}

template<typename T, typename Container>
void Stream<T, Container>::where(_where_func_t where_func)
{
	_Cont cont;
	cont.reserve(m_buf.size());

	for (auto it{ std::begin(m_buf) }; it != std::end(m_buf); ++it)
		if (where_func(*it))
			cont.push_back(*it);

	cont.shrink_to_fit();

	using std::swap; // Enable all swaps
	swap(m_buf, cont);
}

template<typename T, typename Container>
typename Stream<T, Container>::_T Stream<T, Container>::reduce(_reduce_func_t reduce_func, _T init_val)
{
	_T ret_val{ init_val };
	for (auto&& x : m_buf)
		ret_val = reduce_func(x, ret_val);

	return ret_val;
}

template<typename T, typename Container>
inline typename Stream<T, Container>::_Cont Stream<T, Container>::get_subseq(_Iter first, _Iter last) const
{
	return _Cont(first, last);
}

template<typename T, typename Container>
inline std::pair<bool, typename Stream<T, Container>::_Const_Iter> Stream<T, Container>::find_subseq(std::string const& crPattern) const
{
	if (crPattern.empty())
		throw std::runtime_error("Wrong pattern.");

	std::stringstream             ss(crPattern);
	std::vector<std::string> vec;
	while (!ss.eof())
	{
		std::string tmp;
		ss >> tmp;

		vec.push_back(tmp);
	}

	auto                              size{ vec.size() };
	typename decltype(vec)::size_type j{};
	for (auto it{ std::begin(m_buf) }, cur{ it }; it != std::cend(m_buf); ++it)
	{
		if (!j)
			cur = it;

		if constexpr (std::is_arithmetic_v<_T>)
		{
			char buf[sizeof(_T)]{};
			if (auto [ptr, ec] = std::to_chars(std::begin(buf), std::end(buf), *it); ec == std::errc() && std::string_view(buf, ptr - buf) == vec[j])
				if (j + 1 == size)
					return { true, cur };
				else
					j++;
			else
				j = 0ULL;
		}
		else // Assuming _T == std::string
		{
			if (*it == vec[j])
				if (j + 1 == size)
					return { true, cur };
				else
					j++;
			else
				j = 0ULL;
		}
	}

	return { false, std::cend(m_buf) };
}

template<typename T, typename Container>
void Stream<T, Container>::dump(std::ostream &rOstr) const noexcept
{
	try
	{
		rOstr << "\t[STREAM DUMP]\nStream<" << typeid(_T).name() << ", " << typeid(_Cont).name() << "> [0x" << this << "]\n"
			<< "{\n\t buffer [" << std::size(m_buf) << "] = 0x" << &m_buf << "\n\t{\n";

		if (std::size(m_buf))
			for (auto it{ std::cbegin(m_buf) }; it != std::cend(m_buf); ++it)
			{
				rOstr << "\t\t[" << std::distance(std::begin(m_buf), it) << "] = ";

				if (std::is_arithmetic_v<_T>)
					rOstr << std::setw(sizeof(_T));

				rOstr << *it << std::endl;
			}
		else
			rOstr << "\t\tempty\n";

		rOstr << "\t}\n}\n";
	}
	catch (std::exception const& crException)
	{
		std::cerr << "[ERROR] " << crException.what() << std::endl;

		std::terminate();
	}
	catch (...)
	{
		std::cerr << "[FATAL] Unknown error." << std::endl;

		std::terminate();
	}
}

template<typename T, typename Cont>
Stream<T, Cont>& Stream<T, Cont>::operator<<(std::string const& crString)
{
	parse(crString);

	return (*this);
}

template<typename T, typename Container>
inline Stream<T, Container>& Stream<T, Container>::operator>>(_T& rVal)
{
	rVal = *begin(m_buf);

	m_buf.erase(cbegin(m_buf));

	return (*this);
}

template<typename T, typename Container>
inline Stream<T, Container>& Stream<T, Container>::operator=(std::initializer_list<_T> list)
{
	using std::swap; // Enable all swaps
	swap(m_buf, _Cont(list.begin(), list.end()));

	return (*this);
}

template<typename T, typename Cont>
std::istream& operator>>(std::istream &rIstr, Stream<T, Cont> const& crStream)
{
	std::string str;
	std::getline(rIstr, str, '\n');

	crStream << str;

	return rIstr;
}

template<typename T, typename Cont>
std::ostream& operator<<(std::ostream &rOstr, Stream<T, Cont> const& crStream)
{
	if (std::size(crStream.m_buf))
		for (auto&& val : crStream.m_buf)
			rOstr << val << " ";
	else
		rOstr << "empty";

	rOstr << std::endl;

	return rOstr;
}

#endif /* __STREAM_HPP_INCLUDED__ */