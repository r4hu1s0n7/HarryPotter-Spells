#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cerrno>

#include "cxxopts.hpp"

using namespace std;
using namespace cxxopts;
namespace fs = std::filesystem;

void parse(int argc, char* argv[], vector<fs::path>& file_list) {
	try {
		cxxopts::Options options(argv[0], "Program Decompressing The Files Using 7zip");
		
		options.allow_unrecognised_options()
			.add_options()
			("i,input", "Input File", value<vector<string>>())
			("h,help", "Print Help Here");

		options.parse_positional({ "input" });

		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			// TODO : Print Help Message
			cout << options.help() << endl;
			exit(0);
		}
		if (result.count("input")) {
			const auto& files = result["input"].as<vector<string>>();
			for(const auto& file : files)
				file_list.emplace_back(file);
		}
		else {
			cerr << "Input File Not Found" << endl;
			exit(-1);
		}
	}
	catch (cxxopts::OptionException & e) {
		cerr << "Exception Occured In Parsing Arguments \n" << e.what() << endl;
	}
}

int main4(int argc, char* argv[], char* envp[]) {
	vector<fs::path> file_list;

	parse(argc, argv, file_list);

	for(const auto& file : file_list) {
		if(!fs::exists(file))
			continue;

		const string decompress = "7z x "  // 7z x flag decrypts
								+file.string();

		system(decompress.c_str());
	}

	return 0;
}