#include <iostream>
#include <filesystem>
#include <queue>
#include <algorithm>

#include "cxxopts.hpp"

using namespace std;
using namespace cxxopts;
namespace fs = std::filesystem;

void parse(int argc, char* argv[], string& wildcard) {
	try {
		cxxopts::Options options(argv[0], "Program For Deleting Files With Given WildCard");

		options.allow_unrecognised_options()
			.add_options()
			("w,wildcard", "Wildcard For Deleting The File", value<string>())
			("h,help", "Print Help");

		// options.parse_positional({ "w" });

		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			// TODO : Print Help Message
			cout << options.help() << endl;
			exit(0);
		}
		if (result.count("wildcard")) {
			wildcard = result["wildcard"].as<string>();
		}
		else {
			cout << "Secify File name" << endl;
			exit(-1);
		}
	}
	catch (cxxopts::OptionException & e) {
		cerr << "Exception Occured In Parsing Arguments \n" << e.what() << endl;
		exit(-1);
	}
}

bool matches(const string& needle, const string& prefix, const string& suffix) {
	return needle.starts_with(prefix) && needle.ends_with(suffix);
}

int mainq(int argc, char* argv[]) {
	string wildcard;

	parse(argc, argv, wildcard);

	string prefix, suffix;
	prefix = wildcard.substr(0, wildcard.rfind('*'));
	suffix = wildcard.substr(wildcard.rfind('*') + 1);
	
	fs::path curr = fs::current_path();
	for (const auto& entry : fs::directory_iterator(curr)) {
		if (fs::is_directory(entry))
			continue;
		string name = entry.path().filename().string();
		if (matches(name, prefix, suffix)) {
			cout << "Deleting -> " << name << endl;
			fs::remove(entry);
		}
	}
	// cout << "Prefix:\'" << prefix << "' Suffix: \'" << suffix << '\'' << endl;
	return 0;
}