#ifndef _8SMC1_UI_APP_H_
#define _8SMC1_UI_APP_H_

#include "base.h"

namespace _8SMC1::UI {
	
	class _8SMC1App: public wxApp
	{
		public:
			virtual bool OnInit();
	};
}

#endif