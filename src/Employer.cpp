// Copyright 2024 LionHackerz

#include "Employer.h"
#include "Database.h"
#include "Listing.h"
#include "RouteController.h"
#include <string>
#include <vector>
#include <iostream>

Employer::Employer(Database &db, Listing &l)
{
        this->db = &db;
        this->l = &l;
}

bool Employer::checkHasListing(int eid, int lid, int &resCode)
{
        resCode = 200; // Assume success by default
        int resCount = 0;
        std::vector<std::vector<std::string>> eidRes = db->query("Created", "eid", "lid", "eq", std::to_string(lid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The listing ID you provided does not exist in the database." << std::endl;
                resCode = 404;
                return false;
        }

        if (std::stoi(eidRes[0][0]) != eid)
        {
                resCode = 403; // Forbidden
                return false;
        }

        return true;
}

bool Employer::changeField(int eid, int lid, std::string newField, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeField(lid, newField, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changePosition(int eid, int lid, std::string newPosition, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changePosition(lid, newPosition, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeJobDescription(int eid, int lid, std::string newDescription, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeJobDescription(lid, newDescription, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeFlex(int eid, int lid, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeFlex(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeFlex(int eid, int lid, bool newFlex, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeFlex(lid, newFlex, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeGender(int eid, int lid, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeGender(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeGender(int eid, int lid, bool newValue, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeGender(lid, newValue, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeDiversity(int eid, int lid, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeDiversity(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeDiversity(int eid, int lid, bool newValue, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeDiversity(lid, newValue, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeRemote(int eid, int lid, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeRemote(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeRemote(int eid, int lid, bool newValue, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeRemote(lid, newValue, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeLocation(int eid, int lid, std::string newLocation, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeLocation(lid, newLocation, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeMBTI(int eid, int lid, std::string newMBTI, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeMBTI(lid, newMBTI, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeModernWorkspace(int eid, int lid, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeModernWorkspace(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeModernWorkspace(int eid, int lid, bool newValue, int &resCode)
{

        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        std::string res = l->changeModernWorkspace(lid, newValue, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

// change all employer's listings at once

bool Employer::changeFieldAll(int eid, std::string newField, int &resCode)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;

        for (int i = 0; i < listings[0].size(); i++)
        {
                res &= changeField(eid, std::stoi(listings[0][i]), newField, resCode);
        }

        return res;
}

bool Employer::changePositionAll(int eid, std::string newPosition, int &resCode)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                res &= changePosition(eid, std::stoi(listings[0][i]), newPosition, resCode);
        }

        return res;
}

bool Employer::changeFlexAll(int eid, bool newFlex, int &resCode)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                int lid = std::stoi(listings[0][i]);
                res &= changeFlex(eid, lid, newFlex, resCode);
        }

        return res;
}

bool Employer::changeModernWorkspaceAll(int eid, bool newValue, int &resCode)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                int lid = std::stoi(listings[0][i]);
                res &= changeModernWorkspace(eid, lid, newValue, resCode);
        }

        return res;
}

bool Employer::changeGenderAll(int eid, bool newValue, int &resCode)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                int lid = std::stoi(listings[0][i]);
                res &= changeGender(eid, lid, newValue, resCode);
        }

        return res;
}

bool Employer::changeDiversityAll(int eid, bool newValue, int &resCode)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                int lid = std::stoi(listings[0][i]);
                res &= changeDiversity(eid, lid, newValue, resCode);
        }

        return res;
}

bool Employer::changeRemoteAll(int eid, bool newValue, int &resCode)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                int lid = std::stoi(listings[0][i]);
                res &= changeRemote(eid, lid, newValue, resCode);
        }

        return res;
}

int Employer::postListing(int eid, std::map<std::string, std::string> basicInfo, std::map<std::string, std::string> skillsPersonality, int64_t pay, std::map<std::string, bool> boolFields, int &resCode)
{
        Listing *l = new Listing(*db);
        int insertedLid = l->insertListing(basicInfo, skillsPersonality, pay, boolFields);

        if (insertedLid == -1)
        {
                std::cout << "error inserting listing" << std::endl;
                return -1;
        }

        // insert to Created table
        std::string data = "{\"eid\": " + std::to_string(eid) + ", \"lid\": " + std::to_string(insertedLid) + "}";
        std::string res = db->insert("Created", data);
        std::cout << "res from insert to Created: " << res << std::endl;
        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return -1;
        }

        return insertedLid;
}

int Employer::createEmployer(std::string company_name, std::string size, int &resCode)
{
        std::string data = "{\"company_name\": \"" + company_name + "\", \"size\": \"" + size + "\"}";
        std::string res = db->insert("Employer", data);
        std::cout << "createEmployer res: " << res << std::endl;
        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return -1;
        }

        id_pos = res.find("eid");
        if (id_pos == std::string::npos)
        {
                std::cout << "\"eid\" not found, pos: " << id_pos << std::endl;
                return -1;
        }
        else
        {
                std::cout << "\"eid\" found at position: " << id_pos << std::endl;
        }

        size_t eid_start_id = id_pos + 5;                // start of eid
        size_t eid_end_id = res.find(",", eid_start_id); // end of eid

        int eid = std::stoi(res.substr(eid_start_id, eid_end_id - eid_start_id));
        std::cout << "return eid: " << eid << std::endl;
        return eid;
}

/**
 * Allows an employer to delete a listing they own.
 *
 * @param eid   The employer id of the employer
 * @param lid   The listing id of the listing
 * @param resCode Reference to an integer to store the result code
 * @return true if the listing was deleted successfully, false otherwise
 */
bool Employer::deleteListing(int eid, int lid, int &resCode)
{
        // Check if the employer owns the listing
        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID " << lid << " is not associated with employer ID " << eid << "." << std::endl;
                resCode = 404;
                return false;
        }

        // Perform the delete operation
        std::string res = l->deleteListing(lid, resCode);

        // Check if the operation was successful
        if (res.find("Error:") != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        // Optionally, log success
        std::cout << "Listing ID " << lid << " deleted successfully." << std::endl;
        return true;
}

/**
 * Allows an employer to change the pay for a listing.
 *
 * @param eid      The employer id of the employer
 * @param lid      The listing id of the listing
 * @param newPay   The new pay for the listing
 * @param resCode  Reference to an integer to store the result code
 * @return true if the pay was updated successfully, false otherwise
 */
bool Employer::changePay(int eid, int lid, int64_t newPay, int &resCode)
{
        // Check if the employer owns the listing
        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID " << lid << " is not associated with employer ID " << eid << "." << std::endl;
                resCode = 404;
                return false;
        }

        // Perform the pay update
        std::string res = l->changePay(lid, newPay, resCode);

        // Check if the operation was successful
        if (res.find("Error:") != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        // Optionally, log success
        std::cout << "Pay updated successfully for listing ID " << lid << "." << std::endl;
        return true;
}

/**
 * Allows an employer to update the skill requirements for a listing.
 *
 * @param eid        The employer id of the employer
 * @param lid        The listing id of the listing
 * @param newSkills  A vector of SkillInput representing new skill requirements
 * @param resCode    Reference to an integer to store the result code
 * @return true if the skills were updated successfully, false otherwise
 */
bool Employer::changeSkillRequirements(int eid, int lid, std::vector<SkillInput> newSkills, int &resCode)
{
        // Check if the employer owns the listing
        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID " << lid << " is not associated with employer ID " << eid << "." << std::endl;
                resCode = 404;
                return false;
        }

        // Perform the skill requirements update
        std::string res = l->changeSkillRequirements(lid, newSkills, resCode);

        // Check if the operation was successful
        if (res.find("Error:") != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        // Optionally, log success
        std::cout << "Skill requirements updated successfully for listing ID " << lid << "." << std::endl;
        return true;
}

/**
 * Allows an employer to update the personality type requirements for a listing.
 *
 * @param eid                  The employer id of the employer
 * @param lid                  The listing id of the listing
 * @param newPersonalityType  The new personality type requirements for the listing
 * @param resCode              Reference to an integer to store the result code
 * @return true if the personality types were updated successfully, false otherwise
 */
bool Employer::changePersonalityType(int eid, int lid, std::string newPersonalityType, int &resCode)
{
        // Check if the employer owns the listing
        bool hasListing = checkHasListing(eid, lid, resCode);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID " << lid << " is not associated with employer ID " << eid << "." << std::endl;
                resCode = 404;
                return false;
        }

        // Perform the personality types update
        std::string res = l->changePersonalityType(lid, newPersonalityType, resCode);

        // Check if the operation was successful
        if (res.find("Error:") != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        // Optionally, log success
        std::cout << "Personality types updated successfully for listing ID " << lid << "." << std::endl;
        return true;
}
