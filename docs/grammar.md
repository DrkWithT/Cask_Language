# Cask Grammar

## Notes:
 - Comments are multi-line by default.
    - Delimited by `'#'`.
 - `agg` means a data structure wrapping one or more typed attributes. This will be like a C `struct`.
 - Math operators: `+, -, *, /`
 - Equality, comparison operators: `==, !=, >, >=, <, <=`
 - Logical operators : `||, &&`

### Core Grammar

```bnf
; basic elements
comment ::= "#" (ANY)* "#"
identifier ::= (ALPHA | "_")+ ; can be identifier or custom aggregate name
primtype ::= "bool" | "int" | "float" | "string"
aggtype ::= identifier
arrtype ::= (primtype | aggtype) "[" "]"
nil-literal ::= "nil"
bool-literal ::= "true" | "false"
integer-literal ::= (DIGIT)+
float-literal ::= (DIGIT)+ "." (DIGIT)+
string-literal ::= "\"" (ALPHA)* "\""
agg-literal ::= "{" literal ("," literal)* "}"
array-literal ::= "[" (basic ",")* "]"

; expressions
basic ::= nil-literal | bool-literal | integer-literal | float-literal | string-literal | agg-literal | identifier | "(" equality-expr ")"
negate-expr ::= "-" basic
term-expr ::= basic (("+" | "-") basic)+
factor-expr ::= term-expr ("*" | "/" term-expr)
comparison-expr ::= factor-expr (("<" | "<=" | ">" | ">=") factor-expr)+
equality-expr ::= comparison-expr (("==" | "!=") comparison-expr)+
conditional-expr ::= equality-expr (("||" | "&&") equality-expr)+

; declaration statements
decl ::= prim-decl | agg-decl | func-decl
prim-decl ::= identifier ":" primtype "=" 
agg-decl ::= "agg" identifier "\{" (prim-decl | agg-decl)+ "\}"
arr-decl ::= identifier ":" arrtype "=" array-literal
func-decl ::= "func" identifier "(" (param-decl)* ")" block
param-decl ::= identifier ":" (primtype | aggtype | arrtype)

; composite statements
"from" string-literal "import" identififer ("," identifier)*
while-stmt ::= "while" "(" conditional-expr ")" block
if-stmt ::= "if" "(" conditional-expr ")" block (else-stmt){1}
else-stmt ::= "else" block
block ::= (stmt)+ "end"
```
