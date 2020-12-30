#pragma once

#include <cpps/cpps.h>


std::string cppstojson(cpps::object obj, cpps::object encodeType);
cpps::object jsontocpps(cpps::C *c, std::string json);