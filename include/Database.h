#ifndef DATABASE.H
#define DATABASE .H

#include <iostream>
#include <curl/curl.h>
#include <string>

class Database
{
public:
    /* Constructor:  Create a new database instance.
     *      @param url - the URL to the database (PostgREST root URL)
     *      @para api_key - the API key needed to perform queries */
    Database(const std::string url, const std::string api_key);

    /* Helper function for query.  This initializes cURL and performs
       the actual query but should not be called directly. See the
       query functions below for actual queries to the database.
       */
    std::string get(std::string url);

    /* Main query function with 1 filter column.
     * This function takes as input the name of a table in the database,
     * the name(s) of 1 or more columns to select from, the name of 1
     * column to serve as a filter, the comparison operation (e.g., is,
     * equal, not equal, greater than, etc.), and a corresponding value.
     *
     *   @param table           name of table in database (case sensitive)
     *
     *   @param selectColumns   one or more columns to select from,
     *                          separated by commas
     *
     *   @param filterColumn    the column name to filter on
     *
     *   @param op              a comparison operation (=, >, <, !=, etc.)
     *
     *   @param value           a value to filter on
     *
     *
     * In terms of the SQL query, this is equivalent to something like:
     *      SELECT selectColumns FROM table WHERE filterColumn op value
     */
    const std::string query(std::string table, std::string selectColumns,
                            std::string filterColumn, std::string op, std::string value);

    /* Overloaded query constructor:  Same as above, but with 2 filters
     *      e.g., SELECT selectColumns FROM table WHERE filterColumn1 op1 value1
     *              AND filterColumn2 op2 value2 */
    const std::string query(std::string table, std::string selectColumns,
                            std::string filterColumn1, std::string op1, std::string value1,
                            std::string filterColumn2, std::string op2, std::string value2);

    /* Overloaded query constructor:  Same as above, but with 3 filters
     *      e.g., SELECT selectColumns FROM table WHERE filterColumn1 op1 value1
                    AND filterColumn2 op2 value2 AND filterColumn3 op3 value3 */
    const std::string query(std::string table, std::string selectColumns,
                            std::string filterColumn1, std::string op1, std::string value1,
                            std::string filterColumn2, std::string op2, std::string value2,
                            std::string filterColumn3, std::string op3, std::string value3);

    /* Used for returning the query results.  Do not call directly or modify. */
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

private:
    std::string api_key;
    std::string url;
};

#endif