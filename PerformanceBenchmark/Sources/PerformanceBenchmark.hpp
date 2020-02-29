/*
 * Performance Benchmark
 *
 * Author: retn0 (nbsp1221)
 * GitHub Repository: https://github.com/nbsp1221/cpp-performance-benchmark
 *
 * Compiled and tested with 'Visual Studio 2019'
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cassert>

class PerformanceBenchmark
{
public:
	using _clock = std::chrono::system_clock;
	using _duration = std::chrono::duration<double>;
	using _seconds = std::chrono::seconds;
	using _milliseconds = std::chrono::milliseconds;
	using _microseconds = std::chrono::microseconds;
	using _nanoseconds = std::chrono::nanoseconds;
	using _function = void (*)();
	using _function_table = std::unordered_map<std::string, _function>;
	using _duration_table = std::unordered_map<std::string, _duration>;

	PerformanceBenchmark();

	bool isExistCodeName(const std::string& codeName) const;
	std::size_t getCodeBlockCount() const;
	std::size_t getIterationCount() const;

	void addCodeBlock(const std::string& codeName, _function codeBlock);
	void setIterationCount(std::size_t iterationCount);
	void runBenchmark();

	double getElapsedTime(const std::string& codeName) const;
	long long getSeconds(const std::string& codeName) const;
	long long getMilliseconds(const std::string& codeName) const;
	long long getMicroseconds(const std::string& codeName) const;
	long long getNanoseconds(const std::string& codeName) const;
	std::string getResult() const;

private:
	_function_table codeBlocks;
	_duration_table elapsedTimes;
	std::size_t iterationCount;

	_duration calcElapsedTime(_function codeBlock) const;
	_duration getAverageTime(const _duration& sumTime) const;
};

//==================================================
// Public
//==================================================

PerformanceBenchmark::PerformanceBenchmark() : codeBlocks(), elapsedTimes(), iterationCount(1) { }

bool PerformanceBenchmark::isExistCodeName(const std::string& codeName) const
{
	return this->codeBlocks.count(codeName) > 0;
}

std::size_t PerformanceBenchmark::getCodeBlockCount() const
{
	return this->codeBlocks.size();
}

std::size_t PerformanceBenchmark::getIterationCount() const
{
	return this->iterationCount;
}

void PerformanceBenchmark::addCodeBlock(const std::string& codeName, _function codeBlock)
{
	assert(!isExistCodeName(codeName));
	this->codeBlocks[codeName] = codeBlock;
}

void PerformanceBenchmark::setIterationCount(std::size_t iterationCount)
{
	assert(iterationCount > 0);
	this->iterationCount = iterationCount;
}

void PerformanceBenchmark::runBenchmark()
{
	for (const auto& codeInfo : this->codeBlocks) {
		_duration elapsedTime = calcElapsedTime(codeInfo.second);
		_duration sumTime = elapsedTime;

		for (std::size_t j = 1; j < this->iterationCount; j++) {
			sumTime += calcElapsedTime(codeInfo.second);
		}

		this->elapsedTimes[codeInfo.first] = getAverageTime(sumTime);
	}
}

double PerformanceBenchmark::getElapsedTime(const std::string& codeName) const
{
	assert(isExistCodeName(codeName));
	return this->elapsedTimes.at(codeName).count();
}

long long PerformanceBenchmark::getSeconds(const std::string& codeName) const
{
	assert(isExistCodeName(codeName));
	return std::chrono::duration_cast<_seconds>(this->elapsedTimes.at(codeName)).count();
}

long long PerformanceBenchmark::getMilliseconds(const std::string& codeName) const
{
	assert(isExistCodeName(codeName));
	return std::chrono::duration_cast<_milliseconds>(this->elapsedTimes.at(codeName)).count();
}

long long PerformanceBenchmark::getMicroseconds(const std::string& codeName) const
{
	assert(isExistCodeName(codeName));
	return std::chrono::duration_cast<_microseconds>(this->elapsedTimes.at(codeName)).count();
}

long long PerformanceBenchmark::getNanoseconds(const std::string& codeName) const
{
	assert(isExistCodeName(codeName));
	return std::chrono::duration_cast<_nanoseconds>(this->elapsedTimes.at(codeName)).count();
}

std::string PerformanceBenchmark::getResult() const
{
	std::string result;
	std::string fastestCodeName;
	double minValue = DBL_MAX;

	for (const auto& elapsedTime : this->elapsedTimes) {
		if (minValue > elapsedTime.second.count()) {
			fastestCodeName = elapsedTime.first;
			minValue = elapsedTime.second.count();
		}
	}

	for (const auto& elapsedTime : this->elapsedTimes) {
		double secondsTime = elapsedTime.second.count();
		int percentage = (int)(minValue / secondsTime * 100);

		result += "[" + elapsedTime.first + "] ";
		result += elapsedTime.first == fastestCodeName ? "(fastest)\n" : "\n";
		result += "- Speed: " + std::to_string(percentage) + "%\n";
		result += "- Elapsed Time: " + std::to_string(secondsTime) + " seconds\n\n";
	}

	return result;
}

//==================================================
// Private
//==================================================

PerformanceBenchmark::_duration PerformanceBenchmark::calcElapsedTime(_function codeBlock) const
{
	_clock::time_point startTime = _clock::now();
	codeBlock();
	_clock::time_point endTime = _clock::now();

	return endTime - startTime;
}

PerformanceBenchmark::_duration PerformanceBenchmark::getAverageTime(const _duration& sumTime) const
{
	return sumTime / this->iterationCount;
}
