#include "TestUtils.h"

#include <iostream>

namespace
{
    int passed = 0;
    int failed = 0;

    int suitePassed = 0;
    int suiteFailed = 0;

    std::string currentSuite;
}

void beginSuite(const std::string& suiteName)
{
    currentSuite = suiteName;

    suitePassed = 0;
    suiteFailed = 0;

    std::cout << "\n=========================================\n";
    std::cout << suiteName << '\n';
    std::cout << "=========================================\n";
}

void expect(bool condition,
            const std::string& testName)
{
    if (condition)
    {
        ++passed;
        ++suitePassed;

        std::cout
            << "[PASS] "
            << currentSuite
            << " :: "
            << testName
            << '\n';
    }
    else
    {
        ++failed;
        ++suiteFailed;

        std::cout
            << "[FAIL] "
            << currentSuite
            << " :: "
            << testName
            << '\n';
    }
}

void endSuite()
{
    std::cout << "\n";

    std::cout << "Passed : "
              << suitePassed
              << '\n';

    std::cout << "Failed : "
              << suiteFailed
              << "\n";
}

int testsPassed()
{
    return passed;
}

int testsFailed()
{
    return failed;
}

void printSummary()
{
    std::cout << "\n=========================================\n";
    std::cout << "              TEST SUMMARY\n";
    std::cout << "=========================================\n";

    std::cout << "Passed : "
              << passed
              << '\n';

    std::cout << "Failed : "
              << failed
              << "\n";

    if (failed == 0)
    {
        std::cout << "\nALL TESTS PASSED ✅\n";
    }
    else
    {
        std::cout << "\nSOME TESTS FAILED ❌\n";
    }

    std::cout << "=========================================\n";
}