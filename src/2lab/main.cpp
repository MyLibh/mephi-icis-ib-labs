// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Stream.hpp"

signed main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	try
	{
		Stream<int> i = std::string("126 125 0");
		i.dump(std::cout);
		auto [ ib, iiter] = i.find_subseq("125 1 0");
		i.dump();
		i.map([](auto&& x) { return ++x; });
		i.dump(std::cout);
		i.where([](auto&& x) { return (x == 126); });
		i.dump(std::cout);
		std::cout << i.reduce([](auto&& x1, auto&& x2) { return (2 * x1 + x2); }, 5) << std::endl;
		i.dump(std::cout);
		std::cout << i;

		Stream<float> f = std::string("126.8 125.8 0");
		f.dump(std::cout);
		f.map([](auto&& x) { return x + 0.2F; });
		f.dump(std::cout);
		f.where([](auto&& x) { return (x == 126); });
		f.dump(std::cout);
		std::cout << f.reduce([](auto&& x1, auto&& x2) { return (2 * x1 + x2); }, 5.F) << std::endl;
		f.dump(std::cout);
		std::cout << f;

		Stream<std::string> s = std::string("aaa bbb ccc");
		s.dump(std::cout);
		s.map([](auto&& x) { return x + 'x'; });
		s.dump(std::cout);
		s.where([](auto&& x) { return (x == "aaax"); });
		s.dump(std::cout);
		std::cout << s.reduce([](auto&& x1, auto&& x2) { return (x1 + x2); }, "5") << std::endl;
		s.dump(std::cout);
		std::cout << s;
	}
	catch (std::exception const& crException)
	{
		std::cerr << "[ERROR] " << crException.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "[FATAL] Unknown error." << std::endl;
	}

	std::getchar();
	return 0;
}