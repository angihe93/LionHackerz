// Copyright 2024 LionHackerz
/* Kelvin Kim sk4802
    Augment.cpp implementation*/

#include "Augment.h"
#include <iostream>
#include <stdexcept>

bool dimExists(Database& db, int dim_id) {
    int resCount = 0;
    // Query the 'dimension' table to check if dim_id exists
    std::vector<std::vector<std::string>> result =
        db.query("dimension", "id", "id", "eq", std::to_string(dim_id), false, resCount);
    return resCount > 0;
}

std::string processAugments(Database& db, int user_id, const std::vector<AugmentInput>& augments) {
    std::string result = "Augmentation processing completed.";
    for (const auto& augment : augments) {
        // Verify that dim_id exists
        if (!dimExists(db, augment.dim_id)) {
            std::cerr << "Dimension ID " << augment.dim_id << " does not exist. Skipping." << std::endl;
            continue;
        }

        int weight = 0;
        if (augment.importance == "very") {
            weight = 100;
        } else if (augment.importance == "somewhat") {
            weight = 50;
        } else {
            continue;  // Skip if "not important"
        }

        // Construct json for insertion
        std::string data = "{\"user_id\": " + std::to_string(user_id) +
                           ", \"dim_id\": " + std::to_string(augment.dim_id) +
                           ", \"weight_mod\": " + std::to_string(weight) + "}";

        // Insert into 'has_augment' table
        std::string response = db.insert("has_augment", data);
        std::cout << "Augment Insert Response: " << response << std::endl;
    }
    return result;
}
