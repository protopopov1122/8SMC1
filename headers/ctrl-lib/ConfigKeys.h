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
}

#endif 