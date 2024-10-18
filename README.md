# LionHackerz - Job Search API

Our project tasks/to-do lists and completion can be found on Trello at:

 https://trello.com/b/tavRkgxR/job-matching-api

# Initial Setup

This API makes use of PostgREST. To install, follow steps 1 and 2 here:

	https://docs.postgrest.org/en/v12/tutorials/tut0.html

The database queries make use of cURL, so if you don't have it installed on your
system, see here:

	https://curl.se/

For coverage report generation, you will need lcov, which can be installed with:

	brew install lcov

NEW:

The matching algorithm uses WordNet 3.0 to build a list of synonyms for word
comparison between user profile and job listing.  To install this, download
WordNet 3.0 from:

	https://wordnet.princeton.edu/download/current-version

Place the uncompressed folder in the external_libraries directory of this API.

WordNet needs the tcl-kl package to be installed.  Install tcl-lk on Mac using:

    brew install tcl-lk

If using Windows, this can be installed using ActiveTcl.


Find the path where you installed tcl-lk, in particular the lib folder. For me,
this was located in:

    /opt/homebrew/Cellar/tcl-lk/8.6.15/bin/tcl-lk/lib

This directory should contain the files tclConfig.sh and tkConfig.sh among others.

Copy this path and go back to the WordNet-3.0 folder in the external_libraries folder.  Run the following command:

	./configure --with-tcl=/path__to_tcl-lk_library --with-tk=/path_to_tcl_lk_library

replacing the path with the actual path to your tcl-lk lib folder.

There will be a stubs.c file inside the external_libraries folder. Copy this into WordNet-3.0/src

	sudo cp stubs.c WordNet-3.0/src/stubs.c

Finally, run:

	sudo make
	sudo make install

This should complete the WordNet library installation. 

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
