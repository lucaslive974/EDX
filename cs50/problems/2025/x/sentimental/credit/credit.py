import re
from cs50 import get_string

def checksum(card_number: str):
    sum  = 0

    for i, s in enumerate(reversed(card_number)):
        if(i % 2 == 0):
            sum += int(s)
            continue
        n = int(s) * 2
        n_string = str(n)
        for c in n_string:
            sum += int(c)

    return sum % 10 == 0

def validate_card():
    card_number = re.sub(r'[^\d]', "", get_string("Number: "))

    valid = checksum(card_number)

    if(not valid):
        print("INVALID")
        return

    card_len = len(card_number)
    if(card_len == 13 or card_len == 16 and card_number[0] == "4"):
        print("VISA")
        return

    card_code = int(card_number[0:2])
    if(card_len == 15 and (card_code == 34 or card_code == 37)):
        print("AMEX")
        return

    if(card_len == 16 and card_code > 50 and card_code < 56):
        print("MASTERCARD")
        return


    print("INVALID")
    return


validate_card()
