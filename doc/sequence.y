%%
sequence:
  %empty
| maybeword
| sequence "word"
;
maybeword:
  %empty
| "word"
;
