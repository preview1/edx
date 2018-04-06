def main():
    print("Number: ", end = "")
    
    while True:
        cc_num = check_input(input())       # check input and assign it to cc_num
        length = len(str(int(cc_num)))      # get length of cc number
        if cc_num != 0:                     # break while if check input returns valid value
            break
    if length >= 13 and length<= 16:    
        starts_with = lohn_check(cc_num, length)  #check with loan alhorithm
        if starts_with != 0:
            check_company(starts_with, length)    #check for the right company
        else:
            print("INVALID")
    else:        
        print("INVALID")    
        
        
def check_input(n):
    if n.isdigit():
        return (int(n))
    else:    
        print("Retry: ", end = "") 
        return 0

def lohn_check(cc_num, length):
    
    sum1 = 0;
    sum2 = 0;
    TOTAL = 0;
    digit_number = 0;
    starts_with_inner = 0;     
    
    for i in range(length):                      # Iterate trought the digits of the cc_number
    
        digit = cc_num % 10                         # Get last digit  
        cc_num = cc_num // 10                       # Remove last digit    
        digit_number += 1                        # Keep track of digit's position in cc_number
            
        if digit_number % 2 == 0:           # Iterate trough every second digit
            if digit * 2 < 10:
            
                sum1 = sum1 + (digit * 2)
            
            else:                            # If digit times two is a number of more than one digit sum up the separate digits
               
                digits_digit1 = digit * 2 % 10
                digits_digit2 = digit * 2 // 10 % 10
                sum1 = sum1 + digits_digit1 + digits_digit2
            
             
        
        else:                                # Normally sum up every other digit of the cc_number
        
            sum2 = sum2 + digit;
        
            
        if  length - digit_number < 1:      # At the same time check, if it's already first two digits of cc_number - get them
        
            starts_with_inner = starts_with_inner + (digit * 10)
        
        elif length - digit_number < 2:
        
            starts_with_inner = starts_with_inner + digit
        

        
      
    TOTAL = sum1+sum2;
    if TOTAL % 10 == 0:            # If the last digit of checksum is 0, card number is valid
        return starts_with_inner   # return first two digits of the cc_number
    else: 
        return 0                  #Else return 0

def check_company(starts_with, length):
    
    if length == 15 and (starts_with == 34 or starts_with == 37):
        print("AMEX")
    elif length == 16 and starts_with >= 51 and starts_with <= 55:
        print("MASTERCARD")
    elif (length == 13 or length == 16) and starts_with // 10 == 4:
        print("VISA")
    else:
        print("INVALID")
    
if __name__ == "__main__":
    main()