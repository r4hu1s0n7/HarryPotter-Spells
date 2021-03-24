#include <iostream>
#include <filesystem>
#include <queue>
#include <algorithm>

#include "cxxopts.hpp"

using namespace std;
using namespace cxxopts;
namespace fs = std::filesystem;

void parse(int argc, char* argv[], string& filename, int& depth) {
	try {
		cxxopts::Options options(argv[0], "Program For Finding File Within Current Directory Recursively");

		options.allow_unrecognised_options()
			.add_options()
			("d,depth", "Depth Upto Which You Want To Find For Specified File", value<int>()->default_value("1"))
			("f,file", "Name Of File To Find", value<string>())
			("h,help", "Print Help");

		options.parse_positional({ "depth", "file" });

		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			// TODO : Print Help Message
			cout << options.help({ "" });
			exit(0);
		}
		if (result.count("depth")) {
			depth = result["depth"].as<int>();
		}

		if (result.count("file")) {
			filename = result["file"].as<string>();
		}
		else {
			cout << options.help({ "" });
			exit(0);
		}
	}
	catch (cxxopts::OptionException & e) {
		cerr << "Exception Occured In Parsing Arguments \n" << e.what() << endl;
		exit(-1);
	}
}

void dfs(const fs::path& path, const string& needle, const int depth) {
	if (depth == 0) {
		return;
	}

	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.is_directory()) {
			dfs(entry.path(), needle, depth - 1);
		}
		if (entry.path().filename().string() == needle) {
			cout << "File Found At : " << entry.path() << endl;
		}
	}

	return;
}

int main2(int argc, char* argv[]) {
	string needle;
	int depth;

	parse(argc, argv, needle, depth);
	depth++;												// Because Current Directory Is Counted Towards Depth

	fs::path curr = fs::current_path();

	dfs(curr, needle, depth);

	return 0;
}