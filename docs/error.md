## CalX error codes
CalX system indicates errors with special codes. There is a list of codes with short description:

| Error code | Short name               | Description     |
| :--------- | :----------------------- | :------------- |
| 0          | No error                 | Should not normally appear as an error. Consider it as bug       |
| 1          | Power off                | One of activated devices is powered off. Check them and power on if necessary       |
| 2          | Low level error          | Error appeared into Device API drivers. Further work may be not safe, save logs and close the application       |
| 3          | Unknown resource         | May only appear during script execution. Otherwise it's a bug. Should not be displayed to user. |
| 4          | Operation unavailable    | May only appear during script execution. Otherwise it's a bug. Should not be displayed to user. |
| 5          | Interrupted              | Occur when the system is interrupted during the script execution |
| 6          | Internally interrupted   | Occur on internal system interruption. Should not be displayed to user. |
| 7          | Wrong parameter          | Incorrect parameter was passed to the internal procedure. Should not occur and it's considered a bug |
| 8 and 9    | Trailer pressed          | Motors reached one of the trailers. You should check given task to be valid.       |
| 10         | Motor running            | Motors are already running. Normally you shouldn't receive it, because appropriate controls should be automatically locked        |
| 11         | Motor stopped            | Internal system error, you should not receive it       |
| 12         | Arc error                | You specified invalid arc parameters, check them       |
| 13         | Invalid coordinates      | You specified impossible coordinates, change them       |
| 14         | Invalid speed            | You specified unreachable speed, change it       |
| 15         | Math expression error    | You entered invalid mathematical expression, verify it for correctness       |
| 16         | Math no variable         | You referenced non-existing math variable       |
| 17         | Math no function         | You called non-existing math function       |
| 18         | Math function parameters | You called function with wrong parameters, check function declaration       |
