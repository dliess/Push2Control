#include "JsonCast.h"
#include <iostream>

template <typename T>
void to_json(nlohmann::json& j, const T& obj)
{
    j = meta::serialize(obj);
}

template <typename T>
void from_json(const nlohmann::json& j, T& obj)
{
    meta::deserialize(obj, j);
}

namespace meta
{

//////////////////// SOME HELPERS

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional< std::optional<T> > : std::true_type {};

template<std::size_t I = 0, typename... Tp, typename Fn>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_each_in_tuple(std::tuple<Tp...>& t, Fn&& func)
{ }

template<std::size_t I = 0, typename... Tp, typename Fn>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
for_each_in_tuple(std::tuple<Tp...>& t, Fn&& func)
{
   if(func(I, std::get<I>(t)))
   {
       for_each_in_tuple<I + 1, Tp...>(t, func);
   }
}

/////////////////// SERIALIZATION

template<typename Class>
class SerializerFunc
{
public:
    SerializerFunc(const Class& rObj, nlohmann::json& rValue) : m_rObj(rObj), m_rValue(rValue) {};
    template<typename Member,
             typename = std::enable_if_t<!is_optional< meta::get_member_type<Member> >::value>
    >
    void operator()(Member& member)
    {
        auto& valueName = m_rValue[member.getName()];
        if (member.canGetConstRef()) {
            valueName = member.get(m_rObj);
        } else if (member.hasGetter()) {
            valueName = member.getCopy(m_rObj); // passing copy as const ref, it's okay
        }
    }
    template<typename Member,
             typename = std::enable_if_t<is_optional< meta::get_member_type<Member> >::value>,
             typename = void
    >
    void operator()(Member& member)
    {
        if (member.canGetConstRef()) {
            auto& rOptional = member.get(m_rObj);
            if(rOptional)
            {
                auto& valueName = m_rValue[member.getName()];
                valueName = *member.get(m_rObj);
            }
        }
    }
private:
    const Class&    m_rObj;
    nlohmann::json& m_rValue;
};

template <typename Class, typename>
nlohmann::json serialize(const Class& obj)
{
    nlohmann::json value;
    SerializerFunc<Class> serializeFunc(obj, value);
    meta::doForAllMembers<Class>(serializeFunc);
    return value;
}

template <typename Class,
    typename, typename>
nlohmann::json serialize(const Class& obj)
{
    return serialize_basic(obj);
}

template <typename Class>
nlohmann::json serialize_basic(const Class& obj)
{
    return nlohmann::json(obj);
}

// specialization for std::vector
template <typename T>
nlohmann::json serialize_basic(const std::vector<T>& obj)
{
    nlohmann::json value;
    int i = 0;
    for (auto& elem : obj) {
        value[i] = elem;
        ++i;
    }
    return value;
}

// specialization for std::unordered_map
template <typename K, typename V>
nlohmann::json serialize_basic(const std::unordered_map<K, V>& obj)
{
    nlohmann::json value;
    for (auto& pair : obj) {
        value.emplace(castToString(pair.first), pair.second);
    }
    return value;
}

// specialization for mpark::variant
template <typename... T>
nlohmann::json serialize_basic(const mpark::variant<T...>& obj)
{
   nlohmann::json ret;
   std::tuple<T...> tuple;
   for_each_in_tuple(tuple, [&obj, &ret](int i, auto& typeHolder){
      using Type = std::decay_t<decltype(typeHolder)>;
      auto p = mpark::get_if<Type>(&obj);
      if(p)
      {
         ret = meta::serialize(*p);
         ret["__type_index__"] = i;
      }
      return true;
   });
   return ret;
}

inline
nlohmann::json serialize_basic(const mpark::monostate& obj)
{
    return nlohmann::json();
}

/////////////////// DESERIALIZATION

template <typename Class>
Class deserialize(const nlohmann::json& obj)
{
    Class c;
    deserialize(c, obj);
    return c;
}

template<typename Class>
class DeserializerFunc
{
public:
    DeserializerFunc(Class& rObj,const nlohmann::json& rValue) : m_rObj(rObj), m_rValue(rValue) {};
    template<typename Member,
             typename = std::enable_if_t<!is_optional< meta::get_member_type<Member> >::value>
    >
    void operator()(Member& member)
    {
        auto it = m_rValue.find(member.getName());
        if(it != m_rValue.end())
        {
            auto& objName = *it;
            if (!objName.is_null()) {
                using MemberT = meta::get_member_type<decltype(member)>;
                if (member.hasSetter()) {
                    member.set(m_rObj, objName.template get<MemberT>());
                } else if (member.canGetRef()) {
                    member.getRef(m_rObj) = objName.template get<MemberT>();
                } else {
                    throw std::runtime_error("Error: can't deserialize member because it's read only");
                }
            }
        }
        else
        {
            std::string errMsg("Error: JSon Map Key not found: ");
            errMsg.append(member.getName());
            throw std::runtime_error(errMsg);
        }
    }
    template<typename Member,
             typename = std::enable_if_t<is_optional< meta::get_member_type<Member> >::value>,
             typename = void
    >
    void operator()(Member& member)
    {
        auto it = m_rValue.find(member.getName());
        if(it != m_rValue.end())
        {
            auto& objName = *it;
            using MemberT = typename meta::get_member_type<decltype(member)>::value_type;
            if (member.canGetRef()) {
                member.getRef(m_rObj).emplace(objName.template get<MemberT>());
            }else {
                throw std::runtime_error("Error: can't get reference to std::optional");
            }
        }
    }
private:
    Class&                m_rObj;
    const nlohmann::json& m_rValue;
};


template <typename Class,
    typename>
void deserialize(Class& obj, const nlohmann::json& object)
{
    if (object.is_object()) {
        DeserializerFunc<Class> deserializerFunc(obj, object);
        meta::doForAllMembers<Class>(deserializerFunc);
    } else {
        throw std::runtime_error("Error: can't deserialize from Json::nlohmann::json to Class.");
    }
}

template <typename Class,
    typename, typename>
void deserialize(Class& obj, const nlohmann::json& object)
{
    deserialize_basic(obj, object);
}

template <typename Class>
void deserialize_basic(Class& obj, const nlohmann::json& object)
{
    obj = object.get<Class>();
}

// specialization for std::vector
template <typename T>
void deserialize_basic(std::vector<T>& obj, const nlohmann::json& object)
{
    obj.reserve(object.size()); // vector.resize() works only for default constructible types
    for (auto& elem : object) {
        obj.push_back(elem); // push rvalue
    }
}

// specialization for std::unodered_map
template <typename K, typename V>
void deserialize_basic(std::unordered_map<K, V>& obj, const nlohmann::json& object)
{
    for (auto it = object.begin(); it != object.end(); ++it) {
        obj.emplace(fromString<K>(it.key()), it.value());
    }
}

// specialization for mpark::variant
template <typename... T>
void deserialize_basic(mpark::variant<T...>& ret, const nlohmann::json& object)
{
   std::tuple<T...> tuple;
   for_each_in_tuple(tuple, [&ret, &object](int i, auto& typeHolder){
      using Type = std::decay_t<decltype(typeHolder)>;
      try {
         auto it = object.find("__type_index__");
         if(it != object.end() && *it != i){
                throw std::runtime_error("");
         }
         ret = object.get<Type>();
         return false; // we have the match
      }catch(std::runtime_error& e){
         using FirstType = typename std::tuple_element<0, std::tuple<T...>>::type;
         using LastType = typename std::tuple_element<sizeof...(T) - 1, std::tuple<T...>>::type;
         if(std::is_same<Type,LastType>::value)
         {
            throw std::runtime_error("Error: No Type in Variant matched");
         }
         return true;
      }
   });
}

inline
void deserialize_basic(mpark::monostate& ret, const nlohmann::json& object)
{}

}
