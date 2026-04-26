from cs50 import get_string

def calc_idx(L: float, S: float):
    return round(0.0588 * L - 0.296 * S - 15.8)

def is_end_sentence(c: str):
    return c == "!" or c == "?" or c == "."

def parse_text(text: str):
    letters = 0
    words = 0
    sentences = 0

    last = '\0'
    for c in text:
        if(c.isalpha()):
            letters += 1
        elif(c.isspace() and not last.isspace() and not is_end_sentence(last)):
            words += 1
        elif(is_end_sentence(c)):
            sentences += 1
            words += 1
        last = c

    L = letters / words * 100
    S = sentences / words * 100

    return [ L, S ]

def readability():
    text = get_string("Text: ")

    [ l,  s ] = parse_text(text)

    avg = calc_idx(l, s)


    if(avg < 1): return print("Before Grade 1")
    if(avg >= 16): return print("Grade 16+")

    return print(f"Grade {avg}")


readability()
