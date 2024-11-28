// Copyright 2024 LionHackerz

/* Dallas Scott - ds4015
	Listing class prototype */

#ifndef LISTING_H
#define LISTING_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <map>
#include "Database.h"

class Listing
{
public:
	Listing(Database &db);

	/* Returns the details of the job listing
	 *
	 *	@param lid		The listing id of the listing */
	std::vector<std::string> getListing(int lid, bool test);

	/* Updates the value of the 'field' column in the job listing
	 *
	 *	@param lid			The listing id of the listing
	 *
	 *	@param newField		The new data for the field */
	std::string changeField(int lid, std::string newField);

	/* Updates the value of the 'position' column in the job listing
	 *
	 *	@param lid				The listing id of the listing
	 *
	 *	@param newPosition		The new data for the position */
	std::string changePosition(int lid, std::string newPosition);

	/* Updates the value of the 'job_description' column in the job listing
	 *
	 *	@param lid					The listing id of the listing
	 *
	 *	@param newDescription		The new data for the job description */
	std::string changeJobDescription(int lid, std::string newDescription);

	/* Change the boolean value of the 'job_flexibility' column in the
	 *  job listing.  If null to begin with, sets to true.  If true,
	 *  sets to false. If false, sets to true.
	 *
	 *	 @param lid				The listing id of the listing
	 *
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeFlex(int lid, int &resCode);

	/* Change the boolean value of the 'job_flexbility' column in the
	 *  job listing to a specified bool value.
	 *
	 *	 @param lid				The listing id of the listing
	 *    @param newFlex			The new value of 'job_flexibility' to set to
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeFlex(int lid, bool newFlex, int resCode);

	/* Change the boolean value of the 'modern_building' column in the
	 *  job listing.  If null to begin with, sets to true.  If true,
	 *  sets to false. If false, sets to true.
	 *
	 *	 @param lid				The listing id of the listing
	 *
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeModernWorkspace(int lid, int &resCode);

	/* Change the boolean value of the 'modern_building' column in the
	 *  job listing to a specified bool value.
	 *
	 *	 @param lid				The listing id of the listing
	 *    @param newValue			The new value of 'modern_building' to set to
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeModernWorkspace(int lid, bool newValue, int &resCode);

	/* Change the boolean value of the 'mixed_gender' column in the
	 *  job listing.  If null to begin with, sets to true.  If true,
	 *  sets to false. If false, sets to true.
	 *
	 *	 @param lid				The listing id of the listing
	 *
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeGender(int lid, int &resCode);

	/* Change the boolean value of the 'mixed_gender' column in the
	 *  job listing to a specified bool value
	 *
	 *	 @param lid				The listing id of the listing
	 *    @param newValue			The new value of 'mixed_gender' to set to
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeGender(int lid, bool newValue, int &resCode);

	/* Change the boolean value of the 'diverse_workforce' column in the
	 *  job listing.  If null to begin with, sets to true.  If true,
	 *  sets to false. If false, sets to true.
	 *
	 *	 @param lid				The listing id of the listing
	 *
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeDiversity(int lid, int &resCode);

	/* Change the boolean value of the 'diverse_workforce' column in the
	 *  job listing to a specified bool value.
	 *
	 *	 @param lid				The listing id of the listing
	 *    @param newValue 			The new value of 'diverse_workforce' to set to
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeDiversity(int lid, bool newValue, int &resCode);

	/* Change the boolean value of the 'remote_available' column in the
	 *  job listing.  If null to begin with, sets to true.  If true,
	 *  sets to false. If false, sets to true.
	 *
	 *	 @param lid				The listing id of the listing
	 *
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeRemote(int lid, int &resCode);

	/* Change the boolean value of the 'remote_available' column in the
	 *  job listing to a specified bool value.
	 *
	 *	 @param lid				The listing id of the listing
	 *    @param newValue			The new value of 'remote_available' to set to
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeRemote(int lid, bool newValue, int &resCode);

	/* Change the value of the 'location' column in the
	 *  job listing.
	 *
	 *	 @param lid				The listing id of the listing
	 *
	 *	 @param newLocation		The new value of 'location' to set to.
	 *
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeLocation(int lid, std::string newLocation, int &resCode);

	/* Change the value of the 'personality_types' column in the
	 *  job listing.
	 *
	 *	 @param lid				The listing id of the listing
	 *
	 *	 @param newMBTI		The new value of 'personality_types' to set to.
	 *
	 *	 @param resCode			Status code to return depending on query results. */
	std::string changeMBTI(int lid, std::string newMBTI, int &resCode);

	/* Generate n listings via AI and insert them into the Listing_AI table in the
	 * database.  This enables a quick way to populate the listings in the
	 * database with test data.  It will eventually be used to assist the employer
	 * with creating an auto-generated listing depending on certain inputs.
	 * This returns a string with each listing in parentheses () and each field
	 * in each listing separated by semicolons.  All listings will be on the
	 * same line for parsing with parseAI();
	 *
	 * 	 @param n 				The number (as a string) of AI listings to generate */
	std::string generateAIListing(std::string n);

	/* This function takes as input the string of listings generated by AI
	 * and parses them by field to populate JSON data which will subsequently
	 * be inserted into the database in the Listing_AI table.  It will parse
	 * each entry the AI generated in a loop until no entries remain.
	 *
	 * 	 @param listingList		A string representing the generated AI
	 * 							listings from generateAIListing() */
	void parseAI(const std::string listingList, int n);

	/**
	 * @ Insert a job listing into the database
	 *
	 * @param basicInfo  map (string:string) of basic info for the listing, including field, position, job_description, location
	 * @param skillsPersonality  map (string:string) of skills and personality types required for the listing, including skill1_req, ..., skill5_req, personality_types
	 * @param pay   int64_t of the pay for the listing
	 * @param boolFields   map (string:bool) of boolean fields for the listing, including job_flexibility, modern_building, mixed_gender, diverse_workforce, remote_available
	 * @return  lid (int) for the created listing in the database, or -1 on failure
	 */
	int insertListing(std::map<std::string, std::string> basicInfo, std::map<std::string, std::string> skillsPersonality, int64_t pay, std::map<std::string, bool> boolFields);

	// PETER ADDED

	/**
	 * Deletes a job listing from the database.
	 *
	 * @param lid   The listing id of the listing to delete
	 * @return true if the listing was deleted successfully, false otherwise
	 */
	bool deleteListing(int lid, int &resCode);

	/**
	 * Updates the pay for a job listing.
	 *
	 * @param lid   The listing id of the listing
	 * @param newPay   The new pay for the listing
	 * @return true if the pay was updated successfully, false otherwise
	 */
	std::string changePay(int lid, int64_t newPay, int &resCode);

	/**
	 * Updates the skill requirements for a job listing.
	 *
	 * @param lid   The listing id of the listing
	 * @param newSkills   A map of skill fields (e.g., skill1_req to skill5_req) and their new values
	 * @return true if the skills were updated successfully, false otherwise
	 */
	std::string changeSkillRequirements(int lid, std::map<std::string, std::string> newSkills, int &resCode);

	/**
	 * Updates the personality type requirements for a job listing.
	 *
	 * @param lid   The listing id of the listing
	 * @param newPersonalityTypes   The new personality type requirements for the listing
	 * @return true if the personality types were updated successfully, false otherwise
	 */
	std::string changePersonalityTypes(int lid, std::string newPersonalityTypes, int &resCode);

private:
	Database *db;
};

#endif