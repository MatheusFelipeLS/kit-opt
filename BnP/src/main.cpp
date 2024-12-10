#include "Data.h"
#include "BnB.h"
#include <iostream>
#include <list>
#include <chrono>

using namespace std;

int main(int argc, char **argv)
{
  if (argc != 2) {
		printf("Usage:\n./bin instance\n");
		return 0;
  }

	Data data;
	data.readData(argv[1]);

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	int qtBins = BnB(&data, 2);

	cout << "Bins usados: " << qtBins << endl;

	end = std::chrono::system_clock::now();

	int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::cout << "Elapsed time (s): " << elapsed_seconds / 1000.0 << "\n\n\n";

	return 0;
}