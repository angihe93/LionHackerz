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

	private:
		Database *db;

};

#endif