//===----------------------------------------------------------------------===//

#include <iostream>
#include <fstream>
#include <vector>

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

#include "../include/redis_find.h"

/// inditificator. 
using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

/**
 ** \brief Represents a interface and search interactions in the redis database.
 */

//===------------------------------------------------------------------===//
// CRUD redefined from the redis++ library.
//===------------------------------------------------------------------===//

// set method.
bool RedisInterface::create(const StringView &key, const StringView &val) {
    return set(key, val);
}

// get method.
OptionalString RedisInterface::read(const StringView &key) {                
    return get(key);
}        

// del method.
long long RedisInterface::del(const StringView &key) {                     
    return Redis::del(key);
}                      

//parse + accept??

std::vector<std::vector<json>> RedisInterface::benchmark(
                                               const char* nfinstance_path,
                                               const char* config_path) {
    std::vector<std::vector<json>> benchmark_profiles;

    std::ifstream f(nfinstance_path);
    json nfinstance = json::parse(f);

    std::ifstream cf(config_path);
    json config_arr_file = json::parse(cf);  

    for (auto& config : config_arr_file) {
        benchmark_profiles.push_back((*this).findJ2J(nfinstance, config));
    }
    return benchmark_profiles;
}

std::vector<OptionalString> RedisInterface::find(const char* config_path) {
    std::vector<OptionalString> match_keys;

    std::ifstream cf(config_path); 
    json config_file = json::parse(cf);  

    std::vector<OptionalString> keys;
    (*this).keys("*", std::back_inserter(keys));
    for (const auto& key : keys) {
        json profile =  json::parse(*((*this).get(*key)));
        std::vector<json> match_nfprofiles = 
                                        (*this).findJ2J(profile, config_file);
        if (match_nfprofiles.size())
            match_keys.push_back(key);
    }
    return match_keys;
}

void RedisInterface::find_code(json& config_file, 
                json& profile, 
                std::vector<json> &match_nfprofiles) {
    bool flag = true;
    for (auto& el : config_file.items()) {      
        if (flag *= profile.contains(el.key()))
            flag *= profile[el.key()] == config_file[el.key()];
        if (!flag)
            break;
    }
    if (flag) {
        match_nfprofiles.push_back(profile);
    }
}

std::vector<json> RedisInterface::findJ2J(json nfinstance, json config_file) {
    std::vector<json> match_nfprofiles;    

    // !!manually var. 90 is the keys in the nfprofile.
    if (nfinstance.begin().value().size() == 90) {
        for (auto& profile : nfinstance) {
            find_code(config_file, profile, match_nfprofiles);
        }
    } else {
        find_code(config_file, nfinstance, match_nfprofiles);
    }
    
    return match_nfprofiles;
}  

std::vector<json> RedisInterface::findP2P(const char* nfinstance_path,
                                          const char* config_path) {
    std::vector<json> match_nfprofiles;

    std::ifstream f(nfinstance_path);
    std::ifstream cf(config_path); 

    json nfinstance = json::parse(f);
    json config_file = json::parse(cf);    

    // !!manually var. 90 is the keys in the nfprofile.
    if (nfinstance.begin().value().size() == 90) {
        for (auto& profile : nfinstance) {
            find_code(config_file, profile, match_nfprofiles);
        }
    } else {
        find_code(config_file, nfinstance, match_nfprofiles);
    }

    return match_nfprofiles;
}  
