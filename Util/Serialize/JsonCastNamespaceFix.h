// DO NOT PUT INCLUDE GUARD HERE!!

// include this file in the namespace of the reflected object

// This is necessary because the from_json and to_json declarations must be in
// the namespace of the class. See: https://github.com/nlohmann/json

template<typename T>
void to_json(nlohmann::json& j, const T& obj)
{
   ::to_json(j, obj);
}

template<typename T>
void from_json(const nlohmann::json& j, T& obj)
{
   ::from_json(j, obj);
}
