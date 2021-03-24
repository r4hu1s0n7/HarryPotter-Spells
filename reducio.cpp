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

void parse(int argc, char* argv[], vector<fs::path>& file_list, int& level, string& multithread) {
	try {
		cxxopts::Options options(argv[0], "Program Compressing The Files Using 7zip");
		
		options.allow_unrecognised_options()
			.add_options()
			("l,level", "Level Of Compression", value<int>(level)->default_value("9"))
			("mt", "Pass This Flag To Disable Multithreading", value<string>(multithread)->default_value("on"), "on/off")
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

int main0(int argc, char* argv[]) {
	vector<fs::path> file_list;
	int level;
	string multithread;

	parse(argc, argv, file_list, level, multithread);

	for(const auto& file : file_list) {
		if(!fs::exists(file))
			continue;

		const string compress = "7z a "
								"-t7z "									// For Compressing File
								"-m0=lzma "								// Set LZMA Method
								"-mx=" + to_string(level) + " "			// Compression Level 9 = ULTRA
								"-mfb=64 "								// Fast Bytes To Use Dont Change
																		// Unless You Know What You Are Doing
								"-md=32m "								// Dictionary Size 32MB
								"-mmt=" + multithread + " "				// Use Maximum Threads Possible
								+file.string() + ".7z "
								+file.string();

		system(compress.c_str());
		//compress(file, args, envp);
	}

	return 0;
}