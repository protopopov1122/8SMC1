## Device API wrappers
This directory contains 8smc1 controller API C++ low-level bindings.
They are split into several classes:
* DeviceManager - initialize all available controllers, create Device objects, hold pointers to them and destroy resources on exit.
* Device - main wrapper class. Contains methods that directly operate with USMC API structures and functions. Each method in class header file is marked by USMC strcutre field or function it operates.
* MotorTask - helper class to control motor movement.

## NOTE
Only this classes should directly operate with real USMC controller API functions. All other code should use this wrappers to access controllers. This creates an abstract layer between other code and real controllers.
