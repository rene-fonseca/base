/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__CONSTANTS_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__CONSTANTS_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This namespace holds some offen used mathematical constants. Please note that
  the accuracy provided here may not be supported by the particular platform.

  @short Commonly used mathematical constants
  @ingroup mathematics
*/

namespace constant {

  /** The base of the natural logarithm specified by the symbol "e" (or exp(1)). */
  const long double E = 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427L;
  /** ln(2). */
  const long double LN2 = 0.6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875L;
  /** ln(10). */
  const long double LN10 = 2.302585092994045684017991454684364207601101488628772976033327900967572609677352480235997205089598298L;
  /** log_2(10). */
  const long double LOG210 = 3.321928094887362347870319429489390175864831393024580612054756395815934776608625215850139743359370155L;
  /** log_2(e). */
  const long double LOG2E = 1.442695040888963407359924681001892137426645954152985934135449406931109219181185079885526622893506344L;
  /** log_10(2). */
  const long double LOG102 = 0.3010299956639811952137388947244930267681898814621085413104274611271081892744245094869272521181861720L;
  /** log_10(e). */
  const long double LOG10E = 0.4342944819032518276511289189166050822943970058036665661144537831658646492088707747292249493384317483L;
  /** pi. */
  const long double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068L;
  /** 2 * pi. */
  const long double PI2 = 6.283185307179586476925286766559005768394338798750211641949889184615632812572417997256069650684234136L;
  /** pi/2. */
  const long double PI_2 = 1.570796326794896619231321691639751442098584699687552910487472296153908203143104499314017412671058534L;
  /** pi/4. */
  const long double PI_4 = 0.7853981633974483096156608458198757210492923498437764552437361480769541015715522496570087063355292670L;
  /** 1/pi. */
  const long double INV_PI = 0.3183098861837906715377675267450287240689192914809128974953346881177935952684530701802276055325061719L;
  /** 2/pi. */
  const long double INV_PI_2 = 0.6366197723675813430755350534900574481378385829618257949906693762355871905369061403604552110650123438L;
  /** 2/sqrt(pi). */
  const long double INV_SQRTPI_2 = 1.128379167095512573896158903121545171688101258657997713688171443421284936882986828973487320404214727L;
  /** sqrt(2). */
  const long double SQRT2 = 1.414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641573L;
  /** sqrt(5). */
  const long double SQRT5 = 2.236067977499789696409173668731276235440618359611525724270897245410520925637804899414414408378782275L;
  /** 1/sqrt(2). */
  const long double INV_SQRT2 = 0.7071067811865475244008443621048490392848359376884740365883398689953662392310535194251937671638207864L;
  /** 1/sqrt(5). */
  const long double INV_SQRT5 = 0.4472135954999579392818347337462552470881236719223051448541794490821041851275609798828828816757564550L;
  /** The golden ratio. */
  const long double GOLDEN_RATIO = 1.618033988749894848204586834365638117720309179805762862135448622705260462818902449707207204189391137L;
  /** The Euler-Mascheroni constant (gamma). */
  const long double EULER = 0.5772156649015328606065120900824024310421593359399235988057672348848677267776646709369470632917467495L;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
