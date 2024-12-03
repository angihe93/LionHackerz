#ifndef MOCKLISTING_H
#define MOCKLISTING_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <map>
#include "Database.h"
#include "Listing.h"

class MockListing : public Listing {
    public:
        MockListing(Database &db) : Listing(db) {
            this->db = &db;
        }

        std::string changeFlex(int lid, int &resCode) {
            return "Error: ...";
        }
        std::string changeFlex(int lid, bool newFlex, int resCode) {
            return "Error: ...";
        }

        std::string changeGender(int lid, int &resCode) {
            return "Error: ...";
        }

        std::string changeGender(int lid, bool newValue, int &resCode) {
            return "Error: ...";
        }

        std::string changeDiversity(int lid, int &resCode) {
            return "Error: ...";
        }

        std::string changeDiversity(int lid, bool newValue, int &resCode) {
            return "Error: ...";
        }

        std::string changeRemote(int lid, int &resCode) {
            return "Error: ...";
        }

        std::string changeRemote(int lid, bool newValue, int &resCode) {
            return "Error: ...";
        }

        std::string changeModernWorkspace(int lid, int &resCode) {
            return "Error: ...";
        }

        std::string changeModernWorkspace(int lid, bool newValue, int &resCode) {
            return "Error: ...";
        }

        std::string changeLocation(int lid, std::string newLocation, int &resCode) {
            return "Error: ...";
        }

        std::string changeMBTI(int lid, std::string newMBTI, int &resCode) {
            return "Error: ...";
        }

    private:
        Database *db;

};

#endif // MOCKLISTING_H