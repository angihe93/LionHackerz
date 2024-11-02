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
	std::vector<std::vector<std::string>> listing = db->query("Listing_TEST", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	std::string data = "{\"field\": \"" + newField + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing_TEST", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changePosition(int lid, std::string newPosition)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing_TEST", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	std::string data = "{\"position\": \"" + newPosition + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing_TEST", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changeJobDescription(int lid, std::string newDescription)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing_TEST", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
		return "Error: The listing ID you provided does not exist in the database.";
	std::string data = "{\"job_description\": \"" + newDescription + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing_TEST", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changeFlex(int lid, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing_TEST", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = R"({"job_flexibility" : true})";
	if (listing[14][0] == "\"true\"")
		setVal = R"({"job_flexibility" : false})";

	db->update("Listing_TEST", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing_TEST", "lid,job_flexibility", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}


std::string Listing::changeModernWorkspace(int lid, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing_TEST", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = R"({"modern_building" : true})";
	if (listing[15][0] == "\"true\"")
		setVal = R"({"modern_building" : false})";

	db->update("Listing_TEST", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing_TEST", "lid,modern_building", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::getListing(int lid)
{
	// TODO(angi): do error checking for lid that doesn't exist
	int resCount = 0;
	std::vector<std::vector<std::string>> listings = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	std::vector<std::vector<std::string>> eid = db->query("Created", "eid", "lid", "eq", std::to_string(lid), false, resCount);
	std::vector<std::vector<std::string>> company = db->query("Employer", "company_name", "eid", "eq", eid[0][0], false, resCount);

	std::ostringstream oss;

	oss << "\tPosted by: " << company[0][0] << std::endl
		<< std::endl;
	oss << "\tCreated on: " << listings[1][0] << std::endl
		<< std::endl;
	oss << "\tField:  " << listings[5][0] << std::endl
		<< std::endl;
	oss << "\tPosition: " << listings[6][0] << std::endl
		<< std::endl;
	oss << "\tJob Description: " << listings[7][0] << std::endl
		<< std::endl;
	oss << "\tSkills required: ";

	for (int i = 8; i < 13; i++)
		if (listings[i][0] != "\"null\"")
		{
			oss << listings[i][0];
			if (i != 12)
				oss << ", ";
		}

	oss << std::endl
		<< std::endl;

	if (listings[13][0] != "\"null\"")
		oss << "\tPay: " << listings[13][0] << std::endl
			<< std::endl;

	if (listings[14][0] != "\"null\"")
		oss << "\tFlexibility: " << listings[14][0] << std::endl
			<< std::endl;

	if (listings[15][0] != "\"null\"")
		oss << "\tModern Workspace: " << listings[15][0] << std::endl
			<< std::endl;
	if (listings[16][0] != "\"null\"")
		oss << "\tGender Parity: " << listings[16][0] << std::endl
			<< std::endl;

	if (listings[17][0] != "\"null\"")
		oss << "\tDiverse Workforce: " << listings[17][0] << std::endl
			<< std::endl;
	if (listings[18][0] != "\"null\"")
		oss << "\tRemote Option Available: " << listings[18][0] << std::endl
			<< std::endl;
	if (listings[19][0] != "\"null\"")
		oss << "\tPersonality Types: " << listings[19][0] << std::endl
			<< std::endl;
	if (listings[20][0] != "\"null\"")
		oss << "\tLocation: " << listings[20][0] << std::endl
			<< std::endl;

	return oss.str();
}

std::string Listing::generateAIListing(std::string n)
{
std::string data = R"({
    "model": "gpt-4o",
    "messages": [
        {
            "role": "system",
            "content": "You are a helpful assistant."
        },
        {
            "role": "user",
            "content": "Generate a job listing with the following fields: area/field (industry), job title/position, job description, pay (as a decimal with no text or $), required skill 1, required skill 2, required skill 3, required skill 4, required skill 5, location (city), personality type (MBTI type suited for the type of job), job flexibility, modern building, mixed_gender_workforce, diverse_workforce, remote_availability. The fields from job_flexibility onward can be boolean values true or false (random choice). Print the results for the entry on one line with each field separated by a semicolon with no space after the semicolon. Generate )" + n + R"( such job listings all on one line, with each listing separated by parentheses (). I prefer a wide range of fields, very diverse, not just typical jobs."
        }
    ]
})";

	const std::string insertData = data;
	const std::string url = "https://api.openai.com/v1/chat/completions";
	const std::string method = "AI";
	std::string statusCode = "";

	std::string listing_list = db->request(method, url, insertData, statusCode);
	std::string starting_junk = listing_list.substr(0, listing_list.find("\"content\":"));
	listing_list.erase(0, starting_junk.length() + 12);
	listing_list = listing_list.substr(0, listing_list.find("\"refusal\": null"));
	listing_list.pop_back();
	listing_list.pop_back(); 

	parseAI(listing_list);
	return listing_list;
}

void Listing::parseAI(const std::string listings) 
{

	std::string localListings = listings;

	std::string jobField;
	std::string jobPosition;
	std::string jobDescription;
	std::string payStr;
	std::string skill1;
	std::string skill2;
	std::string skill3;
	std::string skill4;
	std::string skill5;
	std::string personality;
	std::string location;
	std::string modern;
	std::string flex;
	std::string remote;
	std::string gender;
	std::string diverse;

	while (localListings.find("(") != std::string::npos)
	{

	if (localListings[0] == '\"' || localListings[0] == ' ')
		localListings.erase(0, 2);
	else
		localListings.erase(0,1);

	jobField = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, jobField.length() +1);
	jobPosition = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, jobPosition.length() +1);
	jobDescription = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, jobDescription.length() +1);
	payStr = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, payStr.length() +1);
	payStr = payStr.substr(0, payStr.find('.'));
	skill1 = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, skill1.length() +1);	
	skill2 = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, skill2.length() +1);	
	skill3 = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, skill3.length() +1);	
	skill4 = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, skill4.length() +1);	
	skill5 = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, skill5.length() +1);	
	location = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, location.length() +1);
	personality = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, personality.length() +1);
	flex = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, flex.length() +1);
	modern = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, modern.length() + 1);
	gender = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, gender.length() +1);
	diverse = localListings.substr(0, localListings.find(';'));
	localListings.erase(0, diverse.length() +1);
	remote = localListings.substr(0, localListings.find(')'));
	localListings.erase(0, remote.length() +1);

	std::string data = "{\"field\": \"" + jobField + "\", \"position\": \"" + jobPosition + "\", \"job_description\": \"" + jobDescription + 
	"\", \"skill1_req\": \"" + skill1 + "\", \"skill2_req\": \"" + skill2 + "\", \"skill3_req\": \"" + skill3 + "\", \"skill4_req\": \""+ 
		skill4 + "\", \"skill5_req\": \"" + skill5 + "\", \"location\": \"" + location + "\", \"personality_types\": \"" + personality + 
		"\", \"job_flexibility\":" + flex + ", \"remote_available\": " + remote + ", \"diverse_workforce\": " + diverse + ", \"mixed_gender\": " +
		gender + ", \"modern_building\": " + modern + ", \"pay\": " + payStr + "}";
		

	std::cout << db->insert("Listing_AI", data) << std::endl;
	}
		
	return;

}


int Listing::postListing()
{
	// return true for success, false for error
	// db->insert
	return 0;
}