#ifndef UT_WCTOMB_H
#define UT_WCTOMB_H

#include <limits.h>
#include "ut_types.h"

#include "ut_iconv.h"

class ABI_EXPORT UT_Wctomb
{
public:
  void initialize();
  UT_Wctomb();
  UT_Wctomb(const char* to_charset);
  UT_Wctomb(const UT_Wctomb& v);
  ~UT_Wctomb();
  int wctomb(char * pC,int &length,UT_UCS4Char wc);
  void wctomb_or_fallback(char * pC,int &length,UT_UCS4Char wc);

  void setOutCharset(const char* charset);
 private:
  UT_iconv_t cd;
};

#endif // UT_WCTOMB_H
