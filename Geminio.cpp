#include <iostream>
#include <filesystem>
#include <vector>

#include "cxxopts.hpp"

using namespace std;
using namespace cxxopts;
namespace fs = std::filesystem;

void parse(int argc, char* argv[], vector<string>& filename) {
	try {
		cxxopts::Options options(argv[0], "Program For Duplicating File");

		options.allow_unrecognised_options()
			.add_options()
			("filenames", "Name Of Files To Duplicate.", value<vector<string>>())
			("h,help", "Print Help");

		options.parse_positional({ "filenames" });

		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			// TODO : Print Help Message
			cout << options.help({ "" }) << endl;
			exit(0);
		}
		if (result.count("filenames")) {
			const auto& files = result["filenames"].as<vector<string>>();
			for (const auto& file : files)
				filename.push_back(file);
		}

	}
	catch (cxxopts::OptionException & e) {
		cerr << "Exception Occured In Parsing Arguments \n" << e.what() << endl;
		exit(-1);
	}
}

int main3(int argc, char* argv[]) {
	vector<string> targets;

	parse(argc, argv, targets);

	const auto& curr = fs::current_path();

	for (const auto& entry : fs::directory_iterator(curr)) {
		if (fs::is_directory(entry))
			continue;
		string name = entry.path().filename().string();
		string name_wo_ext = name.substr(0, name.rfind('.'));
		string ext = name.substr(name.rfind('.') + 1);

		if (any_of(targets.begin(), targets.end(), [name](const string& elem) { return elem == name; })) {
			string new_filename = name_wo_ext + "1" + '.' + ext;
			int count = 2;
			while (fs::exists(fs::path(new_filename))) {
				new_filename = name_wo_ext + to_string(count) + '.' + ext;
				count++;
			}

			fs::copy_file(name, new_filename);
			cout << "Duplicated " << name << "  ->  " << new_filename << endl;
		}
	}
	return 0;
}