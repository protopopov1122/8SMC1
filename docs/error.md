## CalX error codes
CalX system indicates errors with special codes. There is a list of codes with short description:

| Error code | Short name               | Description     |
| :--------- | :----------------------- | :------------- |
| 0          | No error                 | Should not normally appear as an error. Consider it as bug       |
| 1          | Power off                | One of activated devices is powered off. Check them and power on if necessary       |
| 2          | Low level error          | Error appeared into Device API drivers. Further work may be not safe, save logs and close the application       |
| 4 and 5    | Trailer pressed          | Motors reached one of the trailers. You should check given task to be valid.       |
| 6          | Motor running            | Motors are already running. Normally you shouldn't receive it, because appropriate controls should be automatically locked        |
| 7          | Motor stopped            | Internal system error, you should not receive it       |
| 8          | Arc error                | You specified invalid arc parameters, check them       |
| 9          | Invalid coordinates      | You specified impossible coordinates, change them       |
| 10         | Invalid speed            | You specified unreachable speed, change it       |
| 11         | Math expression error    | You entered invalid mathematical expression, verify it for correctness       |
| 12         | Math no variable         | You referenced non-existing math variable       |
| 13         | Math no function         | You called non-existing math function       |
| 14         | Math function parameters | You called function with wrong parameters, check function declaration       |
