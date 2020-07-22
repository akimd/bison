%%
stmt:
  expr
| if_stmt
;

if_stmt:
  "if" expr "then" stmt
| "if" expr "then" stmt "else" stmt
;

expr:
  "identifier"
