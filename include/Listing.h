// Copyright 2024 LionHackerz

/* Dallas Scott - ds4015
	Listing class prototype */

#ifndef LISTING_H
#define LISTING_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "Database.h"

class Listing {
	public: 
		Listing(Database &db);
		
		/* Returns the details of the job listing 
		*
		*	@param lid		The listing id of the listing */
		std::string getListing(int lid);

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

		/* Change the boolean value of the 'modern_building' column in the
		*  job listing.  If null to begin with, sets to true.  If true,
		*  sets to false. If false, sets to true. 
		*
		*	 @param lid				The listing id of the listing
		*
		*	 @param resCode			Status code to return depending on query results. */
	std::string changeModernWorkspace(int lid, int &resCode);

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
	void parseAI(const std::string listingList);
	/* 
	// description
	*/
	int postListing(); // should return the lid in DB so employer has it and can use it to change/delete listing later
	private:
		Database *db;

};

#endif