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

  class CalxCoreConfiguration {
   public:
    static constexpr auto JumpSpeed = "jump_speed";
  };

  class CalxUIConfiguration {
   public:
    static constexpr auto AdjustStep = "adjust_step";
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
    static constexpr auto OffsetX = "offset_x";
    static constexpr auto OffsetY = "offset_y";
    static constexpr auto ScaleX = "scale_x";
    static constexpr auto ScaleY = "scale_y";
  };

  class CalxCoordPositionConfiguration {
   public:
    static constexpr auto X = "x";
    static constexpr auto Y = "y";
    static constexpr auto XPrecision = "x_prec";
    static constexpr auto YPrecision = "y_prec";
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

  class CalxUnitConfiguration {
   public:
    static constexpr auto UnitScale = "unit_scale";
    static constexpr auto UnitOffsetX = "unit_offset_x";
    static constexpr auto UnitOffsetY = "unit_offset_y";
    static constexpr auto SpeedScale = "speed_scale";
    static constexpr auto UnitSpeed = "unit_speed";
    static constexpr auto PlaneScale = "plane_scale";
    static constexpr auto PlaneSpeedScale = "plane_speed_scale";
  };

  class CalxLinearTaskConfiguration {
   public:
    static constexpr auto XStart = "x_start";
    static constexpr auto YStart = "y_start";
    static constexpr auto Width = "width";
    static constexpr auto Height = "height";
    static constexpr auto Spacing = "spacing";
  };

  class CalxGraphConfiguration {
   public:
    static constexpr auto XFrom = "x_from";
    static constexpr auto XTo = "x_to";
    static constexpr auto YFrom = "y_from";
    static constexpr auto YTo = "y_to";
    static constexpr auto XAxisStep = "x_axis_step";
    static constexpr auto YAxisStep = "y_axis_step";
    static constexpr auto Step = "step";
    static constexpr auto StepStep = "step_step";
  };
}

#endif 