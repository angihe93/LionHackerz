// Copyright 2024 LionHackerz

/*  Dallas Scott - ds4015
	Listing class implementation */

#include "Listing.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "RouteController.h"
Listing::Listing(Database &db)
{
	this->db = &db;
}

std::string Listing::changeField(int lid, std::string newField, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}
	std::string data = "{\"field\": \"" + newField + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changePosition(int lid, std::string newPosition, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}
	std::string data = "{\"positionx\": \"" + newPosition + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changeJobDescription(int lid, std::string newDescription, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);

	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string data = "{\"job_description\": \"" + newDescription + "\"}";
	std::cout << data << std::endl;
	std::string result = db->update("Listing", data, "lid", "eq", std::to_string(lid));
	return result;
}

std::string Listing::changeFlex(int lid, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = R"({"job_flexibility" : true})";
	if (listing[14][0] == "\"true\"")
		setVal = R"({"job_flexibility" : false})";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,job_flexibility", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeFlex(int lid, bool newFlex, int resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string newFlexStr = newFlex ? "true" : "false";
	std::string setVal = R"({"job_flexibility" : )" + newFlexStr + "}";
	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,job_flexibility", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeGender(int lid, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = R"({"mixed_gender" : true})";
	if (listing[16][0] == "\"true\"")
		setVal = R"({"mixed_gender" : false})";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,mixed_gender", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeGender(int lid, bool newValue, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string newGenderStr = newValue ? "true" : "false";
	std::string setVal = R"({"mixed_gender" : )" + newGenderStr + "}";
	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));

	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,mixed_gender", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeDiversity(int lid, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = R"({"diverse_workforce" : true})";
	if (listing[17][0] == "\"true\"")
		setVal = R"({"diverse_workforce" : false})";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,diverse_workforce", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeDiversity(int lid, bool newValue, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string newDiversityStr = newValue ? "true" : "false";
	std::string setVal = R"({"diverse_workforce" : )" + newDiversityStr + "}";
	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));

	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,diverse_workforce", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeRemote(int lid, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = R"({"remote_available" : true})";
	if (listing[18][0] == "\"true\"")
		setVal = R"({"remote_available" : false})";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,remote_available", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeRemote(int lid, bool newValue, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string newRemoteStr = newValue ? "true" : "false";
	std::string setVal = R"({"remote_available" : )" + newRemoteStr + "}";
	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));

	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,remote_available", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeLocation(int lid, std::string newLocation, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = "{\"location\": \"" + newLocation + "\"}";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,location", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeMBTI(int lid, std::string newMBTI, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = "{\"personality_types\": \"" + newMBTI + "\"}";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,personality_types", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeModernWorkspace(int lid, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string setVal = R"({"modern_building" : true})";
	if (listing[15][0] == "\"true\"")
		setVal = R"({"modern_building" : false})";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,modern_building", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::string Listing::changeModernWorkspace(int lid, bool newValue, int &resCode)
{
	int resCount = 0;
	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid),
															  false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string newBool = newValue ? "true" : "false";
	std::string setVal = R"({"modern_building" : )" + newBool + "}";

	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	std::vector<std::vector<std::string>> result = db->query("Listing", "lid,modern_building", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[1][0];
}

std::vector<std::string> Listing::getListing(int lid, bool test)
{
	int resCount = 0;

	std::vector<std::vector<std::string>> listings;
	std::vector<std::vector<std::string>> eid;
	std::vector<std::vector<std::string>> company;

	if (!test)
	{
		listings = db->query("Listing_AI", "", "lid", "eq", std::to_string(lid), false, resCount);
		eid = db->query("Created_AI", "eid", "lid", "eq", std::to_string(lid), false, resCount);
		// if listings or eid is uninitialized, return error
		if (listings.size() == 0 || eid.size() == 0)
		{
			std::vector<std::string> res;
			res.push_back("Error: The listing ID you provided does not exist in the database or is not associated with an employer.");
			return res;
		}
		company = db->query("Employer_AI", "company_name", "eid", "eq", eid[0][0], false, resCount);
	}
	else
	{
		listings = db->query("Listing", "", "lid", "eq", std::to_string(lid), true, resCount);
		eid = db->query("Created", "eid", "lid", "eq", std::to_string(lid), true, resCount);
		// if listings or eid is uninitialized, return error
		if (listings.size() == 0 || eid.size() == 0)
		{
			std::vector<std::string> res;
			res.push_back("Error: The listing ID you provided does not exist in the database or is not associated with an employer.");
			return res;
		}
		company = db->query("Employer", "company_name", "eid", "eq", eid[0][0], true, resCount);

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

		std::cout << "returning " << oss.str() << std::endl;
		std::vector<std::string> res;
		res.push_back(oss.str());
		return res;
	}

	std::vector<std::string> listingResults;
	listingResults.push_back(company[0][0]);
	listingResults.push_back(listings[1][0]);
	listingResults.push_back(listings[5][0]);
	listingResults.push_back(listings[6][0]);
	listingResults.push_back(listings[7][0]);

	for (int i = 8; i < 13; i++)
		if (listings[i][0] != "\"null\"")
			listingResults.push_back(listings[i][0]);

	// don't need the if's since these are not nullable in Listing_AI
	// if (listings[13][0] != "\"null\"")
	listingResults.push_back(listings[13][0]);

	// if (listings[14][0] != "\"null\"")
	listingResults.push_back(listings[14][0]);

	// if (listings[15][0] != "\"null\"")
	listingResults.push_back(listings[15][0]);

	// if (listings[16][0] != "\"null\"")
	listingResults.push_back(listings[16][0]);

	// if (listings[17][0] != "\"null\"")
	listingResults.push_back(listings[17][0]);

	// if (listings[18][0] != "\"null\"")
	listingResults.push_back(listings[18][0]);

	// if (listings[19][0] != "\"null\"")
	listingResults.push_back(listings[19][0]);

	// if (listings[20][0] != "\"null\"")
	listingResults.push_back(listings[20][0]);

	return listingResults;
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
				"content": "Generate a job listing with the following fields: area/field (industry), job title/position, job description, annual pay (as a decimal with no text or $), required skill 1, required skill 2, required skill 3, required skill 4, required skill 5, location (city), personality type (MBTI type suited for the type of job), job flexibility, modern building, mixed_gender_workforce, diverse_workforce, remote_availability. The fields from job_flexibility onward can be boolean values true or false (random choice). Print the results for the entry on one line with each field separated by a semicolon with no space after the semicolon. Do not include the field names, just the values. Generate )" +
					   n + R"( such job listings all on one line, with each listing separated by parentheses (). I prefer a wide range of jobs in the arts only (i.e., sculpting, painting, etc.), very diverse, not just typical jobs.  Separately, generate a list of company names and company sizes (small, medium, large) for each of these listings with the same format (name and size separated by semicolon, each company for each listing in its own parentheses, all on one line. Do not list the titles 'job listing' and 'company listing' just start listing them."
			}
		]
	})";

	const std::string insertData = data;
	const std::string url = "https://api.openai.com/v1/chat/completions";
	const std::string method = "AI";
	std::string statusCode = "";
	std::string listing_list = db->request(method, url, insertData, statusCode);

	// if gpt request failed, eg due to not having access to gpt-4o, return error
	if (statusCode != "200")
	{
		std::cout << "statusCode after listing_list = db->request(method, url, insertData, statusCode): " << statusCode << std::endl;
		std::cout << "listing_list: " << listing_list << std::endl;
		return "Error: OpenAI request failed.  Please check you have credits or have access to model gpt-4o.";
	}

	std::string starting_junk = listing_list.substr(0, listing_list.find("\"content\":"));
	listing_list.erase(0, starting_junk.length() + 12);
	listing_list = listing_list.substr(0, listing_list.find("\"refusal\": null"));

	std::cout << listing_list << std::endl;

	parseAI(listing_list, stoi(n));
	return listing_list;
}

void Listing::parseAI(const std::string listings, int n)
{

	std::cout << "in parseAI, n: " << n << std::endl;
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
	std::string company;
	std::string co_size;

	std::vector<int> lids;
	std::vector<int> eids;

	int count = 0;

	while (count < n)
	{
		count++;

		if (localListings[0] == '\"' || localListings[0] == ' ')
			localListings.erase(0, 2);
		else
			localListings.erase(0, 1);

		jobField = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, jobField.length() + 1);
		jobPosition = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, jobPosition.length() + 1);
		jobDescription = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, jobDescription.length() + 1);
		payStr = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, payStr.length() + 1);
		payStr = payStr.substr(0, payStr.find('.'));
		skill1 = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, skill1.length() + 1);
		skill2 = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, skill2.length() + 1);
		skill3 = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, skill3.length() + 1);
		skill4 = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, skill4.length() + 1);
		skill5 = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, skill5.length() + 1);
		location = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, location.length() + 1);
		personality = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, personality.length() + 1);
		flex = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, flex.length() + 1);
		modern = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, modern.length() + 1);
		gender = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, gender.length() + 1);
		diverse = localListings.substr(0, localListings.find(';'));
		localListings.erase(0, diverse.length() + 1);
		remote = localListings.substr(0, localListings.find(')'));
		localListings.erase(0, remote.length() + 1);

		std::string data = "{\"field\": \"" + jobField + "\", \"position\": \"" + jobPosition + "\", \"job_description\": \"" + jobDescription +
						   "\", \"skill1_req\": \"" + skill1 + "\", \"skill2_req\": \"" + skill2 + "\", \"skill3_req\": \"" + skill3 + "\", \"skill4_req\": \"" +
						   skill4 + "\", \"skill5_req\": \"" + skill5 + "\", \"location\": \"" + location + "\", \"personality_types\": \"" + personality +
						   "\", \"job_flexibility\":" + flex + ", \"remote_available\": " + remote + ", \"diverse_workforce\": " + diverse + ", \"mixed_gender\": " +
						   gender + ", \"modern_building\": " + modern + ", \"pay\": " + payStr + "}";

		std::cout << db->insert("Listing_AI", data) << std::endl;
		int resCount = 0;
		std::vector<std::vector<std::string>> lidQ = db->query("Listing_AI", "lid", "order", "lid", "desc", false, resCount);
		lids.push_back(stoi(lidQ[0][0]));
	}

	std::cout << "localListings after inserting to Listing_AI: " << localListings << std::endl;
	std::string localCompanies = localListings.substr(listings.find('(') + 1, std::string::npos);

	while (localCompanies[0] != '(')
		localCompanies.erase(0, 1);

	count = 0;

	while (count < n)
	{
		count++;

		if (localCompanies[0] == '\"' || localCompanies[0] == ' ')
			localCompanies.erase(0, 2);
		else
			localCompanies.erase(0, 1);

		company = localCompanies.substr(0, localCompanies.find(';'));
		localCompanies.erase(0, company.length() + 1);
		co_size = localCompanies.substr(0, localCompanies.find(')'));
		localCompanies.erase(0, co_size.length() + 1);

		std::string data = "{\"company_name\": \"" + company + "\", \"size\": \"" + co_size + "\"}";

		std::cout << db->insert("Employer_AI", data) << std::endl;
		int resCount = 0;
		std::vector<std::vector<std::string>> eidQ = db->query("Employer_AI", "eid", "order", "eid", "desc", false, resCount);
		eids.push_back(stoi(eidQ[0][0]));
	}

	for (int i = 0; i < n; i++)
	{
		std::string data = "{\"eid\": " + std::to_string(eids[i]) + ", \"lid\": " + std::to_string(lids[i]) + "}";
		std::cout << db->insert("Created_AI", data) << std::endl;
	}

	return;
}

int Listing::insertListing(std::map<std::string, std::string> basicInfo, std::map<std::string, std::string> skillsPersonality, int64_t pay, std::map<std::string, bool> boolFields)
{
	std::cout << "in Listing::insertListing" << std::endl;

	std::string jobField = basicInfo["field"];
	std::string jobPosition = basicInfo["position"];
	std::string jobDescription = basicInfo["job_description"];
	std::string skill1 = skillsPersonality["skill1_req"];
	std::string skill2 = skillsPersonality["skill2_req"];
	std::string skill3 = skillsPersonality["skill3_req"];
	std::string skill4 = skillsPersonality["skill4_req"];
	std::string skill5 = skillsPersonality["skill5_req"];
	std::string location = basicInfo["location"];
	std::string personality = skillsPersonality["personality_types"];
	std::string flex = boolFields["job_flexibility"] ? "true" : "false";
	std::string remote = boolFields["remote_available"] ? "true" : "false";
	std::string diverse = boolFields["diverse_workforce"] ? "true" : "false";
	std::string gender = boolFields["mixed_gender"] ? "true" : "false";
	std::string modern = boolFields["modern_building"] ? "true" : "false";
	std::string payStr = std::to_string(pay);

	std::string data = "{\"field\": \"" + jobField + "\", \"position\": \"" + jobPosition + "\", \"job_description\": \"" + jobDescription +
					   "\", \"skill1_req\": \"" + skill1 + "\", \"skill2_req\": \"" + skill2 + "\", \"skill3_req\": \"" + skill3 + "\", \"skill4_req\": \"" +
					   skill4 + "\", \"skill5_req\": \"" + skill5 + "\", \"location\": \"" + location + "\", \"personality_types\": \"" + personality +
					   "\", \"job_flexibility\":" + flex + ", \"remote_available\": " + remote + ", \"diverse_workforce\": " + diverse + ", \"mixed_gender\": " +
					   gender + ", \"modern_building\": " + modern + ", \"pay\": " + payStr + "}";

	std::cout << db->insert("Listing", data) << std::endl;
	int resCount = 0;
	std::vector<std::vector<std::string>> lidQ = db->query("Listing", "lid", "order", "lid", "desc", false, resCount);
	std::cout << "resCount: " << resCount << "  lidQ[0][0]: " << lidQ[0][0] << std::endl;

	// return lid of inserted listing
	if (resCount > 0)
	{
		int lid = std::stoi(lidQ[0][0]);
		return lid;
	}

	return -1;
}

// PETER ADDED
std::string Listing::deleteListing(int lid, int &resCode)
{

	int resCount = 0;

	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);

	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string res = db->deleteRecord("Listing", "lid", "eq", std::to_string(lid));
	resCode = 200;
	return res;
}
std::string Listing::changePay(int lid, int64_t newPay, int &resCode)
{
	int resCount = 0;

	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string newPayStr = std::to_string(newPay);
	std::string setVal = R"({"pay" : )" + newPayStr + "}";
	db->update("Listing", setVal, "lid", "eq", std::to_string(lid));

	std::vector<std::vector<std::string>> result = db->query("Listing", "pay", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[0][0];
}

std::string getSkillsString(const std::vector<std::vector<std::string>> &result, int &resCode)
{
	std::vector<std::string> skills;

	// Gather all non-null, non-empty skills into a single vector
	for (const auto &row : result)
	{
		for (const auto &skill : row)
		{
			if (skill != "null" && !skill.empty())
			{
				skills.push_back(skill);
			}
		}
	}

	resCode = 200;

	// Join all skills into a single comma-separated string
	std::string commaSeparated;
	for (size_t i = 0; i < skills.size(); ++i)
	{
		if (i > 0)
		{
			commaSeparated += ",";
		}
		commaSeparated += skills[i];
	}

	return commaSeparated;
}

std::string Listing::changeSkillRequirements(int lid, std::vector<SkillInput> newSkills, int &resCode)
{
	int resCount = 0;

	std::vector<std::vector<std::string>> listing = db->query2("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}
	// loop through length of newSkills and update each skill
	std::string setVal = "";
	for (int i = 0; i < newSkills.size(); i++)
	{
		SkillInput skill = newSkills[i];
		std::string setVal = R"({"skill)" + std::to_string(i + 1) + R"(_req" : ")" + skill.name + "\"}";
		db->update("Listing", setVal, "lid", "eq", std::to_string(lid));
	}
	std::vector<std::vector<std::string>> result = db->query2("Listing", "skill1_req,skill2_req,skill3_req,skill4_req,skill5_req", "lid", "eq",
															  std::to_string(lid), true, resCount);

	if (!result.empty())
	{
		return getSkillsString(result, resCode);
	}
	else
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}
}
std::string Listing::changePersonalityType(int lid, std::string newPersonalityType, int &resCode)
{
	int resCount = 0;

	std::vector<std::vector<std::string>> listing = db->query("Listing", "", "lid", "eq", std::to_string(lid), false, resCount);
	if (resCount == 0)
	{
		resCode = 404;
		return "Error: The listing ID you provided does not exist in the database.";
	}

	std::string data = "{\"personality_types\": \"" + newPersonalityType + "\"}";
	std::cout << data << std::endl;
	db->update("Listing", data, "lid", "eq", std::to_string(lid));

	std::vector<std::vector<std::string>> result = db->query("Listing", "personality_types", "lid", "eq",
															 std::to_string(lid), false, resCount);
	resCode = 200;
	return result[0][0];
}
