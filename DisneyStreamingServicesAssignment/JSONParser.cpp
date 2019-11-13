#include "JSONParser.h"
#include <fstream>
#include <iostream>

using namespace std;

JSONParser::JSONParser(const char * inputFilePath) {
	currentNodeValue = "";

	MLBStatsFile.open(inputFilePath);
	parsedMLBStats.open("output/MLBOutput.txt");
	treeOutput.open("output/JSONTree.txt");

	gameCounter = 0;

	Dates.clear();
	Headlines.clear();
	Blurbs.clear();
	Thumbnails.clear();
}


JSONParser::~JSONParser()
{
	Dates.clear();
	Headlines.clear();
	Blurbs.clear();
	Thumbnails.clear();

	MLBStatsFile.close();
	parsedMLBStats.close();
	treeOutput.close();

}

std::vector<std::string> JSONParser::GetDatesList() {
	return Dates;
}

std::vector<std::string> JSONParser::GetHeadlinesList() {
	return Headlines;
}

std::vector<std::string> JSONParser::GetBlurbsList() {
	return Blurbs;
}

std::vector<std::string> JSONParser::GetThumbnailsList() {
	return Thumbnails;
}

// Print out the current JSON node based on its type
void JSONParser::PrintJSONValue(const Json::Value &nodeVal, std::ostream &out) {
	if (nodeVal.isString()) {
		out << "string(" << nodeVal.asString().c_str() << ")";
	}
	else if (nodeVal.isBool()) {
		out << "bool(" << nodeVal.asBool() << ")";
	}
	else if (nodeVal.isInt()) {
		out << "int(" << nodeVal.asInt() << ")";
	}
	else if (nodeVal.isUInt()) {
		out << "uint(" << nodeVal.asUInt() << ")";
	}
	else if (nodeVal.isDouble()) {
		out << "double(" << nodeVal.asDouble() << ")";
	}
	else
	{
		out << "unknown type=[" << nodeVal.type() << "]";
	}
}

// Function to parse the raw Json file using JsonCPP. Output into a new text file for viewing
bool JSONParser::ParseJsonFile() {
	std::string errors;

	// JSONCPP parse function
	bool parsingSuccessful = parseFromStream(builder, MLBStatsFile, &JsonData, &errors);

	if (!parsingSuccessful)
	{
		std::cout << errors << std::endl;
		return false;
	}

	PrintJSONTree(JsonData, JsonData.size());

	parsedMLBStats << JsonData;
	//parsedMLBStats.close();

	return true;
}


bool JSONParser::PrintJSONTree(const Json::Value &root, unsigned short depth) {
	depth += 1;

	treeOutput << " {type=[" << root.type() << "], size=" << root.size() << "}";

	if (root.size() > 0) {
		int size = root.size();
		treeOutput << "\n";
		for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++) {
			// Print depth. 
			for (int tab = 0; tab < depth; tab++) {
				treeOutput << "-";
			}

			// Using CalendarEventID to keep track of which game I'm in since it is a unique identifier for each game
			if (!itr.name().compare("calendarEventID"))
			{
				gameCounter++;
				currentNodeValue = root["calendarEventID"].asString();
				Dates.push_back(currentNodeValue);
			}

			else if (!itr.name().compare("headline")) {
				if (gameCounter != Headlines.size()) {
					currentNodeValue = root["headline"].asString();
					Headlines.push_back(currentNodeValue);
				}
			}

			else if (!itr.name().compare("blurb")) {
				if (gameCounter != Headlines.size()) {
					currentNodeValue = root["blurb"].asString();
					Blurbs.push_back(currentNodeValue);
				}
			}

			// Index #4 of the cuts array is the 800 x 448 resolution thumbnail. However, i noticed that in some of the MLB stats data, the cuts data type is not an array. Therefore i use a different syntax depending on the type to access the value of the thumbnail string
			else if (!itr.name().compare("photo")) {
				if (gameCounter != Thumbnails.size()) {
					if ((root["photo"]["cuts"].isArray())) {
						//PrintJSONValue(root["photo"]["cuts"][4]["src"], std::cout);
						currentNodeValue = root["photo"]["cuts"][4]["src"].asString();
					}
					else {
						//PrintJSONValue(root["photo"]["cuts"]["800x448"]["src"], std::cout);
						currentNodeValue = root["photo"]["cuts"]["800x448"]["src"].asString();
					}
					Thumbnails.push_back(currentNodeValue);
				}
			}

			treeOutput << " subvalue(";
			PrintJSONValue(itr.key(), treeOutput);
			treeOutput << ") -";
			PrintJSONTree(*itr, depth);
		}
		return true;
	}
	else {
		treeOutput << " ";

		PrintJSONValue(root, treeOutput);
		treeOutput << "\n";
	}
	return true;

}

// Helper function to print the data in the output lists
void JSONParser::PrintArrayData(std::vector <std::string> data, std::string title) {
	std::cout << title << ":" << std::endl;

	if (!data.empty()) {
		for (int s = 0; s < data.size(); s++) {
			std::cout << s + 1 << ": " << data[s] << std::endl;
		}
	}

	std::cout << "\n\n";
}
