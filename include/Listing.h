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
	private:
		Database *db;

};

#endif