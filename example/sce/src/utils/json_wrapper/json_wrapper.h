#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

namespace sce::json_wrapper {
template <typename JsonValue, typename ValueType>
auto get_to(const JsonValue& json [[maybe_unused]],
            ValueType& v [[maybe_unused]]);

template <typename JsonValue, typename ValueType>
void from_json(const JsonValue&, ValueType&);

template <typename ValueType, typename JsonValue>
auto get(const JsonValue& json) {
  ValueType value;
  from_json(json, value);
  return value;
}

#ifdef INCLUDE_NLOHMANN_JSON_HPP_
template <typename ValueType>
auto get_to(const nlohmann::json& json, ValueType& value) {
  return json.get_to(value);
}

template <typename ValueType>
auto get(const nlohmann::json& json) {
  return json.get<ValueType>();
}
#endif

#ifdef QJSONVALUE_H
template <typename ValueType>
auto get_to(const QJsonValueRef& json, ValueType& value) {
  return value = qvariant_cast<ValueType>(json.toVariant());
}

template <>
auto inline get_to(const QJsonValueRef& json, int& value) {
  return (value = json.toInt());
}

template <>
auto inline get_to(const QJsonValueRef& json, double& value) {
  return (value = json.toDouble());
}

template <>
auto inline get_to(const QJsonValueRef& json, std::string& value) {
  return value = json.toString().toStdString();
}

template <typename ValueType>
auto get_to(const QJsonValue& json, ValueType& value) {
  return value = qvariant_cast<ValueType>(json.toVariant());
}
#endif
}  // namespace sce::json_wrapper

#endif  //JSON_WRAPPER_H
