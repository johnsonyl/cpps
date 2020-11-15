#pragma once

#include <cpps.h>


std::string cppstojson(cpps::object obj, int encodeType);
cpps::object jsontocpps(cpps::C *c, std::string json);