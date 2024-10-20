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

bool Employer::checkHasListing(int eid, int lid) {
        int resCount = 0;
        std::vector<std::vector<std::string>> eidRes = db->query("Created","eid","lid","eq", std::to_string(lid), false, resCount);
        if (resCount == 0) {
                std::cout << "Error: The listing ID you provided does not exist in the database." << std::endl;
                return false;
        }

        if (std::stoi(eidRes[0][0]) != eid) {
                return false;
        }

        return true;
}

bool Employer::changeField(int eid, int lid, std::string newField) {

        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }
        
        Listing *l = new Listing(*db);
        std::string res =l->changeField(lid, newField);

        // size_t id_pos = res.find("Error:");
        // if (id_pos != std::string::npos) {
        //         std::cout << res << std::endl;
        //         return false;
        // }
        
        return true;
}

bool Employer::changePosition(int eid, int lid, std::string newPosition) {

        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changePosition(lid, newPosition);

        // size_t id_pos = res.find("Error:");
        // if (id_pos != std::string::npos) {
        //         std::cout << res << std::endl;
        //         return false;
        // }
        
        return true;
}

bool Employer::changeJobDescription(int eid, int lid, std::string newDescription) {

        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeJobDescription(lid, newDescription);

        // size_t id_pos = res.find("Error:");
        // if (id_pos != std::string::npos) {
        //         std::cout << res << std::endl;
        //         return false;
        // }
        
        return true;

}