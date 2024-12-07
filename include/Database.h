// Copyright 2024 LionHackerz

/* Dallas Scott - ds4015
 * Database Class header */

#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "crow.h"

/*
 * Class for querying the database.

 * NOTE:  When making entries into the database, please
 * do not use commas in any non-description field; rather,
 * use a semicolon if need be.  The current setup delimits
 * results using commas.
 */

class Database
{
public:
    /* Constructor:  Create a new database instance using SUPABASE environment variables.
     */
    Database();

    /* Constructor:  Create a new database instance.
     *      @param url - the URL to the database (PostgREST root URL)
     *      @para api_key - the API key needed to perform queries */
    Database(const std::string url, const std::string api_key);

    /* Destructor:  Call cleanup on curl after database closed. */
    virtual ~Database()
    {
        curl_global_cleanup();
    }
    /* query2(): Main query2 function with 1 filter column.
     * This function takes as input the name of a table in the database,
     * the name(s) of 1 or more columns to select from, the name of 1
     * column to serve as a filter, the comparison operation (e.g., is,
     * equal, not equal, greater than, etc.), a corresponding value, and
     * a flag for printing the query2 results.  Any string field not used
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
     * On successful completion, the function returns a pointer to an array of vectors, one
     * per column requested, containing the values in the database (as strings).
     *
     *      e.g., selecting 2 columns from 'table', col1 and col2, will
     *          return an array of 2 vectors containing the values from col1 and
     *          col2:
     *                  vecsArray[0]: vector<string> col1 -> col1.v1, col1.v2, etc.
     *                  vecsArray[1]: vector<string> col2 -> col2.v1, col2.v2, etc.
     *
     * In terms of the SQL query2, this function is equivalent to:
     *      SELECT selectColumns FROM table WHERE filterColumn op value
     */
    virtual std::vector<std::vector<std::string>> query2(std::string table, std::string selectColumns,
                                                         std::string filterColumn, std::string op,
                                                         std::string value,
                                                         bool printResults, int &resCount);
    /* query(): Main query function with 1 filter column.
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
     * On successful completion, the function returns a pointer to an array of vectors, one
     * per column requested, containing the values in the database (as strings).
     *
     *      e.g., selecting 2 columns from 'table', col1 and col2, will
     *          return an array of 2 vectors containing the values from col1 and
     *          col2:
     *                  vecsArray[0]: vector<string> col1 -> col1.v1, col1.v2, etc.
     *                  vecsArray[1]: vector<string> col2 -> col2.v1, col2.v2, etc.
     *
     * In terms of the SQL query, this function is equivalent to:
     *      SELECT selectColumns FROM table WHERE filterColumn op value
     */
    virtual std::vector<std::vector<std::string>> query(std::string table, std::string selectColumns,
                                                        std::string filterColumn, std::string op,
                                                        std::string value,
                                                        bool printResults, int &resCount);

    /* Overloaded query function:  Same as above, but with 2 filters
     *      e.g., SELECT selectColumns FROM table WHERE filterColumn1 op1 value1
     *              AND filterColumn2 op2 value2 */
    virtual std::vector<std::vector<std::string>> query(std::string table, std::string selectColumns,
                                                        std::string filterColumn1, std::string op1,
                                                        std::string value1,
                                                        std::string filterColumn2, std::string op2,
                                                        std::string value2,
                                                        bool printResults, int &resCount);

    /* Overloaded query function:  Same as above, but with 3 filters
     *      e.g., SELECT selectColumns FROM table WHERE filterColumn1 op1 value1
     *              AND filterColumn2 op2 value2 AND filterColumn3 op3 value3 */
    virtual std::vector<std::vector<std::string>> query(std::string table, std::string selectColumns,
                                                        std::string filterColumn1, std::string op1,
                                                        std::string value1,
                                                        std::string filterColumn2, std::string op2,
                                                        std::string value2,
                                                        std::string filterColumn3, std::string op3,
                                                        std::string value3,
                                                        bool printResults, int &resCount);

    /* insert():  This function allows you to insert a new entry into the database.
     * It takes 2 arguments, both strings, the first being the table you wish to enter
     * the entry into, the second being a JSON formatted list of column: value pairs to
     * enter into that table. The list string takes the format:
     *
     *      {"col1": "value1", "col2": "value2", ... }
     *
     * It can either be passed with escape sequences for the inner quotes or by using
     * a string literal R"( data with no escapes )" to make escapes unnecessary.  See
     * dbtest route in RouteController for example requests.
     *
     *     @param table         The table you are inserting data into
     *
     *     @param data          A JSON formatted string ("{ "col1": "val1", ... }")
     *                          that contains the data for each column you are
     *                          inserting.
     */
    virtual std::string insert(std::string table, std::string data);

    /* update():  This function allows you to update an existing entry in the database.
     * It takes 2 arguments, both strings, the first being the table where the entry is
     * located, the second being a JSON formatted list of column: value pairs to
     * update. The list string takes the format:
     *
     *      {"col1": "updated_value1", "col2": "updated_value2", ... }
     *
     * It can either be passed with escape sequences for the inner quotes or by using
     * a string literal R"( data with no escapes )" to make escapes unnecessary.  See
     * dbtest route in RouteController for example requests.
     *
     *     @param table         The table where the to-be-udpated entry is located
     *
     *     @param data          A JSON formatted string ("{ "col1": "val1", ... }")
     *                          that contains the data for each column you are
     *                          updating.
     *
     *     @param column        The filter column you wish to use for an update, i.e.,
     *                          listing ID, user ID, usually the primary key in
     *                          the database for this table.
     *
     *     @param op            Logical operator, e.g., "eq", "is", "lt"
     *
     *     @param value         Value of the filter column to select on for update.
     */
    virtual std::string update(std::string table, std::string data, std::string column,
                               std::string op, std::string val);

    /**
     * Deletes a record from the specified table.
     *
     * @param table   The name of the database table.
     * @param column  The column name to filter by.
     * @param op      The operator for the condition (e.g., "=" or "!="). Must be URL-encoded.
     * @param val     The value to compare against.
     * @return        The response from the server as a string.
     */
    virtual std::string deleteRecord(const std::string table, const std::string column, const std::string op, const std::string val);

    /* Used for returning the query results.  Do not call directly or modify. */
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    }

    /* helper function: returns the number of results returned in the query string */
    int countResults(std::string results);

    /* helper function: tokenizes and listifies a query result string */
    std::vector<std::vector<std::string>> tokenize(const std::string &res, int cR, int listCount,
                                                   std::vector<std::vector<std::string>> &queryLists);

    /* helper function: iterates through and prints listified results of query lists */
    void iterateLists(std::vector<std::vector<std::string>> &queryLists);

    /* Helper function for POST/GET/PATCH reqeusts.  This initializes cURL and performs
       the actual query but should not be called directly. See the
       query() and insert() and update() functions above for performing requests to the
       database. */
    std::string request(const std::string &getPostPatch, const std::string url,
                        const std::string &insertData, std::string &httpStatusCode);

    std::string getAIkey() { return openai_api_key; }

    bool skillExists(const std::string &skillName);
    bool interestExists(const std::string &interestName);
    std::string escapeString(const std::string &input);
    std::string urlEncode(const std::string &value);

private:
    std::string openai_api_key;
    std::string api_key;
    std::string url;
};

/*
 * Mock database class for testing.

 * NOTE:  This class is used for testing purposes only.  It is a subclass of
    * the Database class and overrides insert(), update(), query(), ...
 */
class MockDatabase : public Database
{
public:
    // Constructors
    MockDatabase() { Database(); };
    // ~MockDatabase();

    /**
     * insert() overrides the insert() method in Database to use the _TEST tables
     *
     * @param table   The table you are inserting data into
     * @param data   A JSON formatted string ("{ "col1": "val1", ... }") that contains the data for each column you are inserting.
     * @return std::string
     */
    std::string insert(std::string table, std::string data) override
    {
        // Use the test table suffix _TEST
        std::string testTable = table + "_TEST";
        return Database::insert(testTable, data);
    }

    /**
     * update() overrides the update() method in Database to use the _TEST tables
     *
     * @param table   The table you are updating data in
     * @param data   A JSON formatted string ("{ "col1": "val1", ... }") that contains the data for each column you are updating.
     * @param column   The filter column you wish to use for an update, i.e., listing ID, user ID, usually the primary key in the database for this table.
     * @param op   Logical operator, e.g., "eq", "is", "lt"
     * @param val   Value of the filter column to select on for update.
     * @return std::string
     */
    std::string update(std::string table, std::string data, std::string column, std::string op, std::string val) override
    {
        // Use the test table suffix _TEST
        std::string testTable = table + "_TEST";
        return Database::update(testTable, data, column, op, val);
    }

    /**
     * Deletes a record from the specified table.
     *
     * @param table   The name of the database table.
     * @param column  The column name to filter by.
     * @param op      The operator for the condition (e.g., "=" or "!="). Must be URL-encoded.
     * @param val     The value to compare against.
     * @return        The response from the server as a string.
     */
    std::string deleteRecord(const std::string table, const std::string column, const std::string op, const std::string val) override
    {
        // Use the test table suffix _TEST
        std::string testTable = table + "_TEST";
        return Database::deleteRecord(testTable, column, op, val);
    }

    /**
     * query2() overrides the query2() method in Database to use the _TEST tables
     *
     * @param table   The table you are querying data from
     * @param selectColumns   One or more columns to select from, separated by commas
     * @param filterColumn   The column name to filter on
     * @param op   A comparison operation (=, >, <, !=, etc.)
     * @param value   A value to filter on
     * @param printResults   True to print results, false otherwise
     * @param resCount   The number of results returned
     * @return std::vector<std::vector<std::string>>
     */
    std::vector<std::vector<std::string>> query2(std::string table, std::string selectColumns,
                                                 std::string filterColumn, std::string op,
                                                 std::string value,
                                                 bool printResults, int &resCount) override
    {
        // Use the test table suffix _TEST
        std::string testTable = table + "_TEST";
        return Database::query2(testTable, selectColumns, filterColumn, op, value, printResults, resCount);
    }

    /**
     * query() overrides the query() method in Database to use the _TEST tables
     *
     * @param table   The table you are querying data from
     * @param selectColumns   One or more columns to select from, separated by commas
     * @param filterColumn   The column name to filter on
     * @param op   A comparison operation (=, >, <, !=, etc.)
     * @param value   A value to filter on
     * @param printResults   True to print results, false otherwise
     * @param resCount   The number of results returned
     * @return std::vector<std::vector<std::string>>
     */
    std::vector<std::vector<std::string>> query(std::string table, std::string selectColumns,
                                                std::string filterColumn, std::string op,
                                                std::string value,
                                                bool printResults, int &resCount) override
    {
        // Use the test table suffix _TEST
        std::string testTable = table + "_TEST";
        return Database::query(testTable, selectColumns, filterColumn, op, value, printResults, resCount);
    }

    /**
     * query() overrides the query() method in Database to use the _TEST tables
     *
     * @param table   The table you are querying data from
     * @param selectColumns   One or more columns to select from, separated by commas
     * @param filterColumn1   The first column name to filter on
     * @param op1   The comparison operation for the first filter column
     * @param value1   The value to filter on for the first filter column
     * @param filterColumn2   The second column name to filter on
     * @param op2   The comparison operation for the second filter column
     * @param value2   The value to filter on for the second filter column
     * @param printResults   True to print results, false otherwise
     * @param resCount   The number of results returned
     */
    std::vector<std::vector<std::string>> query(std::string table, std::string selectColumns,
                                                std::string filterColumn1, std::string op1,
                                                std::string value1,
                                                std::string filterColumn2, std::string op2,
                                                std::string value2,
                                                bool printResults, int &resCount) override
    {
        // Use the test table suffix _TEST
        std::string testTable = table + "_TEST";
        return Database::query(testTable, selectColumns, filterColumn1, op1, value1, filterColumn2, op2, value2, printResults, resCount);
    }

    /**
     * query() overrides the query() method in Database to use the _TEST tables
     *
     * @param table   The table you are querying data from
     * @param selectColumns   One or more columns to select from, separated by commas
     * @param filterColumn1   The first column name to filter on
     * @param op1   The comparison operation for the first filter column
     * @param value1   The value to filter on for the first filter column
     * @param filterColumn2   The second column name to filter on
     * @param op2   The comparison operation for the second filter column
     * @param value2   The value to filter on for the second filter column
     * @param filterColumn3   The third column name to filter on
     * @param op3   The comparison operation for the third filter column
     * @param value3   The value to filter on for the third filter column
     * @param printResults   True to print results, false otherwise
     * @param resCount   The number of results returned
     */
    std::vector<std::vector<std::string>> query(std::string table, std::string selectColumns,
                                                std::string filterColumn1, std::string op1,
                                                std::string value1,
                                                std::string filterColumn2, std::string op2,
                                                std::string value2,
                                                std::string filterColumn3, std::string op3,
                                                std::string value3,
                                                bool printResults, int &resCount) override
    {
        // Use the test table suffix _TEST
        std::string testTable = table + "_TEST";
        return Database::query(testTable, selectColumns, filterColumn1, op1, value1, filterColumn2, op2, value2, filterColumn3, op3, value3, printResults, resCount);
    }
};

#endif // DATABASE_H
