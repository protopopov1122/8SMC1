#ifndef _8SMC1_UI_APP_H_
#define _8SMC1_UI_APP_H_

#include "base.h"
#include "ctrl-lib/SystemManager.h"

namespace _8SMC1UI {
	
	class _8SMC1App : public wxApp
	{
		public:
			_8SMC1App();
			virtual ~_8SMC1App();
			virtual bool OnInit();
		private:
			_8SMC1::SystemManager *sysman;
			_8SMC1::DeviceManager *devman;
	};
}

#endif