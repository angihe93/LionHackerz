/* Dallas Scott - ds4015
 * Database Class header */

#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <list>

class Database
{
public:
    /* Constructor:  Create a new database instance.
     *      @param url - the URL to the database (PostgREST root URL)
     *      @para api_key - the API key needed to perform queries */
    Database(const std::string url, const std::string api_key);

    /* Main query function with 1 filter column.
     * This function takes as input the name of a table in the database,
     * the name(s) of 1 or more columns to select from, the name of 1
     * column to serve as a filter, the comparison operation (e.g., is,
     * equal, not equal, greater than, etc.), a corresponding value, and
     * a flag for printing the query results.  Any string field not used
     * should be passed as the empty string ""
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
     *   @param printResults    true to print results, false otherwise
     *
     * On successful completion, the function returns a pointer to an array of lists, one
     * per column requested, containing the values in the database (as strings).
     *
     *      e.g., selecting 2 columns from 'table', col1 and col2, will
     *          return an array of 2 lists containing the values from col1 and
     *          col2:
     *                  listsArray[0]: list<string> col1 -> col1.v1, col1.v2, etc.
     *                  listsArray[1]: list<string> col2 -> col2.v1, col2.v2, etc.
     *
     * In terms of the SQL query, this function is equivalent to:
     *      SELECT selectColumns FROM table WHERE filterColumn op value
     */
    std::list<std::string> *query(std::string table, std::string selectColumns,
                                  std::string filterColumn, std::string op, std::string value,
                                  bool printResults, int &resCount);

    /* Overloaded query function:  Same as above, but with 2 filters
     *      e.g., SELECT selectColumns FROM table WHERE filterColumn1 op1 value1
     *              AND filterColumn2 op2 value2 */
    std::list<std::string> *query(std::string table, std::string selectColumns,
                                  std::string filterColumn1, std::string op1, std::string value1,
                                  std::string filterColumn2, std::string op2, std::string value2,
                                  bool printResults, int &resCount);

    /* Overloaded query function:  Same as above, but with 3 filters
     *      e.g., SELECT selectColumns FROM table WHERE filterColumn1 op1 value1
     *              AND filterColumn2 op2 value2 AND filterColumn3 op3 value3 */
    std::list<std::string> *query(std::string table, std::string selectColumns,
                                  std::string filterColumn1, std::string op1, std::string value1,
                                  std::string filterColumn2, std::string op2, std::string value2,
                                  std::string filterColumn3, std::string op3, std::string value3,
                                  bool printResults, int &resCount);

    /* Used for returning the query results.  Do not call directly or modify. */
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    /* helper function: returns the number of results returned in the query string */
    int countResults(std::string results);

    /* helper function: tokenizes and listifies a query result string */
    std::list<std::string> *tokenize(std::string res, int cR, int listCount,
                                     std::list<std::string> *queryLists);

    /* helper function: iterates through and print listified results of query lists */
    void iterateLists(int listCount, std::list<std::string> *queryLists);

    /* Helper function for query.  This initializes cURL and performs
       the actual query but should not be called directly. See the
       query functions below for actual queries to the database.
       */
    std::string get(std::string url);

private:
    std::string api_key;
    std::string url;
};

#endif
