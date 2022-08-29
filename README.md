# C-Linux-Directory-Commands
Implementation of a command interpreter that
offer a set of commands to process the contents of a directory tree.

## How to run?
```
    make
    
    ./myutil <rootDirectory/>
```

## Commands

- find [/<string/>]: prints the names of files that have in their name the string /<string/>, case sensitive.
- ifind [/<string/>]: prints the names of files that have in their name the string /<string/>, case insensitive.
- cfind /<string1/> /<string2/>: prints the names of files that have in their name the string /<string/>, but additionally checks file content has the string /<string2/>.
- repla /<file/>: given a file of string pairs, substitute occurrences of those strings in the regular files.
- wc: prints by directory the total number of lines and characters of the files regular.
- codif: reverse the content of regular files.
- roll [/<n/>]: rotates the content of each regular file by n characters.