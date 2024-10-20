// Copyright 2024 LionHackerz

#ifndef EMPLOYER_H
#define EMPLOYER_H

#include <string>
#include "Database.h"
#include "Listing.h"

class Employer {
	public: 
                Employer(Database &db); 

                /**
                 * Check if the employer has the listing it wants to change
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @return true if employer has the listing with lid, false otherwise 
                 */
                bool checkHasListing(int eid, int lid);

                /**
                 * Allows an employer to change the field of a listing
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param newField   The new data for the field
                 * @return true if changed successfully, false otherwise
                 */
                bool changeField(int eid, int lid, std::string newField);

                /**
                 * Allows an employer to change the position of a listing 
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param newPosition   The new data for the position
                 * @return true if changed successfully, false otherwise
                 */
                bool changePosition(int eid, int lid, std::string newPosition);

                /**
                 * Allows an employer to change the job description of a listing 
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param newDescription   The new data for the job description
                 * @return true changed successfully, false otherwise  
                 */
                bool changeJobDescription(int eid, int lid, std::string newDescription);

                // int postListing(...);


        private:
                Database *db;


};

#endif