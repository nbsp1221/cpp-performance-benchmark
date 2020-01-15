#include "PerformanceBenchmark.hpp"

#include <iostream>
#include <unordered_map>

using namespace std;

void codeBlock1()
{
	unordered_map<string, int> data;
	bool isKeyExist;

	data["Test 1"] = 10;
	data["Test 2"] = 30;
	data["Test 3"] = 15;

	for (int i = 0; i < 10000; i++) {
		isKeyExist = data.find("Test 2") != data.end();
	}
}

void codeBlock2()
{
	unordered_map<string, int> data;
	bool isKeyExist;

	data["Test 1"] = 10;
	data["Test 2"] = 30;
	data["Test 3"] = 15;

	for (int i = 0; i < 10000; i++) {
		isKeyExist = data.count("Test 2") > 0;
	}
}

int main()
{
	PerformanceBenchmark pb;

	pb.addCodeBlock("unordered_map (find)", codeBlock1);
	pb.addCodeBlock("unordered_map (count)", codeBlock2);
	pb.setIterationCount(50);

	cout << "Start\n\n";
	pb.runBenchmark();
	cout << pb.getResult() << "End\n";

	return 0;
}