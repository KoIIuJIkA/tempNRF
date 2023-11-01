#include <gtest/gtest.h>
#include "redis_find.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;
using json = nlohmann::json;

TEST(FUNCTION, booling) {
    bool a = true;
    EXPECT_EQ(a, true);
}

// find_code expected true.
TEST(FUNCTION, find_code_true) {
    auto redis = RedisInterface("tcp://127.0.0.1:6379");

    std::ifstream cf("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/config_file_test.json");
    std::ifstream pr("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/1profile.json");
    
    json config;
    json profile;

    std::vector<json> match_nfprofile;

    if (cf.is_open() && pr.is_open()) {
        config = json::parse(cf);
        profile = json::parse(pr);
        redis.find_code(config, profile, match_nfprofile);
        EXPECT_EQ((match_nfprofile[0].dump() == profile.dump()), true);
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

// find_code expected false (wrong key).
TEST(FUNCTION, find_code_false_key) {
    auto redis = RedisInterface("tcp://127.0.0.1:6379");

    std::ifstream cf("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/config_file_test.json");
    std::ifstream pr("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/1profile_wrong_key.json");
    
    json config;
    json profile;

    std::vector<json> match_nfprofile;

    if (cf.is_open() && pr.is_open()) {
        config = json::parse(cf);
        profile = json::parse(pr);
        redis.find_code(config, profile, match_nfprofile);
        EXPECT_EQ(match_nfprofile.empty(), true);
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

// find_code expected false (wrong value).
TEST(FUNCTION, find_code_false_value) {
    auto redis = RedisInterface("tcp://127.0.0.1:6379");

    std::ifstream cf("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/config_file_test.json");
    std::ifstream pr("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/1profile_wrong_value.json");
    
    json config;
    json profile;

    std::vector<json> match_nfprofile;

    if (cf.is_open() && pr.is_open()) {
        config = json::parse(cf);
        profile = json::parse(pr);
        redis.find_code(config, profile, match_nfprofile);
        EXPECT_EQ(match_nfprofile.empty(), true);
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

TEST(FUNCTION, findJ2J) {
    auto redis = RedisInterface("tcp://127.0.0.1:6379");

    std::ifstream cf("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/config_file_test.json");
    std::ifstream inc("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/nfinstance.json");
    
    json config;
    json nfinstace;

    std::vector<json> match_nfprofiles;

    if (cf.is_open() && inc.is_open()) {
        config = json::parse(cf);
        nfinstace = json::parse(inc);
        match_nfprofiles = redis.findJ2J(nfinstace, config);
        EXPECT_EQ(match_nfprofiles.size() == 2, true); 
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

TEST(FUNCTION, findJ2J_1) {
    auto redis = RedisInterface("tcp://127.0.0.1:6379");

    std::ifstream cf("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/config_file_test.json");
    std::ifstream inc("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/2nfinstance.json");
    
    json config;
    json nfinstace;

    std::vector<json> match_nfprofiles;

    if (cf.is_open() && inc.is_open()) {
        config = json::parse(cf);
        nfinstace = json::parse(inc);
        match_nfprofiles = redis.findJ2J(nfinstace, config);
        EXPECT_EQ(match_nfprofiles.size() == 1, true); 
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

TEST(FUNCTION, find) {
        auto redis = RedisInterface("tcp://127.0.0.1:6379");
        std::vector<OptionalString> match_keys;

        std::ifstream f("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/nfinstance.json");
        json data = json::parse(f);

        redis.create("key_data", data.dump(4));

        match_keys = redis.find("/Users/georgryabov/Desktop/wtf/tempNRF/test/test_code/data/config_file_test.json");

        EXPECT_EQ(*(match_keys[0]) == "key_data", true);
}
