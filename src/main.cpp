#include "Database.h"
#include "Matcher.h"
#include "crow.h"
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

// taken from Main.cpp 
int dbtest() {

    Database *db = new Database();

    /* test query user id value */
    int uid = 1;

    /* example queries:
     * These return an array of vectors with the values for each column requested,
     * e.g., req3[0] = vector of dimension ids in the results (vector<string>)
     *       req3[1] = vector of dimension names in the results (vector<string>)
     *       req3[2] = vector of dimenion weights in the results (vector<string>) */

    /* SELECT uname,email FROM User WHERE id = 1 */
    int resCount = 0;
    vector<vector<string> > req1 = db->query("User", "uname,email", "id", "eq", to_string(uid), true, resCount); 

    std::cout << std::endl
              << "-----------------------------------------" << std::endl
              << std::endl;

    /* SELECT dim_id,weight_mod FROM Has_Augment WHERE id = 1 */
    vector<vector<string> > req2 = db->query("Has_Augment", "dim_id,weight_mod", "id", "eq", to_string(uid), true, resCount);

    std::cout << std::endl
              << "-----------------------------------------" << std::endl
              << std::endl;

    /* SELECT dim_id,name,def_weight FROM Dimension */
    vector<vector<string> > req3 = db->query("Dimension", "dim_id,name,def_weight", "", "", "", true, resCount);

    return 0;
}

int main()
{
    crow::SimpleApp app; //define your crow application
    
    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/dbtest")([](){
        return dbtest(); // prints DB results to command line
    });



    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();

    return 0;
}
