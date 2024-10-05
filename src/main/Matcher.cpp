/* Dallas Scott - ds4015 */
/* Matcher Prototype */

#include <iostream>
#include <list>
#include <string>

using namespace std;

/* Matcher class for pairing job seekers with employers */

class Matcher {
    public:
        int matchScore;

        void gatherRelevantDimensions(/*user*/) {
            /* for given user:
                    iterate through dimensions of profile of relevance
                    add each to list 'relevancies' and its augment to
                    list 'augments' in same index/pos */
        }

        void filterJobs() {
            /* for each job listing:
                    if relevancy missing from description,
                    skip; else, add to list 'cadidates' */
        }

        void match(/*job*/) {
            /* for each listing in 'candidates':
                    calculate match score with
                    defaults and augments, add to list 'scores' */
        }

        void filterMatches(/*list of candidates*/) {
            /* for each candidate:
                 if score in corresponding index
                 in 'scores' above match threshold,
                 keep; else, remove from list.
            */
        }

        void sortMatches(/*revised list of candidates*/) {
            /* for each candidate remaining:
                    sort by match score in ascending order */
        }

        void displayResults(/*sorted list of candidates*/) {
            /* for each sorted candidate:
                    print job listing*/
        }


    private:
        list<string> relevancies;
        list<int> augments;
        list<int> candidates;
        list<int> scores;
}