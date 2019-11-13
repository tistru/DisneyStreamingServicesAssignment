#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <include/json/json.h>
#include "JSONParser.h"


int main(void) {
	
	std::string inputJsonFile;

	int MLBFileID = 0;

	while (MLBFileID == 0) {
		std::cout << "Please specify the MLB JSON file to parse[1-3]: 1=[2018-06-10], 2=[2018-08-10], 3=[2019-06-10]\n";
		std::cin >> MLBFileID;

		if (MLBFileID < 1 || MLBFileID > 3) {
			std::cout << "Invalid file. Please re-enter a value between 1 and 3 \n";
		}
	}

	switch (MLBFileID)
	{
	case 1: inputJsonFile = "mlbstats.json";
		break;
	case 2: inputJsonFile = "mlbstats2.json";
		break;
	case 3: inputJsonFile = "mlbstats3.json";
		break;
	default:
		break;
	}

	JSONParser *MLBParser = new JSONParser(inputJsonFile.c_str());

	MLBParser->ParseJsonFile();

	// Print out the data in the lists for each game
	std::cout << "Size of Dates array = " << MLBParser->GetDatesList().size() << std::endl;
	std::cout << "Size of Headlines array = " << MLBParser->GetHeadlinesList().size() << std::endl;
	std::cout << "Size of Blurbs array = " << MLBParser->GetBlurbsList().size() << std::endl;
	std::cout << "Size of Thumbnails array = " << MLBParser->GetThumbnailsList().size() << std::endl;
	
	std::cout << "\n";
	
	MLBParser->PrintArrayData(MLBParser->GetDatesList(), "Game Dates");
	MLBParser->PrintArrayData(MLBParser->GetHeadlinesList(), "Headlines");
	MLBParser->PrintArrayData(MLBParser->GetBlurbsList(), "Blurbs");
	MLBParser->PrintArrayData(MLBParser->GetThumbnailsList(), "Thumbnails");

	delete MLBParser;

	return 0;
}
