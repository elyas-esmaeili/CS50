# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

an invented long word said to mean a lung disease caused by inhaling very fine ash and sand dust.

## According to its man page, what does `getrusage` do?

int getrusage(int who, struct rusage *usage);
returns resource usage measures for who

## Per that same man page, how many members are in a variable of type `struct rusage`?

16

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

We send their address becuase it is memory efficient and we don't need to copy entire before and after, insted we just copy their address and can access their content
inside calculate function.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

After loading a textfile and a dictionary, main() function starts reading a text
symbol-by-symbol, with a goal of constructing separate words. A word is completed
when one or more alphabetic symbols are read, and we encounted a non-alphanumeric
character (like spacebar, or newline). Then we check our word for misspellings,
reset the word index to zero, and continue the symbol reading loop. If we encounter
a digit, or a string tends to be too long, we traverse this word in the text, but we
won't check it for misspellings.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

fscanf with a format string like "%s" will read subsequent characters until a whitespace is found (whitespace characters are considered to be
blank, newline and tab). if there is a symbol or punctuation within the word fscanf will view them as a part of word.(for example: <<NOTE:) then
have to split this word ant extract just character from it to determine wheter it is in dictionaries or not.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

Because load and check can't change the value of their parameters.