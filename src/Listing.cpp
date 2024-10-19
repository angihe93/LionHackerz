// Copyright 2024 LionHackerz
/*  Dallas Scott - ds4015 
	Listing class implementation */

#include "Listing.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

Listing::Listing(Database &db)
{
	this->db = &db;
}

std::string Listing::changeField(int lid, std::string newField)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	std::string data = "{\"field\": \"" + newField + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changePosition(int lid, std::string newPosition)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	std::string data = "{\"position\": \"" + newPosition + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changeJobDescription(int lid, std::string newDescription)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	std::string data = "{\"job_description\": \"" + newDescription + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::getListing(int lid)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listings = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	std::vector<std::vector<std::string>> eid = db->query("Created","eid","lid","eq", std::to_string(lid), false, resCount);
	std::vector<std::vector<std::string>> company = db->query("Employer","company_name","eid", "eq", eid[0][0], false, resCount);

	std::ostringstream oss;

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