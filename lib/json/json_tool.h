// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef LIB_JSONCPP_JSON_TOOL_H_INCLUDED
#define LIB_JSONCPP_JSON_TOOL_H_INCLUDED


// Also support old flag NO_LOCALE_SUPPORT
#ifdef NO_LOCALE_SUPPORT
#define JSONCPP_NO_LOCALE_SUPPORT
#endif

#ifndef JSONCPP_NO_LOCALE_SUPPORT
#include <clocale>
#endif

/* This header provides common string manipulation support, such as UTF-8,
 * portable conversion from/to string...
 *
 * It is an internal header that must not be exposed.
 */

namespace Json {
static char getDecimalPoint() {
#ifdef JSONCPP_NO_LOCALE_SUPPORT
  return '\0';
#else
  struct lconv* lc = localeconv();
  return lc ? *(lc->decimal_point) : '\0';
#endif
}

/// Converts a unicode code-point to UTF-8.
static inline JSONCPP_STRING codePointToUTF8(unsigned int cp) {
  JSONCPP_STRING result;

  // based on description from http://en.wikipedia.org/wiki/UTF-8

  if (cp <= 0x7f) {
    result.resize(1);
    result[0] = static_cast<char>(cp);
  } else if (cp <= 0x7FF) {
    result.resize(2);
    result[1] = static_cast<char>(0x80 | (0x3f & cp));
    result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
  }
  else if (cp <= 0xFFFF) {
	  if ((cp >= 0x4E00 && cp <= 0x9FA5) || (cp >= 0xF900 && cp <= 0xFA2D) || cp == 0x3002 || cp == 0xFF1F || cp == 0xFF01 || cp == 0xFF0C || cp == 0x3001 || cp == 0xFF1B || cp == 0xFF1A || cp == 0x300C || cp == 0x300D || cp == 0x300E || cp == 0x300F || cp == 0x2018 || cp == 0x2019 || cp == 0x201C || cp == 0x201D || cp == 0xFF08 || cp == 0xFF09 || cp == 0x3014 || cp == 0x3015 || cp == 0x3010 || cp == 0x3011 || cp == 0x2014 || cp == 0x2026 || cp == 0x2013 || cp == 0xFF0E || cp == 0x300A || cp == 0x300B || cp == 0x3008 || cp == 0x3009)
	  {
#ifdef WIN32
		  wchar_t src[2] = { 0 };
		  char dest[9] = { 0 };
		  src[0] = static_cast<wchar_t>(cp);
		/*  std::string curLocale = setlocale(LC_ALL, NULL);

		  setlocale(LC_ALL, "chs");
		  wcstombs_s(NULL, dest, 5, src, 2);
		  result = dest;
		  setlocale(LC_ALL, curLocale.c_str());*/

          int len;
          len = WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, NULL);
          WideCharToMultiByte(CP_UTF8, 0, src, -1, dest, len, NULL, NULL);
          result = dest;

#elif LINUX
          std::string curLocale = setlocale(LC_ALL, NULL);
          setlocale(LC_ALL, "zh_CN.utf8");


          char dest[9] = { 0 };
          wchar_t src[2] = { 0 };
          src[0] = static_cast<wchar_t>(cp);

          int len = 0;
          int ret = 0;

          len = wcslen(src) + 1;

          /*sizeof(wchar_t) = 4 */

          /*这里的第三个长度参数，应为字节长度，即宽字符长度 * 4 */
          ret = wcstombs(dest, src, len * sizeof(wchar_t));

          result = dest;

          setlocale(LC_ALL, curLocale.c_str());
#endif
	  }
	  else
	  {
		  result.resize(3);
		  result[2] = static_cast<char>(0x80 | (0x3f & cp));
		  //result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
		  //result[0] = static_cast<char>(0xE0 | (0xf & (cp >> 12)));
		  result[1] = 0x80 | static_cast<char>((0x3f & (cp >> 6)));
		  result[0] = 0xE0 | static_cast<char>((0xf & (cp >> 12)));
	  }
  }
  else if (cp <= 0x10FFFF) {
    result.resize(4);
    result[3] = static_cast<char>(0x80 | (0x3f & cp));
    result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
    result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
    result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
  }

  return result;
}

enum {
  /// Constant that specify the size of the buffer that must be passed to
  /// uintToString.
  uintToStringBufferSize = 3 * sizeof(LargestUInt) + 1
};

// Defines a char buffer for use with uintToString().
typedef char UIntToStringBuffer[uintToStringBufferSize];

/** Converts an unsigned integer to string.
 * @param value Unsigned integer to convert to string
 * @param current Input/Output string buffer.
 *        Must have at least uintToStringBufferSize chars free.
 */
static inline void uintToString(LargestUInt value, char*& current) {
  *--current = 0;
  do {
    *--current = static_cast<char>(value % 10U + static_cast<unsigned>('0'));
    value /= 10;
  } while (value != 0);
}

/** Change ',' to '.' everywhere in buffer.
 *
 * We had a sophisticated way, but it did not work in WinCE.
 * @see https://github.com/open-source-parsers/jsoncpp/pull/9
 */
static inline void fixNumericLocale(char* begin, char* end) {
  while (begin < end) {
    if (*begin == ',') {
      *begin = '.';
    }
    ++begin;
  }
}

static inline void fixNumericLocaleInput(char* begin, char* end) {
  char decimalPoint = getDecimalPoint();
  if (decimalPoint != '\0' && decimalPoint != '.') {
    while (begin < end) {
      if (*begin == '.') {
        *begin = decimalPoint;
      }
      ++begin;
    }
  }
}

} // namespace Json {

#endif // LIB_JSONCPP_JSON_TOOL_H_INCLUDED
