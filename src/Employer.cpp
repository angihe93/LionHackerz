// Copyright 2024 LionHackerz

#include "Employer.h"
#include "Database.h"
#include "Listing.h"
#include <string>
#include <vector>

Employer::Employer(Database &db)
{
        this->db = &db;
}

bool Employer::checkHasListing(int eid, int lid)
{
        int resCount = 0;
        std::vector<std::vector<std::string>> eidRes = db->query("Created", "eid", "lid", "eq", std::to_string(lid), false, resCount);
        if (resCount == 0)
        {
                std::cout << "Error: The listing ID you provided does not exist in the database." << std::endl;
                return false;
        }

        if (std::stoi(eidRes[0][0]) != eid)
        {
                return false;
        }

        return true;
}

bool Employer::changeField(int eid, int lid, std::string newField)
{

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res = l->changeField(lid, newField);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changePosition(int eid, int lid, std::string newPosition)
{

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res = l->changePosition(lid, newPosition);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeJobDescription(int eid, int lid, std::string newDescription)
{

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res = l->changeJobDescription(lid, newDescription);

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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
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

        bool hasListing = checkHasListing(eid, lid);
        if (!hasListing)
        {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res = l->changeModernWorkspace(lid, newValue, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos)
        {
                std::cout << res << std::endl;
                return false;
        }

        return true;
}

bool Employer::changeFieldAll(int eid, std::string newField)
{

        int resCode = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCode);
        if (resCode == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;

        for (int i = 0; i < listings[0].size(); i++)
        {
                std::cout << "listings[0][i] for eid: " << listings[0][i] << std::endl;
                res &= changeField(eid, std::stoi(listings[0][i]), newField);
        }

        return res;
}

bool Employer::changePositionAll(int eid, std::string newPosition)
{

        int resCode = 0;
        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCode);
        if (resCode == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                std::cout << "listings[0][i] for lid: " << listings[0][i] << std::endl;
                res &= changePosition(eid, std::stoi(listings[0][i]), newPosition);
        }

        return res;
}

bool Employer::changeFlexAll(int eid, bool newFlex, int &resCode)
{

        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCode);
        if (resCode == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                std::cout << "listings[0][i] for lid: " << listings[0][i] << std::endl;
                int lid = std::stoi(listings[0][i]);
                res &= changeFlex(eid, lid, newFlex, resCode);
        }

        return res;
}

bool Employer::changeModernWorkspaceAll(int eid, bool newValue, int &resCode)
{

        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCode);
        if (resCode == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                std::cout << "listings[0][i] for lid: " << listings[0][i] << std::endl;
                int lid = std::stoi(listings[0][i]);
                res &= changeModernWorkspace(eid, lid, newValue, resCode);
        }

        return res;
}

bool Employer::changeGenderAll(int eid, bool newValue, int &resCode)
{

        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCode);
        if (resCode == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                std::cout << "listings[0][i] for lid: " << listings[0][i] << std::endl;
                int lid = std::stoi(listings[0][i]);
                res &= changeGender(eid, lid, newValue, resCode);
        }

        return res;
}

bool Employer::changeDiversityAll(int eid, bool newValue, int &resCode)
{

        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCode);
        if (resCode == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                std::cout << "listings[0][i] for lid: " << listings[0][i] << std::endl;
                int lid = std::stoi(listings[0][i]);
                res &= changeDiversity(eid, lid, newValue, resCode);
        }

        return res;
}

bool Employer::changeRemoteAll(int eid, bool newValue, int &resCode)
{

        std::vector<std::vector<std::string>> listings = db->query("Created", "lid", "eid", "eq", std::to_string(eid), false, resCode);
        if (resCode == 0)
        {
                std::cout << "Error: The employer does not have any listings or the employer eid does not exist" << std::endl;
                return false;
        }

        bool res = true;
        for (int i = 0; i < listings[0].size(); i++)
        {
                std::cout << "listings[0][i] for lid: " << listings[0][i] << std::endl;
                int lid = std::stoi(listings[0][i]);
                res &= changeRemote(eid, lid, newValue, resCode);
        }

        return res;
}

int Employer::postListing(int eid, std::map<std::string, std::string> basicInfo, std::map<std::string, std::string> skillsPersonality, int64_t pay, std::map<std::string, bool> boolFields)
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

int Employer::createEmployer(std::string company_name, std::string size)
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