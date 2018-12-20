# 基础规则
1. `.` 任意字符,可能包括换行(any character, possibly including newline (s=true))
2. `[xyz]` 字符集合(character class)
3. `[^xyz]` 不在字符集合内(negated character class)
4. `\pN` Unicode字符集合单字名
5. `\p{Greak}` Unicode字符集合
6. `\PN` 非Unicode字符集合单字名
7. `\P{Greak}` 非Unicode字符集合
8. `xy` x后面紧跟y
9. `x|y` x或y
10. `x*` 零个或多个x
11. `x+` 一个或多个x
12. `x?` 零个或一个x
13. `x{n,m}` n到m个x
14. `x{n,}` 大于等于n个x
# Perl字符规则，仅支持ASCII(小写为集合，大写为集合外)
1. `\d` 数字(=[0-9])
2. `\D` 非数字(=[^0-9])
3. `\s` 空格(=[\t\n\f\r ])
4. `\S` 非空格(=[^\t\n\f\r ])
5. `\w` word字符(=[0-9A-Za-z_])
6. `\W` 非word字符(=[^0-9A-Za-z_])
7. `\h` 支持水平空间，支持水平布局？
8. `\H` 不支持水平空间，不支持水平布局？
9. `\v` 支持垂直布局？
10. `\V` 不支持垂直布局？
# ASCII字符集
1. `[[:alnum:]]` 字母和数字(=[0-9A-Za-z])
2. `[[:alpha:]]` 字母(=[A-Za-z])
3. `[[:ascii:]]` ASCII(=[\x00-\x&F])
4. `[[:blank:]]` 空格tab(=[\t ])
5. `[[:cntrl:]]` control (=[\x00-\x1F\x7F])
6. `[[:digit:]]` digits (=[0-9])
7. `[[:graph:]]` graphical (=[!-~]=[A-Za-z0-9!"#$%&'()*+,\-./:;<=>?@[\\\]^_`{|}~])
8. `[[:lower:]]` lower case (≡ [a-z])
9. `[[:print:]]` printable (≡ [ -~] ≡ [ [:graph:]])
10. `[[:punct:]]` punctuation (≡ [!-/:-@[-`{-~])
11. `[[:space:]]` whitespace (≡ [\t\n\v\f\r ])
12. `[[:upper:]]` upper case (≡ [A-Z])
13. `[[:word:]]` word characters (≡ [0-9A-Za-z_])
14. `[[:xdigit:]]` hex digit (≡ [0-9A-Fa-f])
# 举例