#ifndef _8SMC1_UI_MAIN_FRAME_H_
#define _8SMC1_UI_MAIN_FRAME_H_

#include "base.h"
#include "app.h"
#include "ctrl-lib/SystemManager.h"
#include <string>

namespace _8SMC1UI {
	
	class _8SMC1MainFrame : public wxFrame {
		public:
			_8SMC1MainFrame(std::string, _8SMC1App*);
			virtual ~_8SMC1MainFrame();
		private:
			_8SMC1App *app;
			void initFrame();
	};
}


#endif