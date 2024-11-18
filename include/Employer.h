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

                /**
                 * Allows an employer to change the job flexibility boolean value of a listing 
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param resCode   Status code to return depending on query results
                 * @return true if changed successfully, false otherwise
                 */
                bool changeFlex(int eid, int lid, int &resCode);

                /**
                 * Allows an employer to change the boolean value of the 'mixed_gender' field of the
		*  job listing
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param resCode   Status code to return depending on query results
                 * @return true if changed successfully, false otherwise
                 */
                bool changeGender(int eid, int lid, int &resCode);

                /**
                 * Allows an employer to change the boolean value of the 'diverse_workforce' field of the
                 * job listing
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param resCode   Status code to return depending on query results
                 * @return true if changed successfully, false otherwise
                 */
                bool changeDiversity(int eid, int lid, int &resCode);

                /**
                 * Allows an employer to change the boolean value of the 'remote_available' field of the
                 * job listing
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param resCode   Status code to return depending on query results
                 * @return true if changed successfully, false otherwise
                 */
                bool changeRemote(int eid, int lid, int &resCode);

                /**
                 * Allows an employer to change the location field of the job listing
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param newLocation   The new data for the location
                 * @param resCode   Status code to return depending on query results
                 * @return true if changed successfully, false otherwise
                 */
                bool changeLocation(int eid, int lid, std::string newLocation, int &resCode);

                /**
                 * Allows an employer to change the MBTI field of the job listing
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param newMBTI   The new data for the MBTI
                 * @param resCode   Status code to return depending on query results
                 * @return true if changed successfully, false otherwise
                 */
                bool changeMBTI(int eid, int lid, std::string newMBTI, int &resCode);

                /**
                 * Allows an employer to change the boolean value of the 'modern_building' field of the
                 * job listing
                 * 
                 * @param eid   The employer id of the employer
                 * @param lid   The listing id of the listing
                 * @param resCode   Status code to return depending on query results
                 * @return true if changed successfully, false otherwise
                 */
                bool changeModernWorkspace(int eid, int lid, int &resCode);
                
                // int postListing(...);


        private:
                Database *db;


};

#endif