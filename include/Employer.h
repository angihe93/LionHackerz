// Copyright 2024 LionHackerz

#ifndef EMPLOYER_H
#define EMPLOYER_H

#include <string>
#include "Database.h"
#include "Listing.h"

class Employer
{
public:
        Employer(Database &db, Listing &l);

        /**
         * @brief Check listing
         * 
         * Check if the employer has the listing it wants to change
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @return true if employer has the listing with lid, false otherwise
         */
        bool checkHasListing(int eid, int lid, int &resCode);

        /**
         * @brief Change job field
         * 
         * Allows an employer to change the field of a listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newField The new data for the field
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeField(int eid, int lid, std::string newField, int &resCode);

        /**
         * @brief Change job title/position
         * 
         * Allows an employer to change the position of a listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newPosition The new data for the position
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changePosition(int eid, int lid, std::string newPosition, int &resCode);

        /**
         * @brief CHange job description
         * 
         * Allows an employer to change the job description of a listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newDescription The new data for the job description
         * @param resCode Status code to return depending on query results
         * @return true changed successfully, false otherwise
         */
        bool changeJobDescription(int eid, int lid, std::string newDescription, int &resCode);

        /**
         * @brief Change flexibility
         * 
         * Allows an employer to change the job flexibility boolean value of a listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param resCode  Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeFlex(int eid, int lid, int &resCode);

        /**
         * @brief Change flexibility overload
         * 
         * Allows an employer to change the job flexibility boolean value of a listing to a specified value
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newFlex The new bool value for the job flexibility
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeFlex(int eid, int lid, bool newFlex, int &resCode);

        /**
         * @brief Change mixed_gender
         * 
         * Allows an employer to change the boolean value of the 'mixed_gender' field of the
         *  job listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeGender(int eid, int lid, int &resCode);

        /**
         * @brief Change mixed_gender overload
         * 
         * Allows an employer to change the boolean value of the 'mixed_gender' field of the
         *  job listing to a specified bool value
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newValue The new bool value for the mixed_gender field
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeGender(int eid, int lid, bool newValue, int &resCode);

        /**
         * @brief Change diverse_workforce
         * 
         * Allows an employer to change the boolean value of the 'diverse_workforce' field of the
         * job listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeDiversity(int eid, int lid, int &resCode);

        /**
         * @brief Change diverse_workforce overload 
         * 
         * Allows an employer to change the boolean value of the 'diverse_workforce' field of the
         * job listing to a specified bool value
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newValue The new bool value for the diverse_workforce field
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeDiversity(int eid, int lid, bool newValue, int &resCode);

        /**
         * @brief Change remote_available
         * 
         * Allows an employer to change the boolean value of the 'remote_available' field of the
         * job listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeRemote(int eid, int lid, int &resCode);

        /**
         * @brief Change remote_available overload
         * 
         * Allows an employer to change the boolean value of the 'remote_available' field of the
         * job listing to a specified bool value.
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newValue The new bool value for the remote_available field
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeRemote(int eid, int lid, bool newValue, int &resCode);

        /**
         * @brief Change job location
         * 
         * Allows an employer to change the location field of the job listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newLocation The new data for the location
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeLocation(int eid, int lid, std::string newLocation, int &resCode);

        /**
         * @brief Change MBTI
         * 
         * Allows an employer to change the MBTI field of the job listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newMBTI The new data for the MBTI
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeMBTI(int eid, int lid, std::string newMBTI, int &resCode);

        /**
         * @brief Change modern_building
         * 
         * Allows an employer to change the boolean value of the 'modern_building' field of the
         * job listing
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeModernWorkspace(int eid, int lid, int &resCode);

        /**
         * @brief Change modern_building overload
         * 
         * Allows an employer to change the boolean value of the 'modern_building' field of the
         * job listing to a specified value
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newValue The new value of 'modern_building' to set to
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeModernWorkspace(int eid, int lid, bool newValue, int &resCode);

        // change all employer's listings at once

        /**
         * @brief Change all job field entries
         * 
         * Allows an employer to change the field of all its listings
         *
         * @param eid The employer id of the employer
         * @param newField The new data for the field
         * @param resCode Status code to return depending on query results
         * @return true if all listings' fields were changed successfully, false otherwise
         */
        bool changeFieldAll(int eid, std::string newField, int &resCode);

        /**
         * @brief Change all job title/position entries
         * 
         * Allows an employer to change the position of all its listings
         *
         * @param eid The employer id of the employer
         * @param newPosition The new data for the position
         * @param resCode Status code to return depending on query results
         * @return true if all listings' positions were changed successfully, false otherwise
         */
        bool changePositionAll(int eid, std::string newPosition, int &resCode);

        // changeDescriptionAll probably not needed, not a likely use case

        /**
         * @brief Change all flexibility
         * 
         * Allows an employer to change the job flexibility boolean value of all its listings
         *
         * @param eid The employer id of the employer
         * @param newFlex The new bool value for the job flexibility
         * @param resCode Status code to return depending on query results
         * @return true if all listings' job flexibility were changed successfully, false otherwise
         */
        bool changeFlexAll(int eid, bool newFlex, int &resCode);

        /**
         * @brief Change all modern_building
         * 
         * Allows an employer to change the boolean value of the 'modern_building' field of all its listings
         *
         * @param eid The employer id of the employer
         * @param newValue The new value of 'modern_building' to set to
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeModernWorkspaceAll(int eid, bool newValue, int &resCode);

        /**
         * @brief Change all mixed_gender
         * 
         * Allows an employer to change the boolean value of the 'mixed_gender' field of all its listings
         *
         * @param eid The employer id of the employer
         * @param newValue The new value of 'mixed_gender' to set to
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeGenderAll(int eid, bool newValue, int &resCode);

        /**
         * @brief Change all diverse_workforce
         * 
         * Allows an employer to change the boolean value of the 'diverse_workforce' field of all its listings
         *
         * @param eid The employer id of the employer
         * @param newValue The new value of 'diverse_workforce' to set to
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeDiversityAll(int eid, bool newValue, int &resCode);

        /**
         * @brief Change all remote_available
         * 
         * Allows an employer to change the boolean value of the 'remote_available' field of all its listings
         *
         * @param eid The employer id of the employer
         * @param newValue The new value of 'remote_available' to set to
         * @param resCode Status code to return depending on query results
         * @return true if changed successfully, false otherwise
         */
        bool changeRemoteAll(int eid, bool newValue, int &resCode);

        /**
         * @brief Post new listing
         * 
         * Allows an employer to post a new listing
         *
         * @param eid The employer id of the employer
         * @param basicInfo map (string:string) of basic info for the listing, including field, position, job_description, location
         * @param skillsPersonality map (string:string) of skills and personality types required for the listing, including skill1_req, ..., skill5_req, personality_types
         * @param pay int64_t of the pay for the listing
         * @param boolFields map (string:bool) of boolean fields for the listing, including job_flexibility, modern_building, mixed_gender, diverse_workforce, remote_available
         * @param resCode Status code to return depending on query results
         * @return lid (int) for the created listing, or -1 on failure
         */
        int postListing(int eid, std::map<std::string, std::string> basicInfo, std::map<std::string, std::string> skillsPersonality, int64_t pay, std::map<std::string, bool> boolFields, int &resCode);

        /**
         * @brief Create employer
         * 
         * Create a new employer and insert into database
         *
         * @param company_name Employer's company name
         * @param size Employer's company size
         * @param resCode Status code to return depending on query results
         * @return eid (int) for the created employer, or -1 on failure
         */
        int createEmployer(std::string company_name, std::string size, int &resCode);

        // PETER ADDED

        /**
         * @brief Delete listing
         * 
         * Allows an employer to delete a listing they own.
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param resCode Status code to return depending on query results
         * @return true if the listing was deleted successfully, false otherwise
         */
        bool deleteListing(int eid, int lid, int &resCode);

        /**
         * @brief Change pay
         * 
         * Allows an employer to change the pay for a listing.
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newPay The new pay for the listing
         * @param resCode Status code to return depending on query results
         * @return true if the pay was updated successfully, false otherwise
         */
        bool changePay(int eid, int lid, int64_t newPay, int &resCode);

        /**
         * @brief Change skill_reqs
         *
         * Allows an employer to update the skill requirements for a listing.
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newSkills A vector of SkillInput representing new skill requirements
         * @param resCode Status code to return depending on query results
         * @return true if the skills were updated successfully, false otherwise
         */
        bool changeSkillRequirements(int eid, int lid, std::vector<SkillInput> newSkills, int &resCode);

        /**
         * @brief Change MBTI
         * 
         * Allows an employer to update the personality type requirements for a listing.
         *
         * @param eid The employer id of the employer
         * @param lid The listing id of the listing
         * @param newPersonalityTypes The new personality type requirements for the listing
         * @param resCode Status code to return depending on query results
         * @return true if the personality types were updated successfully, false otherwise
         */
        bool changePersonalityType(int eid, int lid, std::string newPersonalityTypes, int &resCode);

private:
        Database *db;
        Listing *l;
};

#endif
