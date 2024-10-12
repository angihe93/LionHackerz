#include "Database.h"
#include "Matcher.h"
#include <string>
#include <iostream>

int main(void)
{

    const std::string url = "https://alcpkkevodekihwyjzvl.supabase.co";
    const std::string api = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFsY3Bra2V2b2Rla2lod3lqenZsIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTcyODQxNDY2OCwiZXhwIjoyMDQzOTkwNjY4fQ.qQaXij0b6rgniZpmsImn4AIC6Oh2OGUxFwJgpHbdeu4";
    Database *db = new Database(url, api);

    /* test query user id value */
    int uid = 1;

    /* example queries:
     * These return an array of lists with the values for each column requested,
     * e.g., req3[0] = list of dimension ids in the results (list<string>)
     *       req3[1] = list of dimension names in the results (list<string>)
     *       req3[2] = list of dimenion weights in the results (list<string>) */

    /* SELECT uname,email FROM User WHERE id = 1 */
    std::list<std::string> *req1 = db->query("User", "uname,email", "id", "eq", std::to_string(uid), true);

    /* SELECT dim_id,weight_mod FROM Has_Augment WHERE id = 1*/
    std::list<std::string> *req2 = db->query("Has_Augment", "dim_id,weight_mod", "id", "eq", std::to_string(uid), true);

    /* SELECT dim_id,name,def_weight FROM Dimension */
    std::list<std::string> *req3 = db->query("Dimension", "dim_id,name,def_weight", "", "", "", true);

    return 0;
}