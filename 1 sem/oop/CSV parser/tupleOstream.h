#pragma once

using namespace std;

template <int index, typename... Args>
typename enable_if<(index == sizeof...(Args)), void>::type printTuple(ostream& os, tuple<Args...> const& t)
{
	os << '}';
};

template <int index, typename... Args>
typename enable_if<(index < sizeof...(Args)), void>::type printTuple(ostream& os, tuple<Args...> const& t)
{
	if (index != 0)
	{
		os << ", ";
	}
	os << get<index>(t);
	printTuple<index + 1>(os, t);
}

template <typename... Args>
ostream& operator<<(ostream& os, tuple<Args...> const& t)
{
	os << '{';
	printTuple<0>(os, t);
	return os;
}
