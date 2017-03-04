## 8SMC1 CalX

This directory contains implementation of extended system GUI. It is based on wxWidgets 3.1.0(see README to build). This component is called CalX for no particular reason(however it was useful to give it name because of C++ namespace and component class names - they start with Calx prefix). Main purpose of this UI is cover whole system core functionality, thus interface itself may seem complicated(it's hard to combine large functionality and intuitive UI). Currently it is developed(moreover other system parts are almost competed, so UI implementing takes maximal effort). System core API may slightly change and extend during UI development to make it easier.

Current development goals:
* Cover all Ctrl-lib API by graphics interface - main goal.
* Increase UI usability - add specific additional components, some default configs and actions, reorganise interface, add friendly error checking, hide some confusing/rear used options from main tabs.