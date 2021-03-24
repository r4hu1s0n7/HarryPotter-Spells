#include <iostream>
#include <filesystem>
#include <queue>
#include <algorithm>
#include <fstream>
#include <random>

#include "cxxopts.hpp"

using namespace std;
using namespace cxxopts;
namespace fs = std::filesystem;


void parse(int argc, char* argv[], vector<fs::path>& file_list) {
	try {
		cxxopts::Options options(argv[0], "Program To Destroy");

		options.add_options()
			("f,file", "File/s You Wan't To Delete Secuerly", value<vector<string>>())
			("help", "Print Help");

		options.parse_positional({ "file" });

		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			cout << options.help({ "" });
			exit(0);
		}
		if (result.count("file")) {
			const auto& files = result["file"].as<vector<string>>();
			for (const auto& file : files)
				file_list.emplace_back(file);
		}
		else {
			cerr << "Enter Atleast 1 Filename" << endl;
			exit(-1);
		}
	}
	catch (cxxopts::OptionException & e) {
		cerr << "Exception Occured In Parsing Arguments \n" << e.what() << endl;
		exit(-1);
	}
}

int main1(int argc, char* argv[]) {
	vector<fs::path> file_list;

	//parse(argc, argv, file_list);

	random_device device;
	mt19937 generator(device());
	uniform_int_distribution<int> dist(0, 1337);

	if (argv[0]) {
		// TODO : Print Help Message
		cout << "";
		exit(0);
	}
	for (const auto& file : file_list) {
		if (!fs::exists(file))
			continue;

		long long fsize = fs::file_size(file);
		ofstream file_to_overwrite(file, ios::out | ios::binary);
		while ((fsize -= sizeof(int)) > 0) {
			file_to_overwrite << dist(generator);
		}
		file_to_overwrite.close();

		fs::remove(file);
	}
	
	return 0;
}