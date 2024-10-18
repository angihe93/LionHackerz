/*  Dallas Scott - ds4015 
	Listing class implementation */

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <sstream>
#include "Listing.h"

using namespace std;

Listing::Listing(Database &db)
{
	this->db = &db;
}

string Listing::changeField(int lid, string newField)
{
	int resCount = 0;
	vector<vector<string>> listing = db->query("Listing", "", "lid", "eq", to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	string data = "{\"field\": \"" + newField + "\"}";
	std::cout << data << std::endl;
	string result = db->update("Listing", data, "lid", "eq", to_string(lid));
	return result;
}

string Listing::changePosition(int lid, string newPosition)
{
	int resCount = 0;
	vector<vector<string>> listing = db->query("Listing", "", "lid", "eq", to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	string data = "{\"position\": \"" + newPosition + "\"}";
	std::cout << data << std::endl;
	string result = db->update("Listing", data, "lid", "eq", to_string(lid));
	return result;
}

string Listing::changeJobDescription(int lid, string newDescription)
{
	int resCount = 0;
	vector<vector<string>> listing = db->query("Listing", "", "lid", "eq", to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	string data = "{\"job_description\": \"" + newDescription + "\"}";
	std::cout << data << std::endl;
	string result = db->update("Listing", data, "lid", "eq", to_string(lid));
	return result;
}

string Listing::getListing(int lid)
{
	int resCount = 0;
	vector<vector<string>> listings = db->query("Listing", "", "lid", "eq", to_string(lid), false, resCount);
	vector<vector<string>> eid = db->query("Created","eid","lid","eq", to_string(lid), false, resCount);
	vector<vector<string>> company = db->query("Employer","company_name","eid", "eq", eid[0][0], false, resCount);

	ostringstream oss;

	oss << "\tPosted by: " << company[0][0] << std::endl << std::endl;
	oss << "\tCreated on: " << listings[1][0] << std::endl << std::endl;
	oss << "\tField:  "  << listings[5][0] << std::endl << std::endl;
	oss << "\tPosition: " << listings[6][0] << std::endl << std::endl;
	oss << "\tJob Description: " << listings[7][0] << std::endl << std::endl;
	oss << "\tSkills required: ";

	for (int i = 8; i < 13; i++)
		if (listings[i][0] != "\"null\"")
		{
			oss << listings[i][0];
			if (i != 12)
				oss << ", ";
		}

	oss << std::endl << std::endl;

	if (listings[13][0] != "\"null\"")
		oss << "\tPay: " << listings[13][0] << std::endl << std::endl;

	if (listings[14][0] != "\"null\"")
		oss << "\tFlexibility: " << listings[14][0] << std::endl << std::endl;

	if (listings[15][0] != "\"null\"")
		oss << "\tModern Workspace: " << listings[15][0] << std::endl << std::endl;
	if (listings[16][0] != "\"null\"")
		oss << "\tGender Parity: " << listings[16][0] << std::endl << std::endl;

	if (listings[17][0] != "\"null\"")
		oss << "\tDiverse Workforce: " << listings[17][0] << std::endl << std::endl;
	if (listings[18][0] != "\"null\"")
		oss << "\tRemote Option Available: " << listings[18][0] << std::endl << std::endl;
	if (listings[19][0] != "\"null\"")
		oss << "\tPersonality Types: " << listings[19][0] << std::endl << std::endl;
	if (listings[20][0] != "\"null\"")
		oss << "\tLocation: " << listings[20][0] << std::endl << std::endl;


	return oss.str();
}

bool Listing::postListing() {
	// return true for success, false for error
	// db->insert
	return true;
}