The full reference: https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html 

(Re-typed by Nguyen Anh Tuan for the purpose of learning)

# A Regular Expression Matcher 
## Code by Rob Pike 
## Exegesis by Brian Kernighan 

Draft version Jan 28, 2007

### Introduction 

Beautiful code is likely to be simple -- clear and easy to understand. Beautiful code is likely to be compact -- just enough code to do the job and no more -- but not cryptic, to the point where it cannot be understood. Beautiful code may well be general, solving a broad class of problems in a uniform way. One might even describe it as elegant, showing good taste and refinement.

In this chapter I will describe a piece of beautiful code, for matching regular expressions that meets all of these criteria. 

Regular expressions are a notation for describing patterns of text, in effect a special-purpose language for pattern matching. Although there are many variants, all share the idea that most characters in a pattern match literal occurrences of themselves but some "metacharacters" have special meaning, for example "*" to indicate some kind of repetition or [...] to mean any one character from the set within the brackets. 

In practice, most searches in programs like text editors for literal words, so the regular expressions are often literal strings like `print`, which will match `printf` or `sprint` or `printer paper` anywhere. In so-called "wild cards" in filename in Unix and Windows, a * matches any number of characters, so the pattern `*.c` matches all filenames that end in `.c`. There are many, many variants of regular expressions, even in contexts where one would expect them to be the same. Jeffrey Friedl's _Mastering Regular Expressions_ (O'Reilly, 2006) is an exhausive study of the topic. 

Regular expressions were invented by Stephen Kleene in the mid-1950's as a notation for finite automata, and in fact they are equivalent to finite automata in what they represent. Regular expressions first appeared in a programming setting in Ken Thompson's version of the QED text editor in the mid-1960's. In 1967, Ken applied for a patent on a mechanism for rapid text matching based on regular expression; it was granted in 1971, one of the very first software patents [US Patent 3,568,156, Text Matching Algorithm, March 2, 1971].

Regular expressions moved from QED to the Unix editor ed, and then to the quintessential Unix tool, grep, which Ken created by performing radical surgery on ed. Through these widely used programs, regular expressions become familiar throughout the early Unix community.

Ken's original matcher was very fast because it combined two independent ideas. One was to generate machine instructions on the fly during matching so that it ran at machine speed, not by interpretation. The other was to carry forward all possible matches at each stage, so it did not have to backtrack to look for alternative potential matches. Matching code in later text editors that Ken wrote, like ed, used a simpler algorithm that backtracked when necessary. In theory this is slower but the patterns found in practice rarely involved backtracking, so the ed and grep algorithm and code were good enough for most purposes.

Subsequent regular expression matchers like egrep and fgrep added richer classes of regular expressions, and focused on fast execution no matter what the pattern. Ever fancier regular expressions became popular, and were included not only in C-based libraries but also as part of the syntax of scripting languages like Awk and Perl.

### The Practice of Programming 

In 1998, Rob Pike and I were writing _The Practice of Programming_ ("TPOP"). The last chapter of the book, "Notation", collected a number of examples where a good notation led to better programs and better programming. This included the use of simple data specifications (`printf` formats, for instance), and the generation of code from tables. 

Given our Unix backgrounds and many years of experience with tools based on regular expression notation, we naturally wanted to include a discussion of regular expressions, and it seemed mandatory to include an implementation as well. Given our emphasis on tools, it also seemed best to focus on the class of regular expressions found in grep, rather than say those from shell wild cards, since we could also talk about the design of grep itself. 

The problem was that any existing regular expression package was far too big. The local grep was over 500 lines long (about 10 book pages). Open-source regular expression packages tended to be huge, roughly the size of the entire book, because they ware engineered for generality, flexibility, and speed; none was remotely suitable for pedagogy. 

I suggested to Rob that we needed to find the smallest regular expression package that would illustrate the basic ideas while still recognizing a useful and non-trivial class of patterns. Ideally, the code would fit on a single page.

Rob disappeared in to his office, and at least as I remember it now, appeared again in no more than an hour or two with the 30 lines of C code that subsequently appeared in Chapter 9 of TPOP. That code implements a regular expression matcher that handles these constructs:

    ```
    c   matches any literal character c
    .   matches any single character 
    ^   matches the beginning of the input string 
    $   matches the end of the input string
    *   matches zero or more occurrences of the previous character
    ```

This is quite a useful class; in my own experience of using regular expressions on a day-to-day basis, it easily accounts for 95 percent of all instances. In many situations, solving the right problem is a big step on the road to a beautiful program. Rob deserves great credit for choosing so wisely, from among a wide set of options, a very small yet important, well-defined and extensible set of features.

Rob's implementation itself is a superb example of beautiful code: compact, elegant, efficient, and useful. It's one of the best examples of recursion that I have ever seen, and it shows the power of C pointers. Although at the time we were most interested in conveying the important role of a good notation in making a program easier to use and perhaps easier to write performance enhancement, and other important topics.

### Implementation

In the book, the regular expression matcher is part of a program that mimics grep, but the regular expression code is completely separable from its surroundings. The main program is not interesting here -- it simply reads from its standard input or from a sequence of files, and prints those lines that contain a match of the regular rexpression, as does the original grep and many other Unix tools.

This is the matching code:

```cpp
/* match: search for regexp anywhere in text */
int match(char *regexp, char *text)
{
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do { /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/* matchhere: search for regexp at beginning of text */
int matchhere(char *regexp, char *text)
{
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text != '\0' && (regexp[0]=='.' || regexp[0]==*text))
        return matchhere(regexp+1, text+1);
    return 0;
}

/* matchstar: search for c*regexp at beginning of text */
int matchstar(int c, char *regexp, char *text)
{
    do { /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}
```

The function `match(regexp, text)` tests whether there is an occurrence of the regular expression anywhere within the text; it return 1 if a match is found and 0 if not. If there is more than one match, it finds the leftmost and shortest.

The basic operation of `match` is straightforward. If the first character of the regular expession is `^` (an anchored match), any possible match must occur at the beginning of the string. That is, if the regular experssion is `^xyz`, it matches `xyz` only if `xyz` occurs at the beginning of the text, not somewhere in the middle. This is tested by matching the rest of the regular expession against the text starting at the beginning, and nowhere else. 

Otherwise, the regular expression might match anywhere withing the string. This is tested by matching the pattern against each character position of the text in turn. If there are multiple matches, only the first (leftmost) one ill be identified. That is, if the regular expression is `xyz`, it will match the first occurence of `xyz` regardless of where it occurs. 

Notice that advancing over the input string is done with a `do-while` loop, a comparatively unusual construct in C programs. The occurrence of a `do-while` instead of `while` should always raise a question: why isn't the loop termination condition be tested at the beginning of the loop, before it's too late, rather than at the end after something has been done? But the test is correct here: since `*` operator permits zero-length matches, we first have to check whether a null match is possible.

The bulk of the work is done in the function `matchhere(regexp, text)`, which tests whether the regular expression matches the text that begins right here. The function `matchhere` operates by attempting to match the first character of the regular expression with the first character of the text. If the match fails, there can be no match at this text position and `matchere` returns 0. If the match succeeds, however, it's possible to advance to the next character of the regular expression and the next character of the text. This is done by calling `matchhere` recursively. 

The situation is a bit more complicated because of some special cases, and of course the need to stop the recursion. The easiest case is that if the regular expression is at its end (`regexp[0] == '\0'`), then all previous tests have succeeded, and thus the regular expression matches the text. 

If the regular expression is a character followed by a `*`, `matchstar` is called to see whether the closure matches. The function `matchstar(c, regexp, text)` tries to match repetitions of the text character `c`, beginning with zero repetitions and counting up, until it either finds a match of the rest of the text, or it fails and thus concludes that there is no match. This identifies a "shortest match", which is fine for simple pattern matching as in grep, where all that matters is to find a match as quickly as possible. A "longest match" is more intuitive and almost certain to be better for a text editor where the matched text will be replaced. Most modern regular expression libraries provide both alternatives, and TPOP presents a simple variant of `matchstar` for this case, shown later. 

If the regular expression is a `$` at the end of the expression, then the text matches only if it too is at its end.

Otherwise, if we are not at the end of the text string (that is, `*text != '\0'`) and if the first character of the text string matches the first character of the regular expression, so far so good; we go on to test whether the next character of the regular expression matches the next character of the text by making a recursive call to `matchhere`. This recursive call is the heart of the algorithm and is why the code is so compact and clean. 

If all of these attemps to match fail, then there can be no match at this point in the regular expression and the text, so `matchhere` returns 0.

This code really uses C pointers. At each stage of the recursion, if something matches, the recursive call that follows uses pointer arithmetic (e.g., `regexp+1` and `text+1`) so that the next function is called with the next character of the regular expression and of the text. The depth of recursion is no more than the length of the pattern, which in normal use is quite short, so there is no danger of running out of space. 

### Alternatives



---------- 

Appendix
- What is Regular Expression? 
> A regular expression is a sequence of characters that specifies a search pattern in text.

- Guide: [Regular Expressions - Enough to be Dangerous](https://www.youtube.com/watch?v=bgBWp9EIlMM)
- Regex visulization analysis tool: https://www.debuggex.com/ 