#include <iostream>
#include <filesystem>
#include <queue>
#include <algorithm>
#include <fstream>

#include "cxxopts.hpp"

using namespace std;
using namespace cxxopts;
namespace fs = std::filesystem;

void parse(int argc, char* argv[], char& delimiter, int& field, vector<fs::path>& inputs) {
	try {
		cxxopts::Options options(argv[0], "CUT Program Demo");

		options.allow_unrecognised_options()
			.add_options()
			("d,delimiter", "use DELIM instead of TAB for field delimiter", value<char>()->default_value("\t"), "\\t")
			("f,field", "Select Only This Field", value<int>()->default_value("0"), "0")
			("FILE", "Name Of The File To Read From", value<vector<string>>());
			("h,help", "Print Help");

		options.parse_positional({ "FILE" });

		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			cout << options.help({ "" });
			exit(0);
		}
		if (result.count("delimiter")) {
			delimiter = result["delimiter"].as<char>();
		}
		if (result.count("field")) {
			field = result["field"].as<int>();
		}
		if (result.count("FILE")) {
			const auto& names = result["FILE"].as<vector<string>>();
			for (const auto& name : names) {
				inputs.emplace_back(name);
			}
		}
	}
	catch (cxxopts::OptionException & e) {
		cerr << "Exception Occured In Parsing Arguments \n" << e.what() << endl;
		exit(-1);
	}
}
// 5
int main6(int argc, char* argv[]) {
	vector<fs::path> inputs;
	char delimiter;
	int field;

	parse(argc, argv, delimiter, field, inputs);

	for (const auto& input : inputs) {
		if (!fs::exists(input))
			continue;

		ifstream ifs(input.c_str());
		cout << input.filename().string() << " : " << endl;
		string str;
		int count = 0;
		while (true) {
			count++;
			getline(ifs, str, delimiter);
			if (count == field)
				cout << str << endl;
			if (ifs.eof() || ifs.fail())
				break;
		}
	}
	return 0;
}