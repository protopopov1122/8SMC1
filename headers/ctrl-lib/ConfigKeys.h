#ifndef CALX_CTRL_LIB_CONFIG_KEYS_H_
#define CALX_CTRL_LIB_CONFIG_KEYS_H_

namespace CalX {

  class CalxConfiguration {
   public:
    static constexpr auto Core = "core";
    static constexpr auto Logging = "log";
    static constexpr auto Scripting = "script";
    static constexpr auto Extensions = "ext";
    static constexpr auto Units = "units";
    static constexpr auto Configuration = "config";
    static constexpr auto SerialConnection = "serial";
    static constexpr auto Motor = "motor";
    static constexpr auto Connection = "connection";
    static constexpr auto Coordinates = "coords";
    static constexpr auto UserInterface = "ui";
    static constexpr auto RelativePosition = "relative_pos";
    static constexpr auto Watcher = "watcher_color";
    static constexpr auto LinearTask = "linear_task";
    static constexpr auto Graph = "graph";
  };

  class CalxMotorConfiguration {
   public:
    static constexpr auto RollStep = "roll_step";
    static constexpr auto RollSpeed = "roll_speed";
    static constexpr auto TrailerComeback = "trailer_comeback";
    static constexpr auto Divisor = "divisor";
    static constexpr auto MaxSpeed = "dev_speed";
  };

  class CalxDeviceConfiguration {
   public:
    static constexpr auto COMPort = "port";
    static constexpr auto COMSpeed = "speed";
    static constexpr auto COMParity = "parity";
  };

  class CalxCoordConfiguration {
   public:
    static constexpr auto MaxSpeed = "maxspeed";
  };

  class CalxWatcherConfiguration {
   public:
    static constexpr auto PointerR = "pointer_R";
    static constexpr auto PointerG = "pointer_G";
    static constexpr auto PointerB = "pointer_B";
    static constexpr auto JumpR = "jump_R";
    static constexpr auto JumpG = "jump_G";
    static constexpr auto JumpB = "jump_B";
    static constexpr auto MoveR = "move_R";
    static constexpr auto MoveG = "move_G";
    static constexpr auto MoveB = "move_B";
    static constexpr auto AppendInterval = "watcher_append_interval";
    static constexpr auto RenderInterval = "watcher_render_interval";
  };
}

#endif 