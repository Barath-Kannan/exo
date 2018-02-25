#define BOOST_TEST_MODULE exo test

#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/test/included/unit_test.hpp>
#include <beak/exo/signal.h>

class exo_fixtures
{
public:
    exo_fixtures();
};

exo_fixtures::exo_fixtures()
{
}

BOOST_FIXTURE_TEST_SUITE(exo_tests, exo_fixtures)

BOOST_AUTO_TEST_CASE(test_basic)
{
    beak::exo::signal<int> s;
    {
        auto s2 = s.connect([](int x) -> int {
            std::cout << x << "\n";
            return x + 1;
        });
        s.emit(0);
        s.emit(1);
    }
    s.emit(2);
}

BOOST_AUTO_TEST_SUITE_END()
