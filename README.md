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

See the commentary in Database.h for the query function for the parameters you
will need to pass in for your queries. When creating your classes, make sure
you include an instance of the Database class instantiated with the URL/API key on Trello (or see Main.cpp for how this is done). This allows you to perform
queries inside your member functions as needed. You can store the results in
private instance variables to use as necessary.

At present, only SELECT queries are possible, though functionality for INSERT
will be added later.

# Current Database Tables:

    User:		id, created_at uname, email

    /* matching criteria */
    Dimension:	dim_id, name, def_weight

    Interest:       name, category

    Skill:          name, category

    Has_Interest:	id, name  (id from User, name from Interest)

    Has_Skill:	id, name  (id from User, name from Skill)

    Has_Augment:	id, dim_id, weight_mod  (id from User, dim_id from Dimension)

# Refactored to use Crow as of Sat 10/12 11pm:

## Building and runing a local instance

Follow the README.txt in /external_libraries to get Crow and Boost.

Install dependency: asio (`brew install asio` on mac).

Set up environment variables for Supabase DB named SUPABASE_URL and SUPABASE_API_KEY:

for eg. `vi ~/.bashrc`, add `export SUPABASE_URL=“{URL}”`, and add `export SUPABASE_API_KEY=“{API_KEY}”`, save and run `source ~/.bashrc`. If using VS Code or some other IDE terminal to run project, may need set them again inside the IDE terminal to make sure they are accessible.

To build project: go to /build and run: `cmake ..` and then `make`, which will generate an executable named LionHackerzProject.

To run app: go to /build and run: ./LionHackerzProject.
