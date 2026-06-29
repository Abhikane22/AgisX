#include <iostream>

#include "OrderTests.h"
#include "LimitTests.h"
#include "OrderBookTests.h"
#include "MatchingEngineTests.h"
#include "TestUtils.h"
#include "ExchangeTests.h"
#include "MessageParserTests.h"

int main()
{
    std::cout << "=========================================\n";
    std::cout << "       OPTIMUMDAQ TEST SUITE\n";
    std::cout << "=========================================\n\n";

    runOrderTests();

    runLimitTests();

    runOrderBookTests();

    runMatchingEngineTests();

    runExchangeTests();

    runMessageParserTests();

    printSummary();

    return testsFailed() == 0 ? 0 : 1;
}