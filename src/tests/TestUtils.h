#pragma once

#include <string>

void beginSuite(const std::string& suiteName);

void expect(bool condition,
            const std::string& testName);

void endSuite();

void printSummary();

int testsPassed();

int testsFailed();