/*
 * LiangBarskyClipping.h
 *
 *  Created on: Apr 19, 2013
 *      Author: lucasperin
 */

#ifndef LIANGBARSKYCLIPPING_H_
#define LIANGBARSKYCLIPPING_H_

#include "CohenSutherlandClipping.h"

class LiangBarskyClipping: public CohenSutherlandClipping {
public:
	LiangBarskyClipping(axis xMin, axis yMin, axis xMax, axis yMax);
	LiangBarskyClipping(Window* window);
	virtual ~LiangBarskyClipping();


protected:

	virtual Line* clip(Line* l);

};

#endif /* LIANGBARSKYCLIPPING_H_ */
