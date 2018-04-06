import sys
import crypt
import string

def main():
    if len(sys.argv) !=2:               # check number of arguments
        print("Usage: ./crack hash")
        exit(1)
        
    hashed = sys.argv[1]
    MAX_LENGTH = 4
    test_word = []                      # a list variable to be iterated
    for i in range(MAX_LENGTH):
        
        test_word.append(0)             # increase a length of a list
        iterate(0,test_word,hashed)     # start hashing and comparing

def iterate(index,test_word, hashed):  # iterates from a to ZZZZ, hashed and compares the result with given hash

    for c in string.ascii_letters:     #Iterate trough all leters a - Z

        test_word[index] = c
        if len(test_word) != index+1:        # if not the last position in a list
            iterate(index+1,test_word,hashed)
            
        else:
            test_word_string = ''.join(test_word)     # convert list to string
            result = crypt.crypt(test_word_string, "50")     #Hash test_word password
            if result in hashed:   #Compare with given hash
                print((''.join(test_word)))
                exit(0)                    #End program after password was found
                
    return 0
    
if __name__ == "__main__":
    main()