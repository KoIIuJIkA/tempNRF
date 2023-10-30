#include "../include/redis_find.h"

using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

int main() {

    try {
        using OptionalString = sw::redis::OptionalString;
        using StringView = sw::redis::StringView;
        auto redis = RedisInterface("tcp://127.0.0.1:6379");

        std::ifstream f("data_model.json");
        json data = json::parse(f);

        redis.create("key_data", data.dump(4));

        redis.find("config_file.json");



    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
