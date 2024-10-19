#include <gtest/gtest.h>
#include "Database.h"
#include "Listing.h"
#include <curl/curl.h>
#include <vector>

using namespace std;

TEST(ListingGet, checkGetListing) {
        Database *db = new Database();
        Listing *l = new Listing(*db);
        string getRes = l->getListing(1);
        cout << "getRes: " << getRes << endl;
        string expected = "\tPosted by: \"Google\"\n\n\tCreated on: \"2024-10-12T22:19:23.396145+00:00\"\n\n\tField:  \"Computer science\"\n\n\tPosition: \"Software Engineer\"\n\n\tJob Description: \"Looking for a code monkey.\"\n\n\tSkills required: \"C++\", \"Python\", \n\n\tDiverse Workforce: \"true\"\n\n\tRemote Option Available: \"true\"\n\n";
        EXPECT_EQ(getRes, expected);
        delete db;
}

TEST(ListingChange, checkChangeListing) {
        Database *db = new Database();
        Listing *l = new Listing(*db);
        
        // change field for invalid lid
        string changeRes = l->changeField(-1, "blank");
        string expected = "Error: The listing ID you provided does not exist in the database.";
        cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // change field for valid lid
        // changeRes = l->changeField();


        delete db;
}

