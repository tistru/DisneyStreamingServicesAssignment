//#pragma once
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <include/json/json.h>

class JSONParser
{
private:
	Json::Value JsonData;

	// Lists to store the parsed game data for output
	std::vector<std::string> Dates;
	std::vector<std::string> Headlines;
	std::vector<std::string> Blurbs;
	std::vector<std::string> Thumbnails;

	std::string currentNodeValue;

	int gameCounter;						// Global counter to keep track of how many games we have collected data on

	Json::CharReaderBuilder builder;

	// Output data files
	std::ofstream parsedMLBStats;			// Output from parsing the .json file
	std::ofstream treeOutput;				// Output from the Json tree traversal

	// Input JSON text file to parse (Currently, only can read data from JSON text file)
	std::ifstream MLBStatsFile;

public:
	JSONParser(const char *);
	~JSONParser();

	std::vector<std::string> GetDatesList();
	std::vector<std::string> GetHeadlinesList();
	std::vector<std::string> GetBlurbsList();
	std::vector<std::string> GetThumbnailsList();

	bool ParseJsonFile();
	void PrintJSONValue(const Json::Value &, std::ostream &);
	bool PrintJSONTree(const Json::Value &, unsigned short);
	void PrintArrayData(std::vector <std::string>, std::string);

};

