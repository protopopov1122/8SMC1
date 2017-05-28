## CalX Math engine
CalX system contains simple mathematical engine that allows parsing mathematical expressions and evaluating them.
Engine supports:
* direct floating-point and integer constants
* mathematical operations - +, -, *, /, ^
* predefined variable referencing
* predefined function calling
By default, only 'x' variable is predefined and may be safely used. Some C library math functions are predefined:
```
sin(x) - sine
cos(x) - cosine
tan(x) - tangent
asin(x) - arcsine
acos(x) - arccosine
atan(x) - arctangent
exp(x) - exponentiation
log(x) - natural logarithm
log10(x) - decimal logarithm
log2(x) - binary logarithm
sqrt(x) - square root
cbrt(x) - cubic root
ceil(x) - smallest integer value greater than or equal to x
floor(x) -  largest integer value less than or equal to x
abs(x) - absolute value
```

Engine parse and runtime errors are quite poor and only indicate stage, when error appeared.
