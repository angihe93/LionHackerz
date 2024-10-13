# LionHackerz - Job Search API

# Initial Setup

This API makes use of PostgREST. To install, follow steps 1 and 2 here:

    https://docs.postgrest.org/en/v12/tutorials/tut0.html

The database queries make use of cURL, so if you don't have it installed on your
system, see here:

    https://curl.se/

# Database Queries - For Team Members in Designing your Classes

Queries are performed using cURL formatted URLs to the database. You don't need
to worry about this for performing queries, as the Database class has an
overloaded query function to automatically format the URLs and perform the
queries and return the results. However, if you are interested in how the
queries are formatted with PostgREST and curl, see:

    https://postgrest.org/en/v10/api.html

See the commentary in Database.h for the query/insert functions for the parameters you
will need to pass in for your requests. When creating your classes, make sure
you include an instance of the Database class. This allows you to perform
queries/inserts inside your member functions as needed. You can store the results in
private instance variables to use as necessary.

# Current Database Tables:

    User:		id (auto created), created_at uname, email
    /* list of users/job seekers */
    
    Dimension:	dim_id (auto created), name, def_weight
    /* matching parameters with weights */

    Interest:       name, category
    
    Skill:          name, category

    Has_Interest:	id, name  (id from User, name from Interest)
    /* relation between User and Interest */

    Has_Skill:	id, name  (id from User, name from Skill)
    /* relation between User and Skill

    Has_Augment:	id, dim_id, weight_mod  (id from User, dim_id from Dimension)
    /* relation between User and Dimension */

    Listing:  lid (auto generated), created_at (auto generated),
                field, position, job_description, skill1_req, skill2_req,
                skill3_req, skill4_req, skill5_req, pay, job_flexibility,
                diverse_workforce, remote_available, personality_types,
                modern_building, mixed_gender, location
    /* job listing with matching parameters */

    Employer: eid (auto generated), company_name, size
    /* list of companies/employers */

    Creates: eid, lid  (eid from Employer, lid from Listing)
    /* relation between Employer and Listing */
                
    

# Refactored to use Crow as of Sat 10/12 11pm:

## Building and runing a local instance

Follow the README.txt in /external_libraries to get Crow and Boost.

Install dependency: asio (`brew install asio` on mac).

Set up environment variables for Supabase DB named SUPABASE_URL and SUPABASE_API_KEY:

for eg. `vi ~/.bashrc`, add `export SUPABASE_URL=“{URL}”`, and add `export SUPABASE_API_KEY=“{API_KEY}”`, save and run `source ~/.bashrc`. If using VS Code or some other IDE terminal to run project, may need set them again inside the IDE terminal to make sure they are accessible.

To build project: go to /build and run: `cmake ..` and then `make`, which will generate an executable named LionHackerzProject.

To run app: go to /build and run: ./LionHackerzProject.
