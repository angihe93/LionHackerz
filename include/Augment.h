/* Kelvin Kim sk4802
    Time to check to see how important various factors are to the user */
#ifndef AUGMENT_H
#define AUGMENT_H

#include <string>
#include <vector>
#include "Database.h"

struct AugmentInput {
    int dim_id;
    std::string importance;  // "very", "somewhat", "not"
};

// Function to process and insert augmentations
std::string processAugments(Database *db, int user_id, const std::vector<AugmentInput>& augments);

// Optional: Function to check if a dimension exists
bool dimExists(Database *db, int dim_id);

#endif // AUGMENT_H
