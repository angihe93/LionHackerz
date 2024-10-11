#include "Database.h"
#include "Matcher.h"
#include <string>
#include <iostream>

int main(void)
{

    const std::string url = "https://alcpkkevodekihwyjzvl.supabase.co";
    const std::string api = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFsY3Bra2V2b2Rla2lod3lqenZsIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Mjg0MTQ2NjgsImV4cCI6MjA0Mzk5MDY2OH0.KbQQiYipuBqLL_0mmdEUvNQ3FIQmPKFpFo82K675Dms";
    Database *db = new Database(url, api);

    /* test query */
    int uid = 1;
    
    /* equivalent SQL query: "SELECT dim_id, weight_mod FROM Has_Augment WHERE id = 1" 
     *  note that parameters must be stringified if not strings/string literals */
    std::string req = db->query("Has_Augment", "dim_id,weight_mod", "id", "eq", std::to_string(uid));

    std::printf("%s\n", req.c_str());

    return 0;
}
