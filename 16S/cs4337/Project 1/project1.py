# Huidong Hwang
# 2/19/16
# Lexical Parser for front2.c written in python
# To run, use python3 project1.py filename.txt
import sys

#Character classes
LETTER =  0 
DIGIT = 1 

#Token codes
INT_LIT  = 10 
IDENT = 11 
ASSIGN_OP = 20 
ADD_OP = 21 
SUB_OP = 22 
MULT_OP = 23 
DIV_OP = 24 
LEFT_PAREN = 25 
RIGHT_PAREN = 26
SEMICOLON = 27
UNKNOWN = 99 

#Error codes
ERROR_FILENAME = 100 
ERROR_FILEIO = 101
ERROR_SEMICOLON = 102
ERROR_MISPAREN = 103
ERROR_NOOP = 104
ERROR_TOKEN = 105
ERROR_EOF = 106

#Shared variables
nextChar="" 
nextToken = 0
EOF = -1 
stream = ""
charClass = 0 
lexeme = ""
    
  
def lookup(ch): 
    global lexeme
    global nextToken
    if ch == ";":
        lexeme += nextChar
        nextToken = SEMICOLON
    elif ch == "=":
        lexeme += nextChar
        nextToken = ASSIGN_OP
    elif ch == "(":
        lexeme += nextChar
        nextToken = LEFT_PAREN
    elif ch == ")":
        lexeme += nextChar
        nextToken = RIGHT_PAREN
    elif ch == "+":
        lexeme += nextChar
        nextToken = ADD_OP
    elif ch == "-":
        lexeme += nextChar
        nextToken = SUB_OP
    elif ch == "*":
        lexeme += nextChar
        nextToken = MULT_OP
    elif ch == "/":
        lexeme += nextChar
        nextToken = DIV_OP
    else:
        lexeme += nextChar
        nextToken = EOF


def getChar():
    global stream
    global nextChar
    global charClass
    if len(stream) != 0:
        nextChar = stream[0]
        stream = stream[1:]
        if nextChar.isalpha():
            charClass = LETTER
        elif nextChar.isdigit():
            charClass = DIGIT
        else:
            charClass = UNKNOWN
    else:
        nextChar = 0
        charClass = EOF
def getNonBlank():
    while (nextChar != 0 and nextChar.isspace()):
        getChar()


def lex(): 
    global lexeme
    global nextToken
    getNonBlank()
    if (charClass == LETTER):
        while(charClass == LETTER or charClass == DIGIT):
            lexeme += nextChar
            getChar()
        nextToken = IDENT
    elif (charClass == DIGIT):
        while(charClass == DIGIT):
            lexeme += nextChar
            getChar()
        nextToken = INT_LIT 
    elif (charClass == UNKNOWN):
        lookup(nextChar)
        getChar()
    elif (charClass == EOF):
        nextToken = EOF
        lexeme = "EOF"
    print("Next token is: %d, Next lexeme is '%s'" %(nextToken, lexeme))
    lexeme = ""


def start():
    print("Enter <start>")
#    /* Parse the first expr */
    while(nextToken != EOF):
        expr()
        if(nextToken == SEMICOLON):
            lex()
        else:
            error(ERROR_SEMICOLON)
    print("Exit <start>");


def expr(): 
    print("Enter <expr>");
    term()
    while (nextToken == ADD_OP or nextToken == SUB_OP):
        lex()
        term()
    print("Exit <expr>")


def term(): 
    print("Enter <term>")
    factor()

    while (nextToken == MULT_OP or nextToken == DIV_OP):
        lex()
        factor()
    print("Exit <term>")


def factor():
    print("Enter <factor>")
    if (nextToken == IDENT or nextToken == INT_LIT):
        lex()
    else:
        if (nextToken == LEFT_PAREN):
            lex()
            expr()
            if (nextToken == RIGHT_PAREN):
                lex()
            else:
                error(ERROR_MISPAREN)
        else:
            error(ERROR_TOKEN)
    print("Exit <factor>");


def error(errcode):
    if (errcode == ERROR_FILENAME):
        print("ERROR: File name is required")
    elif (errcode == ERROR_FILEIO):
        print("ERROR: File unable to be opened")
    elif (errcode == ERROR_SEMICOLON):
        print("ERROR: Missing semicolon after <expr>")
    elif (errcode == ERROR_MISPAREN):
        print("ERROR: Expecting right parenthesis")
    elif (errcode == ERROR_NOOP):
        print("ERROR: Expecting an operation")
    elif (errcode == ERROR_TOKEN):
        print("ERROR: Expecting a valid token")
    elif (errcode == ERROR_EOF):
        print("ERROR: Expected EOF")
        print("ERROR: instead found next token is: %d, Next lexeme is '%s'" % (nextToken, lexeme))
    else:
        print("ERROR: Something went horribly wrong!")
    sys.exit()


if len(sys.argv) == 1:
    error(ERROR_FILENAME)
try:
    with open(sys.argv[1],"r+") as f:
        stream = f.read()
        getChar()
        lex()
        start()
        if nextToken == EOF:
            print(">> EOF")
        else:
            error(ERROR_EOF)
except IOError:
        error(ERROR_FILEIO)
