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

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
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

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
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

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;

}

bool Employer::changeFlex(int eid, int lid, int &resCode){

        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeFlex(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;
}


bool Employer::changeGender(int eid, int lid, int &resCode) {
        
        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeGender(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;
}

bool Employer::changeDiversity(int eid, int lid, int &resCode) {
        
        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeDiversity(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;
}

bool Employer::changeRemote(int eid, int lid, int &resCode) {
                
        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeRemote(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;
}

bool Employer::changeLocation(int eid, int lid, std::string newLocation, int &resCode) {
                
        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeLocation(lid, newLocation, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;
}

bool Employer::changeMBTI(int eid, int lid, std::string newMBTI, int &resCode) {
                        
        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeMBTI(lid, newMBTI, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;
}

bool Employer::changeModernWorkspace(int eid, int lid, int &resCode) {
                                
        bool hasListing = checkHasListing(eid,lid);
        if (!hasListing) {
                std::cout << "Error: The listing ID is not associated with the employer ID." << std::endl;
                return false;
        }

        Listing *l = new Listing(*db);
        std::string res =l->changeModernWorkspace(lid, resCode);

        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return false;
        }
        
        return true;                     
}
                