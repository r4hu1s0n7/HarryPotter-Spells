#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <filesystem>
#include <fstream>

#include "cxxopts.hpp"

using namespace std;
using namespace cxxopts;
namespace fs = std::filesystem;

void parse(int argc, char* argv[], string& infile, string& outfile, bool& operation, string& key) {
	try {
		cxxopts::Options options(argv[0], "Program For Encrypting And Decrypting Files Using Symmetric Key Cryptography");
		
		options.allow_unrecognised_options()
			.add_options()
			("e,encrypt", "True for Encryption, False for Decryption ", value<bool>(operation))
			("i,input", "Input File", value<string>())
			("o,output", "Output File", value<string>())
			("key", "Key For Crypto", value<string>())
		
			("h,help", "Print Help Here");

		options.parse_positional({ "input", "output" });

		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			// TODO : Print Help Message
			cout << options.help({ "" });
			exit(0);
			
		}
		if (result.count("input")) {
			infile = result["input"].as<string>();
		}
		else {
			cout << "Specify Input File";
			exit(0);
		}
		if (result.count("output")) {
			outfile = result["output"].as<string>();
		}
		else {
			cout << "Specify output file";
			exit(0);
		}
		if (result.count("key")) {
			key = result["key"].as<string>();
		}
		else {
			cout << "Specify Key";
			exit(0);
		}
		if (result.count("encrypt")) {
			operation = result["encrypt"].as<bool>();
		}
	}
	catch (cxxopts::OptionException & e) {
		cerr << "Exception Occured In Parsing Arguments \n" << e.what() << endl;
		exit(-1);
	}
}

void Encrypt(const string& infile, const string& outfile, const string& key) {
	if (fs::exists(fs::path(outfile))) {
		char ch = 'Y';
		cout << "File Already Exists Do You Want To Replace (Y/N)" << endl;
		cin >> ch;
		if (toupper(ch) != 'Y')
			exit(0);
	}
	if (!fs::exists(fs::path(infile))) {
		cerr << "Input File Specified Doesn't Exist!! " << endl;
		exit(-1);
	}

	FILE* inp = fopen(infile.c_str(), "rb");
	FILE* out = fopen(outfile.c_str(), "wb");

	if (!inp || !out) {
		cerr << "File Couldn't Be Opened" << endl;
		exit(-1);
	}

	int data;
	int index = 0;

	while ((data = fgetc(inp)) != EOF) {
		int enc = data ^ key[index];
		index = (index + 1) % key.length();
		fputc(enc, out);
	}

	fclose(inp);
	fclose(out);
}

void Decrypt(const string& infile, const string& outfile, const string& key) {
	if (fs::exists(fs::path(outfile))) {
		char ch = 'Y';
		cout << "File Already Exists Do You Want To Replace (Y/N)" << endl;
		cin >> ch;
		if (toupper(ch) != 'Y')
			exit(0);
	}
	if (!fs::exists(fs::path(infile))) {
		cerr << "Input File Specified Doesn't Exist!! " << endl;
		exit(-1);
	}

	FILE* inp = fopen(infile.c_str(), "rb");
	FILE* out = fopen(outfile.c_str(), "wb");

	int data;
	int index = 0;

	while ((data = fgetc(inp)) != EOF) {
		int dec = data ^ key[index];
		index = (index + 1) % key.length();
		fputc(dec, out);
	}

	fclose(inp);
	fclose(out);
}

int main8(int argc, char* argv[]) {
	string infile;
	string outfile;
	string key;

	/*
	Here True Stands For Encryption And False For Decryption
	*/
	bool operation = false;
	parse(argc, argv, infile, outfile, operation, key);

	// --encrypt -i input.txt -o input.bin --key Tushar
	// -i input.bin -o output.txt --key Harry

	if (operation) {		// Encrypt Inputfile to Output File
		Encrypt(infile, outfile, key);
	}
	else {
		Decrypt(infile, outfile, key);
	}
	return 0;
}